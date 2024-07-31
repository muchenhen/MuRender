#include "FragmentShader.h"

// #include "Logger.h"

FragmentShader DefaultFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr)
{
    Eigen::Vector3f Color = Eigen::Vector3f(1, 1, 1);
    if (MaterialPtr != nullptr)
    {
        Color = MaterialPtr->GetBaseColor();
        if (MaterialPtr->HasTexture())
        {
            Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
        }
    }
    return Eigen::Vector4f(Color.x(), Color.y(), Color.z(), 1);
};

SimpleLitFragmentShader DefaultSimpleLitFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr, const DirectionalLight* LightPtr)
{
    Eigen::Vector3f Color = Eigen::Vector3f(1, 1, 1);
    if (MaterialPtr != nullptr)
    {
        Color = MaterialPtr->GetBaseColor();
        if (MaterialPtr->HasTexture())
        {
            Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
        }
    }

    // Ambient
    float AmbientStrength = 0.1f;
    Eigen::Vector3f AmbientColor = AmbientStrength * Color;

    Eigen::Vector3f LightDir = LightPtr->Direction.normalized();

    // Half Lambert
    float NdotL = -Input.WorldNormal.dot(LightDir);
    float HalfLambert = 0.5f * NdotL + 0.5f;
    // HalfLambert = HalfLambert * HalfLambert; 

    // Diffuse
    Eigen::Vector3f LightColor = LightPtr->Color;
    float LightIntensity = LightPtr->Intensity;

    // float DiffuseFactor = std::max(0.0f, Input.WorldNormal.dot(-LightDir));

    Eigen::Vector3f DiffuseColor = HalfLambert * LightColor * LightIntensity;

    Eigen::Vector3f LightingColor = AmbientColor + DiffuseColor;

    Eigen::Vector3f FinalColor = Color.cwiseProduct(LightingColor);

    FinalColor = FinalColor.cwiseMin(1.0f).cwiseMax(0.0f);
    return Eigen::Vector4f(FinalColor.x(), FinalColor.y(), FinalColor.z(), 1);
};