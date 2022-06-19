#pragma once
#include "Vector.h"
#include <stdlib.h>

namespace ColorsL
{
	//按照RGBA存储颜色值
	//基本颜色
	extern Vector4 AliceBlue ;
	extern Vector4 AntiqueWhite ;
	extern Vector4 Aqua ;
	extern Vector4 Aquamarine ;
	extern Vector4 Azure ;
	extern Vector4 Beige ;
	extern Vector4 Bisque ;
	extern Vector4 Black ;
	extern Vector4 BlanchedAlmond ;
	extern Vector4 Blue ;
	extern Vector4 BlueViolet;
	extern Vector4 Brown;
	extern Vector4 BurlyWood ;
	extern Vector4 CadetBlue;
	extern Vector4 Chartreuse;
	extern Vector4 Chocolate;
	extern Vector4 Coral;
	extern Vector4 CornflowerBlue;
	extern Vector4 Cornsilk;
	extern Vector4 Crimson;
	extern Vector4 Cyan;
	extern Vector4 DarkBlue;
	extern Vector4 DarkCyan;
	extern Vector4 DarkGoldenrod;
	extern Vector4 DarkGray;
	extern Vector4 DarkGreen;
	extern Vector4 DarkKhaki;
	extern Vector4 DarkMagenta;
	extern Vector4 DarkOliveGreen;
	extern Vector4 DarkOrange;
	extern Vector4 DarkOrchid;
	extern Vector4 DarkRed;
	extern Vector4 DarkSalmon;
	extern Vector4 DarkSeaGreen;
	extern Vector4 DarkSlateBlue;
	extern Vector4 DarkSlateGray;
	extern Vector4 DarkTurquoise;
	extern Vector4 DarkViolet;
	extern Vector4 DeepPink;
	extern Vector4 DeepSkyBlue;
	extern Vector4 DimGray;
	extern Vector4 DodgerBlue;
	extern Vector4 Firebrick;
	extern Vector4 FloralWhite;
	extern Vector4 ForestGreen;
	extern Vector4 Fuchsia;
	extern Vector4 Gainsboro;
	extern Vector4 GhostWhite;
	extern Vector4 Gold;
	extern Vector4 Goldenrod;
	extern Vector4 Gray;
	extern Vector4 Green;
	extern Vector4 GreenYellow;
	extern Vector4 Honeydew;
	extern Vector4 HotPink;
	extern Vector4 IndianRed;
	extern Vector4 Indigo;
	extern Vector4 Ivory;
	extern Vector4 Khaki;
	extern Vector4 Lavender;
	extern Vector4 LavenderBlush;
	extern Vector4 LawnGreen;
	extern Vector4 LemonChiffon;
	extern Vector4 LightBlue;
	extern Vector4 LightCoral;
	extern Vector4 LightCyan;
	extern Vector4 LightGoldenrodYellow;
	extern Vector4 LightGreen;
	extern Vector4 LightGray;
	extern Vector4 LightPink;
	extern Vector4 LightSalmon;
	extern Vector4 LightSeaGreen;
	extern Vector4 LightSkyBlue;
	extern Vector4 LightSlateGray;
	extern Vector4 LightSteelBlue;
	extern Vector4 LightYellow;
	extern Vector4 Lime;
	extern Vector4 LimeGreen;
	extern Vector4 Linen;
	extern Vector4 Magenta;
	extern Vector4 Maroon;
	extern Vector4 MediumAquamarine;
	extern Vector4 MediumBlue;
	extern Vector4 MediumOrchid;
	extern Vector4 MediumPurple;
	extern Vector4 MediumSeaGreen;
	extern Vector4 MediumSlateBlue;
	extern Vector4 MediumSpringGreen;
	extern Vector4 MediumTurquoise;
	extern Vector4 MediumVioletRed;
	extern Vector4 MidnightBlue;
	extern Vector4 MintCream;
	extern Vector4 MistyRose;
	extern Vector4 Moccasin;
	extern Vector4 NavajoWhite;
	extern Vector4 Navy;
	extern Vector4 OldLace;
	extern Vector4 Olive;
	extern Vector4 OliveDrab;
	extern Vector4 Orange;
	extern Vector4 OrangeRed;
	extern Vector4 Orchid;
	extern Vector4 PaleGoldenrod;
	extern Vector4 PaleGreen;
	extern Vector4 PaleTurquoise;
	extern Vector4 PaleVioletRed;
	extern Vector4 PapayaWhip;
	extern Vector4 PeachPuff;
	extern Vector4 Peru;
	extern Vector4 Pink;
	extern Vector4 Plum;
	extern Vector4 PowderBlue;
	extern Vector4 Purple;
	extern Vector4 Red;
	extern Vector4 RosyBrown;
	extern Vector4 RoyalBlue;
	extern Vector4 SaddleBrown;
	extern Vector4 Salmon;
	extern Vector4 SandyBrown;
	extern Vector4 SeaGreen;
	extern Vector4 SeaShell;
	extern Vector4 Sienna;
	extern Vector4 Silver;
	extern Vector4 SkyBlue;
	extern Vector4 SlateBlue;
	extern Vector4 SlateGray;
	extern Vector4 Snow;
	extern Vector4 SpringGreen;
	extern Vector4 SteelBlue;
	extern Vector4 Tan;
	extern Vector4 Teal;
	extern Vector4 Thistle;
	extern Vector4 Tomato;
	extern Vector4 Transparent;
	extern Vector4 Turquoise;
	extern Vector4 Violet;
	extern Vector4 Wheat;
	extern Vector4 White;
	extern Vector4 WhiteSmoke;
	extern Vector4 Yellow;
	extern Vector4 YellowGreen;
}

union FloatRand
{
	struct
	{
		unsigned long Frac : 23;
		unsigned long Exp : 8;
		unsigned long Signed : 1;
	} BitArea;
	float Value;
	unsigned long Binary; /* for debug only */
};

extern float GetFloatRand();

extern Vector4 randColor();