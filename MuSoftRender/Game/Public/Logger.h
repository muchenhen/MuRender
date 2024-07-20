#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include <sstream>

class Logger
{
public:
    enum class ELogLevel
    {
        Debug,
        Info,
        Warning,
        Error
    };

private:
    Logger() :
        LogLevel(ELogLevel::Info)
    {
    }

    ~Logger()
    {
        if (LogFile.is_open())
        {
            LogFile.close();
        }
    }


    template <typename T>
    void LogPart(std::stringstream& Ss, T&& Arg)
    {
        Ss << std::forward<T>(Arg);
    }

    template <typename T, typename... Args>
    void LogPart(std::stringstream& Ss, T&& Arg, Args&&... InArgs)
    {
        LogPart(Ss, std::forward<T>(Arg));
        LogPart(Ss, std::forward<Args>(InArgs)...);
    }

    template <typename... Args>
    void Log(ELogLevel InLevel, Args&&... InArgs)
    {
        if (InLevel < LogLevel) return;

        std::stringstream ss;
        switch (InLevel)
        {
            case ELogLevel::Debug: ss << "[DEBUG] ";
                break;
            case ELogLevel::Info: ss << "[INFO] ";
                break;
            case ELogLevel::Warning: ss << "[WARNING] ";
                break;
            case ELogLevel::Error: ss << "[ERROR] ";
                break;
        }

        LogPart(ss, std::forward<Args>(InArgs)...);
        ss << "\n";

        std::lock_guard<std::mutex> Lock(Mutex);
        std::cout << ss.str();
        if (LogFile.is_open())
        {
            LogFile << ss.str();
            LogFile.flush();
        }
    }

    ELogLevel LogLevel;
    std::ofstream LogFile;
    std::mutex Mutex;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;


    static Logger& GetInstance()
    {
        static Logger Instance;
        return Instance;
    }

    void SetLogLevel(ELogLevel InLevel)
    {
        LogLevel = InLevel;
    }

    void SetLogFile(const std::string& InFilename)
    {
        if (std::ifstream{InFilename})
        {
            if (std::remove(InFilename.c_str()))
            {
                return;
            }
        }
        LogFile.open(InFilename, std::ios::out | std::ios::app);
    }

    template <typename... Args>
    void Debug(Args... args)
    {
        Log(ELogLevel::Debug, args...);
    }

    template <typename... Args>
    void Info(Args... args)
    {
        Log(ELogLevel::Info, args...);
    }

    template <typename... Args>
    void Warning(Args... args)
    {
        Log(ELogLevel::Warning, args...);
    }

    template <typename... Args>
    void Error(Args... args)
    {
        Log(ELogLevel::Error, args...);
    }
};

#define LOG_DEBUG(...) Logger::GetInstance().Debug(__VA_ARGS__)
#define LOG_INFO(...) Logger::GetInstance().Info(__VA_ARGS__)
#define LOG_WARNING(...) Logger::GetInstance().Warning(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance().Error(__VA_ARGS__)