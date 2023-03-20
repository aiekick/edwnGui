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
            wnd.SetGraphicsWindowTitle("EGui, fps: " + std::to_string(timing.getFrameRate()) + " Highest fps: " + std::to_string(timing.getFrameRateMax()) + " Float Time: " + std::to_string(timing.getRealTime()));
            render_list.FilledRectangle({ 10, 10 }, { 100, 100 }, { 255, 255, 255, 255 }, 5);

            render_list.DrawRenderData();
            //EGui.DemoWindow();
        }
        EGui.Render();

    } EGui.End();

    return 0;
}