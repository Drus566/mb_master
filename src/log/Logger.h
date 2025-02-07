/* */

#ifndef LOGGER_H
#define LOGGER_H

#define MODBUS_LOG_DEFAULT_FILENAME "ModbusLog.txt"

#define LOG_TYPE_NONE "NONE"
#define LOG_TYPE_ALWAYS "ALWAYS"
#define LOG_TYPE_SESSION "SESSION"

#include <fstream>
#include <string>
#include <mutex>

namespace mb {
namespace log {

enum LogMode {
    CONSOLE,
    FILE,
    CONSOLE_AND_FILE
};

LogMode stringToLogMode(const std::string& log_mode);

enum LogLevel {
    INFO,
    WARN,
    ERROR,
    TX,
    RX,
    MESSAGE
};

std::string logLevelToString(LogLevel level);


class Logger {
public:    
    static Logger* Instance();
    static Logger* Instance(std::string& filename);
    static void setLogMode(const std::string &log_mode);

    bool setLogFile(std::string& filename);
    void rawLog(const std::string &message);
    void log(LogLevel level, const std::string& message);
    void rawLog(const char *format, ...);
    void log(LogLevel level, const char* format, ...);
    bool setActive(bool mode);

private:
    Logger();
    Logger(std::string& filename);

    ~Logger();
    static Logger* m_instance;
    static LogMode m_log_mode;

    std::ofstream m_log_file;
    std::mutex m_mtx;
    char m_current_time[30];
    bool m_active;

    bool m_write_file;
    bool m_write_console;

    void setCurrentTime();
    void setWriteFlags();
    void doLog(std::ostringstream& oss);
};

} // mb
} // log

#endif // LOGGER_H 