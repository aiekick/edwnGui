#pragma once

struct Font {
    LPD3DXFONT font;
    bool anti_alias;
    bool drop_shadow;
    bool outline;

    Font() {}
    Font(LPD3DXFONT _font, bool _anti_alias, bool _drop_shadow, bool _outline)
        : font(_font), anti_alias(_anti_alias), drop_shadow(_drop_shadow), outline(_outline) {}
};