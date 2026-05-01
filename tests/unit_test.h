#pragma once
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace UnitTestSupport
{
    class CUnitTest : public testing::Test
    {
    protected:

        // test support functions
        static std::string getExecutablePath();

        static std::string getTextFileData(const std::string& fileName);

        static std::vector<std::string> getTextFileList(const std::string& fileName);

        static void logText(const std::string& text);

        static std::string getTestPath(const std::string& base);

        static bool setupFolder(const std::string& path);
        static bool cleanupFolder(const std::string& path);

    };
}
