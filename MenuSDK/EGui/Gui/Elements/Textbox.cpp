#include <codecvt>

#include "../../EGui.hpp"

struct Textbox_info {
    bool typing;
    float outline_alpha;

    //typing bar near the end of the text.
    bool bar;
    float bar_time;
    float bar_x;
};

static std::map<int, Textbox_info> textbox_info;

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
    if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
        textbox_info[GetItemIdentifier()].typing = !textbox_info[GetItemIdentifier()].typing;

    if ((Input.IsKeyDown(VK_LBUTTON) && !Input.IsMouseHoveringRect(NextDrawPos, Size)) || !Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
        textbox_info[GetItemIdentifier()].typing = false;

    if (textbox_info[GetItemIdentifier()].typing) {
        for (int i = 32; i <= 222; i++) {
            if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
                continue;

            if (i > 57 && i <= 90) {
                if (Input.IsKeyPressed(i)) {
                    str += (Input.IsKeyDown(VK_SHIFT) || GetKeyState(VK_CAPITAL)) ? static_cast<char>(i) : static_cast<char>(i + 32);
                    value_changed = true;
                }
            }
            else {
                if (Input.IsKeyPressed(i)) {
                    for (int j = 0; j < sizeof(special_characters); j++) {
                        if (special_characters[j].vk_key == i) {
                            str += (Input.IsKeyDown(VK_SHIFT) || GetKeyState(VK_CAPITAL)) ? special_characters[j].upper : special_characters[j].regular;
                            value_changed = true;
                        }
                    }
                }
            }
        }

		if (Input.IsKeyPressed(VK_BACK) && !str.empty()) {
            Input.IsKeyDown(VK_CONTROL) ? str.clear() : str.pop_back();
            value_changed = true;
		}

        if (Input.IsKeyPressed(VK_ESCAPE) || Input.IsKeyPressed(VK_RETURN))
            textbox_info[GetItemIdentifier()].typing = !textbox_info[GetItemIdentifier()].typing;
    }

    float delta_time = timing.getDeltaTime();

    if (textbox_info[GetItemIdentifier()].typing)
        textbox_info[GetItemIdentifier()].outline_alpha = Math.Clamp(Animations.lerp(textbox_info[GetItemIdentifier()].outline_alpha, 255.f, delta_time * 8), 0.f, 255.f);
    else
        textbox_info[GetItemIdentifier()].outline_alpha = Math.Clamp(Animations.lerp(textbox_info[GetItemIdentifier()].outline_alpha, 0.f, delta_time * 8), 0.f, 255.f);

    if (Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h))) {
        renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
        renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

        renderer.PushAlpha(textbox_info[GetItemIdentifier()].outline_alpha);
        renderer.Rectangle(NextDrawPos, Size, EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
        renderer.PopAlpha();
    }

    //change this because it needs to work with custom menu sizes and is just shit lol. @zoiak shit code bro.
    std::string temp = str;
    if (str.length() > 53U) //This can vary depending on your textbox size, make this dynamic in the future.
        temp = str.substr(0U, 53U).append(("..."));

    Vec2 TextSize = renderer.GetTextSize(Fonts.Primary, temp.c_str());

    //Typing bar
    if (textbox_info[GetItemIdentifier()].typing) {
        if (textbox_info[GetItemIdentifier()].bar_x == NULL)
            textbox_info[GetItemIdentifier()].bar_x = TextSize.x;

        textbox_info[GetItemIdentifier()].bar_x = Animations.lerp(textbox_info[GetItemIdentifier()].bar_x, TextSize.x, timing.getDeltaTime() * 8);

        if (timing.getRealTime() >= textbox_info[GetItemIdentifier()].bar_time + 0.5f) {
            textbox_info[GetItemIdentifier()].bar_time = timing.getRealTime();
            textbox_info[GetItemIdentifier()].bar = !textbox_info[GetItemIdentifier()].bar;
        }

        if (value_changed) {
            textbox_info[GetItemIdentifier()].bar_time = timing.getRealTime();
            textbox_info[GetItemIdentifier()].bar = true;
        }

        if (textbox_info[GetItemIdentifier()].bar)
            renderer.Line(NextDrawPos + Vec2(4 + textbox_info[GetItemIdentifier()].bar_x, 4), NextDrawPos + Vec2(4 + textbox_info[GetItemIdentifier()].bar_x, 4 + TextSize.y), EGuiColors.TextColor);
    
        //same as the else statement but when you type or delete text its animated using clipping.
        renderer.PushClip(NextDrawPos + Vec2(5, 4), Vec2(textbox_info[GetItemIdentifier()].bar_x, TextSize.y));
        renderer.Text(Fonts.Primary, (str.empty() && !textbox_info[GetItemIdentifier()].typing) ? title : temp.c_str(), NextDrawPos + Vec2(4, 4), EGuiColors.TextColor, LEFT);
        renderer.PopClip();
    }
    else if (Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
        renderer.Text(Fonts.Primary, (str.empty() && !textbox_info[GetItemIdentifier()].typing) ? title : temp.c_str(), NextDrawPos + Vec2(4, 4), EGuiColors.TextColor, LEFT);

    // Restore original draw position.
    SetNextDrawPos(OriginalPos);

    // Set next draw position below dropdown menu.
    SetNextDrawPosEx({ 0, 18 + EGuiStyle.Padding });

    // Return whether the selected value has changed.
    return value_changed;
}