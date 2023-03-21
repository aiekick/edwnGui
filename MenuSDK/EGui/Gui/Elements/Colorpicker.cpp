#include "../../EGui.hpp"

struct ColorData {
	const char* title;
	Vec2 pos;
	Vec2 size;
	Color clr;
	bool alpha_bar;
	float hue;
	float saturation;
	float value;
};

static std::unordered_map<int, ColorData> color_data;
static std::unordered_map<int, bool> color_open;

bool EGuiMain::ColorPicker(const char* title, Color* selected, bool alpha_bar) {
    SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 size = { 25, 10 };
	Vec2 pos = { (GetChildPos().x + GetChildSize().x - 12 - EGuiStyle.Padding) - size.x, PreviousDrawPos.y };

	renderer.FilledRectangle(pos, size, *selected, EGuiStyle.ElementRounding);
	renderer.Rectangle(pos, size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);

	if (Input.ButtonBehaviour(pos, size, PRESS))
		color_open[GetItemIdentifier()] = !color_open[GetItemIdentifier()];

	//create color popup.
	if (color_open[GetItemIdentifier()]) {
		Vec2 text_size = renderer.GetTextSize(renderer.Verdana, title);
		Vec2 PickerSize = { 200, 175 + text_size.y };

		if (!Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0), PickerSize) && Input.IsKeyPressed(VK_LBUTTON))
			color_open[GetItemIdentifier()] = false;

		float Saturation = Color::RGBtoHSV(*selected).Saturation;
		float Value = Color::RGBtoHSV(*selected).Value;
		float Hue = Color::RGBtoHSV(*selected).Hue;
		float Alpha = 0;

		//Primary picker
		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0) + Vec2(5, text_size.y + 10), PickerSize - Vec2(25, 40)) && Input.IsKeyDown(VK_LBUTTON)) {
			Saturation = (Input.GetMousePos().x - (pos.x + size.x + 14)) / (PickerSize.x - 25);
			Saturation = Math.Clamp(Saturation, 0.f, 1.f);
			Value = (Input.GetMousePos().y - (pos.y + size.y + 14)) / (PickerSize.y - 40);
			Value = Math.Clamp(Value, 0.f, 1.f);
		}

		//Hue bar
		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0) + Vec2(PickerSize.x - 15, text_size.y + 10), { 10, PickerSize.y - 40 }) && Input.IsKeyDown(VK_LBUTTON)) {
			Hue = Input.GetMousePos().y - (pos.y + size.y + 14);
			Hue = Math.GetPercent(Hue, PickerSize.y - 40) * 3.60;
			Hue = Math.Clamp(Hue, 0.f, 360.f);
		}

		//Alpha bar
		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0) + Vec2(5, PickerSize.y - 15), { PickerSize.x - 25, 10 }) && Input.IsKeyDown(VK_LBUTTON)) {
			Alpha = Input.GetMousePos().x - (pos.x + size.x + 14);
			*selected = Color(selected->r(), selected->g(), selected->b(), Math.GetPercent(Alpha, PickerSize.x - 25) * 2.55);
		}

		//todo: fix math for saturation and value, there like opposite. run it and drag to bottom right you will see what I mean lol. I will clean up code dont worry.
		*selected = Color::HSVtoRGB(Hue, Saturation, Value);

		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0), PickerSize))
			SetWindowDragability(false);

		ColorData temp;
		temp.title = title;
		temp.pos = pos + Vec2(size.x + 10, 0);
		temp.size = PickerSize;
		temp.clr = *selected;
		temp.alpha_bar = alpha_bar;
		temp.hue = Hue;
		temp.saturation = Saturation;
		temp.value = Value;

		color_data[color_data.size() + 1] = temp;
	}

	return true;
}

void EGuiMain::RenderColorPickers() {
	//render
	for (int i = 1; i <= color_data.size(); i++) {
		Vec2 text_size = renderer.GetTextSize(renderer.Verdana, color_data[i].title);
		Vec2 size = color_data[i].size;
		Color clr = color_data[i].clr;

		//frame
		renderer.BorderedRectangle(color_data[i].pos, size, EGuiColors.ChildBgColor, EGuiColors.ElementBorderColor);
		renderer.BorderedRectangle(color_data[i].pos, {size.x, text_size.y + 5}, EGuiColors.ChildHeaderColor, EGuiColors.ElementBorderColor);

		//color gradient
		renderer.Gradient(color_data[i].pos + Vec2(5, text_size.y + 10), size - Vec2(25, 40), Color(255, 255, 255, 255), Color(clr.r(), clr.g(), clr.b(), 255));
		renderer.Gradient(color_data[i].pos + Vec2(5, text_size.y + 10), size - Vec2(25, 40), Color(0, 0, 0, 0), Color(0, 0, 0, 255), true);
		renderer.Rectangle(color_data[i].pos + Vec2(5, text_size.y + 10) + Vec2(color_data[i].saturation * (size.x - 25), color_data[i].value * (size.y - 40)), { 5, 5 }, Color(255, 255, 0, 255));

		//color bar
		for (int j = 0; j < size.y - 40; j++) {
			float Hue = ( (float)j / (size.y - 40) ) * 360.f;
			Color HueColor = Color::HSVtoRGB(Hue, 1, 1);

			renderer.Line(color_data[i].pos + Vec2(size.x - 15, text_size.y + 10 + j), color_data[i].pos + Vec2(size.x - 5, text_size.y + 10 + j), HueColor);
		}

		renderer.Rectangle(color_data[i].pos + Vec2(size.x - 15, text_size.y + 10), { 10, size.y - 40 }, Color(0, 0, 0, 255));
		renderer.Rectangle(color_data[i].pos + Vec2(size.x - 15, text_size.y + 10 + ((color_data[i].hue / 3.60) * ((size.y - 40) / 100))), { 10, 5 }, Color(255, 255, 0, 255));

		//alpha bar
		float alpha_amount = (clr.a() / 2.55) * (size.x - 25) / 100;
		renderer.Gradient(color_data[i].pos + Vec2(5, size.y - 15), { size.x - 25, 10 }, Color(0, 0, 0, 255), Color(clr.r(), clr.g(), clr.b(), 255));
		renderer.Rectangle(color_data[i].pos + Vec2(5, size.y - 15), { size.x - 25, 10 }, Color(0, 0, 0, 255));
		renderer.Rectangle(color_data[i].pos + Vec2(5 + alpha_amount, size.y - 15), Vec2(5, 10), Color(255, 255, 0, 255));

		//title
		renderer.Text(renderer.Verdana, color_data[i].title, { color_data[i].pos.x + 4, color_data[i].pos.y + 2 }, EGuiColors.TextColor, LEFT);
	}

	//clear.
	color_data.clear();
}