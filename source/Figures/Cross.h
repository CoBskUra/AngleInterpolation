#pragma once
#include "Cylinder.h"

class Cross : public Mesh {
private:
    static constexpr int cylindersNumber = 3;
    Cylinder cylinders[cylindersNumber];
    glm::mat4 rotationCylinder[cylindersNumber];
    glm::vec4 colors[cylindersNumber];

public:
    Cross(int samples, float height, float radius);

    void Draw(GLFWwindow* window, const Camera& camera) override;
    glm::mat4 SetScale(glm::vec3 scale) override;
    glm::mat4 SetTranslation(glm::vec3 transVector) override;
    glm::mat4 SetRotation(float radian, glm::vec3 axis) override;
    glm::mat4 SetRotation(glm::mat4 rotation) override;
    glm::vec4 SetShowColor(glm::vec4 color) override;
};
