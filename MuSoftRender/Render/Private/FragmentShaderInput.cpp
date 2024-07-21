#include "FragmentShaderInput.h"

FragmentShader DefaultFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr)
{
    Eigen::Vector3f Color = MaterialPtr->GetBaseColor();
    if (MaterialPtr->HasTexture())
    {
        Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
    }
    return Eigen::Vector4f(Color.x(), Color.y(), Color.z(), 1);
};