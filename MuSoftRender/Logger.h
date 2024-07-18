#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include <sstream>

class Logger
{
public:
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& GetInstance()
    {
        static Logger instance;
        return instance;
    }

    void SetLogLevel(LogLevel level)
    {
        m_logLevel = level;
    }

    void SetLogFile(const std::string& filename)
    {
        // 如果filename的文件已经存在，先删除文件
        if (std::ifstream{filename})
        {
            std::remove(filename.c_str());
        }
        m_logFile.open(filename, std::ios::out | std::ios::app);
    }

    template <typename... Args>
    void Debug(Args... args)
    {
        Log(LogLevel::DEBUG, args...);
    }

    template <typename... Args>
    void Info(Args... args)
    {
        Log(LogLevel::INFO, args...);
    }

    template <typename... Args>
    void Warning(Args... args)
    {
        Log(LogLevel::WARNING, args...);
    }

    template <typename... Args>
    void Error(Args... args)
    {
        Log(LogLevel::ERROR, args...);
    }

private:
    Logger() :
        m_logLevel(LogLevel::INFO)
    {
    }
    ~Logger()
    {
        if (m_logFile.is_open())
        {
            m_logFile.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template <typename T>
    void LogPart(std::stringstream& ss, T&& arg)
    {
        ss << std::forward<T>(arg);
    }

    template <typename T, typename... Args>
    void LogPart(std::stringstream& ss, T&& arg, Args&&... args)
    {
        LogPart(ss, std::forward<T>(arg));
        LogPart(ss, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Log(LogLevel level, Args&&... args)
    {
        if (level < m_logLevel) return;

        std::stringstream ss;
        switch (level)
        {
            case LogLevel::DEBUG: ss << "[DEBUG] "; break;
            case LogLevel::INFO: ss << "[INFO] "; break;
            case LogLevel::WARNING: ss << "[WARNING] "; break;
            case LogLevel::ERROR: ss << "[ERROR] "; break;
        }

        LogPart(ss, std::forward<Args>(args)...);
        ss << std::endl;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << ss.str();
        if (m_logFile.is_open())
        {
            m_logFile << ss.str();
            m_logFile.flush();
        }
    }

    LogLevel m_logLevel;
    std::ofstream m_logFile;
    std::mutex m_mutex;
};

// 方便使用的宏
#define LOG_DEBUG(...) Logger::GetInstance().Debug(__VA_ARGS__)
#define LOG_INFO(...) Logger::GetInstance().Info(__VA_ARGS__)
#define LOG_WARNING(...) Logger::GetInstance().Warning(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance().Error(__VA_ARGS__)