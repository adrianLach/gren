#include <GLFW/glfw3.h>
#include <grn/input.h>
#include "grn/vector.h"
#include "grn/logger.h"

namespace grn
{

    Vector2f grn::Input::getPlayerNavigation()
    {
        Vector2f direction;
        if (KeyInput.getKeyPressed(GLFW_KEY_W))
            direction.y += 1;
        if (KeyInput.getKeyPressed(GLFW_KEY_S))
            direction.y -= 1;
        if (KeyInput.getKeyPressed(GLFW_KEY_A))
            direction.x += 1;
        if (KeyInput.getKeyPressed(GLFW_KEY_D))
            direction.x -= 1;
        return direction.normalize();
    }

} // namespace grn