#pragma once
#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
const float PI_f = 3.14159265358979323846f;
const double PI_d = 3.14159265358979323846;
inline float rad2deg(float r) { return (r * (180.0f / PI_f)); }
inline float deg2rad(float d) { return (d * (PI_f / 180.0f)); }
inline double rad2deg(double r) { return (r * (180.0 / PI_d)); }
inline double deg2rad(double d) { return (d * (PI_d / 180.0f)); }

struct vec2;
struct vec3;
struct vec4;
struct rect;

struct vec2 {
	float x, y;
	vec2() : x(0.0f), y(0.0f) {};
	vec2(int _x, int _y) : x(_x), y(_y) {};
	vec2(unsigned int _x, unsigned int _y) : x(_x), y(_y) {};
	vec2(float _x, float _y) : x(_x), y(_y) {};
	vec2(double _x, double _y) : x(_x), y(_y) {};
	~vec2() {};
	inline void operator=(const vec2& v) { x = v.x; y = v.y; }

	inline vec2 operator+(vec2 v) { return { x + v.x, y + v.y }; }
	inline vec2 operator-(vec2 v) { return { x - v.x, y - v.y }; }
	inline vec2 operator-() { return { -x, -y }; }
	inline vec2 operator*(float f) { return { x * f, y * f }; }
	inline vec2 operator/(float f) { return { x / f, y / f }; }
	inline vec2 operator/(vec2 v) { return { x / v.x, y / v.y }; }
	inline vec2 operator*(vec2 v) { return { x * v.x, y * v.y }; }

	inline void operator+=(vec2 v) { x += v.x; y += v.y; }
	inline void operator-=(vec2 v) { x -= v.x; y -= v.y; }
	inline void operator*=(float f) { x *= f; y *= f; }
	inline void operator/=(float f) { x /= f; y /= f; }

	inline bool operator==(vec2 v) { return x == v.x && y == v.y; }
	inline bool operator!=(vec2 v) { return !(v == *this); }

	inline float length() { return (sqrtf(powf(x, 2.0f) + powf(y, 2.0f))); }
	inline float distance_to(vec2 v) { vec2 temp = (v - (*this)); return (sqrtf(powf(temp.x, 2.0f) + powf(temp.y, 2.0f))); }
	inline void normalize() { if (length() == 0) { return; }vec2 temp = *this;  temp.x /= length(); temp.y /= length(); *this = temp; }
	inline vec2 normalized() { if (length() == 0) { return { 0, 0 }; }return { x / length(), y / length() }; }

	inline void rotate(float rad) { float s = sin(rad), c = cos(rad); float _x = x * c - y * s; float _y = x * s + y * c; x = _x; y = _y; }
	inline void rotateBased(float deg, vec2 base) {
		vec2 ret = { 0, 0 };
		ret.x = (x - base.x) * cos(deg2rad(deg)) - (y - base.y) * sin(deg2rad(deg)) + base.x;
		ret.y = (x - base.x) * sin(deg2rad(deg)) + (y - base.y) * cos(deg2rad(deg)) + base.y;
		this->x = ret.x;
		this->y = ret.y;
	}
	inline vec2 rotated(float rad) { float s = sin(rad), c = cos(rad);  float _x = x * c - y * s; float _y = x * s + y * c; x = _x; y = _y; return { _x, _y }; }
	inline vec2 rotatedBased(float deg, vec2 base) {
		vec2 ret = { 0, 0 };
		ret.x = (x - base.x) * cos(deg2rad(deg)) - (y - base.y) * sin(deg2rad(deg)) + base.x;
		ret.y = (x - base.x) * sin(deg2rad(deg)) + (y - base.y) * cos(deg2rad(deg)) + base.y;
		return ret;
	}
	inline void scaleBased(vec2 amount, vec2 base) {
		vec2 result = (*this);
		result -= base;
		result.x *= amount.x;
		result.y *= amount.y;
		result += base;
		*this = result;
	}
	inline vec2 scaledBased(vec2 amount, vec2 base) {
		vec2 result = (*this);
		result -= base;
		result.x *= amount.x;
		result.y *= amount.y;
		result += base;
		return result;
	}
	inline void scale(vec2 amount, vec2 base) {
		vec2 result = (*this);
		result += vec2(result.x * amount.x, result.y * amount.y);
		result -= vec2(base.x * amount.x, base.y * amount.y);
		(*this) = result;
	}
	inline vec2 scaled(vec2 amount, vec2 base) {
		vec2 result = (*this);
		result += vec2(result.x * amount.x, result.y * amount.y);
		result -= vec2(base.x * amount.x, base.y * amount.y);
		return result;
	}
	inline float getRad() { return atan2f(y, x); }
	inline float getDegree() { return rad2deg(atan2f(y, x)); }

	inline static float dot(vec2 v1, vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }
	inline static float cross(vec2 v1, vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }
	inline static vec2 cross(vec2 v, float f) { return { f * v.y, -f * v.x }; }
	inline static vec2 cross(float f, vec2 v) { return { -f * v.y, f * v.x }; }

	inline float scala() { return (x + y) / 2.0f; }

	inline void print() { std::cout << "( " << x << ", " << y << " )\n"; }
	inline std::wstring str() { return L"vec2(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L")"; }
	inline float* data() { float* result = new float[2]; result[0] = x; result[1] = y; return result; }
};

struct vec3 {
	float x, y, z;
	vec3() : x(0.0f), y(0.0f), z(0.0f) {};
	vec3(int x, int y, int z) : x(x), y(y), z(z) {};
	vec3(float x, float y, float z) : x(x), y(y), z(z) {};
	vec3(double x, double y, double z) : x(x), y(y), z(z) {};
	~vec3() {};
	inline void operator=(const vec3& v) { x = v.x; y = v.y; }

	inline vec3 operator+(vec3& v) { return { x + v.x, y + v.y, z + v.z }; }
	inline vec3 operator-(vec3& v) { return { x - v.x, y - v.y, z - v.z }; }
	inline vec3 operator*(float f) { return { x * f, y * f, z * f }; }
	inline vec3 operator/(float f) { return { x / f, y / f, z / f }; }

	inline void operator+=(vec3& v) { x += v.x; y += v.y; z += v.z; }
	inline void operator-=(vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
	inline void operator*=(float f) { x *= f; y *= f; z *= f; }
	inline void operator/=(float f) { x /= f; y /= f; z /= f; }

	inline float distance() { return (sqrtf(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f))); }
	inline void normalize() { float d = distance(); x /= d; y /= d; z /= d; }
	inline vec3 normalized() { float d = distance();  return { x / d, y / d, z / d }; }

	inline void rotateX(float rad) { vec3 _v = (*this); _v.x = 1 * this->x + 0 * this->y + 0 * this->z; _v.y = 0 * this->x + cos(rad) * this->y + sin(rad) * this->z; _v.z = 0 * this->x + -sin(rad) * this->y + cos(rad) * this->z; (*this) = _v; }
	inline void rotateY(float rad) { vec3 _v = (*this); _v.x = cos(rad) * this->x + 0 * this->y + -sin(rad) * this->z; _v.y = 0 * this->x + 1 * this->y + 0 * this->z; _v.z = sin(rad) * this->x + 0 * this->y + cos(rad) * this->z; (*this) = _v; }
	inline void rotateZ(float rad) { vec3 _v = (*this); _v.x = cos(rad) * this->x + sin(rad) * this->y + 0 * this->z; _v.y = -sin(rad) * this->x + cos(rad) * this->y + 0 * this->z; _v.z = 0 * this->x + 0 * this->y + 1 * this->z; (*this) = _v; }
	inline vec3 rotatedX(float rad) { vec3 _v = (*this); _v.x = 1 * this->x + 0 * this->y + 0 * this->z; _v.y = 0 * this->x + cos(rad) * this->y + sin(rad) * this->z; _v.z = 0 * this->x + -sin(rad) * this->y + cos(rad) * this->z; return _v; }
	inline vec3 rotatedY(float rad) { vec3 _v = (*this); _v.x = cos(rad) * this->x + 0 * this->y + -sin(rad) * this->z; _v.y = 0 * this->x + 1 * this->y + 0 * this->z; _v.z = sin(rad) * this->x + 0 * this->y + cos(rad) * this->z; return _v; }
	inline vec3 rotatedZ(float rad) { vec3 _v = (*this); _v.x = cos(rad) * this->x + sin(rad) * this->y + 0 * this->z; _v.y = -sin(rad) * this->x + cos(rad) * this->y + 0 * this->z; _v.z = 0 * this->x + 0 * this->y + 1 * this->z; return _v; }

	inline float getRadX() { return atan2f(y, x); }
	inline float getRadY() { return atan2f(y, x); }
	inline float getRadZ() { return atan2f(y, x); }
	inline float getDegreeX() { return rad2deg(atan2f(y, x)); }
	inline float getDegreeY() { return rad2deg(atan2f(y, x)); }
	inline float getDegreeZ() { return rad2deg(atan2f(y, x)); }


	inline static float dot(vec3& v1, vec3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
	inline static vec3 cross(vec3& v1, vec3& v2) { return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x }; }

	inline float* data() { float* result = new float[3]; result[0] = x; result[1] = y; result[2] = z; return result; }
	inline void print() { std::cout << x << "  " << y << "  " << z << "\n"; }
};

struct vec4 {
	float x, y, z, w;
	vec4() : x(0), y(0), z(0), w(0) {};
	vec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {};
	vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {};
	vec4 operator+(vec4 v) {
		return { x + v.x, y + v.y, z + v.z, w + v.w };
	}
	vec4 operator-(vec4 v) {
		return { x - v.x, y - v.y, z - v.z, w - v.w };
	}
	void operator+=(vec4 v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}
	void operator-=(vec4 v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}
	vec4 operator*(float f) {
		return { x * f, y * f, z * f, w * f };
	}
	vec4 operator/(float f) {
		return { x / f, y / f, z / f, w / f };
	}
	void operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;
		w *= f;
	}
	void operator/=(float f) {
		x /= f;
		y /= f;
		z /= f;
		w /= f;
	}
	std::string toString() {
		return "vec4(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + ")";
	}
	float* data() {
		float* arr = new float[4];
		arr[0] = x; arr[1] = y;
		arr[2] = z; arr[3] = w;
		return arr;
	}
	bool _is255format() {
		if (x > 1.0f) return true;
		if (y > 1.0f) return true;
		if (z > 1.0f) return true;
		if (w > 1.0f) return true;
		return false;
	}
	void _to1format() {
		x /= 255.0f;
		y /= 255.0f;
		z /= 255.0f;
		w /= 255.0f;
	}
	void print() {
		std::cout << "(" << x << " " << y << " " << z << " " << w << ")\n";
	}
};

struct rect {
	float x = 0, y = 0, w = 0, h = 0;
	rect() {};
	rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {};
	rect(vec2 pos, vec2 size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {};
	vec2 xy() {
		return vec2(x, y);
	}
	vec2 wh() {
		return vec2(w, h);
	}
	vec2 getCenter() {
		return { x + w / 2, y + h / 2 };
	}
	bool empty() {
		return (x == 0 && y == 0 && w == 0 && h == 0);
	}
	inline static bool rectVsrect(rect r1, rect r2) {
		return (r1.x + r1.w >= r2.x && r1.x <= r2.x + r2.w && r1.y + r1.h >= r2.y && r1.y <= r2.y + r2.h);
	}
	inline static bool pointVsrect(vec2 point, rect rect) {
		return (rect.x <= point.x) && (point.x <= rect.x + rect.w) && (rect.y <= point.y) && (point.y <= rect.y + rect.h);
	}
	inline void print() {
		std::cout << "\n";
		std::cout << "x = " << x << "\n";
		std::cout << "y = " << y << "\n";
		std::cout << "w = " << w << "\n";
		std::cout << "h = " << h << "\n";
		std::cout << "\n";
	}
};

struct polygon {
	std::vector<vec2> vertices{};
	static bool pointVspolygon(vec2 point, polygon poly) {
		bool result = false;
		int next = 0;
		std::vector<vec2> v = poly.vertices;
		for (int current = 0; current < v.size(); current++) {
			next = current + 1;
			if (next == v.size()) next = 0;
			vec2 vc = v[current];
			vec2 vn = v[next];
			if (((vc.y >= point.y && vn.y < point.y) || (vc.y < point.y && vn.y >= point.y)) &&
				(point.x < (vn.x - vc.x) * (point.y - vc.y) / (vn.y - vc.y) + vc.x)) {
				result = !result;
			}
		}
		return result;
	}
};

struct mat4 {
	float _m[16];
	mat4(
		float _0, float _4, float _8, float _12,
		float _1, float _5, float _9, float _13,
		float _2, float _6, float _10, float _14,
		float _3, float _7, float _11, float _15
		) {
		_m[0] = _0;
		_m[1] = _1;
		_m[2] = _2;
		_m[3] = _3;

		_m[4] = _4;
		_m[5] = _5;
		_m[6] = _6;
		_m[7] = _7;

		_m[8] = _8;
		_m[9] = _9;
		_m[10] = _10;
		_m[11] = _11;

		_m[12] = _12;
		_m[13] = _13;
		_m[14] = _14;
		_m[15] = _15;
	}
	float* data() {
		return _m;
	}
};

struct character {
	std::wstring full_name;
	vec4 color;
	character() : full_name(L""), color({ 0, 0,0, 1 }) {};
	character(std::wstring n, vec4 c) : full_name(n), color(c) {};
	character(std::wstring n) : full_name(n), color({0, 0, 0, 1}) {};
	void print() {
		std::wcout << full_name << "  (" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << "\n";
	}
};

#endif
