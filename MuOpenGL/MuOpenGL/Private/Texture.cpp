#include "Texture.h"
#include <iostream>
#include <filesystem>
#include "Constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace fs = std::filesystem;

Texture::Texture() :
    m_TextureID(0), m_Width(0), m_Height(0), m_Channels(0)
{
}

Texture::~Texture()
{
    Delete();
}

bool Texture::Load(const std::string& fileName)
{
    Delete(); // 删除任何现有的纹理

    fs::path executablePath = EXECUTABLE_PATH;
    fs::path newPath = executablePath.parent_path().parent_path();
    auto filePath = newPath / TEXTURE_PATH / fileName;

    unsigned char* data = stbi_load(filePath.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture: " << fileName << '\n';
        return false;
    }

    GLint format;
    if (m_Channels == 1) format = GL_RED;
    else if (m_Channels == 3) format = GL_RGB;
    else if (m_Channels == 4) format = GL_RGBA;
    else
    {
        std::cout << "Unsupported number of channels: " << m_Channels << '\n';
        stbi_image_free(data);
        return false;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    m_FilePath = fileName;
    return true;
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    if (m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = 0;
    }
}

void Texture::SetFlip(bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
}