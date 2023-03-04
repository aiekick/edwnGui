#pragma once
#include "../../EGui.hpp"
#include <memory>

#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)

#define ExtractAlpha(x) ((x>>24)&255)
#define ExtractRed(x) ((x>>16)&255)
#define ExtractGreen(x) ((x>>12)&255)
#define ExtractBlue(x) (x&255)

#define PI_SQUARED 9.86960440109

enum circle_type { FULL, HALF, QUARTER };
enum text_alignment { LEFT, CENTER, CENTER_XY, RIGHT };

template<typename T> static inline T Min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T Max(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }

enum EGuiFlags {
    RoundCornersTopLeft = 1 << 4,
    RoundCornersTopRight = 1 << 5,
    RoundCornersBottomLeft = 1 << 6,
    RoundCornersBottomRight = 1 << 7,
    RoundCornersNone = 1 << 8,
    RoundCornersTop = RoundCornersTopLeft | RoundCornersTopRight,
    RoundCornersBottom = RoundCornersBottomLeft | RoundCornersBottomRight,
    RoundCornersLeft = RoundCornersBottomLeft | RoundCornersTopLeft,
    RoundCornersRight = RoundCornersBottomRight | RoundCornersTopRight,
    RoundCornersAll = RoundCornersTopLeft | RoundCornersTopRight | RoundCornersBottomLeft | RoundCornersBottomRight
};

struct FontData {
    LPD3DXFONT D3DXFont;
    std::string name = "Verdana";
    int weight = 400;
    int quality = 0;
    int size = 12;
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

    void Init();
    void CreateObjects();
    void ReleaseObjects();
    void Reset();
    FontData NewFont(std::string name, int weight, int quality, int size);

    D3DCOLOR ColorToD3DColor(Color clr) {
	    return D3DCOLOR_RGBA((int)clr.r(), (int)clr.g(), (int)clr.b(), (int)clr.a());
    }

    void PushFont(FontData font);
    void PopFont();

    void PushClip(Vec2 Pos, Vec2 Size);
    void PushClip(Rect Rectangle);
    void PopClip();

    void PushAlpha(int alpha);
    void PopAlpha();

    void Line(Vec2 Pos, Vec2 Pos2, Color clr);

    void Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    void FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f, EGuiFlags flags = RoundCornersNone);
    void BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding = 0.f);
    void Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color RColor, bool Vertical = false);
    void Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor);

    void Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr);

    void Circle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    void FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    void BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion = FULL, float rotation = 0.f);

    void Text(const char* Text, Vec2 Pos, int Orientation, FontData Font, bool Bordered, Color clr, Vec2 MaxSize = Vec2(0, 0));
    Vec2 GetTextSize(FontData Font, const char* Text);

    void Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr = Color(255, 255, 255, 255));

    //Shared data D3D & D2D.
    bool NeedsUpdate = true;
    bool Initialized = false;

    bool PushingFont = false;
    bool PushingAlpha = false;
    int PushingAlphaAmount = 0;
};

extern ERenderer renderer;