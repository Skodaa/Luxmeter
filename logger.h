
#ifndef LOGGER_H
#define LOGGER_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Logger
{
public:
    Logger();
    void write_log(string log);
    void write_error(string error);
    void write_debug(string debug);
    void save(string& message);
    string getCurrentTime();

private:
    string log_file;
    ofstream file;

};

#endif // LOGGER_H
