#include "Egui.hpp"

guiStyle EGuiStyle;
guiColors EGuiColors;
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

// for demo wnd.
enum {
    HOME = 0,
    LEGIT,
    RAGE,
    DEFENSE,
    VISUALS,
    MISC
};

float Grav = 9.81f;
void EGuiMain::DemoPhysics(Object* object) {
    float Gravity = Grav * object->Size * object->Weight;

    // apply gravity to the block
    object->Velocity.y += Gravity * timing.getDeltaTime();

    // apply friction to the block
    float friction = object->Friction * object->Velocity.x;
    object->Velocity.x -= friction * timing.getDeltaTime();

    object->Position.x += object->Velocity.x * timing.getDeltaTime();
    object->Position.y += object->Velocity.y * timing.getDeltaTime();

    object->Position.x = std::clamp(object->Position.x, 0.f, wnd.GetWindowSize().x - object->Size * 2);
    object->Position.y = std::clamp(object->Position.y, 0.f, wnd.GetWindowSize().y - object->Size * 2);

    if (object->Position.y >= wnd.GetWindowSize().y - object->Size * 2 - 1 && object->Velocity.y <= 0.5f)
        object->Velocity.x -= object->Friction * object->Velocity.x;

    if (object->Position.x >= wnd.GetWindowSize().x - object->Size * 2 || object->Position.x <= 0)
        object->Velocity.x = -object->Velocity.x * object->BounceFactor;

    if (object->Position.y >= wnd.GetWindowSize().y - object->Size * 2 || object->Position.y <= 0)
        object->Velocity.y = -object->Velocity.y * object->BounceFactor;

    renderer.FilledCircle(object->Position, object->Size, object->Color);
}

void EGuiMain::DemoShapes() {
    renderer.FilledCircle({ 10, 10 }, 50, { 255, 255, 255, 255 });
    //renderer.FilledRectangle({ 10, 10 }, { 100, 100 }, { 255, 255, 255, 255 }, 25);
}

void EGuiMain::DemoWindow() {
    Vec2 Menu_Size = { 750, 490 };
    SetNextWindowPos({ wnd.GetWindowSize().x / 2 - Menu_Size.x / 2, wnd.GetWindowSize().y / 2 - Menu_Size.y / 2 });
    SetNextWindowSize({ 650, 390 }, Menu_Size);

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
                static bool test_checkbox = false;
                Checkbox("Enable", &test_checkbox);

                static bool test_keybind = false;
                Keybind("test bind", &test_keybind);

                if (test_keybind)
                    renderer.Text(renderer.Verdana, "Key on", { 2, 2 }, {255, 255, 255, 255}, LEFT);

                /*static Color test_color = { 255, 0, 0, 255 };
                ColorPicker("Test color picker", &test_color, true);*/

                static float test_float_slider = 50.f;
                Slider("Test slider", 0.f, 100.f, &test_float_slider, " splatzys");

                static std::string test_textbox = "monkey balls 420";
                Textbox("Example text_box", test_textbox);

                static int button_press_count = 0;
                if (Button("test button"))
                    button_press_count++;

                Label((std::string("Button press count: ") + std::to_string(button_press_count)).c_str());

                static int combo_selected = 0;
                static const char* items[] = { "test", "test 1" };
                //Combobox("Test combo", &combo_selected, items);


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