#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include "File_Manager.h"
#include "Colored_Console.h"

std::string FileManager::currentDirectory;

void FileManager::displayCurrentDirectory() {
    char currentDir[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, currentDir) > 0) {
        currentDirectory = currentDir;
        ColoredConsole::setConsoleColor(PATH_COLOR);
        std::cout << currentDir << "> ";
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    } else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to retrieve current directory." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

std::string FileManager::getAbsolutePath(const std::string& path) {
    if (path.empty() || path[0] == '.') {
        return currentDirectory + "\\" + path;
    }

    return path;
}

void FileManager::navigateDirectory(const std::string& directoryPath) {
    if (!SetCurrentDirectoryA(directoryPath.c_str())) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to change directory to: " << directoryPath << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

void FileManager::createFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (file) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to create file. The file with the same name already exists." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);

        return;
    }

    HANDLE hFile = CreateFileA(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        ColoredConsole::setConsoleColor(SUCCESS_COLOR);
        std::cout << "\nCreated file: " << fileName << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        CloseHandle(hFile);
    } else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to create file: " << fileName << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

void FileManager::createDirectory(const std::string& directoryName) {
    std::string absolutePath = getAbsolutePath(directoryName);
    DWORD fileAttributes = GetFileAttributesA(absolutePath.c_str());

    if (fileAttributes != INVALID_FILE_ATTRIBUTES && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to create directory. The directory with the same name already exists." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        return;
    }

    if (CreateDirectoryA(absolutePath.c_str(), NULL)) {
        ColoredConsole::setConsoleColor(SUCCESS_COLOR);
        std::cout << "\nCreated directory: " << absolutePath << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    } else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to create directory: " << absolutePath << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

void FileManager::renameFileOrDirectory(const std::string& currentName, const std::string& newName) {
    DWORD currentAttributes = GetFileAttributesA(currentName.c_str());
    if (currentAttributes == INVALID_FILE_ATTRIBUTES) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nThe file or directory '" << currentName << "' does not exist." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        return;
    }

    DWORD newAttributes = GetFileAttributesA(newName.c_str());
    if (newAttributes != INVALID_FILE_ATTRIBUTES) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to rename. The file or directory with the name '" << newName << "' already exists." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        return;
    }

    if (currentAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (MoveFileExA(currentName.c_str(), newName.c_str(), MOVEFILE_REPLACE_EXISTING)) {
            ColoredConsole::setConsoleColor(SUCCESS_COLOR);
            std::cout << "\nRenamed directory '" << currentName << "' to '" << newName << "'" << std::endl << std::endl;
            ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        }
        else {
            ColoredConsole::setConsoleColor(ERROR_COLOR);
            std::cout << "\nFailed to rename directory '" << currentName << "' to '" << newName << "'" << std::endl << std::endl;
            ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        }
    }
    else {
        if (MoveFileA(currentName.c_str(), newName.c_str())) {
            ColoredConsole::setConsoleColor(SUCCESS_COLOR);
            std::cout << "\nRenamed file '" << currentName << "' to '" << newName << "'" << std::endl << std::endl;
            ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        }
        else {
            ColoredConsole::setConsoleColor(ERROR_COLOR);
            std::cout << "\nFailed to rename file '" << currentName << "' to '" << newName << "'" << std::endl << std::endl;
            ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        }
    }
}

void FileManager::deleteFileOrDirectory(const std::string& name) {
    if (DeleteFileA(name.c_str())) {
        ColoredConsole::setConsoleColor(SUCCESS_COLOR);
        std::cout << "\nDeleted file: " << name << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    } else if (RemoveDirectoryA(name.c_str())) {
        ColoredConsole::setConsoleColor(SUCCESS_COLOR);
        std::cout << "\nDeleted directory: " << name << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    } else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to delete file or directory: " << name << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

void FileManager::moveFileOrDirectory(const std::string& source, const std::string& destination) {
    DWORD destinationAttributes = GetFileAttributesA(destination.c_str());
    if (destinationAttributes == INVALID_FILE_ATTRIBUTES || !(destinationAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to move " << source << " to " << destination << ". The destination directory does not exist." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        return;
    }

    std::string sourceFileName = FileManager::getFileNameFromPath(source);
    std::string fullDestinationPath = FileManager::combinePaths(destination, sourceFileName);

    if (MoveFileA(source.c_str(), fullDestinationPath.c_str())) {
        ColoredConsole::setConsoleColor(SUCCESS_COLOR);
        std::cout << "\nMoved " << source << " to " << fullDestinationPath << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
    else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to move " << source << " to " << fullDestinationPath << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

std::string FileManager::getFileNameFromPath(const std::string& filePath) {
    std::size_t found = filePath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filePath.substr(found + 1);
    }
    return filePath;
}

std::string FileManager::combinePaths(const std::string& path1, const std::string& path2) {
    if (path1.empty()) {
        return path2;
    }
    if (path2.empty()) {
        return path1;
    }
    char lastChar = path1.back();
    if (lastChar != '/' && lastChar != '\\') {
        return path1 + '\\' + path2;
    }
    return path1 + path2;
}

void FileManager::listFilesAndDirectories(const std::string& directoryPath) {
    std::string absolutePath = getAbsolutePath(directoryPath);
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((absolutePath + "\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        std::cout << std::endl;

        do {
            std::cout << findData.cFileName << ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? " [DIR]" : "") << std::endl;
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
        std::cout << std::endl;
    } else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to list files and directories in " << absolutePath << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

void FileManager::createDirectoryStructureFile(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to create directory structure file." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        return;
    }

    createDirectoryStructureFileHelper(currentDirectory, file, "", "");

    file.close();

    ColoredConsole::setConsoleColor(SUCCESS_COLOR);
    std::cout << "\nDirectory structure file created successfully." << std::endl << std::endl;
    ColoredConsole::setConsoleColor(DEFAULT_COLOR);
}

void FileManager::createDirectoryStructureFileHelper(const std::string& directoryPath, std::ofstream& outputFile, const std::string& indentation, const std::string& prefix) {
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((directoryPath + "\\*").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string itemName = findData.cFileName;
            if (itemName != "." && itemName != "..") {
                std::string itemPath = directoryPath + "\\" + itemName;
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    outputFile << indentation << prefix << itemName << std::endl;
                    createDirectoryStructureFileHelper(itemPath, outputFile, indentation + "  ", "|-- ");
                }
                else {
                    outputFile << indentation << prefix << itemName << std::endl;
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
}

void FileManager::setFileOrDirectoryPermissions(const std::string& name, const DWORD& permissions) {
    if (SetFileAttributesA(name.c_str(), permissions)) {
        ColoredConsole::setConsoleColor(SUCCESS_COLOR);
        std::cout << "\nPermissions set successfully for " << name << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
    else {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to set permissions for " << name << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}

void FileManager::openFileExplorer(const std::string& directoryPath) {
    int wideCharLen = MultiByteToWideChar(CP_UTF8, 0, directoryPath.c_str(), -1, nullptr, 0);
    std::wstring wideDirectoryPath(wideCharLen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, directoryPath.c_str(), -1, &wideDirectoryPath[0], wideCharLen);

    SHELLEXECUTEINFO shellInfo;
    ZeroMemory(&shellInfo, sizeof(shellInfo));
    shellInfo.cbSize = sizeof(shellInfo);
    shellInfo.lpVerb = L"open";
    shellInfo.lpFile = wideDirectoryPath.c_str();
    shellInfo.nShow = SW_SHOWDEFAULT;

    if (ShellExecuteEx(&shellInfo) == FALSE) {
        ColoredConsole::setConsoleColor(ERROR_COLOR);
        std::cout << "\nFailed to open File Explorer." << std::endl << std::endl;
        ColoredConsole::setConsoleColor(DEFAULT_COLOR);
    }
}