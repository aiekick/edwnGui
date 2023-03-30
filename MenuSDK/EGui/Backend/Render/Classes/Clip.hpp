#pragma once
#include <stack>

struct Clip_info {
    bool PushingClip;
    Rect Clip;
    std::stack<std::pair<bool, Rect>> OldClips;

    void clear() {
        PushingClip = false;
        Clip = {};
        while (!OldClips.empty()) {
            OldClips.pop();
        }
    }
};