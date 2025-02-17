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
	Sphere(int stackCount = 10, int sectorCount = 10, float radius = 1);
	void Draw(GLFWwindow* window, const Camera& camera) override;
private:
	void CreateVAO(int stackCount, int sectorCount, float radius);
	static glm::vec3 SphereParam(float alfa, float beta, float r);
	static glm::vec3 SphereDerative_alfa(float alfa, float beta, float r);
	static glm::vec3 SphereDerative_beta(float alfa, float beta, float r);
	static glm::vec3 SphereNormal(float alfa, float beta);
};
