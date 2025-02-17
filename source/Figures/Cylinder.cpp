#include "Cylinder.h"

Cylinder::Cylinder(int samples, float height, float radius)
    : sphere_1(samples, samples, radius), sphere_2(samples, samples, radius), _shader(StaticShaders::GetPhongShader()), basicTrans(0, 0, height) {
    CreateVAO(samples, height, radius);
    sphere_1.SetShowColor(showColor);
    sphere_2.SetShowColor(showColor);
    SetTranslation(GetTranslationVector());
    SetRotation(GetRotation());
    SetScale(GetScaleVector());
}

glm::mat4 Cylinder::SetScale(glm::vec3 scale) {
    auto oldValue = Mesh::SetScale(scale);
    scale.z = scale.y;
    sphere_1.SetScale(scale);
    sphere_2.SetScale(scale);
    return oldValue;
}

glm::mat4 Cylinder::SetTranslation(glm::vec3 transVector) {
    auto oldValue = Mesh::SetTranslation(transVector);
    sphere_1.SetTranslation(transVector);
    auto tmp = glm::vec4{ basicTrans, 0 };
    tmp = rotation * tmp;
    sphere_2.SetTranslation(transVector + glm::vec3(tmp));
    return oldValue;
}

glm::vec4 Cylinder::SetShowColor(glm::vec4 color) {
    auto oldValue = Mesh::SetShowColor(color);
    sphere_1.SetShowColor(color);
    sphere_2.SetShowColor(color);
    return oldValue;
}

glm::mat4 Cylinder::SetRotation(float radian, glm::vec3 axis) {
    return SetRotation(glm::rotate(glm::mat4{ 1 }, radian, axis));
}

glm::mat4 Cylinder::SetRotation(glm::mat4 rotation) {
    auto oldValue = Mesh::SetRotation(rotation);
    sphere_1.SetRotation(rotation);
    sphere_2.SetRotation(rotation);
    SetTranslation(GetTranslationVector());
    return oldValue;
}

void Cylinder::Draw(GLFWwindow* window, const Camera& camera) {
    _shader.Activate();
    vao.Bind();
    {
        glUniformMatrix4fv(glGetUniformLocation(_shader.ID, "MODEL_MATRIX"),
            1, GL_FALSE, glm::value_ptr(GetTransformMatrix()));
        glUniform4f(glGetUniformLocation(_shader.ID, "COLOR"), showColor.x, showColor.y, showColor.z, showColor.w);
        camera.SaveMatrixToShader(_shader.ID);
        auto cameraPos = camera.GetPosition();
        glUniform3f(glGetUniformLocation(_shader.ID, "CAMERA_POS"), cameraPos.x, cameraPos.y, cameraPos.z);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numberOfVs);
    }
    vao.Unbind();
    sphere_1.Draw(window, camera);
    sphere_2.Draw(window, camera);
}

void Cylinder::CreateVAO(int samples, float height, float radius) {
    std::vector<float> vs;
    if (samples > 0) {
        for (int i = 0; i < samples + 1; i++) {
            float alfa = M_PI * 2.0f * (float)(i % samples) / (float)samples;
            float cos_alfa = cosf(alfa);
            float sin_alfa = sinf(alfa);
            glm::vec3 down_left{ cos_alfa * radius, sin_alfa * radius, 0 };
            glm::vec3 up_left{ cos_alfa * radius, sin_alfa * radius, height };
            glm::vec3 normal{ cos_alfa, sin_alfa, 0 };
            OpenGLHelper::AddVecToVector(vs, down_left);
            OpenGLHelper::AddVecToVector(vs, normal);
            OpenGLHelper::AddVecToVector(vs, up_left);
            OpenGLHelper::AddVecToVector(vs, normal);
        }
    }
    numberOfVs = samples * 2 + 2;
    vao.Reactive();
    vao.Bind();
    VBO vbo(vs, GL_STATIC_DRAW);
    vbo.Bind();
    vao.LinkAttrib(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
    vao.LinkAttrib(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.Unbind();
    vbo.Unbind();
}
