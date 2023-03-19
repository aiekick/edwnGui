#include "../../EGui.hpp"

struct ColorData {
	const char* title;
	Vec2 pos;
	Color clr;
	bool alpha_bar;
};

static std::unordered_map<int, ColorData> color_data;
static std::unordered_map<int, bool> color_open;

bool EGuiMain::ColorPicker(const char* title, Color* selected, bool alpha_bar) {
    SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 size = { 25, 10 };
	Vec2 pos = { (GetChildPos().x + GetChildSize().x - EGuiStyle.Padding) - size.x, PreviousDrawPos.y };

	renderer.FilledRectangle(pos, size, *selected, EGuiStyle.ElementRounding);
	renderer.Rectangle(pos, size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	if (Input.ButtonBehaviour(pos, size, PRESS))
		color_open[GetItemIdentifier()] = !color_open[GetItemIdentifier()];

	//create color popup.
	if (color_open[GetItemIdentifier()]) {
		ColorData temp;
		temp.title = title;
		temp.pos = pos + Vec2(size.x + 10, 0);
		temp.clr = *selected;
		temp.alpha_bar = alpha_bar;

		color_data[color_data.size() + 1] = temp;
	}

	return true;
}

void EGuiMain::RenderColorPickers() {
	//render
	for (int i = 1; i <= color_data.size(); i++) {
		Vec2 text_size = renderer.GetTextSize(renderer.Verdana, color_data[i].title);
		Vec2 size = { 200, 175 + text_size.y };

		//frame
		renderer.BorderedRectangle(color_data[i].pos, size, EGuiColors.ChildBgColor, EGuiColors.ElementBorderColor);
		renderer.BorderedRectangle(color_data[i].pos, {size.x, text_size.y + 5}, EGuiColors.ChildHeaderColor, EGuiColors.ElementBorderColor);

		//color gradient
		renderer.Gradient(color_data[i].pos + Vec2(5, text_size.y + 10), size - Vec2(25, 40), Color(255, 255, 255, 255), color_data[i].clr);
		renderer.Gradient(color_data[i].pos + Vec2(5, text_size.y + 10), size - Vec2(25, 40), Color(0, 0, 0, 0), Color(0, 0, 0, 255), true);

		//color bar

		renderer.Rectangle(color_data[i].pos + Vec2(size.x - 15, text_size.y + 10), { 10, size.y - 40 }, Color(0, 0, 0, 255));

		//alpha bar
		renderer.Gradient(color_data[i].pos + Vec2(5, size.y - 15), { size.x - 25, 10 }, Color(0, 0, 0, 255), color_data[i].clr);
		renderer.Rectangle(color_data[i].pos + Vec2(5, size.y - 15), { size.x - 25, 10 }, Color(0, 0, 0, 255));

		//title
		renderer.Text(renderer.Verdana, color_data[i].title, { color_data[i].pos.x + 4, color_data[i].pos.y + 2 }, EGuiColors.TextColor, LEFT);
	}

	//clear.
	color_data.clear();
}