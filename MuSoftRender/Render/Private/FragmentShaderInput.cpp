#include "FragmentShaderInput.h"

// #include "Logger.h"

FragmentShader DefaultFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr)
{
    Eigen::Vector3f Color = MaterialPtr->GetBaseColor();
    if (MaterialPtr->HasTexture())
    {
        Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
    }
    return Eigen::Vector4f(Color.x(), Color.y(), Color.z(), 1);
};

SimpleLitFragmentShader DefaultSimpleLitFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr, const DirectionalLight* LightPtr)
{
    Eigen::Vector3f Color = MaterialPtr->GetBaseColor();
    if (MaterialPtr->HasTexture())
    {
        Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
    }

    // Diffuse
    Eigen::Vector3f LightDir = LightPtr->Direction;
    Eigen::Vector3f LightColor = LightPtr->Color;
    float LightIntensity = LightPtr->Intensity;

    float DiffuseFactor = std::max(0.0f, Input.WorldNormal.dot(-LightDir));
    Eigen::Vector3f DiffuseColor = DiffuseFactor * LightColor * LightIntensity;

    Eigen::Vector3f FinalColor = Color.cwiseProduct(DiffuseColor);

    FinalColor = FinalColor.cwiseMin(1.0f).cwiseMax(0.0f);
    return Eigen::Vector4f(FinalColor.x(), FinalColor.y(), FinalColor.z(), 1);
};