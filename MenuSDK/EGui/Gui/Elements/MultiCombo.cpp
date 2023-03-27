#include "../../EGui.hpp"

struct MultiComboInfo {
    bool open;
    float alpha;
    float clip_y;
};

struct MultiComboRenderInfo {
    std::vector<std::string> items;
    Vec2 pos;
    Vec2 size;
    std::vector<bool> selected;
    float alpha;
    float Clip_y;
};

std::map<int, MultiComboInfo> multicombo_info;
std::map<int, MultiComboRenderInfo> multicombo_render_info;

bool EGuiMain::MultiCombobox(const char* title, std::vector<bool>& selected, std::vector<std::string> options) {
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
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
        multicombo_info[GetItemIdentifier()].open = !multicombo_info[GetItemIdentifier()].open;

    // Draw dropdown button.
    renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding, (multicombo_info[GetItemIdentifier()].open ? CORNER_TOP : CORNER_ALL));
    renderer.Rectangle(NextDrawPos, Size, multicombo_info[GetItemIdentifier()].open ? EGuiColors.MenuTheme : EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding, (multicombo_info[GetItemIdentifier()].open ? CORNER_TOP : CORNER_ALL));

    if (multicombo_info[GetItemIdentifier()].open) {
        if (!Input.IsMouseHoveringRect(NextDrawPos, Vec2(Size.x, Size.y * (options.size() + 1))) && Input.IsKeyDown(VK_LBUTTON))
            multicombo_info[GetItemIdentifier()].open = false;

        selected.resize(options.size());

        for (int item_index = 0; item_index < options.size(); ++item_index) {
            if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, Size.y * (item_index + 1)), Size, PRESS))
                selected[item_index] = !selected[item_index];
        }

        multicombo_info[GetItemIdentifier()].alpha = Animations.lerp(multicombo_info[GetItemIdentifier()].alpha, 255.f, timing.getDeltaTime() * 15);
        multicombo_info[GetItemIdentifier()].clip_y = Animations.lerp(multicombo_info[GetItemIdentifier()].clip_y, Size.y + Size.y * (options.size()), timing.getDeltaTime() * 35);

        MultiComboRenderInfo combo;
        combo.items = options;
        combo.pos = NextDrawPos + Vec2(0, Size.y);
        combo.size = Size;
        combo.selected = selected;
        combo.alpha = multicombo_info[GetItemIdentifier()].alpha;
        combo.Clip_y = multicombo_info[GetItemIdentifier()].clip_y;

        multicombo_render_info[multicombo_render_info.size() + 1] = combo;

        DisableInputArea = Rect(NextDrawPos.x, NextDrawPos.y + Size.y, Size.x, Size.y * (options.size() - 1));
    }
    else {
        multicombo_info[GetItemIdentifier()].alpha = 0.f;
        multicombo_info[GetItemIdentifier()].clip_y = 0.f;
    }

    // Draw text
    renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2(Size.x / 2, 2), EGuiColors.TextColor, CENTER);

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below dropdown menu.
    SetNextDrawPosEx({ 0, Size.y + EGuiStyle.Padding });

    // Return whether the selected value has changed.
    return value_changed;
}

void EGuiMain::RenderMultiCombos() {
    for (int i = 1; i <= multicombo_render_info.size(); i++) {
        std::vector<std::string> options = multicombo_render_info[i].items;
        Vec2 pos = multicombo_render_info[i].pos;
        Vec2 size = multicombo_render_info[i].size;
        std::vector<bool> selected = multicombo_render_info[i].selected;

        renderer.PushClip(pos, Vec2(size.x + 1, multicombo_render_info[i].Clip_y));
        renderer.PushAlpha(multicombo_render_info[i].alpha);
        renderer.FilledRectangle(pos, size + Vec2(0, size.y * (options.size() - 1)), EGuiColors.ElementBackColor, EGuiStyle.ElementRounding, CORNER_BOTTOM);
        renderer.Rectangle(pos, size + Vec2(0, size.y * (options.size() - 1)), EGuiColors.MenuTheme, EGuiStyle.ElementRounding, CORNER_BOTTOM);

        for (int j = 0; j < options.size(); j++)
            renderer.Text(Fonts.Primary, options[j].c_str(), pos + Vec2(size.x / 2, 2 + (size.y * (j))), selected[j] ? EGuiColors.MenuTheme : Color(255, 255, 255, 255), CENTER);

        renderer.PopClip();
        renderer.PopAlpha();
    }

    //clear.
    multicombo_render_info.clear();
}