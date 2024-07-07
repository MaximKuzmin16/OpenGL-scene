#include "Model.h"
#include "input.h"
#include "Camera.h"

unsigned int SCR_WIDTH = 1500;
unsigned int SCR_HEIGHT = 1000;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsed_time = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void set_light(Shader shade, glm::vec3 color, glm::vec3 lightPos);

Camera camera;

//glm::vec3 light_color = glm::vec3(1.0f, 0.7f, 0.4f);
glm::vec3 light_color = glm::vec3(0.2f, 0.2f, 0.3f);
glm::vec3 lightPos(3.0f, -0.5f, -10.0f);

glm::mat4 proj;
glm::mat4 view;


//РИСУЕМ ОБЪЕКТ
void draw_object(Object object)
{
	object.shader.use();
	object.shader.setMat4("projection", proj);
	object.shader.setMat4("view", view);
	glm::mat4 modelMat = glm::mat4(1.0f);

	modelMat = glm::translate(modelMat, object.position); // translate it down so it's at the center of the scene

	modelMat = glm::rotate(modelMat, object.rotation.x * 3.14f / 180.0f, glm::vec3(0, 1.0f, 0));

	modelMat = glm::rotate(modelMat, object.rotation.y * 3.14f / 180.0f, glm::vec3(0, 0, 1.0f));

	modelMat = glm::rotate(modelMat, object.rotation.z * 3.14f / 180.0f, glm::vec3(1.0f, 0, 0));

	modelMat = glm::scale(modelMat, object.scale);

	object.shader.setMat4("model", modelMat);
	set_light(object.shader, light_color, lightPos);
	object.model.Draw(object.shader);
}

// ВЫВОД ИНФЫ
string get_info(float time) {
	string info = "pos (" + to_string((int)camera.Position.x) + ", " + to_string((int)camera.Position.y) + ", " + to_string((int)camera.Position.z) + ")" +
		" rot (" + to_string((int) camera.Rotation.x) + ", " + to_string((int) camera.Rotation.y) + ")" +
		" fac (" + to_string(camera.Facing.x) + ", " + to_string(camera.Facing.y) + ", " + to_string(camera.Facing.z) + ")" + 
		" time " + to_string((int) time);

	if (camera.isLocked) {
		info = "locked " + info;
	}

	return info;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	input = new Input(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// СПАВНИМ ОБЪЕКТЫ

	vector<Object> statics;
	vector<Car> cars;
	vector<Object> trees;

	Object kvartal = { Model("models/kvartal/a.obj"), Shader("vsFinal.vs", "fsFinal.frag") , glm::vec3(0,0,0), glm::vec3(1, 1, 1), glm::vec3(0.0f,0.0f,0.0f) };

	Object props = { Model("models/props/props.obj"), Shader("vsFinal.vs", "fsFinal.frag") , glm::vec3(0,0,0), glm::vec3(1, 1, 1), glm::vec3(0.0f,0.0f,0.0f) };

	Object skybox = { Model("models/skybox/sky2.obj"), Shader("vsFinal.vs", "fsNoLight.frag"), glm::vec3(0,0,0), glm::vec3(10000.0f, 10000.0f, 10000.0f), glm::vec3(0.0f,0.0f,0.0f) };

	float prop_y = -0.2f;

	Model model_tree1 = Model("models/props/tree.obj");
	Model model_tree2 = Model("models/props/tree3.obj");
	Model trash = Model("models/prop/trash.obj");

	statics.push_back(skybox);
	statics.push_back(kvartal);


	//				  МОДЕЛЬ	   ШЕЙДЕР								 ПОЗИЦИЯ				 РАЗМЕР						  ПОВОРОТ
	trees.push_back({ model_tree1, Shader("vsFinal.vs", "fsFinal.frag"), glm::vec3(8,prop_y,15), glm::vec3(1.69f, 1.69f, 1.69f), glm::vec3(0.0f,0.0f,0.0f) });
	trees.push_back({ model_tree2, Shader("vsFinal.vs", "fsFinal.frag"), glm::vec3(-10,prop_y,15), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,0.0f,0.0f) });


	Object car_obj1 = { Model("models/props/car6.obj"), Shader("vsFinal.vs", "fsFinal.frag"), glm::vec3(78.5, prop_y, 95), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,0.0f,0.0f) };
	Car car1 = { car_obj1, -45.0f, 135.0f, -107.0f };
	cars.push_back(car1);

	Object car_obj2 = { Model("models/props/car7.obj"), Shader("vsFinal.vs", "fsFinal.frag"), glm::vec3(82, prop_y, -67), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,0.0f,0.0f) };
	Car car2 = { car_obj2, 35.0f, -107.0f, 135.0f };
	cars.push_back(car2);

	double startTime = glfwGetTime();

	double start_time = glfwGetTime();
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------

		double elapsed = glfwGetTime() - start_time;
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		input->Update();

		if (input->keys[GLFW_KEY_ESCAPE].down) {
			break;
		}

		for (auto object : cars) {

			object.Update(deltaTime);
		}

		camera.Update(deltaTime);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100000.0f);
		view = camera.GetViewMatrix();

		for (auto object : statics) {
			draw_object(object);
		}

		for (auto car : cars) {
			draw_object(car.obj);
		}

		glDisable(GL_CULL_FACE);

		std::map<float, Object*> sorted;
		for (unsigned int i = 0; i < trees.size(); i++)
		{
			float distance = glm::length(camera.Position - trees[i].position);
			sorted[distance] = &trees[i];
		}

		for (std::map<float, Object*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			it->second->rotation.z = sin(elapsed);
			it->second->rotation.y = cos(elapsed / 2 + 2);
			draw_object(*it->second);
		}

		/*for (auto cloth : clothes) {
			cloth.rotation.y = cos(elapsed / 1.2);
			draw_object(cloth);
		}*/

		glEnable(GL_CULL_FACE);

		draw_object(props);

		glfwSetWindowTitle(window, get_info(elapsed).c_str());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	input->UpdateCenter();
}

void set_light(Shader shade, glm::vec3 color, glm::vec3 lightPos)
{
	shade.setVec3("lightColor", color);
	shade.setVec3("lightPos", lightPos);
	shade.setVec3("viewPos", camera.Position);
}
