
/**
 * @file fileLoader.cpp
 * @author Benjamin Carter
 * @brief This class opens a file and parses a shell script.
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "fileLoader.h"

FileLoader::FileLoader(const char* filename)
{
    commands = std::vector<std::string>();
    this->filename = filename;
}
int FileLoader::load()
{
    using namespace std;
    commands.clear();

    ifstream input;
    input.open(filename);

    if(input.fail())
    {
        return -2;
    }

    string line;
    while(!input.eof())
    {
        getline(input, line, '\n');
        if(line.size() > 0 && line.at(0) != '#')
        {
            commands.push_back(line);
        }
    }
    input.close();
    return commands.size();
}

std::vector<std::string> FileLoader::getCommands()
{
    return commands;
}