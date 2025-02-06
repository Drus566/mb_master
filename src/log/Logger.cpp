#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdarg>
#include <sstream>

namespace mb {
namespace log {

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

Logger* Logger::m_instance = nullptr;

Logger* Logger::Instance() {
    if (!m_instance) m_instance = new Logger();
    return m_instance;
}

Logger* Logger::Instance(std::string& filename) {
    if (!m_instance) m_instance = new Logger(filename);
    else { m_instance->setLogFile(filename); }
    return m_instance;
}

Logger::Logger() {
    m_log_file.open(MODBUS_LOG_DEFAULT_FILENAME, std::ios::app);
    if (!m_log_file.is_open()) std::cerr << "Error open log file: " << MODBUS_LOG_DEFAULT_FILENAME << std::endl;
    m_active = false;
}

Logger::Logger(std::string& filename) {
    m_log_file.open(filename, std::ios::app);
    if (!m_log_file.is_open()) std::cerr << "Error open log file: " << filename << std::endl;
    m_active = false;
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
    if (!m_active || !m_log_file.is_open())
        return;

    setCurrentTime();
    m_log_file << message;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || !m_log_file.is_open()) return;

    if (message[0] == '\0' || message[0] == '\n') return;

    setCurrentTime();
    m_log_file << m_current_time << logLevelToString(level) << message;
}

void Logger::rawLog(const char* format, ...) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || !m_log_file.is_open()) return;

    if (*format == '\0' || *format == '\n') return;

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

    setCurrentTime();
    m_log_file << oss.str() << std::endl;
}

void Logger::log(LogLevel level, const char* format, ...) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_active || !m_log_file.is_open()) return;

    if (*format == '\0' || *format == '\n') return;

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

    setCurrentTime();
    m_log_file << m_current_time << logLevelToString(level) << oss.str() << std::endl;
}


void Logger::setCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::strftime(m_current_time, sizeof(m_current_time), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
}

}
}