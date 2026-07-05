
#include "unit_test.h"

#include <fstream>
#include <locale>
#include <sstream>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#include <filesystem>
using namespace std::filesystem;

using namespace UnitTestSupport;


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Returns path of the current executable
//-----------------------------------------------------------------------------
std::string CUnitTest::getExecutablePath()
{
#ifdef _WIN32
    std::string localPath;
    localPath.resize(MAX_PATH + 1);
    GetModuleFileNameA(
        nullptr,
        &localPath[0],
        MAX_PATH
    );
    auto n = localPath.find_last_of('\\');
    if (n != std::string::npos) localPath.resize(n);
    return localPath;
#else
    return current_path().string();
#endif
}


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Returns contents of a text file
//-----------------------------------------------------------------------------
std::string CUnitTest::getTextFileData(const std::string& fileName)
{
    std::ifstream file(fileName);

    if (!file.is_open())
        return std::string();

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Returns contents of a text file in a vector with each item representing a line
//-----------------------------------------------------------------------------
std::vector<std::string> CUnitTest::getTextFileList(const std::string& fileName)
{
    std::vector<std::string> results;

    std::ifstream file(fileName);

    if (!file.is_open())
        return results;

    std::string line;
    while (std::getline(file, line))
    {
        results.push_back(line);
    }
    return results;
}


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Logs a text string to the debug console
//-----------------------------------------------------------------------------
void CUnitTest::logText(const std::string& text)
{
    std::cout << text << std::endl;
}


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Returns a validated directory which can be used for temporary files
//-----------------------------------------------------------------------------
std::string CUnitTest::getTestPath(const std::string& base)
{
    std::string localPath = getExecutablePath() + "\\" + base;
    setupFolder(localPath);
    return localPath;
}


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Cleans and recreates a directory
//-----------------------------------------------------------------------------
bool CUnitTest::setupFolder(const std::string& path)
{
    // clear export folder and recreate
    if (!cleanupFolder(path))
        return false;

    try
    {
        create_directories(path);
        return true;
    }
    catch (std::exception& ex)
    {
        // cannot remove
        std::cout << "CUnitTest::setupFolder(): Failed to create directory " << path << ". Error: " << ex.what() << std::endl;
        return false;
    }
}


//-----------------------------------------------------------------------------
// [TestSupport]
//
// Deletes directory contents and the directory itself
//-----------------------------------------------------------------------------
bool CUnitTest::cleanupFolder(const std::string& path)
{
    if (!exists(path))
        return true;

    try
    {
        remove_all(path);
        return true;
    }
    catch (std::exception& ex)
    {
        // cannot remove
        std::cout << "CUnitTest::cleanupFolder(): Failed to remove directory " << path << ". Error: " << ex.what() << std::endl;
        return false;
    }
}
