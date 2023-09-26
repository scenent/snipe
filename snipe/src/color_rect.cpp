#include "objects/color_rect.h"
#include "scene/scene.h"
#include "root.h"


ColorRect::ColorRect(Scene* _owner, std::wstring _name) : Object2D(_owner, _name){
    type = L"ColorRect";
    size = { 40, 40 };
}

ColorRect::~ColorRect() {
}

void ColorRect::update() {
    FrameEvent event = owner->root->currentFrameEvent;
}

void ColorRect::draw() {
    if (!visible) return;

    glViewport(0.0f, 0.0f, owner->root->WINDOW_WIDTH, owner->root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, owner->root->WINDOW_WIDTH_ORIGIN, 0, owner->root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    if (owner->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(owner->currentCamera->getViewMatrix()));
    }

    glTranslatef(position.x + (pivot_offset.x / scale.x), (owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);
    glRotatef(rotation, 0, 0, 1);
    glScalef(scale.x, scale.y, 1.0f);
    glTranslatef(-(position.x + (pivot_offset.x / scale.x)), -(owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);

    glBegin(GL_QUADS);
    glColor4f(color.x, color.y, color.z, color.w);
    glVertex2f(position.x,          owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y + size.y));
    glVertex2f(position.x + size.x, owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y + size.y));
    glVertex2f(position.x + size.x, owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y));
    glVertex2f(position.x,          owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y));
    glEnd();

    glPopMatrix();
}