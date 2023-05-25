#include "logger.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>


using namespace std;

Logger::Logger()
{
    log_file = "./log/logs.txt";
}

void Logger::write_log(string log)
{
    string time = getCurrentTime();
    string message = time + " [LOG] : " + log;
    save(message);
}

void Logger::write_error(string error)
{
    string time = getCurrentTime();
    string message = time + " [ERR] : " + error;
    save(message);

}

void Logger::write_debug(string debug)
{
    string time = getCurrentTime();
    string message = time + " [DEBUG] : " + debug;
    save(message);
}

void Logger::save(string& message)
{
    file.open(log_file,ios_base::app);
    file << message << endl;
    file.close();

}

string Logger::getCurrentTime()
{
    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);

    stringstream stream;
    stream << put_time(localtime(&current_time),"%Y-%m-%d_%H-%M-%S");
    string dateTime = stream.str();

    return dateTime;
}

