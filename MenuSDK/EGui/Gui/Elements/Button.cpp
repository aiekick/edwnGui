#include "../../EGui.hpp"

static std::map<int, float> button_press_alpha;

bool EGuiMain::Button(const char* title, Vec2 Size) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	if (Size == Vec2(0, 0))
		Size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 };

	bool pressed = false;

	auto OriginalPos = GetNextDrawPos();
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
		pressed = true;

	float delta_time = timing.getDeltaTime();

	if (pressed)
		button_press_alpha[GetItemIdentifier()] = 255;
	else
		button_press_alpha[GetItemIdentifier()] = clamp(Animations.lerp(button_press_alpha[GetItemIdentifier()], 0.f, delta_time * 4), 0.f, 255.f);

	renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
	renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	renderer.PushAlpha(button_press_alpha[GetItemIdentifier()]);
	renderer.Rectangle(NextDrawPos, Size, EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	renderer.PopAlpha();

	renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2(Size.x / 2, 2), EGuiColors.TextColor, CENTER);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({0, Size.y + EGuiStyle.Padding });

	return pressed;
}