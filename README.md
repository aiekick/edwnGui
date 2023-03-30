
![alt text](https://cdn.discordapp.com/attachments/979215332432576532/1091069585320579162/edwnGui.png)
Discord: https://discord.gg/cUpvKPATkk

edwnGui is a DirectX9 Graphical User Interface made by https://github.com/EdwinS7
![alt text](https://cdn.discordapp.com/attachments/979215332432576532/1084328197556015254/image.png)

## How easy exactly is it to use?
```cpp
#include "EGui/EGui.hpp"

int main() {
    wnd.CreateGraphicsWindow("EGui - " + EGui.GetVersion(), { 1280, 800 });

    EGui.Begin();
    while (TRUE){
        if (!wnd.DispatchMessages())
            break;

        EGui.PreRender();
        {
            EGui.DemoWindow();
        }
        EGui.Render();
    }
    EGui.End();

    return EXIT_SUCCESS;
}
```

The coded provided to you above creates a Window, This window is our DirectX9 Test Enviornment. What do we use this for? We can use this to build us a menu or virtually anything you want. **There is no sample yet on how to implement it into a dynamic link library!** Your welcome to try yourself as of right now. you will need to call EGui.SetDevice(hDevice), you will also need to call Graphics.SetupRenderStates(hDevice). Now you should have a working internal version of edwnGui. Keep in mind you will have to also setup wndproc. head over here for more context, https://github.com/EdwinS7/edwnGui/blob/main/MenuSDK/EGui/Backend/Win32/Wnd.cpp#L4
