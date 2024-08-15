#include "Constants.h"

const std::filesystem::path EXECUTABLE_PATH = std::filesystem::current_path();
const std::string SHADER_PATH = "Shaders";
const std::string TEXTURE_PATH = "Resource";
const char* VERTEX_SHADER_FILE = "ProjectionVS.glsl";
const char* FRAGMENT_SHADER_FILE = "TextureFS.glsl";
const std::string VERTEX_SHADER_PATH = (EXECUTABLE_PATH / SHADER_PATH / VERTEX_SHADER_FILE).string();
const std::string FRAGMENT_SHADER_PATH = (EXECUTABLE_PATH / SHADER_PATH / FRAGMENT_SHADER_FILE).string();