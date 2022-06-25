#pragma once
#include "Vector.h"

namespace ColorsL
{
	extern MuVector AliceBlue;
	extern MuVector AntiqueWhite;
	extern MuVector Aqua;
	extern MuVector Aquamarine;
	extern MuVector Azure;
	extern MuVector Beige;
	extern MuVector Bisque;
	extern MuVector Black;
	extern MuVector BlanchedAlmond;
	extern MuVector Blue;
	extern MuVector BlueViolet;
	extern MuVector Brown;
	extern MuVector BurlyWood;
	extern MuVector CadetBlue;
	extern MuVector Chartreuse;
	extern MuVector Chocolate;
	extern MuVector Coral;
	extern MuVector CornflowerBlue;
	extern MuVector Cornsilk;
	extern MuVector Crimson;
	extern MuVector Cyan;
	extern MuVector DarkBlue;
	extern MuVector DarkCyan;
	extern MuVector DarkGoldenrod;
	extern MuVector DarkGray;
	extern MuVector DarkGreen;
	extern MuVector DarkKhaki;
	extern MuVector DarkMagenta;
	extern MuVector DarkOliveGreen;
	extern MuVector DarkOrange;
	extern MuVector DarkOrchid;
	extern MuVector DarkRed;
	extern MuVector DarkSalmon;
	extern MuVector DarkSeaGreen;
	extern MuVector DarkSlateBlue;
	extern MuVector DarkSlateGray;
	extern MuVector DarkTurquoise;
	extern MuVector DarkViolet;
	extern MuVector DeepPink;
	extern MuVector DeepSkyBlue;
	extern MuVector DimGray;
	extern MuVector DodgerBlue;
	extern MuVector Firebrick;
	extern MuVector FloralWhite;
	extern MuVector ForestGreen;
	extern MuVector Fuchsia;
	extern MuVector Gainsboro;
	extern MuVector GhostWhite;
	extern MuVector Gold;
	extern MuVector Goldenrod;
	extern MuVector Gray;
	extern MuVector Green;
	extern MuVector GreenYellow;
	extern MuVector Honeydew;
	extern MuVector HotPink;
	extern MuVector IndianRed;
	extern MuVector Indigo;
	extern MuVector Ivory;
	extern MuVector Khaki;
	extern MuVector Lavender;
	extern MuVector LavenderBlush;
	extern MuVector LawnGreen;
	extern MuVector LemonChiffon;
	extern MuVector LightBlue;
	extern MuVector LightCoral;
	extern MuVector LightCyan;
	extern MuVector LightGoldenrodYellow;
	extern MuVector LightGreen;
	extern MuVector LightGray;
	extern MuVector LightPink;
	extern MuVector LightSalmon;
	extern MuVector LightSeaGreen;
	extern MuVector LightSkyBlue;
	extern MuVector LightSlateGray;
	extern MuVector LightSteelBlue;
	extern MuVector LightYellow;
	extern MuVector Lime;
	extern MuVector LimeGreen;
	extern MuVector Linen;
	extern MuVector Magenta;
	extern MuVector Maroon;
	extern MuVector MediumAquamarine;
	extern MuVector MediumBlue;
	extern MuVector MediumOrchid;
	extern MuVector MediumPurple;
	extern MuVector MediumSeaGreen;
	extern MuVector MediumSlateBlue;
	extern MuVector MediumSpringGreen;
	extern MuVector MediumTurquoise;
	extern MuVector MediumVioletRed;
	extern MuVector MidnightBlue;
	extern MuVector MintCream;
	extern MuVector MistyRose;
	extern MuVector Moccasin;
	extern MuVector NavajoWhite;
	extern MuVector Navy;
	extern MuVector OldLace;
	extern MuVector Olive;
	extern MuVector OliveDrab;
	extern MuVector Orange;
	extern MuVector OrangeRed;
	extern MuVector Orchid;
	extern MuVector PaleGoldenrod;
	extern MuVector PaleGreen;
	extern MuVector PaleTurquoise;
	extern MuVector PaleVioletRed;
	extern MuVector PapayaWhip;
	extern MuVector PeachPuff;
	extern MuVector Peru;
	extern MuVector Pink;
	extern MuVector Plum;
	extern MuVector PowderBlue;
	extern MuVector Purple;
	extern MuVector Red;
	extern MuVector RosyBrown;
	extern MuVector RoyalBlue;
	extern MuVector SaddleBrown;
	extern MuVector Salmon;
	extern MuVector SandyBrown;
	extern MuVector SeaGreen;
	extern MuVector SeaShell;
	extern MuVector Sienna;
	extern MuVector Silver;
	extern MuVector SkyBlue;
	extern MuVector SlateBlue;
	extern MuVector SlateGray;
	extern MuVector Snow;
	extern MuVector SpringGreen;
	extern MuVector SteelBlue;
	extern MuVector Tan;
	extern MuVector Teal;
	extern MuVector Thistle;
	extern MuVector Tomato;
	extern MuVector Transparent;
	extern MuVector Turquoise;
	extern MuVector Violet;
	extern MuVector Wheat;
	extern MuVector White;
	extern MuVector WhiteSmoke;
	extern MuVector Yellow;
	extern MuVector YellowGreen;
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

extern MuVector randColor();