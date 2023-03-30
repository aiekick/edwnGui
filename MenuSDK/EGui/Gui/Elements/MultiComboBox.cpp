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

    bool value_changed = false;
    auto Size = Vec2({ GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 });
    auto OriginalPos = GetNextDrawPos();

    SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

    bool should_render = Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h));

    //Handle input
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
        multicombo_info[GetItemIdentifier()].open = !multicombo_info[GetItemIdentifier()].open;
    else if (!should_render)
        multicombo_info[GetItemIdentifier()].open = false;

    //Render drop menu
    if (multicombo_info[GetItemIdentifier()].open) {
        if (!Input.IsMouseHoveringRect(NextDrawPos, Vec2(Size.x, Size.y * (options.size() + 1))) && Input.IsKeyDown(VK_LBUTTON))
            multicombo_info[GetItemIdentifier()].open = false;

        selected.resize(options.size());

        for (int i = 0; i < options.size(); ++i) {
            if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, Size.y * (i + 1)), Size, PRESS))
                selected[i] = !selected[i];
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

    //Render element
    if (should_render) {
        renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding, (multicombo_info[GetItemIdentifier()].open ? CORNER_TOP : CORNER_ALL));
        renderer.Rectangle(NextDrawPos, Size, multicombo_info[GetItemIdentifier()].open ? EGuiColors.Accent : EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding, (multicombo_info[GetItemIdentifier()].open ? CORNER_TOP : CORNER_ALL));
       
        selected.resize(options.size());

        std::string temp = "";
        for (int i = 0; i < options.size(); i++) {
            if (selected[i])
                temp.append(temp.empty() ? options[i] : ", " + options[i]);
        }

        renderer.PushClip(NextDrawPos, Size);
        renderer.Text(Fonts.Primary, (std::string(title) + " -> {" + temp + "}").c_str(), NextDrawPos + Vec2(6, 3), EGuiColors.TextColor, LEFT);
        renderer.PopClip();
    }

    SetNextDrawPos(OriginalPos);
    SetNextDrawPosEx({ 0, Size.y + EGuiStyle.Padding });

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
        renderer.Rectangle(pos, size + Vec2(0, size.y * (options.size() - 1)), EGuiColors.Accent, EGuiStyle.ElementRounding, CORNER_BOTTOM);

        for (int j = 0; j < options.size(); j++) {
            renderer.Text(Fonts.Primary, options[j].c_str(), pos + Vec2(6, 3 + (size.y * (j))), selected[j] ? EGuiColors.Accent : Color(255, 255, 255, 255), LEFT);
            renderer.PopAlpha();

            if (selected[j]) {
                renderer.FilledRectangle(pos + Vec2(1, size.y * (j)), { 2, size.y }, EGuiColors.Accent);
                renderer.Gradient(pos + Vec2(2, size.y * (j)), { size.x - 3, size.y }, EGuiColors.Accent.OverrideAlpha(75), EGuiColors.Accent.OverrideAlpha(0));
            }
        }

        renderer.PopClip();
    }

    //clear.
    multicombo_render_info.clear();
}