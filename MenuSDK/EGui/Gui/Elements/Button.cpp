#include "../../EGui.hpp"

static std::map<int, float> button_press_alpha;
bool EGuiMain::Button(const char* title, Vec2 Size) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	//dynamic size, gheto I know lol.
	if (Size == Vec2(0, 0))
		Size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 };

	auto OriginalPos = GetNextDrawPos();
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	//handle input
	bool pressed = Input.ButtonBehaviour(NextDrawPos, Size, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h));

	//handle animation
	button_press_alpha[GetItemIdentifier()] = pressed ? 255.f : Math.Clamp(Animations.lerp(button_press_alpha[GetItemIdentifier()], 0.f, timing.getDeltaTime() * 4), 0.f, 255.f);

	//render element
	if (Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h))) {
		renderer.FilledRectangle(NextDrawPos, Size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
		renderer.Rectangle(NextDrawPos, Size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

		renderer.PushAlpha(button_press_alpha[GetItemIdentifier()]);
		renderer.Rectangle(NextDrawPos, Size, EGuiColors.Accent, EGuiStyle.ElementRounding);
		renderer.PopAlpha();

		renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2(Size.x / 2, 2), EGuiColors.TextColor, CENTER);
	}

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({0, Size.y + EGuiStyle.Padding });

	return pressed;
}