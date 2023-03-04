#include "../../EGui.hpp"

bool EGuiMain::Listbox(const char* title, int* selected, const char* items[]) {
	// Keep track of whether the selected value has changed.
    bool value_changed = false;

    // Calculate size of listbox button.
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    // Calculate unique ID for listbox menu.
    int id = 1 + sizeof(title) + sizeof(items);

    auto OriginalPos = GetNextDrawPos();
    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    // Draw listbox.
    renderer.Sprite(renderer.BackgroundTexture, NextDrawPos, Size);
    renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor);
    renderer.Text(title, NextDrawPos + Vec2(Size.x / 2, 2), CENTER, renderer.Verdana, false, EGuiColors.TextColor);

	// Calculate size of menu and draw menu background.
    renderer.FilledRectangle(NextDrawPos + Vec2(0, Size.y), Size + Vec2(0, Size.y * (sizeof(items) - 3)), EGuiColors.ElementBackColor);

    if (Input.IsMouseHoveringRect(NextDrawPos, Size + Vec2(0, Size.y * (sizeof(items) - 2))))
        renderer.Rectangle(NextDrawPos, Size + Vec2(0, Size.y * (sizeof(items) - 2)), EGuiColors.MenuTheme);
    else
        renderer.Rectangle(NextDrawPos, Size + Vec2(0, Size.y * (sizeof(items) - 2)), EGuiColors.ElementBorderColor);

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

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below list menu.
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding + (Size.y * (sizeof(items) - 2)) });

    // Return whether the selected value has changed.
    return value_changed;
}