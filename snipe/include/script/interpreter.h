#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "singleton.h"
#include "script/parser.h"
#include "scene/scene.h"
#include "root.h"

#include <GLFW/include/GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

extern Root* root;

static bool is_skipping_to_last = false;
static bool is_breaked_by_ifstate = false;
static bool is_returned_by_ifstate = false;
static bool force_return = false;

void save_persistent(std::wstring filePath, std::wstring varName, std::wstring content);
std::wstring load_persistent(std::wstring filePath, std::wstring varName);
void interpret_screen(std::wstring src, Scene2D* currentScopeScene = nullptr, bool isContainerScope = false);
void interpret_script(std::wstring src, bool isMainScope, bool isIfScope, bool isWhileScope,  bool isCallback);

#endif