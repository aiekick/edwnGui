#pragma once

struct Clip_info {
    bool OldPushingClip, PushingClip;
    RECT Clip, OldClip;

    void clear() {
        OldPushingClip, PushingClip = NULL;
        Clip, OldClip = {};
    }
};