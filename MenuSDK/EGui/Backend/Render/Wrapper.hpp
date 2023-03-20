#pragma once
#include "../../EGui.hpp"
#include <vector>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

//forward declarations.
struct FontData;

struct RenderList {
    std::vector<std::function<void()>> functionTable;

    void PushFont(FontData font);
    void PopFont();

    void PushClip(Vec2 Pos, Vec2 Size);
    void PopClip();

    void PushAlpha(int alpha);
    void PopAlpha();

    void Line(Vec2 Pos, Vec2 Pos2, Color clr);

    void Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    void FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding = 0.f);
    void BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding = 0.f);
    void Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color RColor, bool Vertical = false);
    void Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor);

    void Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr);

    void Circle(Vec2 Pos, float radius, Color clr, int e_completion = 0, float rotation = 0.f);
    void FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion = 0, float rotation = 0.f);
    void BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion = 0, float rotation = 0.f);

    void Text(FontData Font, const char* text, Vec2 Pos, Color clr, int Orientation);
    Vec2 GetTextSize(FontData Font, const char* Text);

    void Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr = Color(255, 255, 255, 255));

    void DrawRenderData() {
        static std::mutex functionTableMutex;
        static std::condition_variable functionTableCV;

        // lock the mutex and wait for the condition variable to be signaled
        std::unique_lock<std::mutex> lock(functionTableMutex);
        functionTableCV.wait(lock, [this] { return !functionTable.empty(); });

        // @todo: make this multi-threaded
        for (auto& function : functionTable) {
            function();
        }

        functionTable.clear();  
    }
};

extern RenderList render_list;