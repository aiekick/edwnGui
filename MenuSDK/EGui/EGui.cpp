#include "Egui.hpp"

guiStyle EGuiStyle;
guiColors EGuiColors;
EGuiMain EGui;

std::map<int, Vec2> EGuiMain::MenuPos;
std::map<int, Vec2> EGuiMain::MenuSize;

void EGuiMain::Begin() {
    Graphics.Create();
    renderer.Create();
}

void EGuiMain::PreRender() {
    //we need to do this in order to maintain animations.
    timing.updateDeltaTime();
    timing.updateFrameRateAbs();
    Graphics.Begin();
}

void EGuiMain::Render() {
    Graphics.End();

    //move this somewhere else. it wont work with an internal cheat lol.
    Input.UpdateMouseWheelDelta(0.f);
}

/* this should ONLY be called if we are not using an external window for testing, as in we are making it render into another processes window. */
void EGuiMain::Update() {
    //we need to do this in order to maintain animations.
    timing.updateDeltaTime();
    timing.updateFrameRateAbs();
}

void EGuiMain::End() {
    Graphics.Cleanup();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wnd.wc.lpszClassName, wnd.wc.hInstance);
}

bool EGuiMain::InputAreaDisabled() {
    return !(DisableInputArea == Rect(0, 0, 0, 0));
}

// for demo wnd.
enum {
    RAGE = 0,
    VISUALS,
    MISC,
    LUA
};

void EGuiMain::DebugProfiler() {
    wnd.SetGraphicsWindowTitle(
          "EGui, [debug profiler] fps: " + std::to_string(timing.getFrameRateAbs()) +
          " Highest fps: " + std::to_string(timing.getFrameRateAbsMax()) +
          " Float Time: " + std::to_string(timing.getRealTime())
    );
}

void EGuiMain::DemoWindow() {
    SetNextWindowPos({ 100, 100 });
    SetNextWindowSize({ 600, 490 }, {600, 490});

    static int tab = 0;

    if (Window(-1, "edwnGui", true)) {
        auto MenuPos = GetWindowPos();
        auto MenuSize = GetWindowSize();

        /* fixes position for rendering & elements */
        SetNextDrawPos(MenuPos);
        SetNextDrawPosEx({ MenuSize.x - 400, 1 });
        
        /* draws and handles tabs */
        if (Tab("Rage", tab == RAGE)) tab = RAGE;
        if (Tab("Visuals", tab == VISUALS)) tab = VISUALS;
        if (Tab("Misc", tab == MISC)) tab = MISC;
        if (Tab("Lua", tab == LUA)) tab = LUA;

        /* sets position for left group */
        SetNextDrawPos(MenuPos);

        float spacing = 15;
        SetNextDrawPosEx({ spacing, 30.f + spacing });

        Vec2 Size = { (MenuSize.x / 2) - spacing * 1.5f, MenuSize.y - (30.f + spacing * 2.f) };
        Vec2 Size2 = { Size.x, (Size.y / 2) - spacing / 2 };

        Vec2 LeftPos = { MenuPos.x + spacing, MenuPos.y + 30 + spacing };
        Vec2 RightPos = { MenuPos.x + Size.x + spacing * 2, MenuPos.y + 30 + spacing };

        /* handle children (NONONO not those type lol, your weird for thinking that) */
        switch (tab) {
        case RAGE:
            Child("Example group 0", Size);
            {
                static bool test_checkbox = false;
                Checkbox("Enable checkbox", &test_checkbox);

                static bool test_keybind = false;
                Keybind("Example keybind", &test_keybind);

                static float test_float_slider = 50.f;
                Slider("Example slider", 0.f, 100.f, &test_float_slider);

                static std::string test_textbox = "monkey balls 420";
                Textbox("Example text box", test_textbox);

                static int button_press_count = 0;
                if (Button("Example button"))
                    button_press_count++;

                Label((std::string("Button press count: ") + std::to_string(button_press_count)).c_str());

                ColorPicker("Example color picker", &EGuiColors.MenuTheme);

                static int combo_selected = 0;
                Combobox("Example combo", &combo_selected, { "test 1", "test 2", "test 3", "test 4", "test 5", "test 6", "test 21", "test 2565" });

                static std::vector<bool> multi_selected = {};
                MultiCombobox("Example mutli combo", multi_selected, { "test 1", "test 2", "test 3", "test 4", "test 5", "test 6", "test 21", "test 2565" });

                static int listbox_selected = 0;
                ListBox("Example List Box", &listbox_selected, {"Item 1" , "Item 2", "Item 3"});
            }
            EndChild();

            SetNextDrawPos(RightPos);

            Child("Example group 1", Size2);
            {
                
            }
            EndChild();

            SetNextDrawPosEx({ 0, (Size.y / 2) + spacing / 2 });

            Child("Example group 2", Size2);
            {
                
            }
            EndChild();
            break;
        }
    }
    EndWindow();
};