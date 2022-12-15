#include <catch2/catch_test_macros.hpp>
#include "sonority_test_module/TestModuleClass.h"

TEST_CASE("Test Gets Module String") {
    TestModuleClass testModuleClass;
    REQUIRE(testModuleClass.getPrintMe() == "TESTMODULECLASS");
}