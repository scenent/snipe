#pragma once
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "objects/object2d.h"
#include "objects/camera.h"

class Camera2D : public Camera {
public:
    bool modify_movement = true;
public:
    Camera2D(Scene* _owner, std::wstring _name);
    ~Camera2D();
    void update() override;
    void draw() override;
    glm::mat4 getViewMatrix() override;
};

#endif