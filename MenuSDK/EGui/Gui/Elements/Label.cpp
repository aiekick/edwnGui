#include "../../EGui.hpp"

bool EGuiMain::Label(const char* str, Color custom_color) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 Position = GetNextDrawPos() + Vec2(12 + EGuiStyle.Padding, 0);
	Vec2 TextSize = renderer.GetTextSize(Fonts.Primary, str);

	if (Input.IsRectInRect(Position, TextSize, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
		renderer.Text(Fonts.Primary, str, Position, custom_color, LEFT);

	SetNextDrawPosEx({ 0, TextSize.y + EGuiStyle.Padding });

	return true;
}