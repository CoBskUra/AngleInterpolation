
#include"Camera.h"
#include <Main.h>
#include <filesystem>
#include "Figures/Cylinder.h"
#include "Figures/InfinityGrid.h"
#include <Figures/Spere.h>
#include <Figures/Cross.h>
#include <Helper/ImGuiStyles.h>
#include "Animation/InterpolationAnimation.h"

unsigned int width = 1600;
unsigned int height = 1024;

Camera camera(width, height, glm::vec3(0.0f, 1.0f, -2.0f));
GLFWwindow* Init();
void ResizeCallBack(GLFWwindow* window, int w, int h);

int main()
{
	GLFWwindow* window = Init();
	if (window == NULL)
		return -1;

	//// infinity Grid
	/*VAO vao;
	vao.Bind();
	VBO vboInfinityGrid(infinityGrid, GL_STATIC_DRAW);

	vao.LinkAttrib(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);

	vao.Unbind(); vboInfinityGrid.Unbind();
	Shader shader("infinityGrid_vert.glsl", "infinityGrid_frag.glsl");*/

	//InfinityGrid ig;
	////////////////////////////////
	StaticShaders::Init();
	Cylinder cylinder(100, 4, 1);
	cylinder.SetRotation(glm::radians(45.0f), { 0,1,1 });
	Cross cross{ 30, 4, 1 };
	//Sphere shpere{ 30, 30, 1 };
	InfinityGrid infinityGrid{};
	//Camera camera(10, 10, { 0,-10,0 });
	auto trans = glm::scale(glm::mat4{ 1 }, { 0.5, 0.5, 2 });
	cross.SetTranslation({ 0.5, 0.5, 2 });
	cross.SetRotation( glm::radians(45.f), {1, 3, 1});
	InterpolationAnimation animation;
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		camera.Inputs(window);
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::Begin("Camera"); {
			camera.ActiveInterferes();
		}
		ImGui::End();
		animation.InterfersWindows(width, height);
		animation.Draw(window, camera, width, height, cross);
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// DeleteAll all the objects we've created

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	

	// DeleteAll window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


GLFWwindow* Init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "MKMG_lab_1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, ResizeCallBack);

	gladLoadGL();
	glViewport(0, 0, width, height);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	ImGuiStyles::SetupImGuiStyle(true, 0.75f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}

// PóŸniej tutaj dam resize, mam tylko problem, ¿e po risie Ÿle 
// kursor po klikniêciu ustawia mi siê w z³ym miejscu
// 
void ResizeCallBack(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	camera.SetAspect(width*0.5f / (float)height);
}
