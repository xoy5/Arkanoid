/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Colors.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <fstream>
class Color
{
public:
	unsigned int dword;
public:
	constexpr Color() : dword() {}
	constexpr Color(const Color& col)
		:
		dword(col.dword)
	{}
	constexpr Color(unsigned int dw)
		:
		dword(dw)
	{}
	constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
		:
		dword((x << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		:
		dword((r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(Color col, unsigned char x)
		:
		Color((x << 24u) | col.dword)
	{}
	Color& operator =(Color color)
	{
		dword = color.dword;
		return *this;
	}
	bool operator==(const Color& rhs) const
	{
		return dword == rhs.dword;
	}
	bool operator!=(const Color& rhs) const
	{
		return !(*this == rhs);
	}
	constexpr unsigned char GetX() const
	{
		return dword >> 24u;
	}
	constexpr unsigned char GetA() const
	{
		return GetX();
	}
	constexpr unsigned char GetR() const
	{
		return (dword >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const
	{
		return (dword >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const
	{
		return dword & 0xFFu;
	}
	void SetX(unsigned char x)
	{
		dword = (dword & 0xFFFFFFu) | (x << 24u);
	}
	void SetA(unsigned char a)
	{
		SetX(a);
	}
	void SetR(unsigned char r)
	{
		dword = (dword & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG(unsigned char g)
	{
		dword = (dword & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB(unsigned char b)
	{
		dword = (dword & 0xFFFFFF00u) | b;
	}
	static Color GetDarker(const Color& c, const int& darker)
	{
		return Color(unsigned int(std::max((int)c.GetR() - darker, 0)), unsigned int(std::max((int)c.GetG() - darker, 0)), unsigned int(std::max((int)c.GetB() - darker, 0)));
	}
};

namespace Colors
{
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (r << 16) | (g << 8) | b;
	}
	// My Colors
	static constexpr Color RedOrange = MakeRGB(255u, 77u, 0u);
	static constexpr Color Grapefruit = MakeRGB(255u, 20u, 60u);

	static constexpr Color AliceBlue = MakeRGB(240u, 248u, 255u);
	static constexpr Color AntiqueWhite = MakeRGB(250u, 235u, 215u);
	static constexpr Color Aqua = MakeRGB(0u, 255u, 255u);
	static constexpr Color Aquamarine = MakeRGB(127u, 255u, 212u);
	static constexpr Color Azure = MakeRGB(240u, 255u, 255u);
	static constexpr Color Beige = MakeRGB(245u, 245u, 220u);
	static constexpr Color Bisque = MakeRGB(255u, 228u, 196u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color BlanchedAlmond = MakeRGB(255u, 235u, 205u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color BlueViolet = MakeRGB(138u, 43u, 226u);
	static constexpr Color Brown = MakeRGB(165u, 42u, 42u);
	static constexpr Color BurlyWood = MakeRGB(222u, 184u, 135u);
	static constexpr Color CadetBlue = MakeRGB(95u, 158u, 160u);
	static constexpr Color Chartreuse = MakeRGB(127u, 255u, 0u);
	static constexpr Color Chocolate = MakeRGB(210u, 105u, 30u);
	static constexpr Color Coral = MakeRGB(255u, 127u, 80u);
	static constexpr Color CornflowerBlue = MakeRGB(100u, 149u, 237u);
	static constexpr Color Cornsilk = MakeRGB(255u, 248u, 220u);
	static constexpr Color Crimson = MakeRGB(220u, 20u, 60u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color DarkBlue = MakeRGB(0u, 0u, 139u);
	static constexpr Color DarkCyan = MakeRGB(0u, 139u, 139u);
	static constexpr Color DarkGoldenRod = MakeRGB(184u, 134u, 11u);
	static constexpr Color DarkGray = MakeRGB(169u, 169u, 169u);
	static constexpr Color DarkGrey = MakeRGB(169u, 169u, 169u);
	static constexpr Color DarkGreen = MakeRGB(0u, 100u, 0u);
	static constexpr Color DarkKhaki = MakeRGB(189u, 183u, 107u);
	static constexpr Color DarkMagenta = MakeRGB(139u, 0u, 139u);
	static constexpr Color DarkOliveGreen = MakeRGB(85u, 107u, 47u);
	static constexpr Color DarkOrange = MakeRGB(255u, 140u, 0u);
	static constexpr Color DarkOrchid = MakeRGB(153u, 50u, 204u);
	static constexpr Color DarkRed = MakeRGB(139u, 0u, 0u);
	static constexpr Color DarkSalmon = MakeRGB(233u, 150u, 122u);
	static constexpr Color DarkSeaGreen = MakeRGB(143u, 188u, 143u);
	static constexpr Color DarkSlateBlue = MakeRGB(72u, 61u, 139u);
	static constexpr Color DarkSlateGray = MakeRGB(47u, 79u, 79u);
	static constexpr Color DarkSlateGrey = MakeRGB(47u, 79u, 79u);
	static constexpr Color DarkTurquoise = MakeRGB(0u, 206u, 209u);
	static constexpr Color DarkViolet = MakeRGB(148u, 0u, 211u);
	static constexpr Color DeepPink = MakeRGB(255u, 20u, 147u);
	static constexpr Color DeepSkyBlue = MakeRGB(0u, 191u, 255u);
	static constexpr Color DimGray = MakeRGB(105u, 105u, 105u);
	static constexpr Color DimGrey = MakeRGB(105u, 105u, 105u);
	static constexpr Color DodgerBlue = MakeRGB(30u, 144u, 255u);
	static constexpr Color FireBrick = MakeRGB(178u, 34u, 34u);
	static constexpr Color FloralWhite = MakeRGB(255u, 250u, 240u);
	static constexpr Color ForestGreen = MakeRGB(34u, 139u, 34u);
	static constexpr Color Fuchsia = MakeRGB(255u, 0u, 255u);
	static constexpr Color Gainsboro = MakeRGB(220u, 220u, 220u);
	static constexpr Color GhostWhite = MakeRGB(248u, 248u, 255u);
	static constexpr Color Gold = MakeRGB(255u, 215u, 0u);
	static constexpr Color GoldenRod = MakeRGB(218u, 165u, 32u);
	static constexpr Color Gray = MakeRGB(128u, 128u, 128u);
	static constexpr Color Grey = MakeRGB(128u, 128u, 128u);
	static constexpr Color Green = MakeRGB(0u, 128u, 0u);
	static constexpr Color GreenYellow = MakeRGB(173u, 255u, 47u);
	static constexpr Color HoneyDew = MakeRGB(240u, 255u, 240u);
	static constexpr Color HotPink = MakeRGB(255u, 105u, 180u);
	static constexpr Color IndianRed = MakeRGB(205u, 92u, 92u);
	static constexpr Color Indigo = MakeRGB(75u, 0u, 130u);
	static constexpr Color Ivory = MakeRGB(255u, 255u, 240u);
	static constexpr Color Khaki = MakeRGB(240u, 230u, 140u);
	static constexpr Color Lavender = MakeRGB(230u, 230u, 250u);
	static constexpr Color LavenderBlush = MakeRGB(255u, 240u, 245u);
	static constexpr Color LawnGreen = MakeRGB(124u, 252u, 0u);
	static constexpr Color LemonChiffon = MakeRGB(255u, 250u, 205u);
	static constexpr Color LightBlue = MakeRGB(173u, 216u, 230u);
	static constexpr Color LightCoral = MakeRGB(240u, 128u, 128u);
	static constexpr Color LightCyan = MakeRGB(224u, 255u, 255u);
	static constexpr Color LightGoldenRodYellow = MakeRGB(250u, 250u, 210u);
	static constexpr Color LightGray = MakeRGB(211u, 211u, 211u);
	static constexpr Color LightGrey = MakeRGB(211u, 211u, 211u);
	static constexpr Color LightGreen = MakeRGB(144u, 238u, 144u);
	static constexpr Color LightPink = MakeRGB(255u, 182u, 193u);
	static constexpr Color LightSalmon = MakeRGB(255u, 160u, 122u);
	static constexpr Color LightSeaGreen = MakeRGB(32u, 178u, 170u);
	static constexpr Color LightSkyBlue = MakeRGB(135u, 206u, 250u);
	static constexpr Color LightSlateGray = MakeRGB(119u, 136u, 153u);
	static constexpr Color LightSlateGrey = MakeRGB(119u, 136u, 153u);
	static constexpr Color LightSteelBlue = MakeRGB(176u, 196u, 222u);
	static constexpr Color LightYellow = MakeRGB(255u, 255u, 224u);
	static constexpr Color Lime = MakeRGB(0u, 255u, 0u);
	static constexpr Color LimeGreen = MakeRGB(50u, 205u, 50u);
	static constexpr Color Linen = MakeRGB(250u, 240u, 230u);
	static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
	static constexpr Color Maroon = MakeRGB(128u, 0u, 0u);
	static constexpr Color MediumAquaMarine = MakeRGB(102u, 205u, 170u);
	static constexpr Color MediumBlue = MakeRGB(0u, 0u, 205u);
	static constexpr Color MediumOrchid = MakeRGB(186u, 85u, 211u);
	static constexpr Color MediumPurple = MakeRGB(147u, 112u, 219u);
	static constexpr Color MediumSeaGreen = MakeRGB(60u, 179u, 113u);
	static constexpr Color MediumSlateBlue = MakeRGB(123u, 104u, 238u);
	static constexpr Color MediumSpringGreen = MakeRGB(0u, 250u, 154u);
	static constexpr Color MediumTurquoise = MakeRGB(72u, 209u, 204u);
	static constexpr Color MediumVioletRed = MakeRGB(199u, 21u, 133u);
	static constexpr Color MidnightBlue = MakeRGB(25u, 25u, 112u);
	static constexpr Color MintCream = MakeRGB(245u, 255u, 250u);
	static constexpr Color MistyRose = MakeRGB(255u, 228u, 225u);
	static constexpr Color Moccasin = MakeRGB(255u, 228u, 181u);
	static constexpr Color NavajoWhite = MakeRGB(255u, 222u, 173u);
	static constexpr Color Navy = MakeRGB(0u, 0u, 128u);
	static constexpr Color OldLace = MakeRGB(253u, 245u, 230u);
	static constexpr Color Olive = MakeRGB(128u, 128u, 0u);
	static constexpr Color OliveDrab = MakeRGB(107u, 142u, 35u);
	static constexpr Color Orange = MakeRGB(255u, 165u, 0u);
	static constexpr Color OrangeRed = MakeRGB(255u, 69u, 0u);
	static constexpr Color Orchid = MakeRGB(218u, 112u, 214u);
	static constexpr Color PaleGoldenRod = MakeRGB(238u, 232u, 170u);
	static constexpr Color PaleGreen = MakeRGB(152u, 251u, 152u);
	static constexpr Color PaleTurquoise = MakeRGB(175u, 238u, 238u);
	static constexpr Color PaleVioletRed = MakeRGB(219u, 112u, 147u);
	static constexpr Color PapayaWhip = MakeRGB(255u, 239u, 213u);
	static constexpr Color PeachPuff = MakeRGB(255u, 218u, 185u);
	static constexpr Color Peru = MakeRGB(205u, 133u, 63u);
	static constexpr Color Pink = MakeRGB(255u, 192u, 203u);
	static constexpr Color Plum = MakeRGB(221u, 160u, 221u);
	static constexpr Color PowderBlue = MakeRGB(176u, 224u, 230u);
	static constexpr Color Purple = MakeRGB(128u, 0u, 128u);
	static constexpr Color RebeccaPurple = MakeRGB(102u, 51u, 153u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color RosyBrown = MakeRGB(188u, 143u, 143u);
	static constexpr Color RoyalBlue = MakeRGB(65u, 105u, 225u);
	static constexpr Color SaddleBrown = MakeRGB(139u, 69u, 19u);
	static constexpr Color Salmon = MakeRGB(250u, 128u, 114u);
	static constexpr Color SandyBrown = MakeRGB(244u, 164u, 96u);
	static constexpr Color SeaGreen = MakeRGB(46u, 139u, 87u);
	static constexpr Color SeaShell = MakeRGB(255u, 245u, 238u);
	static constexpr Color Sienna = MakeRGB(160u, 82u, 45u);
	static constexpr Color Silver = MakeRGB(192u, 192u, 192u);
	static constexpr Color SkyBlue = MakeRGB(135u, 206u, 235u);
	static constexpr Color SlateBlue = MakeRGB(106u, 90u, 205u);
	static constexpr Color SlateGray = MakeRGB(112u, 128u, 144u);
	static constexpr Color SlateGrey = MakeRGB(112u, 128u, 144u);
	static constexpr Color Snow = MakeRGB(255u, 250u, 250u);
	static constexpr Color SpringGreen = MakeRGB(0u, 255u, 127u);
	static constexpr Color SteelBlue = MakeRGB(70u, 130u, 180u);
	static constexpr Color Tan = MakeRGB(210u, 180u, 140u);
	static constexpr Color Teal = MakeRGB(0u, 128u, 128u);
	static constexpr Color Thistle = MakeRGB(216u, 191u, 216u);
	static constexpr Color Tomato = MakeRGB(255u, 99u, 71u);
	static constexpr Color Turquoise = MakeRGB(64u, 224u, 208u);
	static constexpr Color Violet = MakeRGB(238u, 130u, 238u);
	static constexpr Color Wheat = MakeRGB(245u, 222u, 179u);
	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color WhiteSmoke = MakeRGB(245u, 245u, 245u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color YellowGreen = MakeRGB(154u, 205u, 50u);
}