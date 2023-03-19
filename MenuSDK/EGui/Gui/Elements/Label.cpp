#include "../../EGui.hpp"

bool EGuiMain::Label(const char* str, Color custom_color) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 Position = GetNextDrawPos() + Vec2(12 + EGuiStyle.Padding, 0);
	Vec2 TextSize = renderer.GetTextSize(renderer.Verdana, str);

	bool pressed = Input.ButtonBehaviour(Position, TextSize, PRESS);

	if (pressed)
		custom_color = EGuiColors.MenuTheme;

	renderer.Text(renderer.Verdana, str, Position, custom_color, LEFT);

	SetNextDrawPosEx({ 0, TextSize.y + EGuiStyle.Padding });

	return pressed;
}