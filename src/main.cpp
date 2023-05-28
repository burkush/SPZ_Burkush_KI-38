#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include "File_Manager.h"
#include "Colored_Console.h"

// Sets the console font size to the specified size.
void setConsoleFontSize(int size) {
    CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
    fontInfo.dwFontSize.Y = size;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
}

// Displays the heading of the file manager program.
void showHeading() {
    ColoredConsole::setConsoleColor(HEADING_COLOR);
    std::cout << "File Manager [Version 1.0.0]" << std::endl << std::endl;
    ColoredConsole::setConsoleColor(DEFAULT_COLOR);
}

// Displays the available commands and their usage instructions.
void showHelp() {
    std::cout << "\n -----------------------------------------------------------------------------------------" << std::endl;
    std::cout << "|  Available commands:                                                                    |" << std::endl;
    std::cout << "|-----------------------------------------------------------------------------------------|" << std::endl;
    std::cout << "|                                                                                         |" << std::endl;
    std::cout << "|  cd <dir>                            - Navigate to the specified directory              |" << std::endl;
    std::cout << "|  mkdir <dir>                         - Create a new directory                           |" << std::endl;
    std::cout << "|  mkfile <filename>                   - Create a new file                                |" << std::endl;
    std::cout << "|  rename <name> <new_name>            - Rename a file or directory                       |" << std::endl;
    std::cout << "|  delete <file | dir>                 - Delete a file or directory                       |" << std::endl;
    std::cout << "|  move <source> <dest>                - Move a file or directory to a new location       |" << std::endl;
    std::cout << "|  ls                                  - List files and directories                       |" << std::endl;
    std::cout << "|  tree <filename>                     - Create a directory structure file                |" << std::endl;
    std::cout << "|  permit <file | dir> <access>        - Set permissions for a file or directory          |" << std::endl;
    std::cout << "|  help                                - Show the help and available commands             |" << std::endl;
    std::cout << "|  openfe                              - Open File Explorer in the current directory      |" << std::endl;
    std::cout << "|  clear                               - Clear the console                                |" << std::endl;
    std::cout << "|  exit                                - Exit the program                                 |" << std::endl;
    std::cout << " -----------------------------------------------------------------------------------------" << std::endl << std::endl;
}

// Performs initialization tasks for the file manager program.
void init() {
    HWND consoleHandle = GetConsoleWindow();
    SetWindowTextA(consoleHandle, "File Manager");

    setConsoleFontSize(20);
    showHeading();
}

int main() {
    init();

    std::string command;
    std::string argument1;
    std::string argument2;

    std::string currentDirectory = FileManager::currentDirectory;

    while (true) {
        FileManager::displayCurrentDirectory();

        std::getline(std::cin, command);

        std::stringstream ss(command);
        ss >> command >> argument1 >> argument2;

        if (command == "exit") {
            break;
        }
        else if (command == "cd") {
            if (argument1.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::navigateDirectory(FileManager::getAbsolutePath(argument1));
        }
        else if (command == "mkdir") {
            if (argument1.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::createDirectory(argument1);
        }
        else if (command == "mkfile") {
            if (argument1.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::createFile(FileManager::getAbsolutePath(argument1));
        }
        else if (command == "rename") {
            if (argument1.empty() || argument2.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::renameFileOrDirectory(FileManager::getAbsolutePath(argument1), FileManager::getAbsolutePath(argument2));
        }
        else if (command == "delete") {
            if (argument1.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::deleteFileOrDirectory(FileManager::getAbsolutePath(argument1));
        }
        else if (command == "move") {
            if (argument1.empty() || argument2.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::moveFileOrDirectory(FileManager::getAbsolutePath(argument1), FileManager::getAbsolutePath(argument2));
        }
        else if (command == "ls") {
            FileManager::listFilesAndDirectories(currentDirectory);
        }
        else if (command == "tree") {
            if (argument1.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            FileManager::createDirectoryStructureFile(argument1);
        }
        else if (command == "permit") {
            if (argument1.empty() || argument2.empty()) {
                ColoredConsole::setConsoleColor(ERROR_COLOR);
                std::cout << "\nInvalid number of arguments. See \"help\" for usage instructions." << std::endl << std::endl;
                ColoredConsole::setConsoleColor(DEFAULT_COLOR);
                ss.clear();
                continue;
            }
            DWORD permissions = GetFileAttributesA(FileManager::getAbsolutePath(argument1).c_str());
            if (argument2 == "read") {
                permissions &= ~FILE_ATTRIBUTE_DIRECTORY;
                permissions |= FILE_ATTRIBUTE_READONLY;
            }
            else if (argument2 == "write") {
                permissions &= ~FILE_ATTRIBUTE_DIRECTORY;
                permissions &= ~FILE_ATTRIBUTE_READONLY;
            }
            else if (argument2 == "modify") {
                permissions &= ~FILE_ATTRIBUTE_DIRECTORY;
                permissions &= ~FILE_ATTRIBUTE_READONLY;
            }
            FileManager::setFileOrDirectoryPermissions(FileManager::getAbsolutePath(argument1), permissions);
        }
        else if (command == "help") {
            showHelp();
        }
        else if (command == "openfe") {
            FileManager::openFileExplorer(currentDirectory);
        }
        else if (command == "clear") {
            system("cls");
        }
        else {
            ColoredConsole::setConsoleColor(ERROR_COLOR);
            std::cout << "\nInvalid command. Please try again." << std::endl << std::endl;
            ColoredConsole::setConsoleColor(DEFAULT_COLOR);
        }

        ss.str("");
        ss.clear();
        argument1.clear();
        argument2.clear();
    }

    return 0;
}