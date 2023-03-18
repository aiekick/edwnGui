#pragma once
#include "../../EGui.hpp"

struct ViewCamera {
    D3DXVECTOR3 m_position;
    D3DXVECTOR3 m_lookAt;
    D3DXVECTOR3 m_upVector;

    float m_yaw;
    float m_pitch;
    float m_roll;

    void SetPosition(Vec ang);
    void Update();
};

extern ViewCamera Camera;