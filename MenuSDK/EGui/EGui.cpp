#include "Egui.hpp"

guiStyle EGuiStyle;
guiColors EGuiColors;
EGuiDebugWindow DebugWindow;
EGuiMain EGui;

std::unordered_map<int, Vec2> EGuiMain::MenuPos;
std::unordered_map<int, Vec2> EGuiMain::MenuSize;

void EGuiMain::Begin() {
    graphics.Create();
    renderer.CreateObjects();
}

void EGuiMain::PreRender() {
    //we need to do this in order to maintain animations.
    timing.updateDeltaTime();
    graphics.Begin();
}

void EGuiMain::Render() {
    graphics.End();
}

void EGuiMain::End() {
    graphics.Cleanup();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wnd.wc.lpszClassName, wnd.wc.hInstance);
}

bool EGuiMain::InputAreaDisabled() {
    return !(DisableInputArea == Rect(0, 0, 0, 0));
}

void EGuiMain::SetCursor(CursorStyle style)
{
    g_CursorStyle = static_cast<int>(style);
}

// for demo wnd.
enum {
    HOME = 0,
    LEGIT,
    RAGE,
    DEFENSE,
    VISUALS,
    MISC
};

void EGuiMain::DemoPhysics() {
    static Vec2 Block_Pos = { 200, 200 };
    static Vec2 Block_Speed = { 1, 1 };
    static Vec2 Block_Size = { 20, 20 };

    if (Block_Pos.x >= Input.GetWindowSize().x)
        Block_Speed.x = -Block_Speed.x;

    if (Block_Pos.x <= 0)
        Block_Speed.x = -Block_Speed.x;

    if (Block_Pos.y >= Input.GetWindowSize().y)
        Block_Speed.y = -Block_Speed.y;

    if (Block_Pos.y <= 0)
        Block_Speed.y = -Block_Speed.y;

    //Update
    Block_Pos = Block_Pos + Block_Speed;

    renderer.FilledRectangle(Block_Pos, Block_Size, Color(255, 255, 255, 255));
}

void EGuiMain::DemoWindow() {
    SetNextWindowPos({ 100, 100 });
    SetNextWindowSize({ 650, 390 }, { 750, 490 });

    static int tab = 0;

    if (Window(-1, "BotLucky", true)) {
        auto MenuPos = GetWindowPos();
        auto MenuSize = GetWindowSize();

        /* fixes position for rendering & elements */
        SetNextDrawPos(MenuPos);
        SetNextDrawPosEx({ 140, 1 });

        Vec2 TabSize = {(MenuSize.x - 140) / 6, 29};
        
        /* draws and handles tabs */
        if (Tab("HOME", tab == HOME, TabSize)) tab = HOME;
        if (Tab("LEGIT", tab == LEGIT, TabSize)) tab = LEGIT;
        if (Tab("RAGE", tab == RAGE, TabSize)) tab = RAGE;
        if (Tab("DEFENSE", tab == DEFENSE, TabSize)) tab = DEFENSE;
        if (Tab("VISUALS", tab == VISUALS, TabSize)) tab = VISUALS;
        if (Tab("MISC", tab == MISC, TabSize)) tab = MISC;

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

                if (Button("Example button"))
                    printf("Button pressed\n");
            }
            EndChild();

            SetNextDrawPos(RightPos);

            Child("User Interface", { Size.x, (Size.y / 2) - spacing / 2 });
            {

            }
            EndChild();

            SetNextDrawPosEx({ 0, (Size.y / 2) + spacing / 2 });

            Child("Post Process (Too lazy to add switching)", { Size.x, (Size.y / 2) - spacing / 2 });
            {

            }
            EndChild();
            break;
        }
    }
    EndWindow();
};