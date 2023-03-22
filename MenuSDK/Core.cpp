#include "Common.hpp"

int main() {
    wnd.CreateGraphicsWindow("EGui - " + EGui.GetVersion(), { 1280, 800 });
    EGui.Begin();

    MSG msg;
    std::memset(&msg, 0, sizeof(MSG));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        EGui.PreRender();
        {
            //fps, max fps, float time (up time)
            wnd.SetGraphicsWindowTitle("EGui, fps: " + std::to_string(timing.getFrameRateAbs()) + " Highest fps: " + std::to_string(timing.getFrameRateAbsMax()) + " Float Time: " + std::to_string(timing.getRealTime()));
            //renderer.FilledRectangle({ 10, 10 }, { 100, 100 }, { 255, 255, 255, 255 }, 5);
            //renderer.Rectangle({ 10, 10 }, { 1000, 500 }, { 255, 255, 255, 255 }, 50, CORNER_ALL);
            //render_list.DrawRenderData();
            //renderer.Rectangle({ 10, 10 }, { 100, 100 }, { 255, 0, 0, 255 }, 25, EGuiRoundingFlags::CORNER_ALL);
            EGui.DemoWindow();
        }
        EGui.Render();

    } EGui.End();

    return 0;
}