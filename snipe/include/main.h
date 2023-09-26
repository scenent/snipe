#pragma once
#ifndef MAIN_H
#define MAIN_H
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define MINIAUDIO_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

bool SNIPE_USING_MULTITHREAD = true;

#include <vector>
#include <string>
#include <iostream>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "root.h"
#include "scene/scene.h"
#include "objects/object.h"
#include "utils.h"
#include "math.h"
#include "audio/audio.h"
#include "script/parser.h"

#include "script/interpreter.h"

#endif
