#include "../../EGui.hpp"
#include <string>

static std::unordered_map<int, bool> dragging;
static std::unordered_map<int, float> slider_x;

bool EGuiMain::Slider(const char* title, int min, int max, int* currentValue, const char* format) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 text_size = renderer.GetTextSize(renderer.Verdana, title);
	Vec2 slider_size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 10 };
	Vec2 button_size = renderer.GetTextSize(renderer.Verdana, "+");

	text_size.y += 2;

	Vec2 OriginalPos = NextDrawPos;
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, text_size.y), slider_size, HOLD))
		dragging[GetItemIdentifier()] = true;
	else
		dragging[GetItemIdentifier()] = false;

	bool hovering = Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, text_size.y), slider_size);

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

	slider_x[GetItemIdentifier()] = clamp(Animations.lerp(slider_x[GetItemIdentifier()], utility.map(*currentValue, min, max, 0, slider_size.x), timing.getDeltaTime() * 8), 0.f, slider_size.x);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
	renderer.Text(title, NextDrawPos, LEFT, renderer.Verdana, false, EGuiColors.TextColor);

	renderer.Text("-", NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), LEFT, renderer.Verdana, false, EGuiColors.TextColor);
	renderer.Text("+", NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), LEFT, renderer.Verdana, false, EGuiColors.TextColor);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), slider_size.y), EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	renderer.Rectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	renderer.FilledCircle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.MenuTheme);
	renderer.Circle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.ElementBorderColor);

	renderer.Text(Value.c_str(), NextDrawPos + Vec2(slider_size.x, 0), RIGHT, renderer.Verdana, false, EGuiColors.TextColor);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({ 0, text_size.y + slider_size.y + EGuiStyle.Padding });

	return true;
}

bool EGuiMain::Slider(const char* title, float min, float max, float* currentValue, const char* format) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 text_size = renderer.GetTextSize(renderer.Verdana, title);
	Vec2 slider_size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 10 };
	Vec2 button_size = renderer.GetTextSize(renderer.Verdana, "+");

	text_size.y += 2;

	auto OriginalPos = NextDrawPos;
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (Input.ButtonBehaviour(NextDrawPos + Vec2(0, text_size.y), slider_size, HOLD))
		dragging[GetItemIdentifier()] = true;
	else
		dragging[GetItemIdentifier()] = false;

	bool hovering = Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, text_size.y), slider_size);

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

	slider_x[GetItemIdentifier()] = clamp(Animations.lerp(slider_x[GetItemIdentifier()], utility.map(*currentValue, min, max, 0, slider_size.x), timing.getDeltaTime() * 8), 0.f, slider_size.x);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
	renderer.Text(title, NextDrawPos, LEFT, renderer.Verdana, false, EGuiColors.TextColor);

	renderer.Text("-", NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), LEFT, renderer.Verdana, false, EGuiColors.TextColor);
	renderer.Text("+", NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), LEFT, renderer.Verdana, false, EGuiColors.TextColor);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), slider_size.y), EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	renderer.Rectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	renderer.FilledCircle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.MenuTheme);
	renderer.Circle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.ElementBorderColor);

	renderer.Text(Value.c_str(), NextDrawPos + Vec2(slider_size.x, 0), RIGHT, renderer.Verdana, false, EGuiColors.TextColor);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({ 0, text_size.y + slider_size.y + EGuiStyle.Padding });

	return true;
}