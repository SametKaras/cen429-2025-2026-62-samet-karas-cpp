# tests/CMakeLists.txt

# Download and include GoogleTest
include(FetchContent)

# OFFLINE 
# ----------------------------------------------------------------------------------
# Include GoogleTest
set(GTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest)

# Add GoogleTest as a subdirectory
add_subdirectory(${GTEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

# Include the Google Test framework
include_directories(${GTEST_DIR}/googletest/include)
# ----------------------------------------------------------------------------------

# ONLINE 
# ----------------------------------------------------------------------------------
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
#   DOWNLOAD_EXTRACT_TIMESTAMP true
# )
#
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
# ----------------------------------------------------------------------------------

# Include the Google Test framework
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# SQLite kaynak dosyasının ve başlık dosyasının yollarını ayarla
set(SQLITE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../utility/src)   # sqlite3.c'nin bulunduğu dizin
set(SQLITE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../utility/header)  # sqlite3.h'nin bulunduğu dizin

# Utility tests
if(ENABLE_UTILITY)
    add_subdirectory(utility)
    target_compile_options(utility_tests PRIVATE /MD)
endif()

# Ayrıca, Google Test ve test projeleri için MDd ayarını yap
    if(TARGET gtest)
        target_compile_options(gtest PRIVATE /MD)
    endif()

    if(TARGET gtest_main)
        target_compile_options(gtest_main PRIVATE /MD)
    endif()

# LocalEventPlanner tests
if(ENABLE_LocalEventPlanner)
    add_subdirectory(localeventplanner)
    # SQLite kaynak dosyasını derleme için ekle
    target_sources(LocalEventPlanner_tests PRIVATE ${SQLITE_SRC_DIR}/sqlite3.c)

    # SQLite başlık dosyasını include edilecek dizinlere ekle
    target_include_directories(LocalEventPlanner_tests PRIVATE ${SQLITE_INCLUDE_DIR})
target_compile_options(LocalEventPlanner_tests PRIVATE /MD)
endif()

