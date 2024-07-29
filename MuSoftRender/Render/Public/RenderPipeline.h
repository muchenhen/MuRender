#pragma once
#include "FragmentShader.h"
#include "VertexShader.h"

class RenderPipeline
{
public:
    StandardVertexShader VS;
    FragmentShader FS;

    RenderPipeline(StandardVertexShader vs, FragmentShader fs) :
        VS(std::move(vs)), FS(std::move(fs))
    {
    }
};

class NormalRenderPipeline
{
public:
    NormalVertexShader VS;
    SimpleLitFragmentShader FS;

    NormalRenderPipeline(NormalVertexShader vs, SimpleLitFragmentShader fs) :
        VS(std::move(vs)), FS(std::move(fs))
    {
    }
};