#include <codecvt>

#include "../../EGui.hh"

EGUI_API bool EGuiMain::Textbox(const char* title, std::string &str) {
    SetItemIdentifier(GetItemIdentifier() + 1);

    // Keep track of whether we are typing or not.
    static std::unordered_map<int, bool> typing;

    // Keep track of whether the selected value has changed.
    bool value_changed = false;

    // Calculate size of dropdown button.
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    // Save current draw position.
    auto OriginalPos = GetNextDrawPos();

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

    // Draw dropdown button.
    drawList.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
    drawList.Rectangle(NextDrawPos, Size, typing[GetItemIdentifier()] ? EGuiColors.MenuTheme : EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

    drawList.Text((str.empty() && !typing[GetItemIdentifier()]) ? title : str.c_str(), NextDrawPos + Vec2(Size.x / 2, 2), CENTER, drawList.Verdana, false, EGuiColors.TextColor);

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below dropdown menu.
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding });

    // Return whether the selected value has changed.
    return value_changed;
}