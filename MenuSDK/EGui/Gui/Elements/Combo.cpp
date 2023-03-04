#include "../../EGui.hpp"

bool EGuiMain::Combobox(const char* title, int* selected, const char* items[]) {
    SetItemIdentifier(GetItemIdentifier() + 1);

    // this_state is an array of booleans that will store the open/closed state of the dropdown for each instance.
    static bool this_state[] = { false };

    // Keep track of whether the selected value has changed.
    bool value_changed = false;

    // Calculate size of dropdown button.
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    // Ensure open state is either true or false.
    if (this_state[GetItemIdentifier()] != (true || false))
        this_state[GetItemIdentifier()] = false;

    // Save current draw position.
    auto OriginalPos = GetNextDrawPos();

    // Set next draw position to right of dropdown button.
    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    // Toggle open state of dropdown menu when button is pressed.
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
        this_state[GetItemIdentifier()] = !this_state[GetItemIdentifier()];

    // Draw dropdown button.
    renderer.Sprite(renderer.BackgroundTexture, NextDrawPos, Size);
    renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor);
    renderer.Text(title, NextDrawPos + Vec2(Size.x / 2, 2), CENTER, renderer.Verdana, false, EGuiColors.TextColor);

    // If dropdown menu is open, draw menu items.
    if (this_state[GetItemIdentifier()]) {
        // Calculate size of menu and draw menu background.
        renderer.FilledRectangle(NextDrawPos + Vec2(0, Size.y), Size + Vec2(0, Size.y * (sizeof(items) - 3)), EGuiColors.ElementBackColor);
        renderer.Rectangle(NextDrawPos, Size + Vec2(0, Size.y * (sizeof(items) - 2)), EGuiColors.MenuTheme);

        // Iterate over menu items.
        for (size_t i = 0; i < sizeof(items) - 2; ++i) {
            // Update selected value when menu item is pressed.
            if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, Size.y * (i + 1)), Size, PRESS)) {
                *selected = i;
                value_changed = true;
            }

            // Draw menu item.
            renderer.Text(items[i], NextDrawPos + Vec2(Size.x / 2, 2 + Size.y + (Size.y * i)), CENTER, renderer.Verdana, false, *selected == i ? EGuiColors.MenuTheme : Color(255, 255, 255, 255));
        }

        //Correction for background elements.
        DisableInputArea = Rect(NextDrawPos.x, NextDrawPos.y + Size.y, Size.x, Size.y + Size.y * (sizeof(items) - 3));
    }

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below dropdown menu.
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding + (this_state[GetItemIdentifier()] ? Size.y * (sizeof(items) - 2) : 0) });

    // If the selected value has changed, toggle the open state of the dropdown menu.
    if (value_changed)
        this_state[GetItemIdentifier()] = !this_state[GetItemIdentifier()];

    // Return whether the selected value has changed.
    return value_changed;
}