#pragma once

struct Font {
    LPD3DXFONT font = {};
    bool anti_alias, drop_shadow, outline;

    Font() {}
    Font(IDirect3DDevice9* pDevice, const char* name, int size, int weight, bool _anti_alias, bool _drop_shadow, bool _outline) {
        CreateDXFont(pDevice, name, size, weight);
        anti_alias = _anti_alias;
        drop_shadow = _drop_shadow;
        outline = _outline;
    }

    void clear() {
        font = {};
        anti_alias, drop_shadow, outline = NULL;
    }

    HRESULT CreateDXFont(IDirect3DDevice9* pDevice, const char* name, int size, int weight) {
        return D3DXCreateFontA(pDevice, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH, name, &font);
    }
};