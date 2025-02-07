#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdarg>
#include <sstream>

namespace mb {
namespace log {

LogMode Logger::m_log_mode = CONSOLE_AND_FILE;
Logger *Logger::m_instance = nullptr;

Logger* Logger::Instance() {
    if (!m_instance) {
        m_instance = new Logger();
        m_instance->setWriteFlags();
    }
    return m_instance;
}

Logger* Logger::Instance(std::string& filename) {
    if (!m_instance) { 
        m_instance = new Logger(filename);
        m_instance->setWriteFlags();
    }
    else m_instance->setLogFile(filename);
    return m_instance;
}

void Logger::setLogMode(const std::string &log_mode) { 
    m_log_mode = stringToLogMode(log_mode);
    Logger::Instance()->setWriteFlags();
}

Logger::Logger() : m_active(false) {
    m_log_file.open(MODBUS_LOG_DEFAULT_FILENAME, std::ios::app);
    if (!m_log_file.is_open()) std::cerr << "Error open log file: " << MODBUS_LOG_DEFAULT_FILENAME << std::endl;
}

Logger::Logger(std::string& filename) : m_active(false) {
    m_log_file.open(filename, std::ios::app);
    if (!m_log_file.is_open()) std::cerr << "Error open log file: " << filename << std::endl;
}

Logger::~Logger() {
    if (m_log_file.is_open()) m_log_file.close(); 
}

bool Logger::setActive(bool mode) {
    std::lock_guard<std::mutex> lock(m_mtx); 
    m_active = mode; 
}

bool Logger::setLogFile(std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mtx);
    bool prev_active = m_active;

    if (m_log_file.is_open()) {
        m_active = false; 
        m_log_file.close();
    } 
    m_log_file.open(filename, std::ios::app);
    if (!m_log_file.is_open()) {
        std::cerr << "Error open log file: " << filename << std::endl;
        return false;
    }
    m_active = prev_active;
    return true;
}

void Logger::rawLog(const std::string &message) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || message[0] == '\0' || message[0] == '\n') return;

    std::ostringstream oss;

    oss << message;
    doLog(oss);
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || message[0] == '\0' || message[0] == '\n') return;

    std::ostringstream oss;
    setCurrentTime();
    oss << m_current_time << logLevelToString(level) << message;

    doLog(oss);
}

void Logger::rawLog(const char* format, ...) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || *format == '\0' || *format == '\n') return;

    std::ostringstream oss;
    va_list args;
    va_start(args, format);

    const char* p = format;
    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'd': {
                    int i = va_arg(args, int);
                    oss << i;
                    break;
                }
                case 'f': {
                    double d = va_arg(args, double);
                    oss << d;
                    break;
                }
                case 'c': {
                    int c = va_arg(args, int);
                    oss << static_cast<char>(c);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    oss << s;
                    break;
                }
                case '%':
                    oss << '%';
                    break;
                default:
                    oss << '%' << *p;
            }
        } else {
            oss << *p;
        }
        p++;
    }

    va_end(args);

    doLog(oss);
}

void Logger::log(LogLevel level, const char* format, ...) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || *format == '\0' || *format == '\n') return;

    std::ostringstream oss;
    va_list args;
    va_start(args, format);

    setCurrentTime();
    oss << m_current_time << logLevelToString(level);

    const char* p = format;
    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'd': {
                    int i = va_arg(args, int);
                    oss << i;
                    break;
                }
                case 'f': {
                    double d = va_arg(args, double);
                    oss << d;
                    break;
                }
                case 'c': {
                    int c = va_arg(args, int);
                    oss << static_cast<char>(c);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    oss << s;
                    break;
                }
                case '%':
                    oss << '%';
                    break;
                default:
                    oss << '%' << *p;
            }
        } else {
            oss << *p;
        }
        p++;
    }

    va_end(args);

    doLog(oss);
}

void Logger::setCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::strftime(m_current_time, sizeof(m_current_time), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
}

void Logger::setWriteFlags() {
    if (m_log_mode == CONSOLE) {
        m_write_console = true;
        m_write_file = false;
    } 
    else if (m_log_mode == FILE) {
        m_write_console = false;
        m_write_file = true;
    }
    else if (m_log_mode == CONSOLE_AND_FILE) {
        m_write_console = true;
        m_write_file = true;
    }
}

void Logger::doLog(std::ostringstream& oss) {
    if (m_write_console) std::cout << oss.str() << std::endl;
    if (m_write_file && m_log_file.is_open()) m_log_file << oss.str() << std::endl;
}

LogMode stringToLogMode(const std::string& log_mode) {
    LogMode result = CONSOLE_AND_FILE;
    if (log_mode == "CONSOLE") result = CONSOLE;
    else if (log_mode == "CONSOLE_AND_FILE") result = CONSOLE_AND_FILE;
    else if (log_mode == "FILE") result = FILE;
    return result;
}

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case INFO: return " [INFO] ";
        case WARN: return " [WARN] ";
        case ERROR: return " [ERROR] ";
        case MESSAGE: return " ";
        case TX: return " [TX] ";
        case RX: return " [RX] ";
        default: return " [INFO] ";
    }
}

}
}