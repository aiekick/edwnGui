#pragma once

struct Font {
    LPD3DXFONT font = {};
    bool anti_alias, drop_shadow, outline;

    Font() {}
    Font(LPD3DXFONT _font, bool _anti_alias, bool _drop_shadow, bool _outline)
        : font(_font), anti_alias(_anti_alias), drop_shadow(_drop_shadow), outline(_outline) {}

    void clear() {
        font = {};
        anti_alias, drop_shadow, outline = NULL;
    }
};