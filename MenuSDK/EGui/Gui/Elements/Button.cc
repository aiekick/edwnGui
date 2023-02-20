#include "../../EGui.hh"

EGUI_API bool EGuiMain::Button(const char* title, Vec2 Size) {
	if (Size == Vec2(0, 0))
		Size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 };

	bool pressed = false;

	auto OriginalPos = GetNextDrawPos();
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (Input.ButtonBehaviour(NextDrawPos, Size, PRESS))
		pressed = true;

	drawList.Sprite(drawList.BackgroundTexture, NextDrawPos, Size);
	drawList.Rectangle(NextDrawPos, Size, pressed ? EGuiColors.MenuTheme : EGuiColors.ElementBorderColor);
	drawList.Text(title, NextDrawPos + Vec2(Size.x / 2, 2), CENTER, drawList.Verdana, false, EGuiColors.TextColor);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({0, 18 + EGuiStyle.Padding });

	return pressed;
}