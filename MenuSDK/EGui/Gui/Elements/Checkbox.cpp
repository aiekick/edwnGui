#include "../../EGui.hpp"

bool EGuiMain::Checkbox(const char* title, bool* state)
{
	Vec2 Area = Vec2({ 10 + renderer.GetTextSize(renderer.Verdana, title).x + 6, 10 });
	if (Input.ButtonBehaviour(NextDrawPos, Area, PRESS))
		*state = !(*state);

	if (*state)
		renderer.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.MenuTheme, 3);
	else
		renderer.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBackColor, 3);

	renderer.Rectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBorderColor, 3);
	renderer.Text(title, NextDrawPos + Vec2(17, -1), LEFT, renderer.Verdana, false, EGuiColors.TextColor);

	SetNextDrawPosEx({0, 12 + EGuiStyle.Padding});

	return true;
}