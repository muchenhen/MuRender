
#include <iostream>
#include "Render/TGAImage.h"
int main()
{
    TGAImage Image(200,200);
    for (int y=0;y<200;y++)
    {
        for (int x=0;x<200;x++)
        {
            TGAColor color(x%256,y%256,255,255);
            Image.set(x,y,color);
        }
    }
    Image.write_tga_file("test.tga");
}

