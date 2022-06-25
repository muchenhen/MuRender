#include "Texture.h"
#include <fstream>
#include "Device.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::LoadBMP(const std::string& filename)
{
    std::ifstream is(filename, std::ios_base::binary);
    if (!is) return false;
    unsigned char buf[54];
    is.read((char*)buf, sizeof(buf));
    width = *(int*)&buf[18];
    height = abs(*(int*)&buf[22]);
    int bits = buf[28];
    int rowcountBytes = ((width * bits + 31) >> 5) << 2;
    int count = rowcountBytes * height;
    unsigned char* tmp = new unsigned char[count];
    is.read((char*)tmp, count);
    int bytes = bits / 8, cnts = 0;
    int bof = ((width * bits) >> 3) & 3;
    int paddings = (4 - bof) == 4 ? 0 : (4 - bof);
    data = new UINT[width * height];
    for (int i = 0; i < width * height; i++)
    {
        data[i] = RGB(tmp[cnts + 2], tmp[cnts + 1], tmp[cnts + 0]);
        cnts += bytes;
        if (cnts + paddings >= rowcountBytes && (cnts + paddings) % rowcountBytes == 0)
            cnts += paddings;
    }
    delete[] tmp;
    return true;
}

unsigned Texture::SampleColor(MuVector& tc)
{
    float u = MathSet::clamp(tc.X, 0.f, 1.f);
    float v = MathSet::clamp(tc.Y, 0.f, 1.f);

    int u1 = (int)(std::round(u * (width)));
    int v1 = (int)(std::round(v * (height)));

    return data[u1 + v1 * width];
}
