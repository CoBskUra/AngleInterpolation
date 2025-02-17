// Cylinder.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderManadement/VAO.h>
#include "Helper/OpenGLHelper.h"
#include "Mesh.h"
#include "ShaderRefrence/StaticShaders.h"
#include "Sphere.h"

class Cylinder : public Mesh {
private:
    VAO vao;
    Shader _shader = StaticShaders::GetPhongShader();
    int numberOfVs;
    glm::vec3 basicTrans;
    Sphere sphere_1;
    Sphere sphere_2;

public:
    Cylinder(int samples = 0, float height = 1, float radius = 1);
    Cylinder(Cylinder& c) = default;

    glm::mat4 SetScale(glm::vec3 scale) override;
    glm::mat4 SetTranslation(glm::vec3 transVector) override;
    glm::vec4 SetShowColor(glm::vec4 color) override;
    glm::mat4 SetRotation(float radian, glm::vec3 axis) override;
    glm::mat4 SetRotation(glm::mat4 rotation) override;
    void Draw(GLFWwindow* window, const Camera& camera) override;
    void CreateVAO(int samples, float height, float radius);
};


