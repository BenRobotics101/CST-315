
#define DO_NOT_BUILD_ME_SHELL
#ifndef DO_NOT_BUILD_ME_SHELL

#include "shell.h"



Command parseCommand(const std::string& input) {
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

class Shell {
private:
    char buf[4096];
public:
    std::string executeCommand(const std::string& command) {
        // Use system() function to execute the command
        // return system(command.c_str());

        const char* cmd = command.c_str();
        std::array<char, 1024> buffer;
        std::string result;
        FILE* pipe = popen(cmd, "r");
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (!feof(pipe)) {
            if (fgets(buffer.data(), 1024, pipe) != nullptr) {
                result += buffer.data();
            }
        }
        pclose(pipe);
        return result;
    }

    std::string whoami() {
        std::string command = "whoami";
        std::string result = executeCommand(command);
        result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());
        return result;
    }

    std::string listFiles(const Command& cmd) {
        std::string command = "";
        command = buildCommand(cmd);
        // Execute 'ls' command
        return executeCommand(command);
    }

    std::string df(const Command& cmd) {
        std::string command = "";
        command = buildCommand(cmd);
        // Execute 'ls' command
        return executeCommand(command);
    }

    std::string generic(const Command& cmd) {
        std::string command = "";
        command = buildCommand(cmd);
        // Execute 'ls' command
        return executeCommand(command);
    }

    std::string changeDirectory(const Command& cmd) {
        std::string result = "";
        for (const auto& arg : cmd.arguments) {
            result += arg;
        }

        cd(result.c_str());
        return getCurrentDirectory();
    }

    std::string getCurrentDirectory() {
        char buffer[MAX_PATH_LENGTH];
        if (getcwd(buffer, sizeof(buffer)) != nullptr) {
            return std::string(buffer);
        } else {
            return "";
        }
    }

    std::string buildCommand(const Command& cmd) {
        std::string result = cmd.command;
        for (const auto& opt : cmd.options) {
            result += " " + opt;
        }
        for (const auto& arg : cmd.arguments) {
            result += " " + arg;
        }
    return result;
}

    void callCommand(const std::string& input) {
        Command cmd = parseCommand(input);

        std::string output;
        if (cmd.command == "ls") {
            output = listFiles(cmd);
        } else if (cmd.command == "cd") {
            changeDirectory(cmd);
            output = "";
        } else if (cmd.command == "pwd") {
            output = getCurrentDirectory();
        } else if (cmd.command == "whoami") {
            output = whoami();
        } else if (cmd.command == "df") {
            output = df(cmd);
        } else {
            output = generic(cmd);
            // output = "Command '" + cmd.command + "' not found";
        }

        std::cout << output << "\n";
    }
};

int main() {
    system("clear");
    Shell shell;
    std::string input;

    // Bash file input
    system("./src/bash.sh");

    // Get the input from the console
    while(1) {
        std::cout << "\033[38;5;115m" << shell.whoami() << ":" << "\033[38;5;81m" << shell.getCurrentDirectory() << "\033[0m" << "$ ";
        std::getline(std::cin, input);

        shell.callCommand(input);
    }
    return 0;
}

#endif