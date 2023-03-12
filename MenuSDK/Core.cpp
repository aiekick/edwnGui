#include "Common.hpp"

int main() {
    wnd.CreateGraphicsWindow("EGui - " + EGui.GetVersion());
    EGui.Begin();

    MSG msg;
    std::memset(&msg, 0, sizeof(MSG));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        EGui.PreRender();
        {
            wnd.SetGraphicsWindowTitle("EGui, fps: " + std::to_string(timing.getFrameRate()));
            EGui.DemoWindow();
        }
        EGui.Render();

    } EGui.End();

    return 0;
}