#include "../../EGui.hpp"

static std::map<int, float> checkbox_alpha;
bool EGuiMain::Checkbox(const char* title, bool* state) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 Area = Vec2({ 10 + renderer.GetTextSize(Fonts.Primary, title).x + 6, 10 });

	//handle input
	if (Input.ButtonBehaviour(NextDrawPos, Area, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
		*state = !(*state);

	//handle animatiion
	checkbox_alpha[GetItemIdentifier()] = Math.Clamp(Animations.lerp(checkbox_alpha[GetItemIdentifier()], *state ? 255.f : 0.f, timing.getDeltaTime() * 8), 0.f, 255.f);

	//render element
	if (Input.IsRectInRect(NextDrawPos, Area, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h))) {
		renderer.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBackColor, 3);

		renderer.PushAlpha(checkbox_alpha[GetItemIdentifier()]);
		renderer.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.Accent, 3);
		renderer.PopAlpha();

		renderer.Rectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBorderColor, 3);
		renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2(17, -1), EGuiColors.TextColor, LEFT);
	}

	SetNextDrawPosEx({0, Area.y + EGuiStyle.Padding});

	return true;
}