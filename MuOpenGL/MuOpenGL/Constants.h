#pragma once
#include <filesystem>
#include <string>

inline constexpr unsigned int SCR_WIDTH = 800;
inline constexpr unsigned int SCR_HEIGHT = 600;

extern const std::filesystem::path EXECUTABLE_PATH;
extern const std::string SHADER_PATH;
extern const std::string TEXTURE_PATH;

extern const char* VERTEX_SHADER_FILE;
extern const char* FRAGMENT_SHADER_FILE;
extern const std::string VERTEX_SHADER_PATH;
extern const std::string FRAGMENT_SHADER_PATH;