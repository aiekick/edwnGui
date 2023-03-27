#include "Renderer.hpp"
#include "Classes/Vertex.hpp"

ERenderer renderer;
ETextures Textures;
EFonts Fonts;

const void ERenderer::Create() {
	D3DXCreateSprite(EGui.Device, &EGui.Sprite);

	Fonts.Primary = AddFont("Verdana", FW_NORMAL, 12);
	Fonts.TabIcon = AddFont("Distinguished-Tab-Icons", FW_NORMAL, 16);
	Fonts.TitleFont = AddFont("Verdana", FW_NORMAL, 16);
}


const void ERenderer::Release() {
	Fonts.Primary.Font->Release();
	Fonts.TabIcon.Font->Release();
	Fonts.TitleFont.Font->Release();
}

const FontData ERenderer::AddFont(std::string name, int weight, int size, bool anti_alias, bool dropshadow, bool outline) {
	FontData font;
	{
		D3DXCreateFontA(EGui.Device, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH, name.c_str(), &font.Font);
		font.anti_alias = anti_alias;
		font.drop_shadow = dropshadow;
		font.outline = outline;
	}
	return font;
}

const void ERenderer::SetAntiAliasing(bool state) {
	EGui.Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, state);
}

const void ERenderer::PushFont(FontData font) {
	if (!PushingFont) {
		PushingFont = true;
		Fonts.Override = font;
	}
}

const void ERenderer::PopFont() {
	if (PushingFont) {
		PushingFont = false;
		Fonts.Override = {};
	}
}

Clip_info clip_info;
const void ERenderer::PushClip(Vec2 Pos, Vec2 Size) {
	clip_info.PushingClip = true;
	clip_info.OldClip = clip_info.Clip;
	clip_info.Clip = { (int)Pos.x, (int)Pos.y, (int)(Pos.x + Size.x), (int)(Pos.y + Size.y) };
	EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	EGui.Device->SetScissorRect(&clip_info.Clip);
}

const void ERenderer::PopClip() {
	clip_info.PushingClip = clip_info.OldPushingClip;
	clip_info.Clip = clip_info.OldClip;
	EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

const void ERenderer::PushAlpha(int alpha) {
	PushingAlphaAmount = alpha;
	PushingAlpha = true;
}

const void ERenderer::PopAlpha() {
	PushingAlphaAmount = -1;
	PushingAlpha = false;
}

/* from here on out we are drawing shapes and thats it */
std::vector<vertex> line_vertices;
const void ERenderer::Line(Vec2 Pos, Vec2 Pos2, Color clr) {
	D3DCOLOR translated_color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	line_vertices = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, translated_color },
		{ Pos2.x, Pos2.y, 0.0f, 1.0f, translated_color },
	};

	SetAntiAliasing(true);
	EGui.Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, line_vertices.data(), 20);
	SetAntiAliasing(false);

	line_vertices.clear();
}

std::vector<vertex> polyline_vertices;
const void ERenderer::PolyLine(std::vector<Vec2> points, Color clr) {
	D3DCOLOR translated_color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	polyline_vertices.reserve(points.size());

	for (const auto& point : points)
		polyline_vertices.emplace_back(point.x, point.y, 0.0f, 1.0f, translated_color);

	SetAntiAliasing(true);
	EGui.Device->DrawPrimitiveUP(D3DPT_LINESTRIP, points.size(), polyline_vertices.data(), sizeof(vertex));
	SetAntiAliasing(false);

	polyline_vertices.clear();
}

std::vector<vertex> polygon_vertices;
const void ERenderer::Polygon(std::vector<Vec2> points, Color clr) {
	D3DCOLOR translated_color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	polygon_vertices.reserve(points.size());

	for (const auto& point : points)
		polygon_vertices.emplace_back(point.x, point.y, 0.0f, 1.0f, translated_color);

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, points.size(), polygon_vertices.data(), sizeof(vertex));

	polygon_vertices.clear();
}

std::vector<vertex> rectangle_vertices;
const void ERenderer::Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding, RoundingFlags flags) {
	D3DCOLOR translated_color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	if (rounding > 0.5f && flags != CORNER_NONE) {
		const auto round_top_left = (flags & CORNER_TOP_LEFT) != 0;
		const auto round_top_right = (flags & CORNER_TOP_RIGHT) != 0;
		const auto round_bottom_left = (flags & CORNER_BOTTOM_LEFT) != 0;
		const auto round_bottom_right = (flags & CORNER_BOTTOM_RIGHT) != 0;

		const int num_segments = rounding * 4;
		const int num_vertices = (num_segments * 4) + 2;

		for (int i = 0; i < 4; i++) {
			bool round_corner = false;

			switch (i) {
			case 0:
				round_corner = round_top_right;
				break;
			case 1:
				round_corner = round_bottom_right;
				break;
			case 2:
				round_corner = round_bottom_left;
				break;
			case 3:
				round_corner = round_top_left;
				break;
			}

			Vec2 corner_point = {
				/* x */ Pos.x + ((i < 2) ? (Size.x - (round_corner ? rounding : 0)) : round_corner ? rounding : 0),
				/* y */ Pos.y + ((i % 3) ? (Size.y - (round_corner ? rounding : 0)) : round_corner ? rounding : 0)
			};

			if (round_corner) {
				for (int p = 0; p < num_segments; p++) {
					/* get start and end position of the corner */
					const auto angle = DEG2RAD(90.0f * (i - 1) + (90.f / num_segments) * p);

					/* set point */
					rectangle_vertices.push_back(vertex(
						/* x */corner_point.x + rounding * std::cosf(angle),
						/* y */ corner_point.y + rounding * std::sinf(angle),
						/* z */ 0.f,
						/* rhw */ 1.f,
						/* color */ translated_color
					));
				}
			}
			else
				rectangle_vertices.push_back(vertex(corner_point.x, corner_point.y, 0.f, 1.f, translated_color));
		}

		rectangle_vertices.push_back(rectangle_vertices.front());

		EGui.Device->DrawPrimitiveUP(D3DPT_LINESTRIP, rectangle_vertices.size() - 1, rectangle_vertices.data(), sizeof(vertex));

		rectangle_vertices.clear();

		return;
	}

	rectangle_vertices = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, translated_color },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, translated_color },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, translated_color },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, translated_color },
		{ Pos.x, Pos.y, 0.0f, 1.0f, translated_color }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_LINESTRIP, rectangle_vertices.size() - 1, rectangle_vertices.data(), sizeof(vertex));

	rectangle_vertices.clear();
}

std::vector<vertex> f_rectangle_vertices;
const void ERenderer::FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding, RoundingFlags flags) {
	D3DCOLOR translated_color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	if (rounding > 0.5f && flags != CORNER_NONE) {
		const auto round_top_left = (flags & CORNER_TOP_LEFT) != 0;
		const auto round_top_right = (flags & CORNER_TOP_RIGHT) != 0;
		const auto round_bottom_left = (flags & CORNER_BOTTOM_LEFT) != 0;
		const auto round_bottom_right = (flags & CORNER_BOTTOM_RIGHT) != 0;

		const int num_segments = rounding * 4;
		const int num_vertices = (num_segments * 4) + 2;

		for (int i = 0; i < 4; i++) {
			bool round_corner = false;

			switch (i) {
			case 0:
				round_corner = round_top_right;
				break;
			case 1:
				round_corner = round_bottom_right;
				break;
			case 2:
				round_corner = round_bottom_left;
				break;
			case 3:
				round_corner = round_top_left;
				break;
			}

			Vec2 corner_point = {
				/* x */ Pos.x + ((i < 2) ? (Size.x - (round_corner ? rounding : 0)) : round_corner ? rounding : 0),
				/* y */ Pos.y + ((i % 3) ? (Size.y - (round_corner ? rounding : 0)) : round_corner ? rounding : 0)
			};

			if (round_corner) {
				for (int p = 0; p < num_segments; p++) {
					/* get start and end position of the corner */
					const auto angle = DEG2RAD(90.0f * (i - 1) + (90.f / num_segments) * p);

					/* set point */
					f_rectangle_vertices.push_back(vertex(
						/* x */corner_point.x + rounding * std::cosf(angle),
						/* y */ corner_point.y + rounding * std::sinf(angle),
						/* z */ 0.f,
						/* rhw */ 1.f,
						/* color */ translated_color
					));
				}
			}
			else
				f_rectangle_vertices.push_back(vertex(corner_point.x, corner_point.y, 0.f, 1.f, translated_color));
		}

		f_rectangle_vertices.push_back(f_rectangle_vertices.front());

		EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, f_rectangle_vertices.size() - 1, f_rectangle_vertices.data(), sizeof(vertex));

		f_rectangle_vertices.clear();

		return;
	}

	f_rectangle_vertices = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, translated_color },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, translated_color },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, translated_color },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, translated_color },
		{ Pos.x, Pos.y, 0.0f, 1.0f, translated_color }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, f_rectangle_vertices.size() - 1, f_rectangle_vertices.data(), sizeof(vertex));

	f_rectangle_vertices.clear();
}

const void ERenderer::BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding, RoundingFlags flags) {
	FilledRectangle(Pos, Size, clr, rounding, flags);
	Rectangle(Pos, Size, BorderColor, rounding, flags);
}

std::vector<vertex> gradient_vertices;
const void ERenderer::Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color ROtherColor, bool Vertical) {
	D3DCOLOR Color_Left = LColor.TranslateColor(PushingAlpha, PushingAlphaAmount);
	D3DCOLOR Color_Right = ROtherColor.TranslateColor(PushingAlpha, PushingAlphaAmount);

	gradient_vertices = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, Color_Left },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Vertical ? Color_Left : Color_Right },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Vertical ? Color_Right : Color_Left },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, Color_Right }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, gradient_vertices.size() - 1, gradient_vertices.data(), sizeof(vertex));

	gradient_vertices.clear();
}

std::vector<vertex> gradient4_vertices;
const void ERenderer::Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor) {
	D3DCOLOR Color_TL = TopLColor.TranslateColor(PushingAlpha, PushingAlphaAmount);
	D3DCOLOR Color_TR = TopRColor.TranslateColor(PushingAlpha, PushingAlphaAmount);
	D3DCOLOR Color_BL = BomLColor.TranslateColor(PushingAlpha, PushingAlphaAmount);
	D3DCOLOR Color_BR = BomRColor.TranslateColor(PushingAlpha, PushingAlphaAmount);

	gradient4_vertices = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, Color_TL },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Color_TR },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Color_BL },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, Color_BR }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, gradient4_vertices.size() - 1, gradient4_vertices.data(), sizeof(vertex));
}

const void ERenderer::Text(FontData Font, const char* text, Vec2 Pos, Color clr, int Orientation) {
	auto font = PushingFont ? Fonts.Override : Font;

	D3DCOLOR translated_color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	DWORD TextFlags = 0x0;
	RECT clip_rect;

	switch (Orientation) {
	case LEFT:
		TextFlags = DT_LEFT | DT_NOCLIP;
		break;
	case CENTER:
		TextFlags = DT_CENTER | DT_NOCLIP;
		break;
	case CENTER_XY:
		TextFlags = (DT_CENTER | DT_VCENTER) | DT_NOCLIP;
		break;
	case RIGHT:
		TextFlags = DT_RIGHT | DT_NOCLIP;
		break;
	}

	if (font.outline) {
		SetRect(&clip_rect, Pos.x - 1, Pos.y - 1, 0, 0);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x + 1, Pos.y + 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x, Pos.y + 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x + 1, Pos.y, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x, Pos.y - 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x - 1, Pos.y, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x + 1, Pos.y - 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
		SetRect(&clip_rect, Pos.x - 1, Pos.y, Pos.x + 1, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
	}
	else if (font.drop_shadow) {
		SetRect(&clip_rect, Pos.x + 1, Pos.y, Pos.x + 1, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, clr.a()));
	}


	SetRect(&clip_rect, Pos.x, Pos.y, Pos.x, Pos.y);

	SetAntiAliasing(font.anti_alias);
	font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, translated_color);
	SetAntiAliasing(false);
}

const Vec2 ERenderer::GetTextSize(FontData Font, const char* Text) {
	RECT rect; Font.Font->DrawTextA(0, Text, strlen(Text), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
	return Vec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

std::vector<vertex> triangle_vertices;
const void ERenderer::Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr) {
	D3DCOLOR Color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	triangle_vertices = {
		{ Top.x, Top.y, 0.0f, 1.0f, Color },
		{ Right.x, Right.y, 0.0f, 1.0f, Color },
		{ Left.x, Left.y, 0.0f, 1.0f, Color }
	};

	triangle_vertices.push_back(triangle_vertices.front());

	SetAntiAliasing(true);
	EGui.Device->DrawPrimitiveUP(D3DPT_LINESTRIP, triangle_vertices.size() - 1, triangle_vertices.data(), sizeof(vertex));
	SetAntiAliasing(false);

	triangle_vertices.clear();
}

std::vector<vertex> f_triangle_vertices;
const void ERenderer::TriangleFilled(Vec2 Top, Vec2 Left, Vec2 Right, Color clr) {
	D3DCOLOR Color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	f_triangle_vertices = {
		{ Top.x, Top.y, 0.0f, 1.0f, Color },
		{ Right.x, Right.y, 0.0f, 1.0f, Color },
		{ Left.x, Left.y, 0.0f, 1.0f, Color }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, f_triangle_vertices.size() - 1, f_triangle_vertices.data(), sizeof(vertex));

	f_triangle_vertices.clear();
}

std::vector<vertex> circle_vertices;
const void ERenderer::Circle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation) {
	D3DCOLOR Color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);
	Pos = Pos + Vec2(radius, radius);

	const int NUM_VERTICES = clamp((int)radius, 16, 64);

	float angle = rotation * D3DX_PI / 180;
	float pi;

	if (e_completion == FULL) pi = D3DX_PI;
	if (e_completion == HALF) pi = D3DX_PI / 2;
	if (e_completion == QUARTER) pi = D3DX_PI / 3.91;

	for (int i = 0; i < NUM_VERTICES + 1; i++) {
		float t = (float)i / (float)NUM_VERTICES;
		float x = Pos.x + radius * cos(2 * pi * t + angle);
		float y = Pos.y + radius * sin(2 * pi * t + angle);
		circle_vertices.push_back(vertex(x, y, 0.f, 1.f, Color));
	}

	EGui.Device->CreateVertexBuffer((NUM_VERTICES + 1) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &EGui.VertexBuffer, NULL);

	VOID* pVertices;
	EGui.VertexBuffer->Lock(0, (NUM_VERTICES + 1) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle_vertices.front(), (NUM_VERTICES + 1) * sizeof(vertex));
	EGui.VertexBuffer->Unlock();

	EGui.Device->SetStreamSource(0, EGui.VertexBuffer, 0, sizeof(vertex));

	SetAntiAliasing(true);
	EGui.Device->DrawPrimitive(D3DPT_LINESTRIP, 0, NUM_VERTICES);
	SetAntiAliasing(false);

	EGui.VertexBuffer->Release();

	circle_vertices.clear();
}

std::vector<vertex> f_circle_vertices;
const void ERenderer::FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation) {
	D3DCOLOR Color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);
	Pos = Pos + Vec2(radius, radius);

	const int NUM_VERTICES = clamp((int)radius, 16, 64);

	float angle = rotation * D3DX_PI / 180;
	float completion;

	if (e_completion == FULL) completion = D3DX_PI;
	if (e_completion == HALF) completion = D3DX_PI / 2;
	if (e_completion == QUARTER) completion = D3DX_PI / 4;

	for (int i = 0; i < NUM_VERTICES + 1; i++) {
		float t = (float)i / (float)NUM_VERTICES;
		float x = Pos.x + radius * cos(2 * completion * t + angle);
		float y = Pos.y + radius * sin(2 * completion * t + angle);
		f_circle_vertices.push_back(vertex(x, y, 0.f, 1.f, Color));
	}

	EGui.Device->CreateVertexBuffer((NUM_VERTICES + 1) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &EGui.VertexBuffer, NULL);

	VOID* pVertices;
	EGui.VertexBuffer->Lock(0, (NUM_VERTICES + 1) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &f_circle_vertices.front(), (NUM_VERTICES + 1) * sizeof(vertex));
	EGui.VertexBuffer->Unlock();

	EGui.Device->SetStreamSource(0, EGui.VertexBuffer, 0, sizeof(vertex));

	EGui.Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, NUM_VERTICES);

	EGui.VertexBuffer->Release();
}

const void ERenderer::BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion, float rotation) {
	FilledCircle(Pos, radius, clr, e_completion, rotation);
	Circle(Pos, radius, borderClr, e_completion, rotation);
}

const void ERenderer::Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr) {
	D3DCOLOR Color = clr.TranslateColor(PushingAlpha, PushingAlphaAmount);

	D3DXVECTOR3 pos = D3DXVECTOR3(Pos.x, Pos.y, 0.0f);

	D3DSURFACE_DESC desc;
	Texture->GetLevelDesc(0, &desc);

	RECT SrcRect;
	SrcRect.left = 0;
	SrcRect.top = 0;
	SrcRect.right = desc.Width;
	SrcRect.bottom = desc.Height;

	if (SUCCEEDED(EGui.Sprite->Begin(D3DXSPRITE_ALPHABLEND))) {
		EGui.Sprite->Draw(Texture, &SrcRect, NULL, &pos, Color);
		EGui.Sprite->End();
	}
}