#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "cylinder.h"
#include "ShapeGenerator.h"
#include "ShapeData.h"



#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(-2.5f, 1.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// offset variables for plane, sphere
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);


// plane variables
GLuint planeNumIndices;
GLuint planeVertexArrayObjectID;
GLuint planeIndexByteOffset;


// sphere variables
GLuint sphereNumIndices;
GLuint sphereVertexArrayObjectID;
GLuint sphereIndexByteOffset;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(0.2f, 0.0f, 2.0f);


// Function to calculate frames per second (FPS)
double calculateFPS(GLFWwindow* window, double elapsedTime) {
	static double frames = 0.0;
	static double fps = 0.0;
	frames += 1.0;
	if (elapsedTime - fps >= 1.0) {
		std::ostringstream oss;
		oss << "OpenGL 3.1.1 | FPS: " << frames;
		glfwSetWindowTitle(window, oss.str().c_str());
		fps = elapsedTime;
		frames = 0.0;
	}
	return fps;
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Christopher Rode", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Rectangulars 
		// positions          // normals           // texture coords
		2.0f,  0.0f,   0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.0f,   0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.3f,   0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		2.0f,  0.0f,   0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		2.0f,  0.3f,   0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.3f,   0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,

		2.0f,  0.0f,  -2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.0f,  -2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.3f,  -2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		2.0f,  0.0f,  -2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		2.0f,  0.3f,  -2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.3f,  -2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,

		0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.3f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.3f, -2.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.3f, -2.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.0f, -2.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		2.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		2.0f,  0.3f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		2.0f,  0.0f, -2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		2.0f,  0.0f, -2.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		2.0f,  0.3f, -2.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		2.0f,  0.3f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		0.5f,  0.3f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.3f, -2.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		2.0f,  0.3f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		2.0f,  0.3f, -2.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		2.0f,  0.3f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.3f, -2.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		2.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.0f, -2.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		2.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		2.0f,  0.0f, -2.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		2.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.0f, -2.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,


		


	};
	// positions all containers
	glm::vec3 cubePositions[] = {
	glm::vec3(-2.2f,  -0.2f,  0.0f),
	glm::vec3(-5.2f,  -0.0f,  -0.4f)
	};

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
	glm::vec3(-0.0f,  5.0f,  -0.3f),
	glm::vec3(-3.0f,  5.0f,  -0.3f),
	glm::vec3(-6.0f,  5.0f,  -0.3f),
	glm::vec3(-9.0f,  5.0f,  -0.3f)
	};
	
	//position spheres
	glm::vec3 spherePositions[] = {

	glm::vec3(-6.7f, 0.8f, -1.7f),

	};

	//position of cylinder legs
	glm::vec3 legPositions[] = {

	glm::vec3(-8.0f, -0.65f, 2.7f),
	glm::vec3(-8.0f, -1.95f, 2.7f),
	glm::vec3(-8.0f, -3.35f, 2.7f),
	glm::vec3(1.1f, -0.65f, 2.7f),
	glm::vec3(1.1f, -1.95f, 2.7f),
	glm::vec3(1.1f, -3.35f, 2.7f),
	glm::vec3(1.3f, -0.65f, -7.0f),
	glm::vec3(1.3f, -1.95f, -7.0f),
	glm::vec3(1.3f, -3.35f, -7.0f),
	glm::vec3(-8.0f, -0.65f, -7.0f),
	glm::vec3(-8.0f, -1.95f, -7.0f),
	glm::vec3(-8.0f, -3.35f, -7.0f),
	};

	//position black planes
	glm::vec3 planePositions[] = {
	
	glm::vec3(-1.5f, 0.0f, -1.0f),
	};
	

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	
	// plane object data
	ShapeData plane = ShapeGenerator::makePlane(20);

	unsigned int planeVBO{}, planeVAO, planeVAO2;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, plane.vertexBufferSize() + plane.indexBufferSize(), 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();

	planeIndexByteOffset = currentOffset;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	planeNumIndices = plane.numIndices;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeVBO);

	//Sphere object data
	ShapeData sphere = ShapeGenerator::makeSphere();

	unsigned int sphereVBO{}, sphereVAO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);

	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.vertexBufferSize() + sphere.indexBufferSize(), 0, GL_STATIC_DRAW);

	currentOffset = 0;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();

	sphereIndexByteOffset = currentOffset;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);

	sphereNumIndices = sphere.numIndices;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereVBO);


	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap = loadTexture("lightblue2.jpg");
	unsigned int specularMap = loadTexture("redyellowblue.png");
	unsigned int cup = loadTexture("wall.jpg");
	unsigned int countertop = loadTexture("A_black_image.jpg");
	unsigned int spec = loadTexture("Color-Green.jpg");
	unsigned int cup2 = loadTexture("Red_rectangle.svg.png");
	unsigned int counter2top = loadTexture("A_black_image.jpg");
	unsigned int floor = loadTexture("360.jpg");
	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("material.cup", 3);
	lightingShader.setInt("material.countertop", 4);
	lightingShader.setInt("material.spec", 5);



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//display fps in window
		calculateFPS(window, currentFrame);
		
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader.setVec3("dirLight.direction", -5.2f, -0.2f, 0.0f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", -6.7f, 0.8f, -1.7f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		 //bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);


		// render rectangles
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 3; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 0.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, -5.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//assign shapes
		unsigned int  VBO, cubeVAO, cylinderVAO, cylinder2VAO;
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//draw cube attributes
		glBindVertexArray(cubeVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		
		
		//soap bottle
		//draw cylinder 1
		glGenVertexArrays(1, &cylinderVAO);
		glBindVertexArray(cylinderVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		//Add texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cup);
		glBindVertexArray(cylinderVAO);
		
		//set size, position and lighting shader
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-0.95f, 0.89f, -1.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C(0.5, 20, 1.5, true, true, true);
		C.render();
		


		//red cylinder
		//draw cylinder 2 
		glGenVertexArrays(1, &cylinder2VAO);
		glBindVertexArray(cylinder2VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//Add texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cup2);
		glBindVertexArray(cylinderVAO);


		//set size, position and lighting shader
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-3.95f, 0.65f, -2.7f));
		model = glm::scale(model, glm::vec3(0.9f));
		float angle = -50.0f;
		model = glm::rotate(model, glm::radians(angle) , glm::vec3(-3.95f, 0.75f, -2.7f));
		lightingShader.setMat4("model", model);
		C.render();

		/* Modified 4/1/2024
		Created cylinder instancing algorithm for the legs of desk.
		This approach benefits from OpenGL instancing, resulting in better performance, reduced overhead compared to drawing each cylinder separately
		and less redundant code.
		Time complexity began at 0(12) and was reduced to 0(1).
		*/

		// Using a loop for rendering all cylinders at once
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, countertop);
		glBindVertexArray(cylinderVAO);

		// Loop through all legs to render cylinders
		//loops 12 times for desk legs
		for (unsigned int i = 0; i < 12; i++)
		{
			model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
			model = glm::translate(model, legPositions[i]);//get positions
			model = glm::scale(model, glm::vec3(0.9f));//size of cylinders
			lightingShader.setMat4("model", model);//set shader
			C.render();//render
		}


		/* Modified 4/1/2024
		Created sphere instancing algorithm for the tennis ball.
		This approach benefits from OpenGL instancing, resulting in better performance, reduced overhead compared to drawing each sphere separately
		and less redundant code required in the future.
		Time complexity remains the same since we are only creating one sphere right now but is set up like the  cylinders for less redundant code and better performance in the future.
		*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spec);
		glBindVertexArray(sphereVAO);

		//loops once for one sphere
		for (unsigned int i = 0; i < 1; i++)
		{
			model = model = glm::mat4(1.0f);//make sure to initialize matrix to identity matrix first	
			model = glm::translate(model, spherePositions[i]);//get positions
			model = glm::scale(model, glm::vec3(0.7f)); // Make it a smaller sphere
			lightingShader.setMat4("model", model);//set shaders
			//draw sphere
			glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);




			/* Modified 4/1/2024
			Created sphere instancing algorithm for the black plane.
			This approach benefits from OpenGL instancing, resulting in better performance, reduced overhead compared to drawing each plane separately
			and less redundant code required in the future.
			Time complexity remains the same since we are only creating one plane right now but is set up like the cylinders for less redundant code and better performance in the future.
			*/
			glActiveTexture(GL_TEXTURE0);// Activate texture unit 0
			glBindTexture(GL_TEXTURE_2D, countertop);// Bind the countertop texture to the active texture unit
			glBindVertexArray(planeVAO); // Bind the plane Vertex Array Object (VAO)

			//loops once for one black plane
			for (unsigned int i = 0; i < 1; i++)
			{
				model = model = glm::mat4(2.0f);//make sure to initialize matrix to identity matrix first	
				model = glm::translate(model, planePositions[i]); // Translate the model matrix to the plane's position
				model = glm::scale(model, glm::vec3(0.28f)); // Scale the model matrix to make it a smaller plane
				lightingShader.setMat4("model", model);// Set the "model" uniform in the lighting shader

				// draw plane
				glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
			}

		// setup to draw plane2 floor
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floor);
		glBindVertexArray(planeVAO);
		model = model = glm::mat4(4.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.28f)); // Make it a smaller plane
		lightingShader.setMat4("model", model);

		// draw plane
		glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
		
		}



		// also draw the lamp object(s)
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 5; i++)
		{
			model = glm::mat4(2.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}