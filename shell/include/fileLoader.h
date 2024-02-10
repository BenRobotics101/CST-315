
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
    FileLoader(const char* filename);
    int load();
    std::vector<std::string> getCommands();

};


#endif