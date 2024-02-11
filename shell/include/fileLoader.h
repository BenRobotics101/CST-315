/**
 * @file fileLoader.h
 * @author Benjamin Carter
 * @brief This class reads a shell scripting file and returns commands.
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <vector>
#include <fstream>

class FileLoader
{
private:
    std::vector<std::string> commands;
    const char* filename;
public:
    /**
     * @brief Construct a new File Loader object
     * 
     * @param filename 
     */
    FileLoader(const char* filename);
    
    /**
     * @brief Process file
     * 
     * @return int. 0 if success.
     */
    int load();

    /**
     * @brief Return the commands in a vector of strings.
     * 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getCommands();

};


#endif