#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderManadement/VAO.h>
#include "Helper/OpenGLHelper.h"
#include <Camera.h>
#include <ShaderRefrence/StaticShaders.h>


class InfinityGrid
{
public:
	InfinityGrid();

	void  Draw(GLFWwindow* window, const Camera& camera) ;

	bool Inputs(GLFWwindow* window, const Camera& camera);


private:
	VAO vao;
	const Shader& shader = StaticShaders::GetInfinityGrid();

	void CreateInfinityGridVAO();
};

