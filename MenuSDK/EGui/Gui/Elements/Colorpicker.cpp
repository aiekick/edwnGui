#include "../../EGui.hpp"

bool EGuiMain::ColorPicker(const char* title, Color* selected) {
    SetItemIdentifier(GetItemIdentifier() + 1);

    // this_state is an array of booleans that will store the open/closed state of the color picker for each instance.
    static bool this_state[] = { false };

    // Keep track of whether the selected color has changed.
    bool value_changed = false;

    // Calculate size of color picker button.
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    // Calculate unique ID for color picker.

    // Ensure open state is either true or false.
    if (this_state[GetItemIdentifier()] != (true || false))
        this_state[GetItemIdentifier()] = false;

    // Save current draw position.
    auto OriginalPos = GetNextDrawPos();

    // Set next draw position to right of color picker button.
    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    // Toggle open state of color picker when button is pressed.
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
        this_state[GetItemIdentifier()] = !this_state[GetItemIdentifier()];

    // Draw color picker button.
    renderer.Sprite(renderer.BackgroundTexture, NextDrawPos, Size);
    renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor);
    renderer.Text(title, NextDrawPos + Vec2(Size.x / 2, 2), CENTER, renderer.Verdana, true, EGuiColors.TextColor);

    // Draw the selected color on the button
    renderer.FilledRectangle(NextDrawPos + Vec2(Size.x - 10, 2), Vec2(8, 14), *selected);

    // If color picker is open, draw color picker.
    if (this_state[GetItemIdentifier()]) {
        // Calculate size of color picker and draw color picker background.
        renderer.FilledRectangle(NextDrawPos + Vec2(0, Size.y), Size + Vec2(0, Size.y * 2), EGuiColors.ElementBackColor);
        renderer.Rectangle(NextDrawPos, Size + Vec2(0, Size.y * 2), EGuiColors.MenuTheme);

        // Draw color grid
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                // here you can define your own logic to generate the color grid, 
                // for example you could use a color wheel algorithm, or a gradient of colors
                Color color = Color(i * 12, j * 12, 255);
                renderer.FilledRectangle(NextDrawPos + Vec2(i * 10, Size.y + j * 10), Vec2(10, 10), color);
                if (Input.ButtonBehaviour(NextDrawPos + Vec2(i * 10, Size.y + j * 10), Vec2(10, 10), PRESS)) {
                    *selected = color;
                    value_changed = true;
                }
            }
        }
        //Correction for background elements.
        DisableInputArea = Rect(NextDrawPos.x, NextDrawPos.y + Size.y, Size.x, Size.y * 2);
    }

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below color picker.
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding + (this_state[GetItemIdentifier()] ? Size.y * 2 : 0) });

    // If the selected color has changed, toggle the open state of the color picker.
    if (value_changed)
        this_state[GetItemIdentifier()] = !this_state[GetItemIdentifier()];

    // Return whether the selected color has changed.
    return value_changed;
}