#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    Texture();
    ~Texture();

    bool Load(const std::string& fileName);
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetChannels() const { return m_Channels; }
    unsigned int GetID() const { return m_TextureID; }

    static void SetFlip(bool flip);

private:
    unsigned int m_TextureID;
    int m_Width, m_Height, m_Channels;
    std::string m_FilePath;

    void Delete();
};