#pragma once

struct EMath {
	int Clamp(int x, int min, int max);
	float Clamp(float x, float min, float max);
	double Clamp(double x, double min, double max);
	long Clamp(long x, long min, long max);

	int Map(int x, int in_min, int in_max, int out_min, int out_max);
	float Map(float x, float in_min, float in_max, float out_min, float out_max);
	double Map(double x, double in_min, double in_max, double out_min, double out_max);
	long Map(long x, long in_min, long in_max, long out_min, long out_max);

	float GetPercent(float x, float max) {
		return (x / max) * 100;
	}
};

extern EMath Math;