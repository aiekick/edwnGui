#include "Renderer.hh"
#include <chrono>

EdrawList drawList;

EGUI_API void EdrawList::CreateObjects() {
    D3DXCreateSprite(EGui.Device, &EGui.Sprite);

    D3DPRESENT_PARAMETERS pp = {};
    DWORD nFonts = 0;

    Verdana = NewFont("Verdana", FW_NORMAL, CLEARTYPE_NATURAL_QUALITY, 12);
    Tahombd = NewFont("Verdana", FW_BOLD, CLEARTYPE_NATURAL_QUALITY, 12);
    SmallFont = NewFont("Small Fonts", FW_NORMAL, CLEARTYPE_NATURAL_QUALITY, 8);
    TabIcon = NewFont("Verdana", FW_NORMAL, CLEARTYPE_NATURAL_QUALITY, 16);
    TitleFont = NewFont("Verdana", FW_NORMAL, CLEARTYPE_NATURAL_QUALITY, 16);

    D3DXCreateTextureFromFileInMemoryEx(EGui.Device, BgTexture, 424852, 4096, 4096, D3DX_DEFAULT, NULL, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->BackgroundTexture);
    D3DXCreateTextureFromFileInMemoryEx(EGui.Device, TransparencyTexture, 2419, 50, 50, D3DX_DEFAULT, NULL, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->AlphaTexture);
}

EGUI_API void EdrawList::ReleaseObjects() {
    if (EGui.Sprite)
        EGui.Sprite->Release();
    if (Verdana.D3DXFont)
        Verdana.D3DXFont->Release();
    if (Tahombd.D3DXFont)
        Tahombd.D3DXFont->Release();
    if (SmallFont.D3DXFont)
        SmallFont.D3DXFont->Release();
    if (TabIcon.D3DXFont)
        TabIcon.D3DXFont->Release();
    if (TitleFont.D3DXFont)
        TitleFont.D3DXFont->Release();
}

EGUI_API void EdrawList::Init()
{
    if (NeedsUpdate) {
        CreateObjects();
        NeedsUpdate = false;

#ifdef USE_D2D
        D2D1_SIZE_U size = D2D1::SizeU(Input.GetWindowSize().x, Input.GetWindowSize().y);
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &EGui.m_pD2DFactory);
        EGui.m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(EGui.hwnd, size), &EGui.m_pRenderTarget);

        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&EGui.m_pDWriteFactory));
#endif
    }
}

EGUI_API void EdrawList::Reset()
{
    D3DVIEWPORT9 screen;
    EGui.Device->GetViewport(&screen);
}

EGUI_API FontData EdrawList::NewFont(std::string name, int weight, int quality, int size) {
    FontData CpyFont;

    D3DXCreateFontA(EGui.Device, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, quality, DEFAULT_PITCH, name.c_str(), &CpyFont.D3DXFont);
    CpyFont.name = name;
    CpyFont.weight = weight;
    CpyFont.quality = quality;
    CpyFont.size = size;

    return CpyFont;
}

EGUI_API void EdrawList::PushFont(FontData font) {
	if (!PushingFont) {
        PushingFont = true;
        OverrideFont.D3DXFont = font.D3DXFont;
        OverrideFont.name = font.name;
        OverrideFont.quality = font.quality;
        OverrideFont.size = font.size;
        OverrideFont.weight = font.weight;
	}
}

EGUI_API void EdrawList::PopFont() {
    if (PushingFont)
        PushingFont = false;
}

EGUI_API void EdrawList::PushClip(Rect rectangle) {
    RECT protectedRect{ (int)rectangle.x, (int)rectangle.y, (int)(rectangle.x + rectangle.w), (int)(rectangle.y + rectangle.h) };

    EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    EGui.Device->SetScissorRect(&protectedRect);
}

EGUI_API void EdrawList::PushClip(Vec2 Pos, Vec2 Size) {
    RECT protectedRect{ (int)Pos.x, (int)Pos.y, (int)(Pos.x + Size.x), (int)(Pos.y + Size.y)};

    EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    EGui.Device->SetScissorRect(&protectedRect);
}

EGUI_API void EdrawList::PopClip() {
    EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

EGUI_API void EdrawList::PushAlpha(int alpha) {
    PushingAlphaAmount = alpha;
    PushingAlpha = true;
}

EGUI_API void EdrawList::PopAlpha() {
    if (PushingAlphaAmount == -1)
        return;

    PushingAlphaAmount = -1;
    PushingAlpha = false;
}

EGUI_API void EdrawList::Line(Vec2 Pos, Vec2 Pos2, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    D3DCOLOR d3dclr = ColorToD3DColor(clr);

    vertex vertices[2] = {
        { round(Pos.x), round(Pos.y), 0.0f, 1.0f, d3dclr },
        { round(Pos2.x), round(Pos2.y), 0.0f, 1.0f, d3dclr },
    };

    EGui.Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(vertex));
}

EGUI_API void EdrawList::Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding)
{
    if (rounding > 0) {
        Circle(Pos, rounding, clr, QUARTER, 0.f); //Top left
        Line(Pos + Vec2(rounding, 0), Pos + Vec2(Size.x - (rounding), 0), clr); //Top
        Circle(Pos + Vec2(Size.x - (rounding * 2), 0), rounding, clr, QUARTER, 90.f); //Top right
        Line(Pos + Vec2(Size.x, rounding), Pos + Vec2(Size.x, Size.y - (rounding)), clr); //Right
        Circle(Pos + Vec2(Size.x - (rounding * 2), Size.y - (rounding * 2)), rounding, clr, QUARTER, 180.f); //Bom right
        Line(Pos + Vec2(rounding, Size.y), Pos + Vec2(Size.x - (rounding), Size.y), clr); //Top
        Circle(Pos + Vec2(0, Size.y - (rounding * 2)), rounding, clr, QUARTER, 270.f); //Bom left
        Line(Pos + Vec2(0, rounding), Pos + Vec2(0, Size.y - (rounding)), clr); //Left
    }
    else
    {
        FilledRectangle(Pos, Vec2(Size.x, 1), clr, 0.f); // Top
        FilledRectangle(Vec2(Pos.x, Pos.y + Size.y - 1), Vec2(Size.x, 1), clr, 0.f); // Bottom
        FilledRectangle(Vec2(Pos.x, Pos.y + 1), Vec2(1, Size.y - 2 * 1), clr, 0.f); // Left
        FilledRectangle(Vec2(Pos.x + Size.x - 1, Pos.y + 1), Vec2(1, Size.y - 2 * 1), clr, 0.f); // Right
    }
}

auto toRadians = PI / 180;
auto toDegrees = 180 / PI;

EGUI_API void EdrawList::FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding, EGuiFlags flags)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    D3DCOLOR d3dclr = ColorToD3DColor(clr);

    vertex vert[64] = {};

    for (int i = 0; i < 4; i++)
    {
        int xxx = Pos.x + ((i < 2) ? (Size.x - rounding) : rounding);
        int yyy = Pos.y + ((i % 3) ? (Size.y - rounding) : rounding);

        float tttt = 90.f * i;

        for (int j = 0; j < 16; j++)
        {
            float _aaaaaa = (tttt + j * 6.f);
            _aaaaaa = (_aaaaaa * toRadians);

            const auto vert_idx = (i * 16) + j;

            vert[vert_idx].x = float(xxx + rounding * std::sin(_aaaaaa));
            vert[vert_idx].y = float(yyy - rounding * std::cos(_aaaaaa));
            vert[vert_idx].z = 0.0f;
            vert[vert_idx].rhw = 1.0f;
            vert[vert_idx].color = d3dclr;
        }
    }

    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    EGui.Device->SetTexture(NULL, NULL);
    EGui.Device->SetPixelShader(nullptr);

    EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 62, vert, sizeof(vertex));
}

EGUI_API void EdrawList::BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding) {
    FilledRectangle(Pos, Size, clr, rounding);
    Rectangle(Pos, Size, BorderColor, rounding);
}

EGUI_API void EdrawList::Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color ROtherColor, bool Vertical)
{
    if (PushingAlpha) {
        LColor = Color(LColor.r(), LColor.g(), LColor.b(), PushingAlphaAmount);
        ROtherColor = Color(ROtherColor.r(), ROtherColor.g(), ROtherColor.b(), PushingAlphaAmount);
    }

    D3DCOLOR Ld3dclr = ColorToD3DColor(LColor);
    D3DCOLOR Rd3dclr = ColorToD3DColor(ROtherColor);

    vertex vertices[4] = {
        { Pos.x, Pos.y, 0.0f, 1.0f, Ld3dclr },
        { Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Vertical ? Ld3dclr : Rd3dclr },
        { Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Vertical ? Rd3dclr : Ld3dclr },
        { Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, Rd3dclr }
    };

    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    EGui.Device->SetTexture(0, nullptr);
    EGui.Device->SetPixelShader(nullptr);

    EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));
}

EGUI_API void EdrawList::Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor)
{
    if (PushingAlpha) {
        TopLColor = Color(TopLColor.r(), TopLColor.g(), TopLColor.b(), PushingAlphaAmount);
        TopRColor = Color(TopRColor.r(), TopRColor.g(), TopRColor.b(), PushingAlphaAmount);
        BomLColor = Color(BomLColor.r(), BomLColor.g(), BomLColor.b(), PushingAlphaAmount);
        BomRColor = Color(BomRColor.r(), BomRColor.g(), BomRColor.b(), PushingAlphaAmount);
    }

    D3DCOLOR TopLd3dclr = ColorToD3DColor(TopLColor);
    D3DCOLOR TopRd3dclr = ColorToD3DColor(TopRColor);
    D3DCOLOR BomLd3dclr = ColorToD3DColor(BomLColor);
    D3DCOLOR BomRd3dclr = ColorToD3DColor(BomRColor);

    vertex vertices[4] = {
        { Pos.x, Pos.y, 0.0f, 1.0f, TopLd3dclr },
        { Pos.x + Size.x, Pos.y, 0.0f, 1.0f, TopRd3dclr },
        { Pos.x, Pos.y + Size.y, 0.0f, 1.0f, BomLd3dclr },
        { Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, BomRd3dclr }
    };

    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    EGui.Device->SetTexture(0, nullptr);
    EGui.Device->SetPixelShader(nullptr);

    EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));
}

EGUI_API void EdrawList::Text(const char* text, Vec2 Pos, int Orientation, FontData Font, bool Bordered, Color clr, Vec2 TextClipSize)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    auto font = PushingFont ? OverrideFont.D3DXFont : Font.D3DXFont;

    D3DCOLOR d3dclr = ColorToD3DColor(clr);

    int x((int)Pos.x), y((int)Pos.y);

    bool NoClipRect = (TextClipSize.x == 0 && TextClipSize.y == 0) ? true : false;
    DWORD TextFlags = 0x0;

    RECT rect;
    switch (Orientation) {
    case LEFT:
        TextFlags = NoClipRect ? DT_LEFT | DT_NOCLIP : DT_LEFT;
        break;
    case CENTER:
        TextFlags = NoClipRect ? DT_CENTER | DT_NOCLIP : DT_CENTER;
        break;
    case CENTER_XY:
        TextFlags = NoClipRect ? (DT_CENTER | DT_VCENTER) | DT_NOCLIP : DT_CENTER;
        break;
    case RIGHT:
        TextFlags = NoClipRect ? DT_RIGHT | DT_NOCLIP : DT_RIGHT;
        break;
    }

    if (Bordered) {
        NoClipRect ? SetRect(&rect, x - 1, y, x - 1, y) : SetRect(&rect, x - 1, y, (int)TextClipSize.x - 1, (int)TextClipSize.y);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
        NoClipRect ? SetRect(&rect, x + 1, y, x + 1, y) : SetRect(&rect, x + 1, (int)TextClipSize.y, (int)TextClipSize.x + 1, (int)TextClipSize.y);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
        NoClipRect ? SetRect(&rect, x, y - 1, x, y - 1) : SetRect(&rect, x, y - 1, (int)TextClipSize.x, (int)TextClipSize.y - 1);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
        NoClipRect ? SetRect(&rect, x, y + 1, x, y + 1) : SetRect(&rect, x, y + 1, (int)TextClipSize.x, (int)TextClipSize.y + 1);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));

        NoClipRect ? SetRect(&rect, x - 1, y - 1, x - 1, y - 1) : SetRect(&rect, x - 1, y, (int)TextClipSize.x - 1, (int)TextClipSize.y);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
        NoClipRect ? SetRect(&rect, x + 1, y + 1, x + 1, y + 1) : SetRect(&rect, x + 1, (int)TextClipSize.y, (int)TextClipSize.x + 1, (int)TextClipSize.y);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
        NoClipRect ? SetRect(&rect, x - 1, y + 1, x - 1, y + 1) : SetRect(&rect, x, y - 1, (int)TextClipSize.x, (int)TextClipSize.y - 1);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
        NoClipRect ? SetRect(&rect, x + 1, y - 1, x + 1, y - 1) : SetRect(&rect, x, y + 1, (int)TextClipSize.x, (int)TextClipSize.y + 1);
        font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
    }

    NoClipRect ? SetRect(&rect, x, y, x, y) : SetRect(&rect, x, y, (int)TextClipSize.x, (int)TextClipSize.y);
    font->DrawTextA(NULL, text, -1, &rect, TextFlags, d3dclr);
}

EGUI_API void EdrawList::Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    D3DCOLOR d3dclr = ColorToD3DColor(clr);

    vertex vertices[3] = {
        {
            Top.x, Top.y, 0.0f, 1.0f, d3dclr,
        },
        {
            Right.x, Right.y, 0.0f, 1.0f, d3dclr,
        },
        {
            Left.x, Left.y, 0.0f, 1.0f, d3dclr,
        },
    };

    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    EGui.Device->SetTexture(0, nullptr);
    EGui.Device->SetPixelShader(nullptr);

    EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(vertex));
}

EGUI_API void EdrawList::Circle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation) {
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    D3DCOLOR d3dclr = ColorToD3DColor(clr);
    Pos = Pos + Vec2(radius, radius);

    const int NUM_VERTICES = 365; // Use more vertices to get a smoother circle

    std::vector<vertex> circle(NUM_VERTICES + 2);
    float angle = rotation * D3DX_PI / 180;
    float pi;

    if (e_completion == FULL) pi = D3DX_PI;
    if (e_completion == HALF) pi = D3DX_PI / 2;
    if (e_completion == QUARTER) pi = D3DX_PI / 3.91;

    for (int i = 0; i < NUM_VERTICES + 2; i++)
    {
        circle[i].x = (float)(Pos.x - radius * cos(i * (2 * pi / NUM_VERTICES)));
        circle[i].y = (float)(Pos.y - radius * sin(i * (2 * pi / NUM_VERTICES)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = d3dclr;
    }

    // Rotate matrix
    int _res = NUM_VERTICES;
    for (int i = 0; i < _res; i++)
    {
        // translate point back to origin:
        circle[i].x -= Pos.x;
        circle[i].y -= Pos.y;

        // rotate point
        float xnew = circle[i].x * cos(angle) - circle[i].y * sin(angle);
        float ynew = circle[i].x * sin(angle) + circle[i].y * cos(angle);

        // translate point back:
        circle[i].x = xnew + Pos.x;
        circle[i].y = ynew + Pos.y;
    }

    EGui.Device->CreateVertexBuffer((NUM_VERTICES) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &EGui.VertexBuffer, NULL);

    VOID* pVertices;
    EGui.VertexBuffer->Lock(0, (NUM_VERTICES) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (NUM_VERTICES) * sizeof(vertex));
    EGui.VertexBuffer->Unlock();

    EGui.Device->SetTexture(0, NULL);
    EGui.Device->SetPixelShader(NULL);
    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    EGui.Device->SetStreamSource(0, EGui.VertexBuffer, 0, sizeof(vertex));
    EGui.Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 360, pVertices, sizeof(vertex));
    EGui.VertexBuffer->Release();
}

EGUI_API void EdrawList::FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation) {
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    D3DCOLOR d3dclr = ColorToD3DColor(clr);
    Pos = Pos + Vec2(radius, radius);

    const int NUM_VERTICES = 363; // Use more vertices to get a smoother circle

    std::vector<vertex> circle(NUM_VERTICES + 2);
    float angle = rotation * D3DX_PI / 180;
    float pi;

    if (e_completion == FULL) pi = D3DX_PI;
    if (e_completion == HALF) pi = D3DX_PI / 2;
    if (e_completion == QUARTER) pi = D3DX_PI / 4;

    circle[0].x = Pos.x;
    circle[0].y = Pos.y;
    circle[0].z = 0;
    circle[0].rhw = 1;
    circle[0].color = d3dclr;

    for (int i = 0; i < NUM_VERTICES + 2; i++)
    {
        circle[i].x = (float)(Pos.x - radius * cos(i * (2 * pi / NUM_VERTICES)));
        circle[i].y = (float)(Pos.y - radius * sin(i * (2 * pi / NUM_VERTICES)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = d3dclr;
    }

    // Rotate matrix
    int _res = NUM_VERTICES + 2;
    for (int i = 0; i < _res; i++)
    {
        // translate point back to origin:
        circle[i].x -= Pos.x;
        circle[i].y -= Pos.y;

        // rotate point
        float xnew = circle[i].x * cos(angle) - circle[i].y * sin(angle);
        float ynew = circle[i].x * sin(angle) + circle[i].y * cos(angle);

        // translate point back:
        circle[i].x = xnew + Pos.x;
        circle[i].y = ynew + Pos.y;
    }

    EGui.Device->CreateVertexBuffer((NUM_VERTICES + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &EGui.VertexBuffer, NULL);

    VOID* pVertices;
    EGui.VertexBuffer->Lock(0, (NUM_VERTICES + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (NUM_VERTICES + 2) * sizeof(vertex));
    EGui.VertexBuffer->Unlock();

    EGui.Device->SetTexture(0, NULL);
    EGui.Device->SetPixelShader(NULL);
    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    EGui.Device->SetStreamSource(0, EGui.VertexBuffer, 0, sizeof(vertex));
    EGui.Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, NUM_VERTICES - 1);
    EGui.VertexBuffer->Release();
}

EGUI_API void EdrawList::BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion, float rotation) {
    FilledCircle(Pos, radius, clr, e_completion, rotation);
    Circle(Pos, radius, borderClr, e_completion, rotation);
}

EGUI_API Vec2 EdrawList::GetTextSize(FontData Font, const char* Text)
{
    auto rect = RECT();
    Font.D3DXFont->DrawTextA(0, Text, strlen(Text), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

    return Vec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

EGUI_API void EdrawList::Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    D3DCOLOR d3dclr = ColorToD3DColor(clr);

    D3DXVECTOR3 pos = D3DXVECTOR3(Pos.x, Pos.y, 0.0f);
    RECT DrawArea = { (LONG)pos.x, (LONG)pos.y, LONG(pos.x + Size.x), LONG(pos.y + Size.y) };

    EGui.Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    EGui.Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    EGui.Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    EGui.Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    EGui.Device->SetPixelShader(NULL);
    EGui.Sprite->Begin(D3DXSPRITE_ALPHABLEND);
    EGui.Sprite->Draw(Texture, &DrawArea, NULL, &pos, d3dclr);
    EGui.Sprite->End();
}

EGUI_API void EdrawList::D2DLine(Vec2 From, Vec2 To, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &EGui.m_pSolidColorBrush);
    EGui.m_pRenderTarget->DrawLine(D2D1::Point2F(From.x, From.y), D2D1::Point2F(To.x, To.y), EGui.m_pSolidColorBrush);
    EGui.m_pSolidColorBrush->Release();
}

EGUI_API void EdrawList::D2DPolyLine(Vec2* Points, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    /*m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &m_pSolidColorBrush);
    m_pRenderTarget->DrawLines( Points, numPoints, m_pSolidColorBrush);
    m_pSolidColorBrush->Release();*/
}

EGUI_API void EdrawList::D2DFilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    ID2D1RoundedRectangleGeometry* pRoundedRectGeometry;
    EGui.m_pD2DFactory->CreateRoundedRectangleGeometry(D2D1::RoundedRect(D2D1::RectF(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y), rounding, rounding), &pRoundedRectGeometry);

    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &EGui.m_pSolidColorBrush);
    EGui.m_pRenderTarget->FillGeometry(pRoundedRectGeometry, EGui.m_pSolidColorBrush);

    // Release the brush
    EGui.m_pSolidColorBrush->Release();
    pRoundedRectGeometry->Release();
}

EGUI_API void EdrawList::D2DRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &EGui.m_pSolidColorBrush);
    EGui.m_pRenderTarget->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y), rounding, rounding), EGui.m_pSolidColorBrush);

    // Release the brush
    EGui.m_pSolidColorBrush->Release();
}

EGUI_API void EdrawList::D2DFilledCircle(Vec2 Pos, int Radius, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &EGui.m_pSolidColorBrush);
    EGui.m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(Pos.x + Radius, Pos.y + Radius), Radius, Radius), EGui.m_pSolidColorBrush);
    EGui.m_pSolidColorBrush->Release();
}

EGUI_API void EdrawList::D2DCircle(Vec2 Pos, int Radius, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &EGui.m_pSolidColorBrush);
    EGui.m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(Pos.x + Radius, Pos.y + Radius), Radius, Radius), EGui.m_pSolidColorBrush);
    EGui.m_pSolidColorBrush->Release();
}

EGUI_API void EdrawList::D2DText(FontData Font, const char* Text, Vec2 Pos, Color clr)
{
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    IDWriteTextLayout* pTextLayout = NULL;

    std::string stringed(Text);
    std::wstring converted(stringed.begin(), stringed.end());
    std::wstring FontName(Font.name.begin(), Font.name.end());

    EGui.m_pDWriteFactory->CreateTextFormat(FontName.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 32.0f, L"en-us", &EGui.m_pTextFormat);
    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &EGui.m_pSolidColorBrush);

    EGui.m_pDWriteFactory->CreateTextLayout(
        converted.c_str(),
        converted.length(),
        EGui.m_pTextFormat,
        INT_MAX, // max width
        INT_MAX, // max height
        &pTextLayout);


    DWRITE_TEXT_METRICS metrics;
    pTextLayout->GetMetrics(&metrics);

    float width = metrics.width;
    float height = metrics.height;

    EGui.m_pRenderTarget->DrawText(converted.c_str(), converted.length(), EGui.m_pTextFormat, D2D1::RectF(Pos.x, Pos.y, Pos.x + width, Pos.y + height), EGui.m_pSolidColorBrush);
    EGui.m_pTextFormat->Release();
    EGui.m_pSolidColorBrush->Release();
    pTextLayout->Release();
}

EGUI_API void EdrawList::D2DInfinite(Vec2 Pos, float Size, Color clr) {
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    
}


EGUI_API void EdrawList::D2DStar(Vec2 Pos, float Size, Color clr) {
    if (PushingAlpha)
        clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

    ID2D1PathGeometry* pStarGeometry;
    EGui.m_pD2DFactory->CreatePathGeometry(&pStarGeometry);
    ID2D1GeometrySink* pSink = NULL;
    pStarGeometry->Open(&pSink);

    // Create the star shape
    float innerRadius = Size / 3;
    float outerRadius = Size;
    // Define the points of the star shape
    const D2D1_POINT_2F starPoints[] = {
        D2D1::Point2F(Pos.x, Pos.y - outerRadius),
        D2D1::Point2F(Pos.x + innerRadius, Pos.y - innerRadius),
        D2D1::Point2F(Pos.x + outerRadius, Pos.y),
        D2D1::Point2F(Pos.x + innerRadius, Pos.y + innerRadius),
        D2D1::Point2F(Pos.x, Pos.y + outerRadius),
        D2D1::Point2F(Pos.x - innerRadius, Pos.y + innerRadius),
        D2D1::Point2F(Pos.x - outerRadius, Pos.y),
        D2D1::Point2F(Pos.x - innerRadius, Pos.y - innerRadius)
    };

    pSink->BeginFigure(starPoints[0], D2D1_FIGURE_BEGIN_FILLED);
    for (int i = 1; i < 8; i++)
    {
        pSink->AddLine(starPoints[i]);
    }
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();

    ID2D1SolidColorBrush* pBrush;
    EGui.m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r(), clr.g(), clr.b(), clr.a()), &pBrush);
    EGui.m_pRenderTarget->FillGeometry(pStarGeometry, pBrush);
    pStarGeometry->Release();
    pBrush->Release();
    pSink->Release();
}