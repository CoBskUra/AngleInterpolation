#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ShaderManadement/VAO.h>
#include "Helper/OpenGLHelper.h"
#include <Camera.h>
#include "Transformations/Quaternion.h"
#include "Transformations/Rotation.h"
#include "Helper/ImGuiStyles.h"
#include <Helper/DeltaTime.h>
#include <glm/gtx/euler_angles.hpp>      // Do pracy z k¹tami Eulera

class InterpolationAnimation {
	struct Frame {
		glm::vec3 Pos{ 0 };;
		glm::vec3 Rotation{};
		glm::quat Quaterion{};
	};
	Frame start;
	Frame end;

	float animationLength = 0;
	bool quaterionLiniar = true;
	int numberOfSteps{ 0 };
	DeltaTime deltaTime{};
	bool runAnimation = false;
	bool firstTimeStart = true;
	long float timeOfanimationRuning_s = 0;
	InfinityGrid infinityGrid{};

	void ResetAnimation() {
		runAnimation = false;
		timeOfanimationRuning_s = 0;
		deltaTime.Reset();
		start.Quaterion = glm::normalize(start.Quaterion);
		end.Quaterion = glm::normalize(end.Quaterion);
	}

	void DrawFrame(GLFWwindow* window, const Camera& camera, float width, float height, Mesh& animatedObject
	, float t) {

		glm::vec3 currentPos = start.Pos * (1 - t) + t * end.Pos;

		glm::vec3 diff =  start.Rotation - end.Rotation;
		glm::vec3 startAngles = start.Rotation;
		for (int i = 0; i < 3; i++) {
			if (diff[i] > M_PI)
				startAngles[i] -= 2.0 * M_PI;
			else if (diff[i] < -M_PI)
				startAngles[i] += 2.0 * M_PI;
		}
		glm::vec3 currentAngles = startAngles * (1 - t) + t * end.Rotation;
		glm::mat4 currentRotationMat = MathOperations::GetRotationXYZ(currentAngles);

		glm::quat currentQuaterion;			
		if (quaterionLiniar)
		{
			float flipRotation = glm::dot(start.Quaterion, end.Quaterion) < 0 ? -1 : 1;
			currentQuaterion = glm::lerp(flipRotation * start.Quaterion, end.Quaterion, t);
		}
		else
			currentQuaterion = glm::slerp(start.Quaterion, end.Quaterion, t);

		currentQuaterion = glm::normalize(currentQuaterion);
		animatedObject.SetTranslation(currentPos);


		animatedObject.SetRotation(currentRotationMat);
		glViewport(0, 0, width * 0.5f, height);
		animatedObject.Draw(window, camera);

		animatedObject.SetRotation(glm::toMat4(currentQuaterion));
		glViewport(width * 0.5f, 0, width * 0.5f, height);
		animatedObject.Draw(window, camera);
	}

public:

	void Draw(GLFWwindow* window, const Camera& camera, float width, float height, Mesh& animatedObject) {
		if (runAnimation && firstTimeStart) {
			deltaTime.Reset();
			firstTimeStart = false;
		}
		float t = timeOfanimationRuning_s / animationLength;
		
		if (runAnimation) {
			
			timeOfanimationRuning_s += deltaTime.GetDeltaTime_s();
			t = timeOfanimationRuning_s / animationLength;
		}

		if (t >= 1)
			t = 1;
		else if (animationLength <= 0)
			t = 0;

		glViewport(0, 0, width * 0.5f, height);
		infinityGrid.Draw(window, camera);
		glClear(GL_DEPTH_BUFFER_BIT);

		glViewport(width * 0.5f, 0, width * 0.5f, height);
		infinityGrid.Draw(window, camera);
		glClear(GL_DEPTH_BUFFER_BIT);

		if (numberOfSteps > 0) {
			float slice = 1.0f / static_cast<float>(numberOfSteps);
			for (int i = 0; i < numberOfSteps; i++) {
				DrawFrame(window, camera, width, height, animatedObject, i * slice);
			}

			DrawFrame(window, camera, width, height, animatedObject, 1);
		}

		DrawFrame(window, camera, width, height, animatedObject, t);

	}

	void InterfersWindows(float width, float height) {
		auto flags = ImGuiWindowFlags_NoCollapse;
		const float contendWidth = width / 3.0f;
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();

		float titleHeight = 30;
		float dragFloatHeight = (ImGui::GetFontSize() + style.FramePadding.y * 2.0f);
		float windowHeight = 6 * dragFloatHeight + titleHeight;

		ImGui::SetNextWindowPos({ contendWidth * 0,height - windowHeight});
		ImGui::SetNextWindowSize({ contendWidth, windowHeight });
		ImGui::Begin("Start", 0, flags); {
			ImGui::PushID("InterpolationAnimation_Start");
			if (ImGuiStartEnd(start)) {
				ResetAnimation();
			}
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::SetNextWindowPos({ contendWidth * 1,height - windowHeight });
		ImGui::SetNextWindowSize({ contendWidth, windowHeight });
		ImGui::Begin("Animation control", 0, flags); {
			ImGui::PushID("InterpolationAnimation_AnimationControl");
			AnimationSettings();
			ImGui::PopID();
		}
		ImGui::End();

		
		ImGui::SetNextWindowPos({ contendWidth * 2,height - windowHeight });
		ImGui::SetNextWindowSize({ contendWidth, windowHeight });
		ImGui::Begin("End", 0, flags); {
			ImGui::PushID("InterpolationAnimation_End");
			if (ImGuiStartEnd(end))
			{
				ResetAnimation();
			}
			ImGui::PopID();
		}
		ImGui::End();
	}


	bool ImGuiStartEnd(Frame& frame) {
		bool somethingChanged = false;

		if (ImGui::DragFloat3("Position", &frame.Pos[0]))
			somethingChanged = true;

		auto angles = glm::degrees(frame.Rotation);
		if (
			ImGui::DragFloat3("Euler rotation", &angles[0], 1.0f, -480, 180, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		{
			frame.Rotation = glm::radians(angles);
			glm::mat4 rotation_matrix = MathOperations::GetRotationXYZ(frame.Rotation);
			frame.Quaterion = glm::normalize(glm::toQuat(rotation_matrix));
			somethingChanged = true;
		}
				
		if (ImGui::DragFloat4("Quaterion", &frame.Quaterion[0], 0.1f)) {
			frame.Quaterion = glm::normalize(frame.Quaterion);
			glm::extractEulerAngleXYZ(glm::toMat4(frame.Quaterion),
				frame.Rotation.x, frame.Rotation.y, frame.Rotation.z);


			somethingChanged = true;
		}

		return somethingChanged;
	}

	bool AnimationSettings() {
		bool somethingChanges = false;
		if (ImGui::DragFloat("Animation time", &animationLength, 1, M_ESP, M_FLOAT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{	
			somethingChanges = true;
		}

		if (ImGui::InputInt("Number of steps", &numberOfSteps, 1, 10, ImGuiSliderFlags_AlwaysClamp))
		{
			if (numberOfSteps < 0)
				numberOfSteps = 0;
			
			somethingChanges = true;
		}
		ImGui::Text("Quaterion mode: ");
		ImGui::SameLine();
		if (ImGui::RadioButton("Lerp", quaterionLiniar))
		{
			quaterionLiniar = !quaterionLiniar;
			somethingChanges = true;
		}
		ImGui::SameLine();
		if(ImGui::RadioButton("Slerp", !quaterionLiniar))
		{
			quaterionLiniar = !quaterionLiniar;
			somethingChanges = true;
		}

		if (!runAnimation && ImGui::Button("Start")) {
			runAnimation = true;
			if (timeOfanimationRuning_s >= animationLength)
				ResetAnimation();
			firstTimeStart = true;
			
		}

		if (runAnimation && ImGui::Button("Stop")) {
			runAnimation = false;
		}
		

		return somethingChanges;
	}

	
};