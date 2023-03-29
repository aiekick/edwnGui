#include "../../EGui.hpp"

struct ComboInfo {
    bool open;
    float alpha;
    float clip_y;
};

struct ComboRenderInfo {
    std::vector<std::string> items;
    Vec2 pos;
    Vec2 size;
    int selected;
    float alpha;
    float Clip_y;
};

std::map<int, ComboInfo> combo_info;
std::map<int, ComboRenderInfo> combo_render_info;

bool EGuiMain::Combobox(const char* title, int* selected, std::vector<std::string> options) {
    SetItemIdentifier(GetItemIdentifier() + 1);

    // Keep track of whether the selected value has changed.
    bool value_changed = false;

    // Calculate size of dropdown button.
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });

    // Save current draw position.
    auto OriginalPos = GetNextDrawPos();

    // Set next draw position to right of dropdown button.
    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    // Toggle open state of dropdown menu when button is pressed.
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)) && Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
        combo_info[GetItemIdentifier()].open = !combo_info[GetItemIdentifier()].open;
    else if (!Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
        combo_info[GetItemIdentifier()].open = false;

    // Draw dropdown button.
    if (Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h))) {
        renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding, (combo_info[GetItemIdentifier()].open ? CORNER_TOP : CORNER_ALL));
        renderer.Rectangle(NextDrawPos, Size, combo_info[GetItemIdentifier()].open ? EGuiColors.MenuTheme : EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding, (combo_info[GetItemIdentifier()].open ? CORNER_TOP : CORNER_ALL));
        renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2(Size.x / 2, 2), EGuiColors.TextColor, CENTER);
    }

    // If dropdown menu is open, draw menu items.
    if (combo_info[GetItemIdentifier()].open) {
        if (!Input.IsMouseHoveringRect(NextDrawPos, Vec2(Size.x, Size.y * (options.size() + 1))) && Input.IsKeyDown(VK_LBUTTON))
            combo_info[GetItemIdentifier()].open = false;

        // Iterate over menu items.
        for (int i = 0; i < options.size(); ++i) {
            // Update selected value when menu item is pressed.
            if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, Size.y * (i + 1)), Size, PRESS)) {
                *selected = i;

                //close popup
                combo_info[GetItemIdentifier()].open = !combo_info[GetItemIdentifier()].open;
                value_changed = true;
            }
        }

        combo_info[GetItemIdentifier()].alpha = Animations.lerp(combo_info[GetItemIdentifier()].alpha, 255.f, timing.getDeltaTime() * 15);
        combo_info[GetItemIdentifier()].clip_y = Animations.lerp(combo_info[GetItemIdentifier()].clip_y, Size.y + Size.y * (options.size()), timing.getDeltaTime() * 35);

        ComboRenderInfo combo;
        combo.items = options;
        combo.pos = NextDrawPos + Vec2(0, Size.y);
        combo.size = Size;
        combo.selected = *selected;
        combo.alpha = combo_info[GetItemIdentifier()].alpha;
        combo.Clip_y = combo_info[GetItemIdentifier()].clip_y;

        combo_render_info[combo_render_info.size() + 1] = combo;

        DisableInputArea = Rect(NextDrawPos.x, NextDrawPos.y + Size.y, Size.x, Size.y * (options.size() - 1));
    }
    else {
        combo_info[GetItemIdentifier()].alpha = 0.f;
        combo_info[GetItemIdentifier()].clip_y = 0.f;
    }

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below dropdown menu.
    SetNextDrawPosEx({ 0, Size.y + EGuiStyle.Padding });

    // Return whether the selected value has changed.
    return value_changed;
}

void EGuiMain::RenderCombos() {
    for (int i = 1; i <= combo_render_info.size(); i++) {
        std::vector<std::string> options = combo_render_info[i].items;
        Vec2 pos = combo_render_info[i].pos;
        Vec2 size = combo_render_info[i].size;
        int selected = combo_render_info[i].selected;

        renderer.PushClip(pos, Vec2(size.x + 1, combo_render_info[i].Clip_y));
        renderer.PushAlpha(combo_render_info[i].alpha);
        renderer.FilledRectangle(pos, size + Vec2(0, size.y * (options.size() - 1)), EGuiColors.ElementBackColor, EGuiStyle.ElementRounding, CORNER_BOTTOM);
        renderer.Rectangle(pos, size + Vec2(0, size.y * (options.size() - 1)), EGuiColors.MenuTheme, EGuiStyle.ElementRounding, CORNER_BOTTOM);
        
        for (int j = 0; j < options.size(); j++)
            renderer.Text(Fonts.Primary, options[j].c_str(), pos + Vec2(size.x / 2, 2 + (size.y * (j))), selected == j ? EGuiColors.MenuTheme : Color(255, 255, 255, 255), CENTER);

        renderer.PopClip();
        renderer.PopAlpha();
    }

    //clear.
    combo_render_info.clear();
}