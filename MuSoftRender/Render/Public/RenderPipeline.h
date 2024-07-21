#pragma once
#include "FragmentShaderInput.h"
#include "VertexShaderInput.h"

class RenderPipeline
{
public:
    VertexShader VS;
    FragmentShader FS;

    RenderPipeline(VertexShader vs, FragmentShader fs) :
        VS(std::move(vs)), FS(std::move(fs))
    {
    }
};