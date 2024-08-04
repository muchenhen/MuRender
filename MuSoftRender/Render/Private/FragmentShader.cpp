#include "FragmentShader.h"

#include "Logger.h"

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

bool isPrinted = false;

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

    if(!isPrinted)
    {
        LOG_DEBUG("LightSpaceMatrix: \n", LightSpaceMatrix);
        isPrinted = true;
    }
    
    // Shadow calculation
    Eigen::Vector4f LightSpacePos = LightSpaceMatrix * Input.WorldPosition.homogeneous();
    Eigen::Vector3f ProjectedCoords = LightSpacePos.head<3>() / LightSpacePos.w();

    // 将 xyz 都映射到 [0, 1] 范围
    ProjectedCoords = ProjectedCoords * 0.5f + Eigen::Vector3f::Constant(0.5f);

    // TODO：这段阴影采样代码有问题，需要修复
    float Shadow = 1.0f;

    // 检查是否在光源的视锥中
    if (ProjectedCoords.x() >= 0.0f && ProjectedCoords.x() <= 1.0f &&
        ProjectedCoords.y() >= 0.0f && ProjectedCoords.y() <= 1.0f &&
        ProjectedCoords.z() >= 0.0f && ProjectedCoords.z() <= 1.0f)
    {
        float CurrentDepth = ProjectedCoords.z();
        float ClosestDepth = ShadowMap->SampleDepth(ProjectedCoords.x(), ProjectedCoords.y());
        float Bias = 0.005f;
        Shadow = CurrentDepth - Bias > ClosestDepth ? 1.0f : 0.0f;

        std::cout << "WorldPosition: " << Input.WorldPosition.transpose() << std::endl;
        std::cout << "ProjectedCoords: " << ProjectedCoords.transpose() << std::endl;
        std::cout << "CurrentDepth: " << CurrentDepth << ", ClosestDepth: " << ClosestDepth << std::endl;
    }




    Eigen::Vector3f LightingColor = AmbientColor + Shadow * DiffuseColor;

    Eigen::Vector3f FinalColor = Color.cwiseProduct(LightingColor);

    FinalColor = FinalColor.cwiseMin(1.0f).cwiseMax(0.0f);
    return Eigen::Vector4f(FinalColor.x(), FinalColor.y(), FinalColor.z(), 1);
};