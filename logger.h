
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Logger
{
public:
    Logger();
    void write_log(std::string log);
    void write_error(std::string error);
    void write_debug(std::string debug);
    void save(std::string& message);
    std::string getCurrentTime();

private:
    std::string log_file;
    std::ofstream file;

};

#endif // LOGGER_H
