#include "../../EGui.hpp"

struct PopupData {
	const char* text;
	Vec2 pos;
	Color clr;
};

static std::unordered_map<int, PopupData> popup_data;

bool EGuiMain::Popup(const char* text, Vec2 position, Color clr) {
	PopupData temp;
	temp.text = text;
	temp.pos = position;
	temp.clr = clr;

	popup_data[popup_data.size() + 1] = temp;

	return true;
}

void EGuiMain::RenderPopups() {
	//render
	for (int i = 1; i <= popup_data.size(); i++) {
		Vec2 text_size = renderer.GetTextSize(renderer.Verdana, popup_data[i].text);

		renderer.FilledRectangle(popup_data[i].pos, { text_size.x + 8, text_size.y + 4}, EGuiColors.ElementBackColor, EGuiStyle.ElementRounding);
		renderer.Rectangle(popup_data[i].pos, { text_size.x + 8, text_size.y + 4 }, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

		renderer.Text(renderer.Verdana, popup_data[i].text, { popup_data[i].pos.x + 4, popup_data[i].pos.y + 2 }, popup_data[i].clr, LEFT);
	}

	//clear.
	popup_data.clear();
}