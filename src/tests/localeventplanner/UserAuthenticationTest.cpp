#include "gtest/gtest.h"
#include "UserAuthentication.h"
#include "sqlite3.h"
#include <string>
#include <queue>
#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>

// Mock Key Reader Logic
std::queue<int> keyQueue;

int mockKeyReaderImpl() {
    if (keyQueue.empty()) return -1; // Should not happen in good test
    int key = keyQueue.front();
    keyQueue.pop();
    return key;
}

class UserAuthenticationTest : public ::testing::Test {
protected:
    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream test_cin;
    std::stringstream test_cout;

    void SetUp() override {
        // Mock Key Reader
        setMockKeyReader(mockKeyReaderImpl);
        while(!keyQueue.empty()) keyQueue.pop();

        // Redirect cin/cout
        orig_cin = std::cin.rdbuf(test_cin.rdbuf());
        orig_cout = std::cout.rdbuf(test_cout.rdbuf());
    }

    void TearDown() override {
        setMockKeyReader(nullptr);
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);
    }

    // Helper for key input (for getPasswordInput)
    void pushKey(char c) {
        keyQueue.push(c);
    }
    void pushStringKeys(const std::string& str) {
        for (char c : str) keyQueue.push(c);
    }
    void pushEnterKey() {
#ifdef _WIN32
        keyQueue.push('\r');
#else
        keyQueue.push('\n');
#endif
    }
    
    // Helper for cin input (for std::cin >> var)
    void setCinInput(const std::string& input) {
        test_cin.str(""); 
        test_cin.clear();
        test_cin << input;
    }
    
    std::string getCapturedOutput() {
        return test_cout.str();
    }
};

TEST_F(UserAuthenticationTest, GetPasswordInput_Normal) {
    pushStringKeys("Secret123");
    pushEnterKey();
    EXPECT_EQ(getPasswordInput(), "Secret123");
}

TEST_F(UserAuthenticationTest, SecureErase_ClearString) {
    std::string secret = "SensitiveData";
    secureErase(secret);
    ASSERT_FALSE(secret.empty()); 
    EXPECT_EQ(secret[0], '\0');
}

TEST_F(UserAuthenticationTest, OpenUserDatabase_Success) {
    const char* dbName = "test_users_open.db";
    remove(dbName);
    sqlite3* db = openUserDatabase(dbName);
    ASSERT_NE(db, nullptr);
    sqlite3_close(db);
    remove(dbName);
}

TEST_F(UserAuthenticationTest, RegisterUser_Success) {
    const char* dbName = "test_reg_success.db";
    remove(dbName); // Cleanup

    // Input flow:
    // 1. Username: "TestUser"
    // 2. Password (via Keys): "Strong!Pass1"
    // 3. AutoLogin: "N"
    
    setCinInput("TestUser\nN\n"); // Username and AutoLogin (std::cin)
    
    // Password input (mock keys)
    pushStringKeys("Strong!Pass1");
    pushEnterKey();

    registerUser(dbName);

    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("Kayit basarili"), std::string::npos);

    // Verify in DB
    sqlite3* db = openUserDatabase(dbName);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT username FROM users WHERE username='TestUser';", -1, &stmt, nullptr);
    ASSERT_EQ(sqlite3_step(stmt), SQLITE_ROW);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    remove(dbName);
}

TEST_F(UserAuthenticationTest, RegisterUser_WeakPasswordLoop) {
    const char* dbName = "test_reg_weak.db";
    remove(dbName);

    // Input: "UserWeak\n" (Username) -> WeakPass -> StrongPass -> "N" (AutoLogin)
    setCinInput("UserWeak\nN\n");

    // 1. Weak Password
    pushStringKeys("weak");
    pushEnterKey();

    // 2. Strong Password
    pushStringKeys("Strong!Pass1");
    pushEnterKey();

    registerUser(dbName);

    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("Lutfen daha guclu bir sifre girin"), std::string::npos); // Error message
    EXPECT_NE(output.find("Kayit basarili"), std::string::npos); // Success eventually

    remove(dbName);
}

TEST_F(UserAuthenticationTest, LoginUser_Success_Manual) {
    const char* dbName = "test_login_success.db";
    remove(dbName);

    // 1. Register first
    setCinInput("LogUser\nN\n");
    pushStringKeys("Strong!Pass1");
    pushEnterKey();
    registerUser(dbName);
    
    // Clear output/input for Login
    test_cout.str("");
    test_cin.str("");
    
    // 2. Login
    // Input: "LogUser" (Username)
    // Keys: "Strong!Pass1" (Password)
    setCinInput("LogUser\n");
    pushStringKeys("Strong!Pass1");
    pushEnterKey();

    bool result = loginUser(dbName);
    
    EXPECT_TRUE(result);
    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("Giris basarili"), std::string::npos);
    EXPECT_NE(output.find("Hosgeldin LogUser"), std::string::npos);

    remove(dbName);
}

TEST_F(UserAuthenticationTest, LoginUser_Failure_WrongPassword) {
    const char* dbName = "test_login_fail_pass.db";
    remove(dbName);

    // Register
    setCinInput("LogUser\nN\n");
    pushStringKeys("Strong!Pass1");
    pushEnterKey();
    registerUser(dbName);

    test_cout.str("");
    test_cin.str("");

    // Login Fail
    setCinInput("LogUser\n");
    pushStringKeys("Wrong!Pass1");
    pushEnterKey();

    bool result = loginUser(dbName);
    EXPECT_FALSE(result);
    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("Gecersiz kullanici adi veya sifre"), std::string::npos);

    remove(dbName);
}

TEST_F(UserAuthenticationTest, LoginUser_Failure_WrongUsername) {
    const char* dbName = "test_login_fail_user.db";
    remove(dbName);

    // Register
    setCinInput("LogUser\nN\n");
    pushStringKeys("Strong!Pass1");
    pushEnterKey();
    registerUser(dbName);

    test_cout.str("");
    test_cin.str("");

    // Login Fail
    setCinInput("UnknownUser\n");
    pushStringKeys("Anything"); // Won't matter much if user not found, but prompt asks
    pushEnterKey();

    bool result = loginUser(dbName);
    EXPECT_FALSE(result);
    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("Gecersiz kullanici adi veya sifre"), std::string::npos);

    remove(dbName);
}

TEST_F(UserAuthenticationTest, DB_Open_Fail_Register) {
    // We already tested openUserDatabase failure separately, 
    // but verifying registerUser handles nullptr gracefully.
    // However, capturing "failure" from void function is hard unless we check side effects.
    // registerUser prints nothing specific on DB fail?
    // Code says: if (!db) return; 
    // It prints error inside openUserDatabase usually.
    
    // Let's use invalid path
    const char* badDb = "///???///"; 
    
    registerUser(badDb);
    // Should safely return.
    SUCCEED();
}

TEST_F(UserAuthenticationTest, DB_Open_Fail_Login) {
    const char* badDb = "///???///"; 
    bool res = loginUser(badDb);
    EXPECT_FALSE(res);
}
