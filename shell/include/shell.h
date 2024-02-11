/**
 * @file shell.h
 * @author Benjamin Carter and Trevor Pope
 * @brief The header file for the shell class.
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SHELL_CLASS_H
#define SHELL_CLASS_H

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#ifdef _WIN32
#include <direct.h>
// MSDN recommends against using getcwd & chdir names
#define cwd _getcwd
#define cd _chdir
#else
#include "unistd.h"
#define cwd getcwd
#define cd chdir
#endif


#define MAX_PATH_LENGTH 1024
#define RESPONSE_BUFFER_LENGTH 4096

struct Command 
{
    std::string command;
    std::vector<std::string> options;
    std::vector<std::string> arguments;
    std::string rawCommand;
};

struct CommandResponse
{
    std::string response;
    int returnCode;
};

Command parseCommand(const std::string& input);

class Shell
{
private:
    char buf[RESPONSE_BUFFER_LENGTH];
public:
    /**
     * @brief Execute the command
     * 
     * @param command 
     * @return CommandResponse 
     */
    CommandResponse executeCommand(const std::string& command);

    /**
     * @brief Execute the whoami command
     * 
     * @return CommandResponse 
     */
    CommandResponse whoami();

    /**
     * @brief Execute the df command
     * 
     * @param cmd 
     * @return CommandResponse 
     */
    CommandResponse df(const Command& cmd);

    /**
     * @brief Execute the list files command
     * 
     * @param cmd 
     * @return CommandResponse 
     */
    CommandResponse listFiles(const Command& cmd);

    /**
     * @brief Execute a generic shell command
     * 
     * @param cmd 
     * @return CommandResponse 
     */
    CommandResponse generic(const Command& cmd);

    /**
     * @brief Change directory (cd)
     * 
     * @param cmd 
     * @return CommandResponse 
     */
    CommandResponse changeDirectory(const Command& cmd);

    /**
     * @brief Get the Current Directory (pwd)
     * 
     * @return CommandResponse 
     */
    CommandResponse getCurrentDirectory();

    /**
     * @brief Convert a command with arguments into a string
     * 
     * @param cmd 
     * @return std::string 
     */
    std::string buildCommand(const Command& cmd);

    /**
     * @brief Receive the command, process it, and call the appropiate function.
     * 
     * @param input 
     * @return CommandResponse 
     */
    CommandResponse callCommand(const std::string& input);

};

#endif