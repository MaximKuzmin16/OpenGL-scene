#pragma once
#include "GLFW/glfw3.h"
#include "map"

typedef struct {
    bool pressed, up, down;
} Key;

class Input {
public:
    std::map<int,Key> keys;

    GLFWwindow* win;

    int centerX, centerY;

    double mouseDeltaX, mouseDeltaY;

    Input(GLFWwindow* win) {
        this->win = win;

        UpdateCenter();

        for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_WORLD_2; i++)
        {
            Key key = { false, false, false };
            keys.insert(std::pair<int, Key>(i, key));
        }
    }

    void UpdateCenter()
    {
        glfwGetWindowSize(win, &centerX, &centerY);

        centerX /= 2;
        centerY /= 2;
    }

    void Update() {
        glfwGetCursorPos(win, &mouseDeltaX, &mouseDeltaY);

        mouseDeltaX -= centerX;
        mouseDeltaY -= centerY;

        glfwSetCursorPos(win, centerX, centerY);

        for (auto& it : keys) {
            it.second.down = it.second.up = false;
            if (glfwGetKey(win, it.first) == GLFW_PRESS) {
                if (!it.second.pressed) {
                    it.second.down = true;
                }
                it.second.pressed = true;
            }
            else {
                if (it.second.pressed) {
                    it.second.up = true;
                }
                it.second.pressed = false;
            }
        }
    }
};

Input* input;