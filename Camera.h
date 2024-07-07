class Camera
{
public:
    glm::vec3 Position;
    glm::vec2 Rotation;
    glm::vec3 Facing;

    const float SPEED = 5.0f;

    bool isLocked = false;

    Camera()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Rotation = glm::vec2(0.0f, 0.0f);
    }

    Camera(float posX, float posY, float posZ)
    {
        Position = glm::vec3(posX, posY, posZ);
        Rotation = glm::vec2(0, 0);
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Facing, glm::vec3(0,1,0));
    }

    void Update(float deltaTime)
    {
        if (!isLocked)
        {
            /*Rotation.x -= ((SCR_WIDTH / 2) - input->mouseDeltaX) / 2.f;
            Rotation.y += ((SCR_HEIGHT / 2) - input->mouseDeltaY) / 2.f;*/

            Rotation.x += input->mouseDeltaX;
            Rotation.y -= input->mouseDeltaY;

            if (Rotation.y > 89.0f)
                Rotation.y = 89.0f;
            if (Rotation.y < -89.0f)
                Rotation.y = -89.0f;

            while (Rotation.x > 180)
                Rotation.x -= 360;

            while (Rotation.x < -180)
                Rotation.x += 360;

            Facing.x = cos(glm::radians(Rotation.x)) * cos(glm::radians(Rotation.y));
            Facing.y = sin(glm::radians(Rotation.y));
            Facing.z = sin(glm::radians(Rotation.x)) * cos(glm::radians(Rotation.y));

            Facing = glm::normalize(Facing);
        }
        
        float velocity = SPEED * deltaTime;

        if (input->keys[GLFW_KEY_LEFT_CONTROL].pressed)
        {
            velocity *= 12;
        }

        if (input->keys[GLFW_KEY_LEFT_SHIFT].pressed)
        {
            velocity /= 6;
        }

        if (input->keys[GLFW_KEY_W].pressed)
        {
            Position += Facing * velocity;
        }

        if (input->keys[GLFW_KEY_S].pressed)
        {
            Position -= Facing * velocity;
        }

        if (input->keys[GLFW_KEY_D].pressed)
        {
            Position += glm::normalize(glm::cross(Facing, glm::vec3(0,1,0))) * velocity;
        }

        if (input->keys[GLFW_KEY_A].pressed)
        {
            Position -= glm::normalize(glm::cross(Facing, glm::vec3(0, 1, 0))) * velocity;
        }

        if (input->keys[GLFW_KEY_C].down)
        {
            isLocked = !isLocked;
        }
    }
};