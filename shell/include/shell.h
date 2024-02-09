

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
};

struct CommandRespoonse
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
    CommandRespoonse executeCommand(const std::string& command);
    CommandRespoonse whoami();
    CommandRespoonse df(const Command& cmd);
    CommandRespoonse listFiles(const Command& cmd);
    CommandRespoonse generic(const Command& cmd);
    CommandRespoonse changeDirectory(const Command& cmd);
    CommandRespoonse getCurrentDirectory();
    std::string buildCommand(const Command& cmd);
    CommandRespoonse callCommand(const std::string& input);

};

#endif