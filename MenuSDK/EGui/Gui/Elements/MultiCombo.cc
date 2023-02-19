#include "../../EGui.hh"

EGUI_API bool EGuiMain::MultiCombobox(const char* title, std::vector<bool>& selected, const char* items[]) {
    SetItemIdentifier(GetItemIdentifier() + 1);

    static bool this_state[] = { false };
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    if (this_state[GetItemIdentifier()] != (true || false))
        this_state[GetItemIdentifier()] = false;

    auto OriginalPos = GetNextDrawPos();
    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
        this_state[GetItemIdentifier()] = !this_state[GetItemIdentifier()];

    drawList.Sprite(drawList.BackgroundTexture, NextDrawPos, Size);

    drawList.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor);
    drawList.Text(title, NextDrawPos + Vec2(Size.x / 2, 2), CENTER, drawList.Verdana, true, EGuiColors.TextColor);

    if (this_state[GetItemIdentifier()]) {
        drawList.FilledRectangle(NextDrawPos + Vec2(0, Size.y), Size + Vec2(0, Size.y * (sizeof(items) - 3)), EGuiColors.ElementBackColor);
        drawList.Rectangle(NextDrawPos, Size + Vec2(0, Size.y * (sizeof(items) - 2)), EGuiColors.MenuTheme);
        // Resize the selected vector to the correct size.
        selected.resize(sizeof(items) - 2);

        for (size_t item_index = 0; item_index < sizeof(items) - 2; ++item_index) {
            if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, Size.y * (item_index + 1)), Size, PRESS))
                selected[item_index] = !selected[item_index];

            drawList.Text(items[item_index], NextDrawPos + Vec2(Size.x / 2, 2 + Size.y + (Size.y * item_index)), CENTER, drawList.Verdana, true, selected[item_index] ? EGuiColors.MenuTheme : Color(255, 255, 255, 255));
        }
    }

    SetNextDrawPos(OriginalPos);
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding + (this_state[GetItemIdentifier()] ? Size.y * (sizeof(items) - 2) : 0) });

    return true;
}