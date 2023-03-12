#include <codecvt>

#include "../../EGui.hpp"

static std::unordered_map<int, bool> typing;
static std::unordered_map<int, float> textbox_alpha;

bool EGuiMain::Textbox(const char* title, std::string &str) {
    SetItemIdentifier(GetItemIdentifier() + 1);

    // Keep track of whether the selected value has changed.
    bool value_changed = false;

    // Calculate size of dropdown button.
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    // Save current draw position.
    auto OriginalPos = NextDrawPos;

    // Set next draw position to right of dropdown button.
    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    // Handle the typing bool
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
        typing[GetItemIdentifier()] = !typing[GetItemIdentifier()];

    if (Input.IsKeyDown(VK_LBUTTON) && !Input.IsMouseHoveringRect(NextDrawPos, Size))
        typing[GetItemIdentifier()] = false;

    if (typing[GetItemIdentifier()]) {
        for (int i = 32; i <= 222; i++) {
            if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
                continue;

            if (i > 57 && i <= 90) {
                if (Input.IsKeyPressed(i))
                    str += (Input.IsKeyDown(VK_SHIFT) || GetKeyState(VK_CAPITAL)) ? static_cast<char>(i) : static_cast<char>(i + 32);
            }
            else {
                if (Input.IsKeyPressed(i)) {
                    for (int j = 0; j < sizeof(special_characters); j++) {
                        if (special_characters[j].vk_key == i)
                            str += (Input.IsKeyDown(VK_SHIFT) || GetKeyState(VK_CAPITAL)) ? special_characters[j].upper : special_characters[j].regular;
                    }
                }
            }
        }

		if (Input.IsKeyPressed(VK_BACK) && !str.empty()) {
			str.pop_back();
		}

        if (Input.IsKeyPressed(VK_ESCAPE) || Input.IsKeyPressed(VK_RETURN))
            typing[GetItemIdentifier()] = !typing[GetItemIdentifier()];
    }

    float delta_time = timing.getDeltaTime();

    if (typing[GetItemIdentifier()])
        textbox_alpha[GetItemIdentifier()] = clamp(Animations.lerp(textbox_alpha[GetItemIdentifier()], 255.f, delta_time * 8), 0.f, 255.f);
    else
        textbox_alpha[GetItemIdentifier()] = clamp(Animations.lerp(textbox_alpha[GetItemIdentifier()], 0.f, delta_time * 8), 0.f, 255.f);

    // Draw dropdown button.
    renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
    renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

    renderer.PushAlpha(textbox_alpha[GetItemIdentifier()]);
    renderer.Rectangle(NextDrawPos, Size, EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
    renderer.PopAlpha();

    std::string temp = str;
    if (str.length() > 50U) //This can vary depending on your textbox size, make this dynamic in the future.
        temp = str.substr(0U, 50U).append(("..."));

    renderer.Text((str.empty() && !typing[GetItemIdentifier()]) ? title : temp.c_str(), NextDrawPos + Vec2(Size.x / 2, 2), CENTER, renderer.Verdana, false, EGuiColors.TextColor);

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below dropdown menu.
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding });

    // Return whether the selected value has changed.
    return value_changed;
}