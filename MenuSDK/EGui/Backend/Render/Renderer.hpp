#pragma once
#include "../../EGui.hpp"

enum circle_type { 
    FULL,
    HALF,
    QUARTER
};

enum text_alignment { 
    LEFT,
    CENTER,
    CENTER_XY,
    RIGHT
};

enum RoundingFlags {
    CORNER_NONE = 0,

    CORNER_TOP_LEFT = 1 << 0,
    CORNER_TOP_RIGHT = 1 << 1,
    CORNER_BOTTOM_LEFT = 1 << 2,
    CORNER_BOTTOM_RIGHT = 1 << 3,

    CORNER_TOP = CORNER_TOP_LEFT | CORNER_TOP_RIGHT,
    CORNER_RIGHT = CORNER_TOP_RIGHT | CORNER_BOTTOM_RIGHT,
    CORNER_BOTTOM = CORNER_BOTTOM_LEFT | CORNER_BOTTOM_RIGHT,
    CORNER_LEFT = CORNER_TOP_LEFT | CORNER_BOTTOM_LEFT,

    CORNER_ALL = CORNER_TOP | CORNER_RIGHT | CORNER_BOTTOM | CORNER_LEFT
};

struct ETextures {
    IDirect3DTexture9* BackgroundTexture;
    IDirect3DTexture9* AlphaTexture;
    IDirect3DTexture9* MouseTexture;
};

struct EFonts {
    Font Override = {};
    Font Primary = {};
    Font TabIcon = {};
    Font TitleFont = {};
};

extern ETextures Textures;
extern EFonts Fonts;

struct ERenderer {
    bool PushingFont = false;
    bool PushingAlpha = false;
    int PushingAlphaAmount = 0;

    const void Create();
    const void Release();

    const void SetAntiAliasing(bool state);

    const Font AddFont(std::string name, int weight, int size, bool anti_alias = true, bool dropshadow = false, bool outline = false);

    const void PushFont(const Font font);
    const void PopFont();

    const void PushClip(const Vec2 Pos, const Vec2 Size);
    const void PopClip();

    const void PushAlpha(int alpha);
    const void PopAlpha();

    const void Line(const Vec2 Pos, const Vec2 Pos2, const Color clr);
    const void PolyLine(const std::vector<Vec2> points, const Color clr);
    const void Polygon(const std::vector<Vec2> points, const Color clr);

    const void Rectangle(const Vec2 Pos, const Vec2 Size, const Color clr, float rounding_radius = 0.f, const RoundingFlags flags = RoundingFlags::CORNER_ALL);
    const void FilledRectangle(const Vec2 Pos, const Vec2 Size, const Color clr, float rounding_radius = 0.f, const RoundingFlags flags = RoundingFlags::CORNER_ALL);
    const void BorderedRectangle(const Vec2 Pos, const Vec2 Size, const Color clr, const Color BorderColor, float rounding = 0.f, const RoundingFlags flags = RoundingFlags::CORNER_ALL);
    const void Gradient(const Vec2 Pos, const Vec2 Size, const Color LColor, const Color RColor, bool Vertical = false);
    const void Gradient4(const Vec2 Pos, const Vec2 Size, const Color TopLColor, const Color TopRColor, const Color BomLColor, const Color BomRColor);

    const void Triangle(const Vec2 Top, const Vec2 Left, const Vec2 Right, const Color clr);
    const void TriangleFilled(const Vec2 Top, const Vec2 Left, const Vec2 Right, const Color clr);

    const void Circle(const Vec2 Pos, float radius, const Color clr, int e_completion = FULL, float rotation = 0.f);
    const void FilledCircle(const Vec2 Pos, float radius, const Color clr, int e_completion = FULL, float rotation = 0.f);
    const void BorderedCircle(const Vec2 Pos, float radius, const Color clr, Color borderClr, int e_completion = FULL, float rotation = 0.f);

    const void Text(const Font Font, const char* text, const Vec2 Pos, const Color clr, int Orientation);
    const Vec2 GetTextSize(const Font Font, const char* Text);

    const void Sprite(const LPDIRECT3DTEXTURE9 Texture, const Vec2 Pos, const Vec2 Size, const Color clr = { 255, 255, 255, 255 });
};

extern ERenderer renderer;