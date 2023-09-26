#pragma once
#ifndef FRAMEEVENT_H
#define FRAMEEVENT_H

struct KeyInfo { 
    int scancode, action, mods; 
};
struct MouseInfo {
    struct MousePosition { double xPos, yPos; };
    struct MouseButton { int action, mods; };
    struct MouseScroll { double xOffset, yOffset; };
    MousePosition pos;
    MouseButton buttons[3];
    MouseScroll scroll;
};
struct WindowInfo {
    struct WindowSize { int width, height; };
    WindowSize size;
};

struct FrameEvent {
    KeyInfo justKeys[349]; 
    bool pressedKeys[349];  
    MouseInfo mouse; 
    WindowInfo window;
    void clear() {
        for (int i = 0; i < 3; i++) mouse.buttons[i] = { -1, 0 };
        for (int i = 0; i < 349; i++) justKeys[i] = { 0, 0, 0 };
        for (int i = 0; i < 349; i++) pressedKeys[i] = false;
        mouse.scroll = {};
    }
};

#endif