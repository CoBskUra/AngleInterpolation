#include "Sphere.h"

Sphere::Sphere(int stackCount, int sectorCount, float radius) {
	CreateVAO(stackCount, sectorCount, radius);
}

void Sphere::Draw(GLFWwindow* window, const Camera& camera) {
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

void Sphere::CreateVAO(int stackCount, int sectorCount, float radius) {
	std::vector<float> vs;
	std::vector<GLuint> is;
	float stackStep = M_PI / (float)stackCount;
	float sectorStep = 2.0f * M_PI / (float)sectorCount;
	float sectorAngle, stackAngle;
	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = M_PI / 2.0f - ((float)i) * stackStep;
		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = (float)j * sectorStep;
			auto p = SphereParam(stackAngle, sectorAngle, radius);
			OpenGLHelper::AddVecToVector(vs, p);
			auto norm = glm::normalize(SphereNormal(stackAngle, sectorAngle));
			OpenGLHelper::AddVecToVector(vs, norm);
		}
	}

	for (int i = 0; i < stackCount; ++i) {
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;
		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			if (i != 0) {
				is.push_back(k1);
				is.push_back(k2);
				is.push_back(k1 + 1);
			}
			if (i != (stackCount - 1)) {
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

glm::vec3 Sphere::SphereParam(float alfa, float beta, float r) {
	return { r * cosf(alfa) * cosf(beta), r * cosf(alfa) * sinf(beta), r * sinf(alfa) };
}

glm::vec3 Sphere::SphereDerative_alfa(float alfa, float beta, float r) {
	return { -r * sinf(alfa) * cosf(beta), -r * sinf(alfa) * sinf(beta), r * cosf(alfa) };
}

glm::vec3 Sphere::SphereDerative_beta(float alfa, float beta, float r) {
	return { -r * cosf(alfa) * sinf(beta), r * cosf(alfa) * cosf(beta), 0 };
}

glm::vec3 Sphere::SphereNormal(float alfa, float beta) {
	return SphereParam(alfa, beta, 1);
}
