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

    Eigen::Vector3f LightDir = LightPtr->Direction;

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

ShadowMapFragmentShader DefaultShadowMapFragmentShader = [](const FragmentShaderInput& Input,
                                                            const Material* MaterialPtr,
                                                            const DirectionalLight* LightPtr,
                                                            const DepthTexture* ShadowMap,
                                                            const Eigen::Matrix4f& LightSpaceMatrix)
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
    float AmbientStrength = 0.01f;
    Eigen::Vector3f AmbientColor = AmbientStrength * Color;

    Eigen::Vector3f LightDir = LightPtr->Direction;

    // Half Lambert
    float NdotL = -Input.WorldNormal.dot(LightDir);
    float HalfLambert = 0.5f * NdotL + 0.5f;

    // Diffuse
    Eigen::Vector3f LightColor = LightPtr->Color;
    float LightIntensity = LightPtr->Intensity;

    Eigen::Vector3f DiffuseColor = HalfLambert * LightColor * LightIntensity;

    // Shadow calculation
    Eigen::Vector4f LightSpacePos = LightSpaceMatrix * Input.WorldPosition.homogeneous();
    Eigen::Vector3f ProjectedCoords = LightSpacePos.head<3>() / LightSpacePos.w();
    ProjectedCoords = ProjectedCoords * 0.5f + Eigen::Vector3f::Constant(0.5f);

    float ClosestDepth = ShadowMap->SampleDepth(ProjectedCoords.x(), ProjectedCoords.y());
    float CurrentDepth = ProjectedCoords.z();

    float ShadowBias = 0.01f;
    float Shadow = (CurrentDepth + ShadowBias > ClosestDepth) ? 0.5f : 1.0f;

    Eigen::Vector3f LightingColor = AmbientColor + Shadow * DiffuseColor;

    Eigen::Vector3f FinalColor = Color.cwiseProduct(LightingColor);

    FinalColor = FinalColor.cwiseMin(1.0f).cwiseMax(0.0f);
    return Eigen::Vector4f(FinalColor.x(), FinalColor.y(), FinalColor.z(), 1);
};