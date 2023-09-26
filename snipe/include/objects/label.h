#pragma once
#ifndef LABEL_H
#define LABEL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <freetype/include/ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftstroke.h>

#include "shader.h"
#include "objects/object2d.h"

enum class LabelWrapMode {
	Normal = 0, Word = 1
};

enum class LabelAlignMode {
	Begin, Center, End
};

struct Character {
	long int     data;
	unsigned int textureID;
	vec2         size;
	vec2         bearing;
	unsigned int advance;

};

class Label : public Object2D {
public:
	std::vector<Character> outline_characters{};
	std::vector<Character> characters{};
	std::wstring text         = L"";

	std::wstring fontPath     = L"";
	int fontSize              = 16;
	float fontScale           = 1.0f;

	vec2 pivot_offset         = { 0, 0 };
	vec2 glyph_size = { 0, 0 };
	vec4 color                = { 0, 0, 0, 1 };
	bool gradient             = false;
	vec4 gradient_color       = { 0, 0, 0, 1 };
	vec4 outline_color        = { 1, 1, 1, 1 };
	float outline_size        = 0;
	float shearing_amount     = 0.0f;
	float y_separation        = 0.1f;
	float shaking             = 0.0f;

	rect clipping_rect = { 0, 0, 0, 0 };

	double percent_visible    = 1.0;
	bool wrap_text            = true; 
	LabelAlignMode align_mode = LabelAlignMode::Center;
	LabelWrapMode wrap_mode   = LabelWrapMode::Normal;
public:
	Label(Scene* _owner, std::wstring _name);
	~Label();
	void load(std::wstring _fontPath);
	void update() override;
	void draw() override;
};


#endif