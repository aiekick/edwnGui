#include "../../EGui.hh"

EGUI_API bool EGuiMain::Checkbox(const char* title, bool* state)
{
	Vec2 Area = Vec2({ 10 + drawList.GetTextSize(drawList.Verdana, title).x + 6, 10 });
	if (Input.ButtonBehaviour(NextDrawPos, Area, PRESS))
		*state = !(*state);

	if (*state)
		drawList.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.MenuTheme, 3);
	else
		drawList.FilledRectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBackColor, 3);

	drawList.Rectangle(NextDrawPos, { 10, 10 }, EGuiColors.ElementBorderColor, 3);
	drawList.Text(title, NextDrawPos + Vec2(17, -1), LEFT, drawList.Verdana, false, EGuiColors.TextColor);

	SetNextDrawPosEx({0, 12 + EGuiStyle.Padding});

	return true;
}