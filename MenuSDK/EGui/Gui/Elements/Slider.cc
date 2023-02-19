#include "../../EGui.hh"
#include <string>

static std::unordered_map<int, bool> dragging;

EGUI_API bool EGuiMain::Slider(const char* title, int min, int max, int* currentValue, const char* format) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 text_size = drawList.GetTextSize(drawList.Verdana, title);
	Vec2 slider_size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding),14 };
	Vec2 button_size = drawList.GetTextSize(drawList.Verdana, "+");

	auto OriginalPos = GetNextDrawPos();
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, text_size.y), slider_size, HOLD))
		dragging[GetItemIdentifier()] = true;
	else
		dragging[GetItemIdentifier()] = false;

	bool hovering = Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, 15), slider_size);

	if ((Input.IsKeyPressed(VK_LEFT) && hovering) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), button_size, PRESS))
		*currentValue = *currentValue - 1;
	else if ((Input.IsKeyPressed(VK_RIGHT) && hovering) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), button_size, PRESS))
		*currentValue = *currentValue + 1;

	if (dragging[GetItemIdentifier()]) {
		int formula = (Input.GetMousePos().x - GetNextDrawPos().x);
		*currentValue = utility.map(formula, 0, slider_size.x, min, max + 1);
	}

	*currentValue = std::clamp(*currentValue, min, max);

	std::string Value = std::to_string(*currentValue);
	Value.append(format);

	int slider_x = utility.map(*currentValue, min, max, 0, slider_size.x);

	drawList.Sprite(drawList.BackgroundTexture, NextDrawPos + Vec2(0, 15), slider_size);
	drawList.Text(title, NextDrawPos, LEFT, drawList.Verdana, true, EGuiColors.TextColor);

	drawList.Text("-", NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), LEFT, drawList.Verdana, true, EGuiColors.TextColor);
	drawList.Text("+", NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), LEFT, drawList.Verdana, true, EGuiColors.TextColor);

	drawList.FilledRectangle(NextDrawPos + Vec2(0, 15), Vec2(slider_x, slider_size.y), EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	drawList.Rectangle(NextDrawPos + Vec2(0, 15), slider_size, EGuiColors.ElementBorderColor);

	drawList.Text(Value.c_str(), NextDrawPos + Vec2(slider_size.x, 0), RIGHT, drawList.Verdana, true, EGuiColors.TextColor);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({ 0, slider_size.y * 2 + EGuiStyle.Padding });

	return true;
}

EGUI_API bool EGuiMain::Slider(const char* title, int min, int max, float* currentValue, const char* format) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 text_size = drawList.GetTextSize(drawList.Verdana, title);
	Vec2 slider_size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding),14 };
	Vec2 button_size = drawList.GetTextSize(drawList.Verdana, "+");

	auto OriginalPos = GetNextDrawPos();
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, text_size.y), slider_size, HOLD))
		dragging[GetItemIdentifier()] = true;
	else
		dragging[GetItemIdentifier()] = false;

	bool hovering = Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, 15), slider_size);

	if ((Input.IsKeyPressed(VK_LEFT) && hovering) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), button_size, PRESS))
		*currentValue = *currentValue - 1;
	else if ((Input.IsKeyPressed(VK_RIGHT) && hovering) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), button_size, PRESS))
		*currentValue = *currentValue + 1;

	if (dragging[GetItemIdentifier()]) {
		int formula = (Input.GetMousePos().x - GetNextDrawPos().x);
		*currentValue = utility.map(formula, 0, slider_size.x, min, max + 1);
	}

	*currentValue = std::clamp(*currentValue, float(min), float(max));

	std::string Value = std::to_string((int)* currentValue);
	Value.append(format);

	int slider_x = utility.map(*currentValue, min, max, 0, slider_size.x);

	drawList.FilledRectangle(NextDrawPos + Vec2(0, 15), slider_size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
	drawList.Text(title, NextDrawPos, LEFT, drawList.Verdana, true, EGuiColors.TextColor);

	drawList.Text("-", NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), LEFT, drawList.Verdana, true, EGuiColors.TextColor);
	drawList.Text("+", NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), LEFT, drawList.Verdana, true, EGuiColors.TextColor);

	drawList.FilledRectangle(NextDrawPos + Vec2(0, 15), Vec2(slider_x, slider_size.y), EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	drawList.Rectangle(NextDrawPos + Vec2(0, 15), slider_size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	drawList.Text(Value.c_str(), NextDrawPos + Vec2(slider_size.x, 0), RIGHT, drawList.Verdana, true, EGuiColors.TextColor);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({ 0, slider_size.y * 2 + EGuiStyle.Padding });

	return true;
}