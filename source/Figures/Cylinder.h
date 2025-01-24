#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderManadement/VAO.h>
#include "Helper/OpenGLHelper.h"
#include "Mesh.h"
#include "ShaderRefrence/StaticShaders.h"
#include "Spere.h"

class Cylinder: public Mesh {
	VAO vao;
	Shader _shader = StaticShaders::GetPhongShader();
	int numberOfVs;
	glm::vec3 basicTrans{0,0,0};
	Sphere sphere_1;
	Sphere sphere_2;
public:
	Cylinder(Cylinder &c) = default;

	glm::mat4 SetScale(glm::vec3 scale) override {
		auto oldValue = Mesh::SetScale(scale);
		scale.z = scale.y;
		sphere_1.SetScale(scale);
		sphere_2.SetScale(scale);

		return oldValue;
	}
	glm::mat4 SetTranslation(glm::vec3 transVector) override {
		auto oldValue = Mesh::SetTranslation(transVector);

		sphere_1.SetTranslation(transVector);
		auto tmp = glm::vec4{ basicTrans, 0 };
		tmp = rotation* tmp;
		sphere_2.SetTranslation(transVector + glm::vec3(tmp));
		return oldValue;
	}


	glm::vec4 SetShowColor(glm::vec4 color) override {
		auto oldValue = Mesh::SetShowColor(color);
		sphere_1.SetShowColor(color);
		sphere_2.SetShowColor(color);
		return oldValue;
	}


	glm::mat4 SetRotation(float radian, glm::vec3 axis) override {
		auto oldValue = SetRotation (glm::rotate(glm::mat4{ 1 }, radian, axis));
		return oldValue;
	}

	glm::mat4 SetRotation(glm::mat4 rotation) override {
		auto oldValue = Mesh::SetRotation(rotation);
		sphere_1.SetRotation(rotation);
		sphere_2.SetRotation(rotation);
		SetTranslation(GetTranslationVector());

		return oldValue;
	}

	void Draw(GLFWwindow* window, const Camera& camera) override {
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

	Cylinder(int samples = 0, float height = 1, float radius = 1): sphere_1{ samples, samples, radius }, sphere_2{ samples, samples, radius } {
		CreateVAO(samples, height, radius);
		basicTrans = glm::vec3{0, 0, height};
		sphere_1.SetShowColor(showColor);
		sphere_2.SetShowColor( showColor);
		SetTranslation(GetTranslationVector());
		SetRotation(GetRotation());
		SetScale(GetScaleVector());
	}

	void CreateVAO(int samples, float height = 1, float radius = 1) {
		std::vector<float> vs;
		if(samples > 0)
		for (int i = 0; i < samples + 1; i++) {
			float alfa = M_PI * 2.0f * (float)(i % samples) / (float)samples;
			float alfa_next = M_PI * 2.0f * (float)((i + 1) % samples) / (float)samples;
			float cos_alfa = cosf(alfa);
			float cos_alfa_next = cosf(alfa_next);
			float sin_alfa = sinf(alfa);
			float sin_alfa_next = sinf(alfa_next);

			glm::vec3 down_left{ cos_alfa * radius, sin_alfa * radius, 0 };
			glm::vec3 up_left{ cos_alfa * radius, sin_alfa * radius, height };

			glm::vec3 down_left_n{ cos_alfa, sin_alfa, 0 };
			glm::vec3 up_left_n{ cos_alfa, sin_alfa, 0 };

			OpenGLHelper::AddVecToVector(vs, down_left);
			OpenGLHelper::AddVecToVector(vs, down_left_n);

			OpenGLHelper::AddVecToVector(vs, up_left);
			OpenGLHelper::AddVecToVector(vs, up_left_n);
		}
		numberOfVs = samples * 2 + 2;
		vao.Reactive();
		vao.Bind();
		VBO vbo(vs, GL_STATIC_DRAW);
		vbo.Bind();

		vao.LinkAttrib(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
		vao.LinkAttrib(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		vao.Unbind(); vbo.Unbind();
	}
};