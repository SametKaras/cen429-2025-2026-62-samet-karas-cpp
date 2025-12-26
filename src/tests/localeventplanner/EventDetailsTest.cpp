#include "gtest/gtest.h"
#include "EventDetails.h"
#include "sqlite3.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>

// Helper to check if file exists
bool fileExists(const std::string& name) {
    FILE* file = fopen(name.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

class EventDetailsTest : public ::testing::Test {
protected:
    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream test_cin;
    std::stringstream test_cout;

    void SetUp() override {
        // Redirect cin and cout
        orig_cin = std::cin.rdbuf(test_cin.rdbuf());
        orig_cout = std::cout.rdbuf(test_cout.rdbuf());

        // Ensure clean slate for DB
        remove("events.db");
    }

    void TearDown() override {
        // Restore cin and cout
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);

        // Cleanup DB
        remove("events.db");
    }

    void setInput(const std::string& input) {
        test_cin.str(""); 
        test_cin.clear();
        test_cin << input;
    }

    std::string getOutput() {
        return test_cout.str();
    }
    
    void clearOutput() {
        test_cout.str("");
        test_cout.clear();
    }
};

TEST_F(EventDetailsTest, ListEvents_NoEvents) {
    // Act
    listEvents();

    // Assert
    std::string output = getOutput();
    EXPECT_NE(output.find("Henuz kayitli etkinlik bulunmamaktadir"), std::string::npos);
}

TEST_F(EventDetailsTest, ListEvents_WithEvents) {
    // Arrange: Create an event
    // createEvent calls cin.ignore(), so we need a leading char/newline
    setInput("\nTestEvent\n2025-01-01\nTestLoc\nTestDesc\n");
    createEvent();
    clearOutput();

    // Act
    listEvents();

    // Assert
    std::string output = getOutput();
    EXPECT_NE(output.find("TestEvent"), std::string::npos);
    EXPECT_NE(output.find("TestLoc"), std::string::npos);
    EXPECT_NE(output.find("TestDesc"), std::string::npos);
}

TEST_F(EventDetailsTest, ManageEvents_NoEvents) {
    // Act
    manageEvents();

    // Assert
    std::string output = getOutput();
    EXPECT_NE(output.find("Henuz kayitli etkinlik bulunmamaktadir"), std::string::npos);
}

TEST_F(EventDetailsTest, ManageEvents_Cancel) {
    // Arrange
    setInput("\nEventToCancel\n2025-05-05\nLoc\nDesc\n");
    createEvent();
    clearOutput();

    // Input 0 to cancel (manageEvents asks for ID)
    setInput("0\n");

    // Act
    manageEvents();

    // Assert
    std::string output = getOutput();
    EXPECT_NE(output.find("Islem iptal edildi"), std::string::npos);
}

TEST_F(EventDetailsTest, ManageEvents_UpdateEvent_AllFields) {
    // Arrange
    setInput("\nOldName\n2020-01-01\nOldLoc\nOldDesc\n");
    createEvent();
    clearOutput();

    // Get the ID - assuming it's 1 since we cleared DB
    // manageEvents input flow:
    // 1. ID (1)
    // 2. cin.ignore() inside manageEvents (id input leaves newline usually, but cin >> id skips whitespace)
    //    wait, manageEvents uses `std::cin >> selectedId;` then `std::cin.ignore();` later at line 449.
    //    So input should be: "1\n" -> id=1.
    //    Then line 449 `cin.ignore()` consumes the newline after 1?
    //    Then `std::getline` for new fields.
    
    // Constructing input:
    // "1\n" (ID)
    // "NewName\n"
    // "2025-12-31\n"
    // "NewLoc\n"
    // "NewDesc\n"
    setInput("1\nNewName\n2025-12-31\nNewLoc\nNewDesc\n");

    // Act
    manageEvents();

    // Assert
    std::string output = getOutput();
    EXPECT_NE(output.find("Etkinlik basariyla guncellendi"), std::string::npos);

    // Verify update
    clearOutput();
    listEvents();
    std::string listOut = getOutput();
    EXPECT_NE(listOut.find("NewName"), std::string::npos);
    EXPECT_NE(listOut.find("NewLoc"), std::string::npos);
    EXPECT_EQ(listOut.find("OldName"), std::string::npos);
}

TEST_F(EventDetailsTest, ManageEvents_UpdateEvent_PartialFields) {
    // Arrange
    setInput("\nKeepName\n2020-01-01\nKeepLoc\nToChange\n");
    createEvent();
    clearOutput();

    // Input:
    // 1 (ID)
    // Empty (Name)
    // Empty (Date)
    // Empty (Location)
    // "ChangedDesc" (Description)
    setInput("1\n\n\n\nChangedDesc\n");

    // Act
    manageEvents();

    // Assert
    std::string output = getOutput();
    EXPECT_NE(output.find("Etkinlik basariyla guncellendi"), std::string::npos);

    // Verify
    clearOutput();
    listEvents();
    std::string listOut = getOutput();
    EXPECT_NE(listOut.find("KeepName"), std::string::npos); // Should remain
    EXPECT_NE(listOut.find("ChangedDesc"), std::string::npos); // Should change
    EXPECT_EQ(listOut.find("ToChange"), std::string::npos); // Old one gone
}
