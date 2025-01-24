#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderManadement/VAO.h>
#include "Helper/OpenGLHelper.h"
#include <Camera.h>

class Mesh {
protected:
	glm::mat4 scale{1};
	glm::mat4 trans{ 1 };
	glm::mat4 rotation{ 1 };
	glm::vec4 showColor{ 0, 0, 1, 1 };
public:
	void virtual Draw(GLFWwindow* window, const Camera& camera) = 0;
	glm::mat4 GetTransformMatrix() const {
		return trans * rotation * scale;
	}
	glm::mat4 virtual SetScale( glm::vec3 scale) {
		auto oldValue = this->scale;
		this->scale = glm::scale(glm::mat4{ 1 }, scale);
		return oldValue;
	}

	glm::mat4 virtual SetTranslation( glm::vec3 trans) {
		auto oldValue = this->trans;
		this->trans = glm::translate(glm::mat4{ 1 }, trans);
		return oldValue;
	}
	glm::mat4 virtual SetRotation(float radian,  glm::vec3 axis) {
		auto oldValue = this->rotation;
		this->rotation = glm::rotate(glm::mat4{ 1 }, radian, axis);
		return oldValue;
	}


	glm::mat4 virtual SetRotation(glm::mat4 rotation) {
		auto oldValue = this->rotation;
		this->rotation = rotation;
		return oldValue;
	}

	glm::vec4 virtual SetShowColor( glm::vec4 color) {
		auto oldValue = showColor;
		showColor = color;
		return oldValue;
	}

	glm::mat4 GetRotation() const {
		return rotation;
	}

	glm::mat4 GetTranslation() const {
		return trans;
	}

	glm::mat4 GetScale() const {
		return scale;
	}

	glm::vec3 GetScaleVector() const {
		return { scale[0][0], scale[1][1], scale[2][2]};
	}

	glm::vec3 GetTranslationVector() const {
		return {trans[3][0], trans[3][1], trans[3][2]};
	}
};