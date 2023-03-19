#include "../../EGui.hpp"

static std::unordered_map<int, float> checkbox_alpha;
bool EGuiMain::Checkbox(const char* title, bool* state) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 Area = Vec2({ 10 + renderer.GetTextSize(renderer.Verdana, title).x + 6, 10 });
	if (Input.ButtonBehaviour(NextDrawPos, Area, PRESS))
		*state = !(*state);

	float delta_time = timing.getDeltaTime();

	if (*state)
		checkbox_alpha[GetItemIdentifier()] = clamp(Animations.lerp(checkbox_alpha[GetItemIdentifier()], 255.f, delta_time * 8), 0.f, 255.f);
	else
		checkbox_alpha[GetItemIdentifier()] = clamp(Animations.lerp(checkbox_alpha[GetItemIdentifier()], 0.f, delta_time * 8), 0.f, 255.f);

	renderer.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBackColor, 3);

	renderer.PushAlpha(checkbox_alpha[GetItemIdentifier()]);
	renderer.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.MenuTheme, 3);
	renderer.PopAlpha();

	renderer.Rectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBorderColor, 3);
	renderer.Text(renderer.Verdana, title, NextDrawPos + Vec2(17, -1), EGuiColors.TextColor, LEFT);

	SetNextDrawPosEx({0, Area.y + EGuiStyle.Padding});

	return true;
}