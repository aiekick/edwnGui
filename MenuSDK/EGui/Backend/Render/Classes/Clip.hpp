#pragma once

struct Clip_info {
    bool OldPushingClip = false;
    bool PushingClip = false;
    RECT Clip = { -1, -1, -1, -1 };
    RECT OldClip = { -1, -1, -1, -1 };
};