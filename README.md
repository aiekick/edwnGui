[![Typing SVG](https://readme-typing-svg.demolab.com?font=Verdana&duration=2500&color=F77878&background=9B00FF00&width=435&lines=EGui+V2)](https://git.io/typing-svg)

EGui is a Direct3D9 Graphical User Interface made for easy gui designment.

https://cdn.discordapp.com/attachments/979215332432576532/1084328197556015254/image.png

## How easy exactly is it to use?
```cpp
#include "EGui.hpp"

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
        
        //Render data here, run our demo window.
        EGui.DemoWindow();
        
        EGui.Render();

    } EGui.End();

    return 0;
}
```
