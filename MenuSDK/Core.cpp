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
            renderer.PolyLine({ Vec2(10, 10), Vec2(100, 100), Vec2(150, 200), Vec2(200, 60) }, { 255, 255, 255, 255 });
            EGui.DemoWindow();
        }
        EGui.Render();

    } EGui.End();

    return 0;
}