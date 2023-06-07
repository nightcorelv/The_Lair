#pragma once
#include <string>

/// <summary>
/// Tries opening a file and looking for a specific error flag (INVALID_HANDLE_VALUE).
/// </summary>
/// <param name="searchName = The name of the file we want to find."></param>
/// <returns></returns> If the flag we were looking for.
bool FileExists(const std::string& searchName)
{
    WIN32_FIND_DATAA tmp = { 0 };
    HANDLE findFile = FindFirstFileA(searchName.c_str(), &tmp);
    bool result = findFile != INVALID_HANDLE_VALUE;
    FindClose(findFile);

    return result;
}

/// <summary>
/// Counts up for every file we find.
/// </summary>
/// <param name="filename = The name of the file we want to find, separated from the file format."></param>
/// <param name="fileFormat = The file format, separated from the name."></param>
/// <param name="index = The index we are looking for."></param>
/// <returns></returns>
int CountFiles(string filename, string fileFormat, int index = 0)
{
    std::string search = "../DataFiles/" + filename;
    search += to_string(index);
    search += fileFormat;

    if (FileExists(search))
    {
        return CountFiles(filename, fileFormat, index + 1);
    }
    else
    {
        return index;
    }
}
