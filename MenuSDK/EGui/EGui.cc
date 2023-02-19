#include "Egui.hh"

guiStyle EGuiStyle;
guiColors EGuiColors;
EGuiDebugWindow DebugWindow;
EGuiMain EGui;

EGUI_API void EGuiMain::Begin() {
    graphics.CreateD3D();
    graphics.CreateD3DPP();
    graphics.CreateD3DDevice();

    drawList.Init();
}

EGUI_API void EGuiMain::End() {
    graphics.Cleanup();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(window.wc.lpszClassName, window.wc.hInstance);
}

EGUI_API bool EGuiMain::InputAreaDisabled() {
    return !(DisableInputArea == NullRect);
}

EGUI_API void EGuiMain::SetCursor(CursorStyle style)
{
    g_CursorStyle = static_cast<int>(style);
}

enum {
    HOME = 0,
    LEGIT,
    RAGE,
    DEFENSE,
    VISUALS,
    MISC
};

EGUI_API void EGuiMain::DemoWindow() {
    /* Set our constraints for this window */
    //SetNextWindowPos({ 100, 100 });
    SetNextWindowSize({ 750, 490 });

    static int tab = 0;

    if (Window(-1, "BotLucky", true)) {
        /* fixes position for rendering & elements */
        SetNextDrawPos(MenuPos);
        SetNextDrawPosEx({ 140, 0 });
        
        /* draws and handles tabs */
        if (Tab("HOME", tab == HOME)) tab = HOME;
        if (Tab("LEGIT", tab == LEGIT)) tab = LEGIT;
        if (Tab("RAGE", tab == RAGE)) tab = RAGE;
        if (Tab("DEFENSE", tab == DEFENSE)) tab = DEFENSE;
        if (Tab("VISUALS", tab == VISUALS)) tab = VISUALS;
        if (Tab("MISC", tab == MISC)) tab = MISC;

        /* sets position for left group */
        SetNextDrawPos(MenuPos);
        SetNextDrawPosEx({ 15, 45 });

        /* handle children (NONONO not those type lol, your weird for thinking that) */
        switch (tab) {
        case 0:
            Child("Settings", { MenuSize.x / 2 - 25, MenuSize.y - 60});
            EndChild();

            SetNextDrawPosEx({ MenuSize.x / 2 - 10, 0 });

            Child("User Interface", { MenuSize.x / 2 - 20, MenuSize.y / 2 - 34 });
            EndChild();

            SetNextDrawPosEx({ 0, MenuSize.y / 2 - 25 });

            Child("Post Process (Too lazy to add switching)", { MenuSize.x / 2 - 20, MenuSize.y / 2 - 34 });
            EndChild();

            break;
        }
    }
    EndWindow();
};