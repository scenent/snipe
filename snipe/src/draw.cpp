#include "draw.h"
#include "root.h"


void drawLine(Root* root, vec2 p1, vec2 p2, float width, vec4 color) {
    glViewport(0.0f, 0.0f, root->WINDOW_WIDTH, root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, root->WINDOW_WIDTH_ORIGIN, 0, root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    if (root->currentScene->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(root->currentScene->currentCamera->getViewMatrix()));
    }
    glLineWidth(width);
    glBegin(GL_LINES);
    glColor4f(color.x, color.y, color.z, color.w);
    glVertex2f(p1.x, root->WINDOW_HEIGHT_ORIGIN - p1.y);
    glVertex2f(p2.x, root->WINDOW_HEIGHT_ORIGIN - p2.y);
    glEnd();

    glPopMatrix();
}
void drawCircle(Root* root, vec2 pos, float radius, vec4 color) {
    glViewport(0.0f, 0.0f, root->WINDOW_WIDTH, root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, root->WINDOW_WIDTH_ORIGIN, 0, root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    if (root->currentScene->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(root->currentScene->currentCamera->getViewMatrix()));
    }
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(color.x, color.y, color.z, color.w);
    for (int ii = 0; ii < 360; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(360);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x + pos.x - root->currentScene->currentCamera->position.x, root->WINDOW_HEIGHT_ORIGIN + -(y + pos.y) - root->currentScene->currentCamera->position.y);
    }
    glEnd();
    glPopMatrix();
}

void drawPolygon(std::vector<vec2> vertices, vec4 color) {

}
void drawRect(Root* root, vec2 position, vec2 size, vec4 color) {
    glViewport(0.0f, 0.0f, root->WINDOW_WIDTH, root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, root->WINDOW_WIDTH_ORIGIN, 0, root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    if (root->currentScene->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(root->currentScene->currentCamera->getViewMatrix()));
    }
    glBegin(GL_QUADS);
    glColor4f(color.x, color.y, color.z, color.w);
    glVertex2f(position.x, root->WINDOW_HEIGHT_ORIGIN + -(position.y + size.y));
    glVertex2f(position.x + size.x, root->WINDOW_HEIGHT_ORIGIN + -(position.y + size.y));
    glVertex2f(position.x + size.x, root->WINDOW_HEIGHT_ORIGIN + -(position.y));
    glVertex2f(position.x, root->WINDOW_HEIGHT_ORIGIN + -(position.y));
    glEnd();

    glPopMatrix();
}

void drawCircleEx(Root* root, vec2 pos, float radius, float rotation, vec2 pivot_offset, vec2 scale, vec4 color) {
    glViewport(0.0f, 0.0f, root->WINDOW_WIDTH, root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, root->WINDOW_WIDTH_ORIGIN, 0, root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    if (root->currentScene->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(root->currentScene->currentCamera->getViewMatrix()));
    }
    glTranslatef(pos.x + (pivot_offset.x / scale.x), (root->WINDOW_HEIGHT_ORIGIN - pos.y - (pivot_offset.y / scale.y)), 0.0f);
    glRotatef(rotation, 0, 0, 1);
    glScalef(scale.x, scale.y, 1.0f);
    glTranslatef(-(pos.x + (pivot_offset.x / scale.x)), -(root->WINDOW_HEIGHT_ORIGIN - pos.y - (pivot_offset.y / scale.y)), 0.0f);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(color.x, color.y, color.z, color.w);
    for (int ii = 0; ii < 360; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(360);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x + pos.x + root->currentScene->currentCamera->position.x, root->WINDOW_HEIGHT_ORIGIN + -(y + pos.y) - root->currentScene->currentCamera->position.y);
    }
    glEnd();
    glPopMatrix();
}

void drawRectEx(Root* root, vec2 pos, vec2 size, float rotation, vec2 pivot_offset, vec2 scale, vec4 color) {
    glViewport(0.0f, 0.0f, root->WINDOW_WIDTH, root->WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, root->WINDOW_WIDTH_ORIGIN, 0, root->WINDOW_HEIGHT_ORIGIN, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    if (root->currentScene->currentCamera != nullptr) {
        glMultMatrixf(glm::value_ptr(root->currentScene->currentCamera->getViewMatrix()));
    }
    glTranslatef(pos.x + (pivot_offset.x / scale.x), (root->WINDOW_HEIGHT_ORIGIN - pos.y - (pivot_offset.y / scale.y)), 0.0f);
    glRotatef(rotation, 0, 0, 1);
    glScalef(scale.x, scale.y, 1.0f);
    glTranslatef(-(pos.x + (pivot_offset.x / scale.x)), -(root->WINDOW_HEIGHT_ORIGIN - pos.y - (pivot_offset.y / scale.y)), 0.0f);
    
    glBegin(GL_QUADS);
    glColor4f(color.x, color.y, color.z, color.w);
    glVertex2f(pos.x, root->WINDOW_HEIGHT_ORIGIN + -(pos.y + size.y));
    glVertex2f(pos.x + size.x, root->WINDOW_HEIGHT_ORIGIN + -(pos.y + size.y));
    glVertex2f(pos.x + size.x, root->WINDOW_HEIGHT_ORIGIN + -(pos.y));
    glVertex2f(pos.x, root->WINDOW_HEIGHT_ORIGIN + -(pos.y));
    glEnd();

    glPopMatrix();
}