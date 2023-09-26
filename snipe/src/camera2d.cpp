#include "objects/camera2d.h"
#include "scene/scene.h"
#include "root.h"

Camera2D::Camera2D(Scene* _owner, std::wstring _name) : Camera(_owner, _name){
    type = L"Camera2D";
}
Camera2D::~Camera2D() {

}
void Camera2D::update() {
    FrameEvent event = owner->root->currentFrameEvent;
}
void Camera2D::draw() {

}
glm::mat4 Camera2D::getViewMatrix() {
    vec2 off = offset;
    if (modify_movement == true) {
        vec2 pos = position;
        pos.rotate(rotation);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-pos.x, pos.y, 0.0f));
        view = glm::translate(view, glm::vec3(pos.x + (off.x / scale.x), (owner->root->WINDOW_HEIGHT - pos.y - (off.y / scale.y)), 0.0f));
        view = glm::rotate(view, rotation, glm::vec3(0, 0, 1));
        view = glm::scale(view, glm::vec3(zoom.x, zoom.y, 1.0f));
        view = glm::translate(view, glm::vec3(-(pos.x + (off.x / scale.x)), -(owner->root->WINDOW_HEIGHT - pos.y - (off.y / scale.y)), 0.0f));
        return view;
    }
    else {
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, position.y, 0.0f));
        view = glm::translate(view, glm::vec3(position.x + (off.x / scale.x), (owner->root->WINDOW_HEIGHT - position.y - (off.y / scale.y)), 0.0f));
        view = glm::rotate(view, rotation, glm::vec3(0, 0, 1));
        view = glm::scale(view, glm::vec3(zoom.x, zoom.y, 1.0f));
        view = glm::translate(view, glm::vec3(-(position.x + (off.x / scale.x)), -(owner->root->WINDOW_HEIGHT - position.y - (off.y / scale.y)), 0.0f));
        return view;
    }
}