#pragma once
#include <iostream>
#include <string>
#include <memory>

enum class LogLevel
{
    Info,
    Warn,
    Error
};

class MuLog
{
public:
    template<typename ...Args>
    static void LogInfo(const char* Format, Args&&... args)
    {
        Log(LogLevel::Info, Format, std::forward<Args>(args)...);
    }

    template<typename ...Args>
    static void LogWarn(const char* Format, Args&&... args)
    {
        Log(LogLevel::Warn, Format, std::forward<Args>(args)...);
    }

    template<typename ...Args>
    static void LogError(const char* Format, Args&&... args)
    {
        Log(LogLevel::Error, Format, std::forward<Args>(args)...);
    }

private:
    template<typename ...Args>
    static void Log(LogLevel Level, const char* Format, Args&&... args)
    {
        static const std::string LEVEL_NAMES[] = { "Info", "Warn", "Error" };
        static const std::string LEVEL_COLORS[] = { "\033[0;32m", "\033[0;33m", "\033[0;31m" };// 绿色，黄色，红色
        
        const std::string& LevelName = LEVEL_NAMES[static_cast<int>(Level)];
        const std::string& LevelColor = LEVEL_COLORS[static_cast<int>(Level)];
        
        std::cout << LevelColor << LevelName << ": ";
        printf(Format, std::forward<Args>(args)...);
        std::cout << std::endl;
    }
};
