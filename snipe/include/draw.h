#pragma once
#ifndef DRAW_H
#define DRAW_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/include/glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

struct vec2;
struct vec4;
class Root;

void drawLine(Root* root, vec2 p1, vec2 p2, float width, vec4 color);
void drawCircle(Root* root, vec2 pos, float radius, vec4 color);
void drawRect(Root* root, vec2 position, vec2 size, vec4 color);
void drawPolygon(Root* root, std::vector<vec2> vertices, vec4 color);

void drawCircleEx(Root* root, vec2 pos, float radius, float rotation, vec2 pivot_offset, vec2 scale, vec4 color);
void drawRectEx(Root* root, vec2 pos, vec2 size, float rotation, vec2 pivot_offset, vec2 scale, vec4 color);

#endif