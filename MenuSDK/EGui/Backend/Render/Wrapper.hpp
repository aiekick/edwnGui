#pragma once
#include "Renderer.hpp"
#include <functional>


struct RenderList {
    std::vector<std::function<void()>> functionTable;

    void test(Vec2 pos, Vec2 size, Color clr, int rounding_radius)
    {
        functionTable.push_back([=]() {
            // Use the global renderer object to call FilledRectangle
            renderer.FilledRectangle(pos, size, clr, rounding_radius);
            });
    }

    void DrawRenderData() {
        for (const auto& function : functionTable) {
            function();
        }
    }
};

extern RenderList render_list;