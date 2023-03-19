#include "Wrapper.hpp"

RenderList render_list;

void RenderList::PushFont(FontData font)
{
    functionTable.push_back([=]() { renderer.PushFont(font); });
}

void RenderList::PopFont()
{
    functionTable.push_back([=]() { renderer.PopFont(); });
}

void RenderList::PushClip(Vec2 Pos, Vec2 Size)
{
    functionTable.push_back([=]() { renderer.PushClip(Pos, Size); });
}

void RenderList::PopClip()
{
    functionTable.push_back([=]() { renderer.PopClip(); });
}

void RenderList::PushAlpha(int alpha)
{
    functionTable.push_back([=]() { renderer.PushAlpha(alpha); });
}

void RenderList::PopAlpha()
{
    functionTable.push_back([=]() { renderer.PopAlpha(); });
}

void RenderList::Line(Vec2 Pos, Vec2 Pos2, Color clr)
{
    functionTable.push_back([=]() { renderer.Line(Pos, Pos2, clr); });
}

void RenderList::Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding)
{
    functionTable.push_back([=]() { renderer.Rectangle(Pos, Size, clr, rounding); });
}

void RenderList::FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding)
{
    functionTable.push_back([=]() { renderer.FilledRectangle(Pos, Size, clr, rounding); });
}

void RenderList::BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding)
{
    functionTable.push_back([=]() { renderer.BorderedRectangle(Pos, Size, clr, BorderColor, rounding); });
}

void RenderList::Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color RColor, bool Vertical)
{
    functionTable.push_back([=]() { renderer.Gradient(Pos, Size, LColor, RColor, Vertical); });
}

void RenderList::Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor)
{
    functionTable.push_back([=]() { renderer.Gradient4(Pos, Size, TopLColor, TopRColor, BomLColor, BomRColor); });
}

void RenderList::Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr)
{
    functionTable.push_back([=]() { renderer.Triangle(Top, Left, Right, clr); });
}

void RenderList::Circle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation)
{
    functionTable.push_back([=]() { renderer.Circle(Pos, radius, clr, e_completion, rotation); });
}

void RenderList::FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation)
{
    functionTable.push_back([=]() { renderer.FilledCircle(Pos, radius, clr, e_completion, rotation); });
}

void RenderList::BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion, float rotation)
{
    functionTable.push_back([=]() { renderer.BorderedCircle(Pos, radius, clr, borderClr, e_completion, rotation); });
}

void RenderList::Text(FontData Font, const char* text, Vec2 Pos, Color clr, int Orientation)
{
    functionTable.push_back([=]() { renderer.Text(Font, text, Pos, clr, Orientation); });
}

Vec2 RenderList::GetTextSize(FontData Font, const char* Text)
{
    return renderer.GetTextSize(Font, Text);
}

void RenderList::Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr)
{
    functionTable.push_back([=]() { renderer.Sprite(Texture, Pos, Size, clr); });
}