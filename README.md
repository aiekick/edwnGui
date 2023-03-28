[![Typing SVG](https://readme-typing-svg.demolab.com?font=Verdana&duration=2500&color=F77878&background=9B00FF00&width=435&lines=EGui+V2)](https://git.io/typing-svg)

EGui is a Direct3D9 Graphical User Interface made for easy gui designment. (x64/x86)
![alt text](https://cdn.discordapp.com/attachments/979215332432576532/1084328197556015254/image.png)

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
The code provided above creates a window and sets up a directx9 device, then it begins setting up edwnGui to work with this enviornment. It calls PreRender, PreRender is where it clears our scene and prepares for next render. DemoWindow is our testing window. Render is the present and endscene, this renders everything we gave the renderer earlie 

## How do I use this in a dynamic link library?
First, your gonna need to call
```cpp
SetWindowHandle(HWND) -> located in EGui struct. Example: EGui.SetWindowHandle(HWND);
```

Second, your gonna need to call
```cpp
SetDevice(Device) -> located in EGui struct. Example: EGui.SetDevice(pDevice);
```

Third, your gonna need to call (this will fix colors looking off from what you want)
```cpp
SetupRenderStates(Device) -> located in Graphics struct. Example: Graphics.SetupRenderStates(pDevice);
```

Fourth, your gonna need to call
```cpp
Update() -> located in EGui struct. Example: EGui.Update();
```

Fifth, your gonna need to call (once)
```cpp
CreateObjects() -> located in renderer struct. Example: renderer.CreateObjects();
```

Finally, you can call, optional.
```cpp
DemoWindow() -> located in EGui struct. Example: EGui.DemoWindow();
```
