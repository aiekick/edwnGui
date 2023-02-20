#include "Egui.hh"

guiStyle EGuiStyle;
guiColors EGuiColors;
EGuiDebugWindow DebugWindow;
EGuiMain EGui;

std::unordered_map<int, Vec2> EGuiMain::MenuPos;
std::unordered_map<int, Vec2> EGuiMain::MenuSize;

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
    static bool set = false;

    if (!set) {
        SetNextWindowSize({ 750, 490 }, { 750, 490 });
        set = true;
    }

    static int tab = 0;

    if (Window(-1, "BotLucky", true)) {
        auto MenuPos = GetWindowPos();
        auto MenuSize = GetWindowSize();

        /* fixes position for rendering & elements */
        SetNextDrawPos(MenuPos);
        SetNextDrawPosEx({ 140, 1 });
        
        /* draws and handles tabs */
        if (Tab("HOME", tab == HOME)) tab = HOME;
        if (Tab("LEGIT", tab == LEGIT)) tab = LEGIT;
        if (Tab("RAGE", tab == RAGE)) tab = RAGE;
        if (Tab("DEFENSE", tab == DEFENSE)) tab = DEFENSE;
        if (Tab("VISUALS", tab == VISUALS)) tab = VISUALS;
        if (Tab("MISC", tab == MISC)) tab = MISC;

        /* sets position for left group */
        SetNextDrawPos(MenuPos);

        float spacing = 15;
        SetNextDrawPosEx({ spacing, 30.f + spacing });

        Vec2 Size = { (MenuSize.x / 2) - spacing * 1.5f, MenuSize.y - (30.f + spacing * 2.f) };

        Vec2 LeftPos = { MenuPos.x + spacing, MenuPos.y + 30 + spacing };
        Vec2 RightPos = { MenuPos.x + Size.x + spacing * 2, MenuPos.y + 30 + spacing };

        /* handle children (NONONO not those type lol, your weird for thinking that) */
        switch (tab) {
        case 0:
            Child("Settings", Size);
            {
                static bool test_checkbox;
                Checkbox("Example check box", &test_checkbox);

                static int test_keybind;
                Keybind("Example key bind", test_keybind);

                static int test_int_slider = 42;
                Slider("Example slider int", 0, 100, &test_int_slider);

                static float test_float_slider = 56.f;
                Slider("Example slider float", 0.f, 100.f, &test_float_slider);
                
                static string test_textbox = "Your a big fat fucking nigger";
                Textbox("Example text box", test_textbox);
            }
            EndChild();

            SetNextDrawPos(RightPos);

            Child("User Interface", { Size.x, (Size.y / 2) - spacing / 2 });
            EndChild();

            SetNextDrawPosEx({ 0, (Size.y / 2) + spacing / 2 });

            Child("Post Process (Too lazy to add switching)", { Size.x, (Size.y / 2) - spacing / 2 });
            EndChild();

            break;
        }
    }
    EndWindow();

    /*SetNextWindowSize({ 600, 490 });
    if (Window(10, "test", true)) {

    }
    EndWindow();*/
};