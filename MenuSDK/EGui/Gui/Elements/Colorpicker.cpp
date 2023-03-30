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

static std::map<int, ColorData> color_data;
static std::map<int, bool> color_open;

bool EGuiMain::ColorPicker(const char* title, Color* selected, bool alpha_bar) {
    SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 size = { 25, 10 };
	Vec2 pos = { (GetChildPos().x + GetChildSize().x - 12 - EGuiStyle.Padding) - size.x, PreviousDrawPos.y };

	bool should_render = Input.IsRectInRect(pos, size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h));

	//handle input
	if (Input.ButtonBehaviour(pos, size, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
		color_open[GetItemIdentifier()] = !color_open[GetItemIdentifier()];

	//create color popup.
	if (color_open[GetItemIdentifier()] && should_render) {
		Vec2 text_size = renderer.GetTextSize(Fonts.Primary, title);
		Vec2 PickerSize = { 200, 175 + text_size.y };

		if (!Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0), PickerSize) && Input.IsKeyPressed(VK_LBUTTON))
			color_open[GetItemIdentifier()] = false;

		Color::Hsv_t HSV = selected->RGBtoHSV();
		float Alpha = selected->a();

		//Primary picker
		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0) + Vec2(5, text_size.y + 10), PickerSize - Vec2(25, 40)) && Input.IsKeyDown(VK_LBUTTON)) {
			//Saturation
			HSV.Saturation = (Input.GetMousePos().x - (pos.x + size.x + 14)) / (PickerSize.x - 25);
			HSV.Saturation = Math.Clamp(HSV.Saturation, 0.f, 1.f);

			//Value
			HSV.Value = 1.f - ((Input.GetMousePos().y - (pos.y + size.y + 14)) / (PickerSize.y - 40));
			HSV.Value = Math.Clamp(HSV.Value, 0.f, 1.f);
		}

		//Hue bar
		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0) + Vec2(PickerSize.x - 15, text_size.y + 10), { 10, PickerSize.y - 40 }) && Input.IsKeyDown(VK_LBUTTON)) {
			HSV.Hue = Input.GetMousePos().y - (pos.y + size.y + 14);
			HSV.Hue = Math.GetPercent(HSV.Hue, PickerSize.y - 40) * 3.60;
			HSV.Hue = Math.Clamp(HSV.Hue, 0.f, 360.f);
		}

		//Alpha bar
		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0) + Vec2(5, PickerSize.y - 15), { PickerSize.x - 25, 10 }) && Input.IsKeyDown(VK_LBUTTON)) {
			Alpha = Input.GetMousePos().x - (pos.x + size.x + 14);
			Alpha = Math.GetPercent(Alpha, PickerSize.x - 25) * 2.55;
		}

		//Not correct?
		*selected = Color::HSVtoRGB(HSV.Hue, HSV.Saturation, HSV.Value);
		*selected = Color(selected->r(), selected->g(), selected->b(), Alpha);

		if (Input.IsMouseHoveringRect(pos + Vec2(size.x + 10, 0), PickerSize))
			SetWindowDragability(false);

		SetDisableInputArea(pos + Vec2(size.x + 10, 0), PickerSize);

		ColorData temp;
		temp.title = title;
		temp.pos = pos + Vec2(size.x + 10, 0);
		temp.size = PickerSize;
		temp.clr = *selected;
		temp.alpha_bar = alpha_bar;
		temp.hue = HSV.Hue;
		temp.saturation = HSV.Saturation;
		temp.value = HSV.Value;

		color_data[color_data.size() + 1] = temp;
	}

	//render
	if (should_render) {
		renderer.FilledRectangle(pos, size, *selected, EGuiStyle.ElementRounding);
		renderer.Rectangle(pos, size, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding);
	}

	return true;
}

void EGuiMain::RenderColorPickers() {
	//render
	for (int i = 1; i <= color_data.size(); i++) {
		Vec2 text_size = renderer.GetTextSize(Fonts.Primary, color_data[i].title);
		Vec2 size = color_data[i].size;
		Color clr = color_data[i].clr;

		//frame
		Color clr_hue = Color::HSVtoRGB(color_data[i].hue, 1.f, 1.f);
		renderer.BorderedRectangle(color_data[i].pos, size, EGuiColors.ChildBgColor, EGuiColors.ElementBorderColor, EGuiStyle.ChildRounding);
		renderer.BorderedRectangle(color_data[i].pos, {size.x, text_size.y + 5}, EGuiColors.ChildHeaderColor, EGuiColors.ElementBorderColor, EGuiStyle.ChildRounding, CORNER_TOP);

		//color gradient
		renderer.Gradient(color_data[i].pos + Vec2(5, text_size.y + 10), size - Vec2(25, 40), Color(255, 255, 255, 255), Color(clr_hue.r(), clr_hue.g(), clr_hue.b(), 255));
		renderer.Gradient(color_data[i].pos + Vec2(5, text_size.y + 10), size - Vec2(25, 40), Color(0, 0, 0, 0), Color(0, 0, 0, 255), true);
		renderer.Rectangle(color_data[i].pos + Vec2(2, text_size.y + 10) + Vec2(color_data[i].saturation * (size.x - 25), ((size.y - 40) - (color_data[i].value * (size.y - 40)))), { 5, 5 }, Color(255, 255, 0, 255));

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
		renderer.Rectangle(color_data[i].pos + Vec2(5 + alpha_amount - 2.5f, size.y - 15), Vec2(5, 10), Color(255, 255, 0, 255));

		//title
		renderer.Text(Fonts.Primary, color_data[i].title, { color_data[i].pos.x + 4, color_data[i].pos.y + 2 }, EGuiColors.TextColor, LEFT);
	}

	//clear.
	color_data.clear();
}