#pragma once

struct Color
{
	unsigned char RGBA[4];

	Color()
	{
		RGBA[0] = 255;
		RGBA[1] = 255;
		RGBA[2] = 255;
		RGBA[3] = 255;
	}
	Color(int r, int g, int b, int a = 255)
	{
		RGBA[0] = r;
		RGBA[1] = g;
		RGBA[2] = b;
		RGBA[3] = a;
	}
	inline int r() const
	{
		return RGBA[0];
	}
	inline int g() const
	{
		return RGBA[1];
	}
	inline int b() const
	{
		return RGBA[2];
	}
	inline int a() const
	{
		return RGBA[3];
	}
	bool operator!=(Color color)
	{
		return RGBA[0] != color.RGBA[0] || RGBA[1] != color.RGBA[1] || RGBA[2] != color.RGBA[2] || RGBA[3] != color.RGBA[3];
	}
	bool operator==(Color color)
	{
		return RGBA[0] == color.RGBA[0] && RGBA[1] == color.RGBA[1] && RGBA[2] == color.RGBA[2] && RGBA[3] == color.RGBA[3];
	}

	Color OverrideAlpha(int alpha) const {
		return Color(RGBA[0], RGBA[1], RGBA[2], alpha);
	}

	D3DCOLOR TranslateColor(bool alpha_override, int alpha) const {
		return D3DCOLOR_RGBA(RGBA[0], RGBA[1], RGBA[2], alpha_override ? alpha : RGBA[3]);
	}

	static Color HSVtoRGB(float hue, float sat, float val) {
		float red, grn, blu;
		float i, f, p, q, t;
		Color result;

		if (val == 0) {
			red = 0;
			grn = 0;
			blu = 0;
		}
		else {
			hue /= 60;
			i = floor(hue);
			f = hue - i;
			p = val * (1 - sat);
			q = val * (1 - (sat * f));
			t = val * (1 - (sat * (1 - f)));
			if (i == 0) {
				red = val;
				grn = t;
				blu = p;
			}
			else if (i == 1) {
				red = q;
				grn = val;
				blu = p;
			}
			else if (i == 2) {
				red = p;
				grn = val;
				blu = t;
			}
			else if (i == 3) {
				red = p;
				grn = q;
				blu = val;
			}
			else if (i == 4) {
				red = t;
				grn = p;
				blu = val;
			}
			else if (i == 5) {
				red = val;
				grn = p;
				blu = q;
			}
		}

		result = Color(int(red * 255), int(grn * 255), int(blu * 255));
		return result;
	}

	struct Hsv_t {
		float Hue, Saturation, Value;
	};

	static Hsv_t RGBtoHSV(Color a) {
		float red, grn, blu;
		red = (float)a.r() / 255.f;
		grn = (float)a.g() / 255.f;
		blu = (float)a.b() / 255.f;
		float hue, sat, val;
		float x, f, i;
		Hsv_t result;

		x = min(min(red, grn), blu);
		val = max(max(red, grn), blu);
		if (x == val) {
			hue = 0;
			sat = 0;
		}
		else {
			f = (red == x) ? grn - blu : ((grn == x) ? blu - red : red - grn);
			i = (red == x) ? 3 : ((grn == x) ? 5 : 1);
			hue = fmod((i - f / (val - x)) * 60, 360);
			sat = ((val - x) / val);
		}
		result.Hue = hue;
		result.Saturation = sat;
		result.Value = val;

		return result;
	}
};