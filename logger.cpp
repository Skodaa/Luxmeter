#include "logger.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>


Logger::Logger()
{
    log_file = "./log/logs.txt";
}

/**
 * @brief Logger::write_log
 * function to write a normal log
 * @param log
 */
void Logger::write_log(std::string log)
{
    std::string time = getCurrentTime();
    std::string message = time + " [LOG] : " + log;
    save(message);
}

/**
 * @brief Logger::write_error
 * function to write an error log
 * @param error
 */
void Logger::write_error(std::string error)
{
    std::string time = getCurrentTime();
    std::string message = time + " [ERR] : " + error;
    save(message);

}

/**
 * @brief Logger::write_debug
 * function to write a debug log
 * @param debug
 */
void Logger::write_debug(std::string debug)
{
    std::string time = getCurrentTime();
    std::string message = time + " [DEBUG] : " + debug;
    save(message);
}

/**
 * @brief Logger::save
 * function to save the logs in a file
 * @param message
 */
void Logger::save(std::string& message)
{
    file.open(log_file,std::ios_base::app);
    file << message << std::endl;
    file.close();

}

/**
 * @brief Logger::getCurrentTime
 * function to get the current time in order to time the logs
 * @return
 */
std::string Logger::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream stream;
    stream << std::put_time(localtime(&current_time),"%Y-%m-%d_%H-%M-%S");
    std::string dateTime = stream.str();

    return dateTime;
}

