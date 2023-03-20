#pragma once
#include "../../EGui.hpp"
#include <memory>

enum circle_type { FULL, HALF, QUARTER };
enum text_alignment { LEFT, CENTER, CENTER_XY, RIGHT };

template<typename T> static inline T Min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T Max(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }

enum EGuiRoundingFlags {
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

struct FontData {
    FontData() { Font = NULL; drop_shadow = false; outline = false; };

    LPD3DXFONT Font;
    bool drop_shadow;
    bool outline;
};

struct ERenderer {
    FontData OverrideFont;
    FontData Verdana;
    FontData Tahombd;
    FontData SmallFont;
    FontData TabIcon;
    FontData TitleFont;
    IDirect3DTexture9* BackgroundTexture;
    IDirect3DTexture9* AlphaTexture;
    IDirect3DTexture9* MouseTexture;

    bool NeedsUpdate = true;
    bool Initialized = false;

    bool PushingFont = false;
    bool PushingAlpha = false;
    int PushingAlphaAmount = 0;

    void CreateObjects();
    void ReleaseObjects();
    void Reset();
    FontData AddFont(std::string name, int weight, int size, bool dropshadow = false, bool outline = false);

    D3DCOLOR TranslateColor(Color clr) {
	    return D3DCOLOR_RGBA((int)clr.r(), (int)clr.g(), (int)clr.b(), PushingAlpha ? PushingAlphaAmount : (int)clr.a());
    }

    void PushFont(FontData font);
    void PopFont();

    void PushClip(Vec2 Pos, Vec2 Size);
    void PopClip();

    void PushAlpha(int alpha);
    void PopAlpha();

    void Line(Vec2 Pos, Vec2 Pos2, Color clr);

    void Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    void FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f, EGuiRoundingFlags flags = CORNER_NONE);
    void BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding = 0.f);
    void Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color RColor, bool Vertical = false);
    void Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor);

    void Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr);

    void Circle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    void FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    void BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion = FULL, float rotation = 0.f);

    void Text(FontData Font, const char* text, Vec2 Pos, Color clr, int Orientation);
    Vec2 GetTextSize(FontData Font, const char* Text);

    void Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr = Color(255, 255, 255, 255));
};

extern ERenderer renderer;