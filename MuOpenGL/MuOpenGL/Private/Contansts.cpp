#include "Constants.h"

const std::filesystem::path EXECUTABLE_PATH = std::filesystem::current_path();
const std::string SHADER_PATH = "Shaders";
const std::string TEXTURE_PATH = "Resource";
const char* VERTEX_SHADER_FILE = "ProjectionVS.glsl";
const char* FRAGMENT_SHADER_FILE = "TextureFS.glsl";

// normal shader
const char* NORMAL_VERTEX_SHADER_FILE = "NormalVS.glsl";
const char* NORMAL_FRAGMENT_SHADER_FILE = "NormalFS.glsl";

const std::string VERTEX_SHADER_PATH = (EXECUTABLE_PATH / SHADER_PATH / NORMAL_VERTEX_SHADER_FILE).string();
const std::string FRAGMENT_SHADER_PATH = (EXECUTABLE_PATH / SHADER_PATH / NORMAL_FRAGMENT_SHADER_FILE).string();

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;