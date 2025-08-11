#pragma once
#include "grn/vector.h"

namespace grn
{
    struct KeyInput
    {
        bool keysLastFrame[512] = {};
        bool keys[512] = {};

        void setKeyPressed(int key, bool value) { keys[key] = value; }
        bool getKeyPressed(int key) { return keys[key]; }
        bool getKeyDown(int key) { return keys[key] && !keysLastFrame[key]; }

        void updateFrame()
        {
            for (int i = 0; i < 512; i++)
            {
                keysLastFrame[i] = keys[i];
            }
        }
    };
    struct MouseInput
    {

        Vector2f position;
        Vector2f lastPosition;

        bool buttonsLastFrame[512];
        bool buttons[512];
        void setButtonPressed(int button, bool value) { buttons[button] = value; }
        bool getButtonPressed(int button) { return buttons[button]; }
        bool getButtonDown(int button) { return buttons[button] && !buttonsLastFrame[button]; }

        Vector2f getMouseDelta()
        {
            return position - lastPosition;
        }
        void setMousePosition(float x, float y)
        {
            lastPosition = position;
            position.x = x;
            position.y = y;
        }

        void updateFrame()
        {
            for (int i = 0; i < 512; i++)
            {
                buttonsLastFrame[i] = buttons[i];
            }
            lastPosition = position;
        }
    };

    struct Input
    {
        KeyInput KeyInput;
        MouseInput mouseInput;

        Vector2f getPlayerNavigation();
    };
}
