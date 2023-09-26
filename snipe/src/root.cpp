#include "root.h"
extern Root* root;


static void bufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    root->currentFrameEvent.window.size = { width, height };
    root->WINDOW_WIDTH = width;
    root->WINDOW_HEIGHT = height;
    utils::WINDOW_WIDTH = width;
    utils::WINDOW_HEIGHT = height;
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    root->currentFrameEvent.justKeys[key] = { scancode, action, mods };
}
static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    root->currentFrameEvent.mouse.scroll.xOffset = xoffset;
    root->currentFrameEvent.mouse.scroll.yOffset = yoffset;
}
static void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    root->currentFrameEvent.mouse.pos.xPos = xpos;
    root->currentFrameEvent.mouse.pos.yPos = ypos;
}
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    root->currentFrameEvent.mouse.buttons[button] = { action, mods };
}
static void windowCloseCallback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
static void setCharCallback(GLFWwindow* window, unsigned int codepoint) {
    for (auto& p : ((Scene2D*)root->currentScene)->getTextEditList()) {
        p->setCharCallback(codepoint);
    }
}

Root::Root() {
    currentFrameEvent.clear();
}

Root::~Root() {
    for (auto& p : sceneList) {
        delete p;
        p = nullptr;
    }
    sceneList.clear();
}

bool Root::init() {
    utils::WINDOW_WIDTH = WINDOW_WIDTH;
    utils::WINDOW_HEIGHT = WINDOW_HEIGHT;
    utils::WINDOW_TITLE = WINDOW_TITLE;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(window, WINDOW_XPOS, WINDOW_YPOS);
    setWindowFullscreen(WINDOW_FULLSCREEN);
    setWindowIcon(WINDOW_ICON_PATH);
    glfwSetFramebufferSizeCallback(window, bufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowCloseCallback(window, windowCloseCallback);
    glfwSetCharCallback(window, setCharCallback);
    glfwSwapInterval(1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	return true;
}

void Root::process_frame() {
    currentTime = lasttime; while (glfwGetTime() <= lasttime + 1.0 / WINDOW_TARGET_FPS); lasttime += 1.0 / WINDOW_TARGET_FPS;
    glClearColor(0.310f, 0.310f, 0.310f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
    for (int i = 0; i < 349; i++) { currentFrameEvent.pressedKeys[i] = glfwGetKey(window, i); }
    if (WINDOW_QUIT_ON_ESC && currentFrameEvent.justKeys[GLFW_KEY_ESCAPE].action == GLFW_PRESS) {glfwSetWindowShouldClose(window, true); return;}
    currentScene->update();
    currentScene->draw();
    currentFrameEvent.clear();
    glfwSwapBuffers(window);
    return;
}

void Root::quit() {
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
}

void Root::setWindowFullscreen(bool flag) {
    WINDOW_FULLSCREEN = flag;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (flag == true) {
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        WINDOW_WIDTH = mode->width;
        WINDOW_HEIGHT = mode->height;
        utils::WINDOW_WIDTH = mode->width;
        utils::WINDOW_HEIGHT = mode->height;
    }
    else {
        glfwSetWindowMonitor(window, nullptr, (mode->width / 2) - (WINDOW_WIDTH_ORIGIN / 2), (mode->height / 2) - (WINDOW_HEIGHT_ORIGIN / 2), WINDOW_WIDTH_ORIGIN, WINDOW_HEIGHT_ORIGIN, GLFW_DONT_CARE);
        WINDOW_WIDTH = WINDOW_WIDTH_ORIGIN;
        WINDOW_HEIGHT = WINDOW_HEIGHT_ORIGIN;
        utils::WINDOW_WIDTH = WINDOW_WIDTH_ORIGIN;
        utils::WINDOW_HEIGHT = WINDOW_HEIGHT_ORIGIN;
    }
}


void Root::setWindowIcon(std::string filePath) {
    if (filePath.empty()) return;
    GLFWimage images[1];
    stbi_set_flip_vertically_on_load(false);
    images[0].pixels = stbi_load(filePath.c_str(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);
}


Scene2D* Root::addScene2D(std::wstring _name) {
    Scene* s = new Scene2D(this, _name);
    sceneList.push_back(s);
    return (Scene2D*)s;
}

Scene3D* Root::addScene3D(std::wstring _name) {
    Scene* s = new Scene3D(this, _name);
    sceneList.push_back(s);
    return (Scene3D*)s;
}

Scene2D* Root::getScene2D(std::wstring _name) {
    for (auto& p : sceneList) {
        if (p->type == L"Scene2D" && p->name == _name) {
            return (Scene2D*)p;
        }
    }
    alert(L"cannot find scene called " + _name);
    return nullptr;
}