#include "EGui/EGui.hpp"

int main() {
    wnd.CreateGraphicsWindow("EGui - " + EGui.GetVersion(), { 1280, 800 });

    EGui.Begin();
    while (TRUE){
        if (!wnd.DispatchMessages())
            break;

        EGui.PreRender();
        {
            EGui.DebugProfiler();
            EGui.DemoWindow();
        }
        EGui.Render();
    }
    EGui.End();

    return EXIT_SUCCESS;
}