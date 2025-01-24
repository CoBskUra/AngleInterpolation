#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderManadement/VAO.h>
#include "Helper/OpenGLHelper.h"
#include "Mesh.h"
#include "ShaderRefrence/StaticShaders.h"
#include <ShaderManadement/EBO.h>

class Sphere : public Mesh {
	VAO vao;
	const Shader _shader = StaticShaders::GetPhongShader();
	int numberOfVs;
public:

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
			glDrawElements(GL_TRIANGLES, numberOfVs, GL_UNSIGNED_INT, (void*)0);
		}
		vao.Unbind();
	}

	Sphere(int stackCount = 10, int sectorCount = 10, float radius = 1) {
		CreateVAO(stackCount, sectorCount, radius);
	}

	void CreateVAO(int stackCount, int sectorCount, float radius) {
		std::vector<float> vs;
		std::vector<GLuint> is;
		float stackStep = M_PI / (float)stackCount;
		float sectorStep = 2.0f * M_PI / (float)sectorCount;
		float sectorAngle, stackAngle;
		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = M_PI / 2.0f - ((float)i) * stackStep;        // starting from pi/2 to -pi/2
			
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = (float)j * sectorStep;           // starting from 0 to 2pi
				auto p = SphereParam(stackAngle, sectorAngle, radius);
				
				OpenGLHelper::AddVecToVector(vs, p);
				auto norm = glm::normalize(SphereNormal(stackAngle, sectorAngle));
				// normalized vertex normal (nx, ny, nz)
				OpenGLHelper::AddVecToVector(vs, norm);
			}
		}

		for (int i = 0; i < stackCount; ++i)
		{

			int k1 = i * (sectorCount + 1);     // beginning of current stack
			int k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					is.push_back(k1);
					is.push_back(k2);
					is.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					is.push_back(k1 + 1);
					is.push_back(k2);
					is.push_back(k2 + 1);
				}
			}
		}
		numberOfVs = is.size();
		vao.Reactive();
		vao.Bind();
		VBO vbo(vs, GL_STATIC_DRAW);
		EBO ebo(is);

		vao.LinkAttrib(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
		vao.LinkAttrib(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		vao.Unbind(); vbo.Unbind(); ebo.Unbind();
	}


	inline static glm::vec3 SphereParam(float alfa, float beta, float r) {
		float cos_alfa = cosf(alfa);
		float sin_alfa = sinf(alfa);

		float cos_beta = cosf(beta);
		float sin_beta = sinf(beta);

		return { r * cos_alfa * cos_beta , r * cos_alfa * sin_beta, r * sin_alfa };
	}

	inline static glm::vec3 SphereDerative_alfa(float alfa, float beta, float r) {
		float cos_alfa = cosf(alfa);
		float sin_alfa = sinf(alfa);

		float cos_beta = cosf(beta);
		float sin_beta = sinf(beta);


		return { -r * sin_alfa * cos_beta , -r * sin_alfa * sin_beta, r * cos_alfa };
		
	}

	inline static glm::vec3 SphereDerative_beta(float alfa, float beta, float r) {
		float cos_alfa = cosf(alfa);
		float sin_alfa = sinf(alfa);

		float cos_beta = cosf(beta);
		float sin_beta = sinf(beta);


		return {- r * cos_alfa * sin_beta , r * cos_alfa * cos_beta, 0 };
	}

	inline static glm::vec3 SphereNormal(float alfa, float beta) {

		return SphereParam(alfa, beta, 1) ;
	}
};