#include "Renderer.hpp"
#include <chrono>

ERenderer renderer;

void ERenderer::CreateObjects() {
	D3DXCreateSprite(EGui.Device, &EGui.Sprite);

	D3DPRESENT_PARAMETERS pp = {};
	DWORD nFonts = 0;

	Verdana = AddFont("Verdana", FW_NORMAL, 12);
	Tahombd = AddFont("Verdana", FW_BOLD, 12);
	SmallFont = AddFont("Small Fonts", FW_NORMAL, 8);
	TabIcon = AddFont("Verdana", FW_NORMAL, 16);
	TitleFont = AddFont("Verdana", FW_NORMAL, 16);

	D3DXCreateTextureFromFileInMemoryEx(EGui.Device, BgTexture, 424852, 4096, 4096, D3DX_DEFAULT, NULL, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->BackgroundTexture);
	D3DXCreateTextureFromFileInMemoryEx(EGui.Device, TransparencyTexture, 2419, 50, 50, D3DX_DEFAULT, NULL, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->AlphaTexture);
	D3DXCreateTextureFromFileInMemoryEx(EGui.Device, CursorTexture, 550, 10, 15, D3DX_DEFAULT, NULL, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->MouseTexture);
}

void ERenderer::ReleaseObjects() {
	if (Verdana.Font)
		Verdana.Font->Release();
	if (Tahombd.Font)
		Tahombd.Font->Release();
	if (SmallFont.Font)
		SmallFont.Font->Release();
	if (TabIcon.Font)
		TabIcon.Font->Release();
	if (TitleFont.Font)
		TitleFont.Font->Release();
}

void ERenderer::Reset()
{
	D3DVIEWPORT9 screen;
	EGui.Device->GetViewport(&screen);
}

FontData ERenderer::AddFont(std::string name, int weight, int size, bool dropshadow, bool outline) {
	FontData Temp_font;

	D3DXCreateFontA(EGui.Device, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH, name.c_str(), &Temp_font.Font);
	Temp_font.drop_shadow = dropshadow;
	Temp_font.outline = outline;

	return Temp_font;
}

void ERenderer::PushFont(FontData font) {
	if (!PushingFont) {
		PushingFont = true;
		OverrideFont = font;
	}
}

void ERenderer::PopFont() {
	if (PushingFont) {
		PushingFont = false;
		OverrideFont = {};
	}
}

void ERenderer::PushClip(Vec2 Pos, Vec2 Size) {
	RECT protectedRect{ (int)Pos.x, (int)Pos.y, (int)(Pos.x + Size.x), (int)(Pos.y + Size.y) };

	EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	EGui.Device->SetScissorRect(&protectedRect);
}

void ERenderer::PopClip() {
	EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

void ERenderer::PushAlpha(int alpha) {
	PushingAlphaAmount = alpha;
	PushingAlpha = true;
}

void ERenderer::PopAlpha() {
	if (PushingAlphaAmount == -1)
		return;

	PushingAlphaAmount = -1;
	PushingAlpha = false;
}

void ERenderer::Line(Vec2 Pos, Vec2 Pos2, Color clr)
{
	if (PushingAlpha)
		clr = Color(clr.r(), clr.g(), clr.b(), PushingAlphaAmount);

	D3DCOLOR d3dclr = TranslateColor(clr);

	vertex vertices[2] = {
		{ round(Pos.x), round(Pos.y), 0.0f, 1.0f, d3dclr },
		{ round(Pos2.x), round(Pos2.y), 0.0f, 1.0f, d3dclr },
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, 20);
}

auto toRadians = PI / 180;
auto toDegrees = 180 / PI;

void ERenderer::Rectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding, EGuiRoundingFlags flags)
{
	D3DCOLOR d3dclr = TranslateColor(clr);

	if (rounding > 0) {
		const int num_segments = rounding;
		const int num_vertices = (num_segments * 4) + 2;

		vertex* vertices = new vertex[num_vertices];

		// Generate the vertex data
		int current_vertex = 0;

		for (int i = 0; i < 4; i++)
		{
			Vec2 corner_point = { Pos.x + ((i < 2) ? (Size.x - rounding) : rounding), Pos.y + ((i % 3) ? (Size.y - rounding) : rounding) };
			float angle_start = 90.f * (i - 1);
			float angle_end = angle_start + 90.f;

			for (int j = 0; j < num_segments; j++)
			{
				float angle = angle_start + ((angle_end - angle_start) / static_cast<float>(num_segments)) * static_cast<float>(j);
				angle *= toRadians;

				vertices[current_vertex].x = corner_point.x + rounding * std::cos(angle);
				vertices[current_vertex].y = corner_point.y + rounding * std::sin(angle);
				vertices[current_vertex].z = 0.0f;
				vertices[current_vertex].rhw = 1.0f;
				vertices[current_vertex].color = d3dclr;

				current_vertex++;
			}
		}

		// Manually add another vertex at the end to close the loop
		vertices[current_vertex] = vertices[0];
		current_vertex++;

		// Create a vertex buffer and fill it with the vertex data
		IDirect3DVertexBuffer9* vb;
		const int vertex_size = sizeof(vertex);
		EGui.Device->CreateVertexBuffer(num_vertices * vertex_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vb, nullptr);
		void* vb_data;
		vb->Lock(0, num_vertices * vertex_size, &vb_data, D3DLOCK_DISCARD);
		memcpy(vb_data, vertices, num_vertices * vertex_size);
		vb->Unlock();

		// Draw the vertex buffer
		EGui.Device->SetStreamSource(0, vb, 0, vertex_size);
		EGui.Device->DrawPrimitive(D3DPT_LINESTRIP, 0, current_vertex - 1);

		// Release the vertex buffer
		vb->Release();

		//dealloc the memory for vertices
		delete[] vertices;

		//we are done
		return;
	}

	vertex vertices[5] = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x, Pos.y, 0.0f, 1.0f, d3dclr }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vertices, sizeof(vertex));
}

void ERenderer::FilledRectangle(Vec2 Pos, Vec2 Size, Color clr, float rounding, EGuiRoundingFlags flags)
{
	D3DCOLOR d3dclr = TranslateColor(clr);

	if (rounding > 0) {
		const int num_segments = rounding;
		const int num_vertices = (num_segments * 4) + 2;

		vertex* vertices = new vertex[num_vertices];

		// Generate the vertex data
		int current_vertex = 0;

		for (int i = 0; i < 4; i++)
		{
			Vec2 corner_point = { Pos.x + ((i < 2) ? (Size.x - rounding) : rounding), Pos.y + ((i % 3) ? (Size.y - rounding) : rounding) };
			float angle_start = 90.f * (i - 1);
			float angle_end = angle_start + 90.f;

			for (int j = 0; j < num_segments; j++)
			{
				float angle = angle_start + ((angle_end - angle_start) / static_cast<float>(num_segments)) * static_cast<float>(j);
				angle *= toRadians;

				vertices[current_vertex].x = corner_point.x + rounding * std::cos(angle);
				vertices[current_vertex].y = corner_point.y + rounding * std::sin(angle);
				vertices[current_vertex].z = 0.0f;
				vertices[current_vertex].rhw = 1.0f;
				vertices[current_vertex].color = d3dclr;

				current_vertex++;
			}
		}

		// Manually add another vertex at the end to close the loop
		vertices[current_vertex] = vertices[0];
		current_vertex++;

		// Create a vertex buffer and fill it with the vertex data
		IDirect3DVertexBuffer9* vb;
		const int vertex_size = sizeof(vertex);
		EGui.Device->CreateVertexBuffer(num_vertices * vertex_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vb, nullptr);
		void* vb_data;
		vb->Lock(0, num_vertices * vertex_size, &vb_data, D3DLOCK_DISCARD);
		memcpy(vb_data, vertices, num_vertices * vertex_size);
		vb->Unlock();

		// Draw the vertex buffer
		EGui.Device->SetStreamSource(0, vb, 0, vertex_size);
		EGui.Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, current_vertex - 1);

		// Release the vertex buffer
		vb->Release();

		//dealloc the memory for vertices
		delete[] vertices;

		//we are done
		return;
	}

	vertex vertices[4] = {
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x, Pos.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, d3dclr },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, d3dclr }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));
}

void ERenderer::BorderedRectangle(Vec2 Pos, Vec2 Size, Color clr, Color BorderColor, float rounding) {
	FilledRectangle(Pos, Size, clr, rounding);
	Rectangle(Pos, Size, BorderColor, rounding);
}

void ERenderer::Gradient(Vec2 Pos, Vec2 Size, Color LColor, Color ROtherColor, bool Vertical)
{
	D3DCOLOR Color_Left = TranslateColor(LColor);
	D3DCOLOR Color_Right = TranslateColor(ROtherColor);

	vertex vertices[4] = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, Color_Left },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Vertical ? Color_Left : Color_Right },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Vertical ? Color_Right : Color_Left },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, Color_Right }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));
}

void ERenderer::Gradient4(Vec2 Pos, Vec2 Size, Color TopLColor, Color TopRColor, Color BomLColor, Color BomRColor) {
	D3DCOLOR Color_TL = TranslateColor(TopLColor);
	D3DCOLOR Color_TR = TranslateColor(TopRColor);
	D3DCOLOR Color_BL = TranslateColor(BomLColor);
	D3DCOLOR Color_BR = TranslateColor(BomRColor);

	vertex vertices[4] = {
		{ Pos.x, Pos.y, 0.0f, 1.0f, Color_TL },
		{ Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Color_TR },
		{ Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Color_BL },
		{ Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, Color_BR }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));
}

void ERenderer::Text(FontData Font, const char* text, Vec2 Pos, Color clr, int Orientation) {
	auto font = PushingFont ? OverrideFont : Font;

	D3DCOLOR d3dclr = TranslateColor(clr);

	//temperary variables.
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
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x + 1, Pos.y + 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x, Pos.y + 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x + 1, Pos.y, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x, Pos.y - 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x - 1, Pos.y, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x + 1, Pos.y - 1, Pos.x, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
		SetRect(&clip_rect, Pos.x - 1, Pos.y, Pos.x + 1, Pos.y);
		font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, D3DCOLOR_RGBA(15, 15, 15, clr.a()));
	}

	SetRect(&clip_rect, Pos.x, Pos.y, Pos.x, Pos.y);
	font.Font->DrawTextA(NULL, text, -1, &clip_rect, TextFlags, d3dclr);
}

Vec2 ERenderer::GetTextSize(FontData Font, const char* Text) {
	//temp
	RECT rect;

	//calc
	Font.Font->DrawTextA(0, Text, strlen(Text), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

	return Vec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

void ERenderer::Triangle(Vec2 Top, Vec2 Left, Vec2 Right, Color clr) {
	D3DCOLOR Color = TranslateColor(clr);

	vertex vertices[3] = {
		{ Top.x, Top.y, 0.0f, 1.0f, Color },
		{ Right.x, Right.y, 0.0f, 1.0f, Color },
		{ Left.x, Left.y, 0.0f, 1.0f, Color }
	};

	EGui.Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(vertex));
}

void ERenderer::Circle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation) {
	D3DCOLOR Color = TranslateColor(clr);
	Pos = Pos + Vec2(radius, radius);

	const int NUM_VERTICES = clamp((int)radius, 32, 100); // more will look smoother, 72 looks great. there is no reason to go any higher unless you are drawing a huge circle.

	std::vector<vertex> circle(NUM_VERTICES + 1);
	float angle = rotation * D3DX_PI / 180;
	float pi;

	// this is for how much of the circle is shown, FULL = full circle, HALF = half of a circle, Quarter = 1/4 of the circle.
	if (e_completion == FULL) pi = D3DX_PI;
	if (e_completion == HALF) pi = D3DX_PI / 2;
	if (e_completion == QUARTER) pi = D3DX_PI / 3.91;

	// Use a Bezier curve to generate the circle
	for (int i = 0; i < NUM_VERTICES + 1; i++) {
		float t = (float)i / (float)NUM_VERTICES;
		float x = Pos.x + radius * cos(2 * pi * t + angle);
		float y = Pos.y + radius * sin(2 * pi * t + angle);
		circle[NUM_VERTICES - i].x = x;  // <-- Reverse the vertex order here
		circle[NUM_VERTICES - i].y = y;
		circle[NUM_VERTICES - i].z = 0;
		circle[NUM_VERTICES - i].rhw = 1;
		circle[NUM_VERTICES - i].color = Color;
	}

	// create the vertex buffer
	EGui.Device->CreateVertexBuffer((NUM_VERTICES + 1) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &EGui.VertexBuffer, NULL);

	// allocate memory for the vertex buffer
	VOID* pVertices;
	EGui.VertexBuffer->Lock(0, (NUM_VERTICES + 1) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (NUM_VERTICES + 1) * sizeof(vertex));
	EGui.VertexBuffer->Unlock();

	// prepare primitive
	EGui.Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	EGui.Device->SetStreamSource(0, EGui.VertexBuffer, 0, sizeof(vertex));

	// render primitive
	EGui.Device->DrawPrimitive(D3DPT_LINESTRIP, 0, NUM_VERTICES);

	// restore primitive
	EGui.Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	// release vertex buffer
	EGui.VertexBuffer->Release();
}

void ERenderer::FilledCircle(Vec2 Pos, float radius, Color clr, int e_completion, float rotation) {
	D3DCOLOR Color = TranslateColor(clr);
	Pos = Pos + Vec2(radius, radius);

	const int NUM_VERTICES = clamp((int)radius, 32, 100); // more will look smoother, 72 looks great. there is no reason to go any higher unless you are drawing a huge circle.

	std::vector<vertex> circle(NUM_VERTICES + 1);
	float angle = rotation * PI / 180;
	float completion;

	// this is for how much of the circle is shown, FULL = full circle, HALF = half of a circle, Quarter = 1/4 of the circle.
	if (e_completion == FULL) completion = PI;
	if (e_completion == HALF) completion = PI_2;
	if (e_completion == QUARTER) completion = PI / 4;

	// Use a Bezier curve to generate the circle
	for (int i = 0; i < NUM_VERTICES + 1; i++) {
		float t = (float)i / (float)NUM_VERTICES;
		float x = Pos.x + radius * cos(2 * completion * t + angle);
		float y = Pos.y + radius * sin(2 * completion * t + angle);
		circle[NUM_VERTICES - i].x = x;  // <-- Reverse the vertex order here
		circle[NUM_VERTICES - i].y = y;
		circle[NUM_VERTICES - i].z = 0;
		circle[NUM_VERTICES - i].rhw = 1;
		circle[NUM_VERTICES - i].color = Color;
	}

	// create the vertex buffer
	EGui.Device->CreateVertexBuffer((NUM_VERTICES + 1) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &EGui.VertexBuffer, NULL);

	// allocate memory for the vertex buffer
	VOID* pVertices;
	EGui.VertexBuffer->Lock(0, (NUM_VERTICES + 1) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (NUM_VERTICES + 1) * sizeof(vertex));
	EGui.VertexBuffer->Unlock();

	// prepare primitive
	EGui.Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	EGui.Device->SetStreamSource(0, EGui.VertexBuffer, 0, sizeof(vertex));

	// render primitive
	EGui.Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, NUM_VERTICES);

	// restore primitive
	EGui.Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	// release vertex buffer
	EGui.VertexBuffer->Release();
}

void ERenderer::BorderedCircle(Vec2 Pos, float radius, Color clr, Color borderClr, int e_completion, float rotation) {
	FilledCircle(Pos, radius, clr, e_completion, rotation);
	Circle(Pos, radius, borderClr, e_completion, rotation);
}

void ERenderer::Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, Color clr) {
	D3DCOLOR Color = TranslateColor(clr);

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