#include "objects/sprite2d.h"
#include "scene/scene.h"
#include "root.h"

Sprite2D::Sprite2D(Scene* _owner, std::wstring _name) : Object2D(_owner, _name) {
    type = L"Sprite2D";
}

void Sprite2D::load(std::wstring _filePath) {
    filePath = _filePath;
    if (filePath == L"") return;
    if (textureID != 0) glDeleteTextures(1, &textureID);
    int width, height, channels;
    unsigned char* data = stbi_load(ws2s(_filePath).c_str(), &width, &height, &channels, 0);
    this->size.x = width;
    this->size.y = height;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixel_filter == true ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixel_filter == true ? GL_LINEAR : GL_NEAREST);
    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        alert(_filePath + L" :: cannot load image file :: invalid channels " + std::to_wstring(channels));
    stbi_image_free(data);
}

Sprite2D:: ~Sprite2D() {
    glDeleteTextures(1, &textureID);
}

void Sprite2D::update() {

}

void Sprite2D::draw() {
    if (filePath == L"") return;
    if (!visible || textureID == 0) return;
    glViewport(0.0f, 0.0f, owner->root->WINDOW_WIDTH, owner->root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, owner->root->WINDOW_WIDTH_ORIGIN, 0, owner->root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    if (owner->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(owner->currentCamera->getViewMatrix()));
    }

    glTranslatef(position.x + (pivot_offset.x / scale.x), (owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);
    glScalef(scale.x, scale.y, 1.0f);
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(-(position.x + (pivot_offset.x / scale.x)), -(owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);
    

    glColor4f(modulate.x, modulate.y, modulate.z, modulate.w);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); 
    glVertex2f(position.x,          owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y + size.y));
    glTexCoord2f(1.0f, 1.0f); 
    glVertex2f(position.x + size.x, owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y + size.y));
    glTexCoord2f(1.0f, 0.0f); 
    glVertex2f(position.x + size.x, owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y));
    glTexCoord2f(0.0f, 0.0f); 
    glVertex2f(position.x,          owner->root->WINDOW_HEIGHT_ORIGIN + -(position.y));
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}