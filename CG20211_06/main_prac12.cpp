/*---------------------------------------------------------*/
/* ----------------   Proyecto Final ----------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------- Alumno:Cabello Aguilar Héctor Eduardo -----------*/
/*------- Alumno:García Ruiz Andrea -----------------------*/
/*------- Alumno:Rueda Rojas Luz Elena --------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

// OpenAL
#include <AL/alut.h>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>

#include <Skybox.h>
#include <iostream>

/*------------------------------OpenAL-------------------------------------*/
#define FILENAME "resources/audio/TheSimpsons.wav"
ALuint buffer, source;
/*--------------------------------------------------------------------------*/


//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
void getResolution(void);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

// camera
Camera camera(glm::vec3(-28.50f, 102.00f, 594.00f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;



float dx_Homero = 0.08f, dz_Homero = 0.08f, giroHomero = 0.0f, mov_giro = 0.07f;


// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 0.0f, 0.0f);
glm::vec3 lightDirection(0.2f, -1.0f, -0.2f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f,
		homero_x = 0.0f,
		homero_z = 0.0f,
		girpollo = 0.0f, //variable giro pollo
		r1 = 0.0f, //Variables podadora
		pX = 0.0f,
		pZ = 0.0f,
		rotPod = 0.0f,
		r2 = 0.0f,
		r3 = 0.0f,
		r4 = 0.0f,
		r5 = 0.0f,
		rotPoda = 0.0f;

bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		direccionH = true,
		animGlobo = false, // variable movimiento globo
		animPod = false, // variable movimiento podadora
		rec1 = true, //recorridos globo
		rec2 = false, //recorridos podadora
		rec3 = false,
		rec4 = false,
		rec5 = false,
		rec6 = false,
		recp1 = true,
		recp2 = false,
		recp3 = false,
		recp4 = false,
		recp5 = false,
		recp6 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}


void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		if (recorrido1)
		{
			movAuto_z += 3.7f;
			orienta = 0.0f;
			if (movAuto_z > 150.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x -= 3.0f;
			orienta = -90.0f;
			if (movAuto_x < -200.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			movAuto_z -= 3.0f;
			orienta = 180.0f;
			if (movAuto_z < -150.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			movAuto_x += 2.7f;
			orienta = 90.0f;
			if (movAuto_x > 0.0f)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}
	}
	//---------------Animacion globo-------------------
	if (animGlobo) {
		if (rec1) {
			r1 += 5.0f;
			if (r1 > 90.0f)
			{
				rec1 = false;
				rec2 = true;
			}
		}
		if (rec2) {
			r2 += 5.0f;
			if (r2 > 200.0f)
			{
				rec2 = false;
				rec3 = true;
			}
		}
		if (rec3) {
			r3 -= 5.0f;
			if (r3 < -90.0f)
			{
				rec3 = false;
				rec4 = true;
			}
		}
		if (rec4) {
			r2 += 5.0f;
			if (r2 > 90.0f)
			{
				rec4 = false;
				rec5 = true;
			}
		}
		if (rec5) {
			r1 -= 5.0f;
			if (r1 < -605.0f)
			{
				rec5 = false;
				rec6 = true;
			}
		}
		if (rec6) {
			r2 -= 5.0f;
			if (r2 < 0.0f)
			{
				rec6 = false;

			}
		}
	}
	//--------------------------Animacion pollo---------------
	if (360 > girpollo)
		girpollo += 0.9f;
	else
		girpollo = 0.0f;

	//--------------------------Animacion podadora---------------
	if (animGlobo) {
		if (recp1) {
			pX += 5.0f;
			if (pX > 350.0f)
			{
				recp1 = false;
				recp2 = true;
			}
		}
		if (recp2) {
			pZ -= 5.0f;
			rotPoda = 90.0f;
			if (pZ < -100.0f)
			{
				recp2 = false;
				recp3 = true;
			}
		}
		if (recp3) {
			pX -= 5.0f;
			rotPoda = 180.0f;
			if (pX < 220.0f)
			{
				recp3 = false;
				recp4 = true;
			}
		}
		if (recp4) {
			pZ -= 5.0f;
			rotPoda = 90;
			if (pZ < -200.0f)
			{
				recp4 = false;
				recp5 = true;
			}
		}
		if (recp5) {
			pX += 5.0f;
			rotPoda = 360;
			if (pX > 350.0f)
			{
				recp5 = false;
				recp6 = true;
			}
		}
		if (recp6) {
			pZ -= 5.0f;
			rotPoda = 90.0f;
			if (pZ < -300.0f)
			{
				recp6 = false;

			}
		}
	}

	//-----------------------------Homero animación-----------------------------------
	if (homero_x  >=  35.0f || homero_x <= -50.0f) {
		dx_Homero = -dx_Homero;
		direccionH = -direccionH;
	}

	if (homero_z  >= 100.0f || homero_z <= -47.0f) {
		dz_Homero = -dz_Homero;
		direccionH = -direccionH;
	}

	if (direccionH)
		giroHomero += mov_giro;
	else
		giroHomero -= mov_giro;

	homero_x += dx_Homero;
	homero_z += dz_Homero;
	//--------------------------------------------------------------------------------
	
}
void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto final Computación Gráfica", NULL, NULL);
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
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//Datos a utilizar
	myData();
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader staticShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");
	//Shader secondShader("Shaders/shader.vs","Shaders/shader.fs");

	
	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------Homero
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/PersonajeHomero/piernaDer.obj");
	Model piernaIzq("resources/objects/PersonajeHomero/piernaIzq.obj");
	Model torso("resources/objects/PersonajeHomero/torso.obj");
	Model brazoDer("resources/objects/PersonajeHomero/brazoDer.obj");
	Model brazoIzq("resources/objects/PersonajeHomero/brazoIzq.obj");
	Model cabeza("resources/objects/PersonajeHomero/cabeza.obj");
	//
	Model casaVieja("resources/objects/Escenario2/Escenario.obj");
	Model cubo("resources/objects/cubo/cube02.obj");
	Model LUZ("resources/objects/LUZ/LUZ.obj");
	Model homeroDona("resources/objects/HomeroAlberca/HomeroALow.obj");

	//Modelos jardin
	Model globo("resources/objects/globo/globo.obj");
	Model parrilla("resources/objects/parrilla/parrilla.obj");
	Model pollo("resources/objects/pollo/pollo.obj");
	Model podadora("resources/objects/podadora/podadora.obj");

	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	//ModelAnim ninja("resources/objects/Dying/Dying.dae");
	//ninja.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	/*------------------------------OpenAL-------------------------------------*/
	// OpenAL init


	ALint state;

	// Initialize the environment
	alutInit(0, NULL);

	// Capture errors
	alGetError();

	// Load pcm data into buffer
	buffer = alutCreateBufferFromFile(FILENAME);

	// Create sound source (use buffer to fill source)
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);

	// Play
	alSourcePlay(source);

	// Wait for the song to complete
	//do {
	//	alGetSourcei(source, AL_SOURCE_STATE, &state);
	//} while (state == AL_PLAYING);


	/*-------------------------------------------------------------------------*/






	
	
	
	while (!glfwWindowShouldClose(window))
	{

		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.5f);
		staticShader.setFloat("pointLight[0].linear", 0.0009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		//(150.00,37.50,87.00),(234.00,37.50,87.00),(258.00,37.50,157.50),(157.50,37.50,196.50),(118.50,37.50,276.00),(160.50,78.00,87.00),(255.00,78.00,94.50),(259.50,78.00,162.00),(256.50,78.00,258.00)

		staticShader.setVec3("pointLight[1].position", glm::vec3(150.00, 37.50, 87.00));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 2.5f);
		staticShader.setFloat("pointLight[1].linear", 0.0009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(234.00, 37.50, 87.00));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 2.5f);
		staticShader.setFloat("pointLight[2].linear", 0.0009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[3].position", glm::vec3(258.00, 37.50, 157.50));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 2.5f);
		staticShader.setFloat("pointLight[3].linear", 0.0009f);
		staticShader.setFloat("pointLight[3].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[4].position", glm::vec3(157.50, 37.50, 196.50));
		staticShader.setVec3("pointLight[4].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[4].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[4].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[4].constant", 2.5f);
		staticShader.setFloat("pointLight[4].linear", 0.0009f);
		staticShader.setFloat("pointLight[4].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[5].position", glm::vec3(118.50, 37.50, 276.00));
		staticShader.setVec3("pointLight[5].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[5].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[5].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[5].constant", 2.5f);
		staticShader.setFloat("pointLight[5].linear", 0.0009f);
		staticShader.setFloat("pointLight[5].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[6].position", glm::vec3(160.50, 78.00, 87.00));
		staticShader.setVec3("pointLight[6].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[6].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[6].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[6].constant", 2.5f);
		staticShader.setFloat("pointLight[6].linear", 0.0009f);
		staticShader.setFloat("pointLight[6].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[7].position", glm::vec3(255.00, 78.00, 94.50));
		staticShader.setVec3("pointLight[7].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[7].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[7].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[7].constant", 2.5f);
		staticShader.setFloat("pointLight[7].linear", 0.0009f);
		staticShader.setFloat("pointLight[7].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[8].position", glm::vec3(259.50, 78.00, 162.00));
		staticShader.setVec3("pointLight[8].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[8].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[8].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[8].constant", 3.5f);
		staticShader.setFloat("pointLight[8].linear", 0.0009f);
		staticShader.setFloat("pointLight[8].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[9].position", glm::vec3(256.50, 78.00, 258.00));
		staticShader.setVec3("pointLight[9].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[9].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[9].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[9].constant", 3.5f);
		staticShader.setFloat("pointLight[9].linear", 0.0009f);
		staticShader.setFloat("pointLight[9].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[10].position", glm::vec3(274.50, 37.50, 231.00));
		staticShader.setVec3("pointLight[10].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[10].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[10].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[10].constant", 3.5f);
		staticShader.setFloat("pointLight[10].linear", 0.0009f);
		staticShader.setFloat("pointLight[10].quadratic", 0.00032f);
		

		staticShader.setFloat("material_shininess", 32.0f);

		
		glm::mat4 modelTemp = glm::mat4(1.0f);		// Una matriz temporal
		glm::mat4 modelTemp2 = glm::mat4(1.0f);		// Una matriz temporal
		glm::mat4 modelTemp3 = glm::mat4(1.0f);		// Una matriz temporal
		glm::mat4 modelTemp4 = glm::mat4(1.0f);		// Una matriz temporal
		glm::mat4 secondView = glm::mat4(1.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		//animShader.use();
		//animShader.setMat4("projection", projection);
		//animShader.setMat4("view", view);
		//
		//animShader.setVec3("material.specular", glm::vec3(0.5f));
		//animShader.setFloat("material.shininess", 32.0f);
		//animShader.setVec3("light.ambient", ambientColor);
		//animShader.setVec3("light.diffuse", diffuseColor);
		//animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//animShader.setVec3("light.direction", lightDirection);
		//animShader.setVec3("viewPos", camera.Position);
		//
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		casaVieja.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Homero
		// -------------------------------------------------------------------------------------------------------------------------
		
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-190.0f + homero_x, 6.0f, 410.0f + homero_z));
		model = glm::rotate(model, glm::radians(giroHomero), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		homeroDona.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		LUZ.Draw(staticShader);

		//
		//Colocar código aquí
		//staticShader.use();
		//staticShader.setMat4("model", model);
		//staticShader.setMat4("view", view);
		//staticShader.setMat4("projection", projection);

		glBindVertexArray(VAO);

		model = glm::scale(model, glm::vec3(5.0f, 4.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.55f, 0.10f, 0.20f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //Pecho

		modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.25f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.8f, 0.75f)); //Verde
		glDrawArrays(GL_QUADS, 0, 24); //Cuello

		model = glm::translate(modelTemp, glm::vec3(0.0f, 1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.9f, 0.74f, 0.62f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //Cabeza

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.55f, 0.10f, 0.20f)); //Morado
		glDrawArrays(GL_QUADS, 0, 24); //Hombro izquierdo

		model = glm::translate(model, glm::vec3(3.5f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.55f, 0.10f, 0.20f)); //Morado
		glDrawArrays(GL_QUADS, 0, 24); //Hombro derecho

		modelTemp = model = glm::translate(glm::mat4(1), glm::vec3(4.0f, -.75f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 3.5f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.8f, 0.75f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //Brazo derecho

		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.8f, 0.75f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //Brazo izquierdo

		model = glm::translate(glm::mat4(1), glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.25f, 0.25f, 0.25f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //Cintura

		model = glm::translate(glm::mat4(1), glm::vec3(-1.75f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 4.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.2f, 0.2f, 0.2f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //Pierna izquierda

		modelTemp2 = model = glm::translate(glm::mat4(1), glm::vec3(1.75f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 4.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.2f, 0.2f, 0.2f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //Pierna derecha

		model = glm::translate(modelTemp2, glm::vec3(0.5f, -2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.95f, 0.95f, 0.95f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //Pie derecho

		model = glm::translate(model, glm::vec3(-1.8f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 1.5f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.95f, 0.95f, 0.95f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //Pie izquierdo

		modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f, -2.125f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.75f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f)); //Azul
		glDrawArrays(GL_QUADS, 0, 24); //Mano derecha

		modelTemp = model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.0f, 0.5f, 1.0f)); //Azul
		glDrawArrays(GL_QUADS, 0, 24); //Mango derecho

		modelTemp = model = glm::translate(modelTemp, glm::vec3(1.75f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.75f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.0f, 0.25f, 1.0f)); //Azul
		glDrawArrays(GL_QUADS, 0, 24); //Espada derecho

		modelTemp3 = model = glm::translate(glm::mat4(1), glm::vec3(-8.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.25f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Roja
		glDrawArrays(GL_QUADS, 0, 24); //Cresta roja

		model = glm::translate(modelTemp3, glm::vec3(0.0f, -3.65f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 6.25f, 3.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //Tronco

		modelTemp4 = model = glm::translate(modelTemp3, glm::vec3(0.0f, -1.75f, 2.25f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.4f, 0.07f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //Pico

		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -0.24f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //Pico bajo

		model = glm::translate(modelTemp3, glm::vec3(0.0f, -5.15f, -2.35f));
		model = glm::scale(model, glm::vec3(3.0f, 3.25f, 1.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //cola grande

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.67f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.40f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.9f, 0.9f, 0.9f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //cola chica

		model = glm::translate(modelTemp3, glm::vec3(-1.75f, -4.75f, -1.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.25f, 2.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.9f, 0.9f, 0.9f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //ala izquierda

		model = glm::translate(model, glm::vec3(4.75f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.75f, 1.25f, 2.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.9f, 0.9f, 0.9f)); //Blanco
		glDrawArrays(GL_QUADS, 0, 24); //ala derecha

		model = glm::translate(modelTemp4, glm::vec3(-1.51f, 0.2f, -2.0f));
		model = glm::scale(model, glm::vec3(0.0f, .5f, 0.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.1f)); //Negro
		glDrawArrays(GL_QUADS, 0, 24); //ojo izquierdo

		model = glm::translate(modelTemp4, glm::vec3(1.51f, 0.2f, -2.0f));
		model = glm::scale(model, glm::vec3(0.0f, .5f, 0.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.1f)); //Negro
		glDrawArrays(GL_QUADS, 0, 24); //ojo derecho

		model = glm::translate(modelTemp4, glm::vec3(-1.0f, -5.75f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.4f, 0.07f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //pierna izquierda

		model = glm::translate(modelTemp4, glm::vec3(1.0f, -5.75f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.4f, 0.07f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //pierna derecha

		model = glm::translate(modelTemp4, glm::vec3(-1.0f, -6.75f, -3.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.4f, 0.07f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //pie izquierda

		model = glm::translate(modelTemp4, glm::vec3(1.0f, -6.75f, -3.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.4f, 0.07f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //pie derecho

		model = glm::translate(modelTemp4, glm::vec3(-1.5f, -6.75f, -2.125f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.06f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //Dedos izquierda

		model = glm::translate(modelTemp4, glm::vec3(-0.5f, -6.75f, -2.125f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.06f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //Dedos izquierda

		model = glm::translate(modelTemp4, glm::vec3(1.5f, -6.75f, -2.125f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.06f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //Dedos derecha

		model = glm::translate(modelTemp4, glm::vec3(0.5f, -6.75f, -2.125f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.06f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //Dedos derecha

		/*
		modelTemp = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 2.0f, 1.0f)); /* factorDeEscala = valorFinal/valor inicial //Siempre al final del objeto por el
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_QUADS, 0, 24); //B

		model = glm::translate(modelTemp/*glm::mat4(1.0f)/*Ignora a la matriz model y comienza desde la unitaria, glm::vec3(-3.0f, 4.5f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_QUADS, 0, 24); //C


		*/

		glBindVertexArray(0);
		
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::scale(model, glm::vec3(5.0f));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		//staticShader.setMat4("model", model);
		//torso.Draw(staticShader);
		//
		////Pierna Der
		//model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);
		//
		////Pie Der
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		//staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);
		//
		////Pierna Izq
		//model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);
		//
		////Pie Iz
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		//staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase
		//
		////Brazo derecho
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);
		//
		////Brazo izquierdo
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);
		//
		////Cabeza
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		//staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);
		//----------Modelos jardin
		//globo//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(r1, r2, r3));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
		model = glm::rotate(model, glm::radians(rotPod), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1));
		staticShader.setMat4("model", model);
		globo.Draw(staticShader);

		//parrilla//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0, 0.0, -8.0));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.1));
		staticShader.setMat4("model", model);
		parrilla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 12.0, -8.5));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(girpollo), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.015));
		staticShader.setMat4("model", model);
		pollo.Draw(staticShader);
		//podadora//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(155.0 + pX, 0.5, -8.5 + pZ));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
		model = glm::rotate(model, glm::radians(rotPoda), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.2));
		staticShader.setMat4("model", model);
		podadora.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}




	skybox.Terminate();

	glfwTerminate();

	// Clean up sources and buffers
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);

	// Exit everything
	//alutExit();


	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
		
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;


	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
	//Activa globo y podadora
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
		animGlobo ^= true;
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