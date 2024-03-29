/**
 * @file shell.cpp
 * @author Benjamin Carter and Trevor Pope
 * @brief This file contains the source for the shell class.
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "shell.h"

Command parseCommand(const std::string& input) 
{
    Command cmd;

    std::istringstream iss(input);
    cmd.rawCommand = input;
    iss >> cmd.command;

    std::string token;
    while (iss >> token) {
        if (token[0] == '-') {
            // Token is an option
            cmd.options.push_back(token);
        } else {
            // Token is an argument
            cmd.arguments.push_back(token);
        }
    }

    return cmd;
}


CommandResponse Shell::executeCommand(const std::string& command) 
{
    // Use system() function to execute the command
    // return system(command.c_str());

    std::string editedCommand = command + " 2>&1";
    const char* cmd = editedCommand.c_str();
    std::array<char, RESPONSE_BUFFER_LENGTH> buffer;
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (!feof(pipe)) {
        if (fgets(buffer.data(), RESPONSE_BUFFER_LENGTH, pipe) != nullptr) {
            result += buffer.data();
        }
    }
    int status = pclose(pipe)/256;

    CommandResponse cr;
    cr.response = result;
    cr.returnCode = status;
    return cr;
}

CommandResponse Shell::whoami() 
{
    std::string command = "whoami";
    CommandResponse result = Shell::executeCommand(command);
    result.response.erase(std::remove(result.response.begin(), result.response.end(), '\n'), result.response.cend());
    return result;
}

CommandResponse Shell::listFiles(const Command& cmd)
{
    std::string command = "";
    command = Shell::buildCommand(cmd);
    // Execute 'ls' command
    return Shell::executeCommand(command);
}

CommandResponse Shell::df(const Command& cmd)
{
    std::string command = "";
    command = Shell::buildCommand(cmd);
    // Execute 'ls' command
    return Shell::executeCommand(command);
}

CommandResponse Shell::generic(const Command& cmd) 
{
    std::string command = cmd.rawCommand;
    CommandResponse cr = Shell::executeCommand(command);
    if(cr.response.find("not found") < cr.response.size() && cr.response.find("not found") > 0 && cr.response.find("not found") > cr.response.size() - 11)
    {
        std::string old = cr.response;
        cr.response = std::string("Command '") + command + "' could not be found. You can try the following commands: \n"
        " ls\n"
        " df\n"
        " cd\n"
        " whoami\n"
        " pwd\n";
    }
    return cr;
}

CommandResponse Shell::changeDirectory(const Command& cmd) 
{
    std::string result = "";
    for (const auto& arg : cmd.arguments) {
        result += arg;
    }

    cd(result.c_str());
    return Shell::getCurrentDirectory();
}

CommandResponse Shell::getCurrentDirectory() 
{
    char buffer[MAX_PATH_LENGTH];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) 
    {
        CommandResponse cr;
        cr.response = std::string(buffer);
        cr.returnCode = 0;
        return cr;
    } 
    else 
    {
        CommandResponse cr;
        cr.response = "";
        cr.returnCode = -1;
        return cr;
    }
}

std::string Shell::buildCommand(const Command& cmd) 
{
    std::string result = cmd.command;
    for (const auto& opt : cmd.options) {
        result += " " + opt;
    }
    for (const auto& arg : cmd.arguments) {
        result += " " + arg;
    }
    return result;
}

CommandResponse Shell::callCommand(const std::string& input) {
    Command cmd = parseCommand(input);

    CommandResponse output;
    if (cmd.command == "ls") {
        output = Shell::listFiles(cmd);
    } else if (cmd.command == "cd") {
        output = Shell::changeDirectory(cmd);
    } else if (cmd.command == "pwd") {
        output = Shell::getCurrentDirectory();
    } else if (cmd.command == "whoami") {
        output = Shell::whoami();
    } else if (cmd.command == "df") {
        output = Shell::df(cmd);
    } else {
        output = Shell::generic(cmd);
        // output = "Command '" + cmd.command + "' not found";
    }

    return output;
}
