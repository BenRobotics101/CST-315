

#include "shell.h"



Command parseCommand(const std::string& input) 
{
    Command cmd;

    std::istringstream iss(input);
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


CommandRespoonse Shell::executeCommand(const std::string& command) 
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

    CommandRespoonse cr;
    cr.response = result;
    cr.returnCode = status;
    return cr;
}

CommandRespoonse Shell::whoami() 
{
    std::string command = "whoami";
    CommandRespoonse result = Shell::executeCommand(command);
    result.response.erase(std::remove(result.response.begin(), result.response.end(), '\n'), result.response.cend());
    return result;
}

CommandRespoonse Shell::listFiles(const Command& cmd)
{
    std::string command = "";
    command = Shell::buildCommand(cmd);
    // Execute 'ls' command
    return Shell::executeCommand(command);
}

CommandRespoonse Shell::df(const Command& cmd)
{
    std::string command = "";
    command = Shell::buildCommand(cmd);
    // Execute 'ls' command
    return Shell::executeCommand(command);
}

CommandRespoonse Shell::generic(const Command& cmd) 
{
    std::string command = "";
    command = Shell::buildCommand(cmd);
    return Shell::executeCommand(command);
}

CommandRespoonse Shell::changeDirectory(const Command& cmd) 
{
    std::string result = "";
    for (const auto& arg : cmd.arguments) {
        result += arg;
    }

    cd(result.c_str());
    return Shell::getCurrentDirectory();
}

CommandRespoonse Shell::getCurrentDirectory() 
{
    char buffer[MAX_PATH_LENGTH];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) 
    {
        CommandRespoonse cr;
        cr.response = std::string(buffer);
        cr.returnCode = 0;
        return cr;
    } 
    else 
    {
        CommandRespoonse cr;
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

CommandRespoonse Shell::callCommand(const std::string& input) {
    Command cmd = parseCommand(input);

    CommandRespoonse output;
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
