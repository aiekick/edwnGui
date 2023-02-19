#pragma once
#include "../../EGui.hh"
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

EGUI_API enum circle_type { FULL, HALF, QUARTER };
EGUI_API enum text_alignment { LEFT, CENTER, CENTER_XY, RIGHT };

template<typename T> static inline T Min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T Max(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }

EGUI_API enum EGuiFlags {
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

EGUI_API struct FontData {
    LPD3DXFONT D3DXFont;
    std::string name = "Verdana";
    int weight = 400;
    int quality = 0;
    int size = 12;
};

EGUI_API struct EdrawList {
    EGUI_API FontData OverrideFont;
    EGUI_API FontData Verdana;
    EGUI_API FontData Tahombd;
    EGUI_API FontData SmallFont;
    EGUI_API FontData TabIcon;
    EGUI_API FontData TitleFont;
    EGUI_API IDirect3DTexture9* BackgroundTexture;
    EGUI_API IDirect3DTexture9* AlphaTexture;

    EGUI_API void Init();
    EGUI_API void CreateObjects();
    EGUI_API void ReleaseObjects();
    EGUI_API void Reset();
    EGUI_API FontData NewFont(std::string name, int weight, int quality, int size);

    EGUI_API D3DCOLOR ColorToD3DColor(Color clr) {
	    return D3DCOLOR_RGBA((int)clr.r(), (int)clr.g(), (int)clr.b(), (int)clr.a());
    }

    EGUI_API void PushFont(FontData font);
    EGUI_API void PopFont();

    EGUI_API void PushClip(Vec2 Pos, Vec2 Size);
    EGUI_API void PushClip(Rect Rectangle);
    EGUI_API void PopClip();

    EGUI_API void PushAlpha(int alpha);
    EGUI_API void PopAlpha();

    EGUI_API void Line(Vec2 Pos, Vec2 Pos2, Color clr);

    EGUI_API void Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    EGUI_API void FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f, EGuiFlags flags = RoundCornersNone);
    EGUI_API void BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding = 0.f);
    EGUI_API void Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color RColor, bool Vertical = false);
    EGUI_API void Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor);

    EGUI_API void Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr);

    EGUI_API void Circle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    EGUI_API void FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion = FULL, float rotation = 0.f);
    EGUI_API void BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion = FULL, float rotation = 0.f);

    EGUI_API void Text(const char* Text, Vec2 Pos, int Orientation, FontData Font, bool Bordered, Color clr, Vec2 MaxSize = Vec2(0, 0));
    EGUI_API Vec2 GetTextSize(FontData Font, const char* Text);

    EGUI_API void Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr = Color(255, 255, 255, 255));

    // D2D ----------------------------------------------------------------------------------------------------------------------------------
    EGUI_API void D2DLine(Vec2 From, Vec2 To, Color clr);
    EGUI_API void D2DPolyLine(Vec2 *Points, Color clr);
    EGUI_API void D2DFilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    EGUI_API void D2DRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    EGUI_API void D2DFilledCircle(Vec2 Pos, int Radius, Color clr);
    EGUI_API void D2DCircle(Vec2 Pos, int Radius, Color clr);
    EGUI_API void D2DText(FontData Font, const char* Text, Vec2 Pos, Color clr);
    EGUI_API void D2DInfinite(Vec2 Pos, float Size, Color clr);
    EGUI_API void D2DStar(Vec2 Pos, float Size, Color clr);

    //Shared data D3D & D2D.
    EGUI_API bool NeedsUpdate = true;
    EGUI_API bool Initialized = false;

    EGUI_API bool PushingFont = false;
    EGUI_API bool PushingAlpha = false;
    EGUI_API int PushingAlphaAmount = 0;
};

extern EdrawList drawList;