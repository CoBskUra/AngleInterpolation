#include "Cylinder.h"

class Cross : public Mesh {
	const int cylindersNumber = 3;
	Cylinder cylinders[3];
	glm::mat4 rotationCylinder[3]{
		glm::rotate(glm::mat4{1},  glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		glm::rotate(glm::mat4{1},  glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::mat4{1}
	};
	glm::vec4 colors[3]{
		{1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1}
	};
public:

	Cross(int samples, float height, float radius) : 
		cylinders{ 
			{ samples, height, radius },
			{ samples, height, radius }, 
			{ samples, height, radius } } 
	{
		SetShowColor({ 1, 1, 1, 1 });
	}




	void Draw(GLFWwindow* window, const Camera& camera) override {
		for (int i = 0; i < cylindersNumber; i++) {
			cylinders[i].Draw(window, camera);
		}
	}

	glm::mat4 SetScale(glm::vec3 scale) override {
		auto oldValue = Mesh::SetScale(scale);
		for (int i = 0; i < cylindersNumber; i++) {
			cylinders[i].SetScale(scale);
		}

		return oldValue;
	}
	glm::mat4 SetTranslation(glm::vec3 transVector) override {
		auto oldValue = Mesh::SetTranslation(transVector);

		for (int i = 0; i < cylindersNumber; i++) {
			cylinders[i].SetTranslation(transVector);
		}
		return oldValue;
	}
	glm::mat4 SetRotation(float radian, glm::vec3 axis) override {
		auto oldValue = Mesh::SetRotation(radian, axis);
		for (int i = 0; i < cylindersNumber; i++) {
			cylinders[i].SetRotation(rotation * rotationCylinder[i]);
		}
		return oldValue;
	}

	glm::mat4 SetRotation(glm::mat4 rotation) override {
		auto oldValue = Mesh::SetRotation(rotation);
		for (int i = 0; i < cylindersNumber; i++) {
			cylinders[i].SetRotation(rotation * rotationCylinder[i]);
		}
		return oldValue;
	}

	glm::vec4 SetShowColor(glm::vec4 color) override {
		auto oldValue = Mesh::SetShowColor(color);
		for (int i = 0; i < cylindersNumber; i++) {
			glm::vec4 tmpColor{ 0, 0, 0, 1 };
			tmpColor[i] = color[i];
			cylinders[i].SetShowColor(tmpColor);
		}
		return oldValue;
	}
};
