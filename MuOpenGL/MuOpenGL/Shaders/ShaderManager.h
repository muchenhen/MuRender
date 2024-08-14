﻿#pragma once
#include <xstring>

class ShaderManager
{
public:
    static std::string LoadShaderSource(const char* file);

    static unsigned int BuildShader(int type);

    static unsigned int BuildShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
};
