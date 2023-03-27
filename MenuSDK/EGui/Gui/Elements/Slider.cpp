#include "../../EGui.hpp"
#include <string>

static std::unordered_map<int, bool> dragging;
static std::unordered_map<int, float> slider_x;

bool EGuiMain::Slider(const char* title, int min, int max, int* currentValue, const char* format) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 text_size = renderer.GetTextSize(Fonts.Primary, title);
	Vec2 slider_size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 10 };
	Vec2 button_size = renderer.GetTextSize(Fonts.Primary, "+");

	text_size.y += 2;

	Vec2 OriginalPos = NextDrawPos;
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (!dragging[GetItemIdentifier()] && Input.IsKeyDown(VK_LBUTTON) && Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, text_size.y), slider_size))
		dragging[GetItemIdentifier()] = true;
	else if (dragging[GetItemIdentifier()] && !Input.IsKeyDown(VK_LBUTTON))
		dragging[GetItemIdentifier()] = false;

	bool hovering = Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, text_size.y), slider_size);

	if ((hovering && Input.IsKeyPressed(VK_LEFT)) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), button_size, PRESS))
		*currentValue = *currentValue - 1;
	else if ((hovering && Input.IsKeyPressed(VK_RIGHT)) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), button_size, PRESS))
		*currentValue = *currentValue + 1;

	if (dragging[GetItemIdentifier()]) {
		int formula = (Input.GetMousePos().x - GetNextDrawPos().x);
		*currentValue = Math.Map(formula, 0, (int)slider_size.x, min, max + 1);
	}

	*currentValue = std::clamp(*currentValue, min, max);

	std::string Value = std::to_string(*currentValue);
	Value.append(format);

	slider_x[GetItemIdentifier()] = clamp(Animations.lerp(slider_x[GetItemIdentifier()], Math.Map(*currentValue, min, max, 0, (int)slider_size.x), timing.getDeltaTime() * 8), 0.f, slider_size.x);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
	renderer.Text(Fonts.Primary, title, NextDrawPos, EGuiColors.TextColor, LEFT);

	renderer.Text(Fonts.Primary, "-", NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), EGuiColors.TextColor, LEFT);
	renderer.Text(Fonts.Primary, "+", NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), EGuiColors.TextColor, LEFT);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), slider_size.y), EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	renderer.Rectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	renderer.FilledCircle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.MenuTheme);
	renderer.Circle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.ElementBorderColor);

	renderer.Text(Fonts.Primary, Value.c_str(), NextDrawPos + Vec2(slider_size.x, 0), EGuiColors.TextColor, RIGHT);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({ 0, text_size.y + slider_size.y + EGuiStyle.Padding });

	return true;
}

bool EGuiMain::Slider(const char* title, float min, float max, float* currentValue, const char* format) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 text_size = renderer.GetTextSize(Fonts.Primary, title);
	Vec2 slider_size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 10 };
	Vec2 button_size = renderer.GetTextSize(Fonts.Primary, "+");

	text_size.y += 2;

	auto OriginalPos = NextDrawPos;
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (!dragging[GetItemIdentifier()] && Input.IsKeyDown(VK_LBUTTON) && Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, text_size.y), slider_size))
		dragging[GetItemIdentifier()] = true;
	else if (dragging[GetItemIdentifier()] && !Input.IsKeyDown(VK_LBUTTON))
		dragging[GetItemIdentifier()] = false;

	bool hovering = Input.IsMouseHoveringRect(NextDrawPos + Vec2(0, text_size.y), slider_size);

	if (hovering)
		Popup("Hovering slider!", Input.GetMousePos(), EGuiColors.TextColor);

	if ((hovering && Input.IsKeyPressed(VK_LEFT)) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), button_size, PRESS))
		*currentValue = *currentValue - 1.f;
	else if ((hovering && Input.IsKeyPressed(VK_RIGHT)) || Input.ButtonBehaviour(NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), button_size, PRESS))
		*currentValue = *currentValue + 1.f;

	if (dragging[GetItemIdentifier()]) {
		int formula = (Input.GetMousePos().x - GetNextDrawPos().x);
		*currentValue = Math.Map(formula, 0, (int)slider_size.x, (int)min, (int)max + 1);
	}

	*currentValue = std::clamp(*currentValue, min, max);

	std::string Value = std::to_string((int) *currentValue);
	Value.append(format);

	slider_x[GetItemIdentifier()] = clamp(Animations.lerp(slider_x[GetItemIdentifier()], Math.Map((int)*currentValue, (int)min, (int)max, 0, (int)slider_size.x), timing.getDeltaTime() * 8), 0.f, slider_size.x);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
	renderer.Text(Fonts.Primary, title, NextDrawPos, EGuiColors.TextColor, LEFT);

	renderer.Text(Fonts.Primary, "-", NextDrawPos + Vec2(text_size.x + EGuiStyle.Padding, 0), EGuiColors.TextColor, LEFT);
	renderer.Text(Fonts.Primary, "+", NextDrawPos + Vec2(text_size.x + (EGuiStyle.Padding / 2) * 2 + button_size.x, 0), EGuiColors.TextColor, LEFT);

	renderer.FilledRectangle(NextDrawPos + Vec2(0, text_size.y), Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), slider_size.y), EGuiColors.MenuTheme, EGuiStyle.ElementRounding);
	renderer.Rectangle(NextDrawPos + Vec2(0, text_size.y), slider_size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	renderer.FilledCircle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.MenuTheme);
	renderer.Circle(NextDrawPos + Vec2(-7, text_size.y - 2) + Vec2(clamp(slider_x[GetItemIdentifier()], 8.f, (float)INT_MAX), 0), 7, EGuiColors.ElementBorderColor);

	renderer.Text(Fonts.Primary, Value.c_str(), NextDrawPos + Vec2(slider_size.x, 0), EGuiColors.TextColor, RIGHT);

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({ 0, text_size.y + slider_size.y + EGuiStyle.Padding });

	return true;
}