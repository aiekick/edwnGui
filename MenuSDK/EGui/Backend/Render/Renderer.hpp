#pragma once
#include "../../EGui.hpp"
#include <memory>

enum circle_type { FULL, HALF, QUARTER };
enum text_alignment { LEFT, CENTER, CENTER_XY, RIGHT };

template<typename T> static inline T Min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T Max(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }

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

//Used for clipping.
struct Clip_info {
    bool OldPushingClip = false;
    bool PushingClip = false;
    RECT Clip = { -1, -1, -1, -1 };
    RECT OldClip = { -1, -1, -1, -1 };
};

struct FontData {
    LPD3DXFONT Font;
    bool anti_alias;
    bool drop_shadow;
    bool outline;
};

struct EFonts {
    FontData Override; /* used for push font and pop font */
    FontData Primary;
    FontData TabIcon;
    FontData TitleFont;
};

extern EFonts Fonts;

struct ETextures {
    IDirect3DTexture9* BackgroundTexture;
    IDirect3DTexture9* AlphaTexture;
    IDirect3DTexture9* MouseTexture;
};

extern ETextures Textures;

struct ERenderer {
    bool NeedsUpdate = true;
    bool Initialized = false;

    bool PushingFont = false;
    bool PushingAlpha = false;
    int PushingAlphaAmount = 0;

    const void Create();
    const void Release();

    const void SetAntiAliasing(bool state);

    const FontData AddFont(std::string name, int weight, int size, bool anti_alias = true, bool dropshadow = false, bool outline = false);

    const void PushFont(FontData font);
    const void PopFont();

    const void PushClip(Vec2 Pos, Vec2 Size);
    const void PopClip();

    const void PushAlpha(int alpha);
    const void PopAlpha();

    const void Line(Vec2 Pos, Vec2 Pos2, Color clr);
    const void PolyLine(std::vector<Vec2> points, Color clr);
    const void Polygon(std::vector<Vec2> points, Color clr);

    const void Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding_radius = 0.f, RoundingFlags flags = RoundingFlags::CORNER_ALL);
    const void FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding_radius = 0.f, RoundingFlags flags = RoundingFlags::CORNER_ALL);
    const void BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding = 0.f, RoundingFlags flags = RoundingFlags::CORNER_ALL);
    const void Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color RColor, bool Vertical = false);
    const void Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor);

    const void Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr);
    const void TriangleFilled(Vec2 Top, Vec2 Left, Vec2 Right, Color clr);

    const void Circle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    const void FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    const void BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion = FULL, float rotation = 0.f);

    const void Text(FontData Font, const char* text, Vec2 Pos, Color clr, int Orientation);
    const Vec2 GetTextSize(FontData Font, const char* Text);

    const void Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr = Color(255, 255, 255, 255));
};

extern ERenderer renderer;