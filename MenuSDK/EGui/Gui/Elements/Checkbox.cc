#include "../../EGui.hh"

EGUI_API bool EGuiMain::Checkbox(const char* title, bool* state)
{
	Vec2 Area = Vec2({ 12 + drawList.GetTextSize(drawList.Verdana, title).x + 6, 12 });
	if (Input.ButtonBehaviour(NextDrawPos, Area, PRESS))
		*state = !(*state);

	if (*state)
		drawList.FilledRectangle(NextDrawPos, { 12, 12 }, EGuiColors.MenuTheme, 3);
	else
		drawList.FilledRectangle(NextDrawPos, { 12, 12 }, EGuiColors.ElementBackColor, 3);

	drawList.Rectangle(NextDrawPos, { 12, 12 }, EGuiColors.ElementBorderColor, 3);
	drawList.Text(title, NextDrawPos + Vec2(16, -1), LEFT, drawList.Verdana, true, EGuiColors.TextColor);

	SetNextDrawPosEx({0, 12 + EGuiStyle.Padding});

	return true;
}