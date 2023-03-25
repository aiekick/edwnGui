#include "Math.hpp"
#include <stdlib.h>

EMath Math;

int EMath::Clamp(int x, int min, int max) {
	return x > max ? max : x < min ? min : x;
}

float EMath::Clamp(float x, float min, float max) {
	return x > max ? max : x < min ? min : x;
}

double EMath::Clamp(double x, double min, double max) {
	return x > max ? max : x < min ? min : x;
}

long EMath::Clamp(long x, long min, long max) {
	return x > max ? max : x < min ? min : x;
}

int EMath::Map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float EMath::Map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double EMath::Map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long EMath::Map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}