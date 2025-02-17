#include "Cross.h"

Cross::Cross(int samples, float height, float radius)
    : cylinders{
        {samples, height, radius},
        {samples, height, radius},
        {samples, height, radius}
    },
    rotationCylinder{
        glm::rotate(glm::mat4{1}, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::rotate(glm::mat4{1}, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::mat4{1}
    },
    colors{
        {1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1}
    } {
    SetShowColor({ 1, 1, 1, 1 });
}

void Cross::Draw(GLFWwindow* window, const Camera& camera) {
    for (int i = 0; i < cylindersNumber; i++) {
        cylinders[i].Draw(window, camera);
    }
}

glm::mat4 Cross::SetScale(glm::vec3 scale) {
    auto oldValue = Mesh::SetScale(scale);
    for (int i = 0; i < cylindersNumber; i++) {
        cylinders[i].SetScale(scale);
    }
    return oldValue;
}

glm::mat4 Cross::SetTranslation(glm::vec3 transVector) {
    auto oldValue = Mesh::SetTranslation(transVector);
    for (int i = 0; i < cylindersNumber; i++) {
        cylinders[i].SetTranslation(transVector);
    }
    return oldValue;
}

glm::mat4 Cross::SetRotation(float radian, glm::vec3 axis) {
    auto oldValue = Mesh::SetRotation(radian, axis);
    for (int i = 0; i < cylindersNumber; i++) {
        cylinders[i].SetRotation(rotation * rotationCylinder[i]);
    }
    return oldValue;
}

glm::mat4 Cross::SetRotation(glm::mat4 rotation) {
    auto oldValue = Mesh::SetRotation(rotation);
    for (int i = 0; i < cylindersNumber; i++) {
        cylinders[i].SetRotation(rotation * rotationCylinder[i]);
    }
    return oldValue;
}

glm::vec4 Cross::SetShowColor(glm::vec4 color) {
    auto oldValue = Mesh::SetShowColor(color);
    for (int i = 0; i < cylindersNumber; i++) {
        glm::vec4 tmpColor{ 0, 0, 0, 1 };
        tmpColor[i] = color[i];
        cylinders[i].SetShowColor(tmpColor);
    }
    return oldValue;
}
