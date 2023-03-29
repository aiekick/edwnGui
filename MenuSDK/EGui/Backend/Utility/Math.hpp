#pragma once
#include "../../EGui.hpp"
#include <DirectXMath.h>
#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

struct EMath {
	template<class T>
	constexpr const T& Clamp(const T& x, const T& min, const T& max) {
		return x > max ? max : x < min ? min : x;
	}

	template<class T>
	constexpr const T& Min(const T& x, const T& min) {
		return x > min ? x : min;
	}

	const float Max(const float x, const float max) {
		return x < max ? x : max;
	}

	template<class T>
	constexpr const T& Map(const T& x, const T& in_min, const T& in_max, const T& out_min, const T& out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	template<class T>
	constexpr const T& GetPercent(const T& x, const T& max) {
		return (x / max) * 100;
	}

	template<class T>
	constexpr const T& RandomInt(const T& min, const T& max) {
		return rand() % max + min;
	}

	template<class T>
	constexpr const T& RandomFloat(const T& min, const T& max) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;

		return min + r;
	}

	/* https://github.com/Rezonality/mutils/blob/6adc3fa9d160f455092637032cb6fcd6162d0b93/include/mutils/math/math_utils.h#L109 */
	template <typename T>
	inline T Cube(T f) {
		return f * f * f;
	}

	/* https://github.com/Rezonality/mutils/blob/6adc3fa9d160f455092637032cb6fcd6162d0b93/include/mutils/math/math_utils.h#L116 */
	template <typename T>
	inline T Square(T f) {
		return f * f;
	}

	/* https://github.com/Rezonality/mutils/blob/6adc3fa9d160f455092637032cb6fcd6162d0b93/include/mutils/math/math_utils.h#L124 */
	/* time (delta time), x, y, x2, y2 */
	template <typename T>
	inline T Bezier(float t, T p0, T p1, T p2, T p3) {
		return Cube(1 - t) * p0 + 3 * Square(1 - t) * t * p1 + 3 * (1 - t) * Square(t) * p2 + Cube(t) * p3;
	}

	float toRadians = D3DX_PI / 180.f;
	float toDegrees = 180.f / D3DX_PI;
};

extern EMath Math;