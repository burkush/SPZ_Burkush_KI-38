#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <fstream>
#include <windows.h>

/**
 * @class FileManager
 * @brief The FileManager class provides functions for managing files and directories.
 *
 * The FileManager class contains static functions that can be used to perform various file and directory management
 * operations. It provides functionality for creating, deleting, renaming, and navigating files and directories. It also
 * offers methods for obtaining file information, manipulating file permissions, and interacting with the file system.
 *
 * The class utilizes the Windows API to perform low-level file system operations. It abstracts these operations into convenient functions
 * that can be used in C++ applications to manage files and directories efficiently.
 *
 * Note that the FileManager class assumes a Windows environment due to its reliance on the Windows API. It may not be
 * compatible with other operating systems.
 */
class FileManager {
public:
    /**
     * Value of the current directory.
     */
    static std::string currentDirectory;

    /**
     * Displays the current directory in the console.
     */
    static void displayCurrentDirectory();

    /**
     * Returns the absolute path for the given path.
     *
     * @param path The path to convert to an absolute path.
     * @return The absolute path.
     */
    static std::string getAbsolutePath(const std::string& path);

    /**
     * Navigates to the specified directory.
     *
     * @param directoryPath The path of the directory to navigate to.
     */
    static void navigateDirectory(const std::string& directoryPath);

    /**
     * Creates a new file with the specified name.
     *
     * @param fileName The name of the file to create.
     */
    static void createFile(const std::string& fileName);
    
    /**
     * Creates a new directory with the specified name.
     *
     * @param directoryName The name of the directory to create.
     */
    static void createDirectory(const std::string& directoryName);
    
    /**
     * Renames a file or directory.
     *
     * @param currentName The current name of the file or directory.
     * @param newName The new name for the file or directory.
     */
    static void renameFileOrDirectory(const std::string& currentName, const std::string& newName);
    
    /**
     * Deletes a file or directory.
     *
     * @param name The name of the file or directory to delete.
     */
    static void deleteFileOrDirectory(const std::string& name);
    
    /**
     * Moves a file or directory to a new location.
     *
     * @param source The path of the file or directory to move.
     * @param destination The destination path where the file or directory will be moved.
     */
    static void moveFileOrDirectory(const std::string& source, const std::string& destination);
    
    /**
     * Lists all files and directories in the specified directory.
     *
     * @param directoryPath The path of the directory to list.
     */
    static void listFilesAndDirectories(const std::string& directoryPath);
    
    /**
     * Creates a file that represents the directory structure of the current directory and its subdirectories.
     *
     * @param outputFile The name of the output file to create.
     */
    static void createDirectoryStructureFile(const std::string& outputFile);
    
    /**
     * Recursive helper function to create the directory structure file.
     *
     * @param directoryPath The current directory path.
     * @param outputFile The output file stream to write to.
     * @param indentation The indentation string for subdirectories.
     * @param prefix The prefix string for subdirectories.
     */
    static void createDirectoryStructureFileHelper(const std::string& directoryPath, std::ofstream& outputFile, const std::string& indentation, const std::string& prefix);
    
    /**
     * Sets the permissions for a file or directory.
     *
     * @param name The name of the file or directory.
     * @param permissions The permissions to set.
     */
    static void setFileOrDirectoryPermissions(const std::string& name, const DWORD& permissions);
    
    /**
     * Retrieves the file name from a given file path.
     *
     * @param filePath The file path from which to extract the file name.
     * @return The file name.
     */
    static std::string getFileNameFromPath(const std::string& filePath);
    
    /**
     * Combines two paths into a single path.
     *
     * @param path1 The first path.
     * @param path2 The second path.
     * @return The combined path.
     */
    static std::string combinePaths(const std::string& path1, const std::string& path2);
    
    /**
     * Opens the file explorer at the specified directory path.
     * @param directoryPath The path of the directory to open in the file explorer.
     */
    static void openFileExplorer(const std::string& directoryPath);
};

#endif