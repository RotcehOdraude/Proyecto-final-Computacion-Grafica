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
float MovementSpeed = 0.2f; //Velocidad de la camara
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true, dia = true;



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
		rotPoda = 0.0f,
		giroUfo = 0.0f;

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
recp6 = false,
animOvni = false,
rec1UFO = true,
rec2UFO = false,
rec3UFO = false;




//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
rotRodDer = 0.0f,
rotBraDer = 0.0f,
rotBraIzq = 0.0f,
rotBraDerY = 0.0f,
rotBraIzqY = 0.0f,
rotCabeza = 0.0f,
rotCerveza = 0.0f,
posCervezaX = 0.0f,
posCervezaY = 0.0f,
posCervezaZ = 0.0f,
posCabeza = 0.0f,
giroMonito = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
rotRizq = 0.0f,
rotRder = 0.0f,
rotBder = 0.0f,
rotBizq = 0.0f,
rotBderY = 0.0f,
rotBizqY = 0.0f,
rotCab = 0.0f,
rotCer = 0.0f,
posCerX = 0.0f,
posCerY = 0.0f,
posCerZ = 0.0f,
posCab = 0.0f,
giroMonitoInc = 0.0f,
posUFOX = 0.0f,
posUFOY = 0.0f,
posUFOZ = 0.0f;

#define MAX_FRAMES 196
int i_max_steps = 2;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float rotRodDer; //Variables de rotacion y posicion homero keyframes
	float rotBraDer;
	float rotBraIzq;
	float rotBraDerY;
	float rotBraIzqY;
	float rotCabeza;
	float rotCerveza;
	float posCervezaX;
	float posCervezaY;
	float posCervezaZ;
	float posCabeza;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 196;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotBraDer = rotBraDer;
	KeyFrame[FrameIndex].rotBraIzq = rotBraIzq;
	KeyFrame[FrameIndex].rotBraDerY = rotBraDerY;
	KeyFrame[FrameIndex].rotBraIzqY = rotBraIzqY;
	KeyFrame[FrameIndex].rotCabeza = rotCabeza;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].rotCerveza = rotCerveza;
	KeyFrame[FrameIndex].posCervezaX = posCervezaX;
	KeyFrame[FrameIndex].posCervezaY = posCervezaY;
	KeyFrame[FrameIndex].posCervezaZ = posCervezaZ;
	KeyFrame[FrameIndex].posCabeza = posCabeza;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	rotBder = KeyFrame[0].rotBraDer;

	//Posición de la cerveza
	posCerX = KeyFrame[0].posCervezaX;
	posCerY = KeyFrame[0].posCervezaY;
	posCerZ = KeyFrame[0].posCervezaZ;
	rotCer = KeyFrame[0].rotCerveza;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	rotRder = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	rotBder = (KeyFrame[playIndex + 1].rotBraDer - KeyFrame[playIndex].rotBraDer) / i_max_steps;
	rotBizq = (KeyFrame[playIndex + 1].rotBraIzq - KeyFrame[playIndex].rotBraIzq) / i_max_steps;
	rotBderY = (KeyFrame[playIndex + 1].rotBraDerY - KeyFrame[playIndex].rotBraDerY) / i_max_steps;
	rotBizqY = (KeyFrame[playIndex + 1].rotBraIzqY - KeyFrame[playIndex].rotBraIzqY) / i_max_steps;
	rotCab = (KeyFrame[playIndex + 1].rotCabeza - KeyFrame[playIndex].rotCabeza) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	posCab = (KeyFrame[playIndex + 1].posCabeza - KeyFrame[playIndex].posCabeza) / i_max_steps;

	rotCer = (KeyFrame[playIndex + 1].rotCerveza - KeyFrame[playIndex].rotCerveza) / i_max_steps;
	posCerX = (KeyFrame[playIndex + 1].posCervezaX - KeyFrame[playIndex].posCervezaX) / i_max_steps;
	posCerY = (KeyFrame[playIndex + 1].posCervezaY - KeyFrame[playIndex].posCervezaY) / i_max_steps;
	posCerZ = (KeyFrame[playIndex + 1].posCervezaZ - KeyFrame[playIndex].posCervezaZ) / i_max_steps;

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
			rotRodDer += rotRder;
			rotBraDer += rotBder;
			rotBraIzq += rotBizq;
			rotBraDerY += rotBderY;
			rotBraIzqY += rotBizqY;
			rotCabeza += rotCab;
			giroMonito += giroMonitoInc;
			posCabeza += posCab;

			rotCerveza += rotCer;
			posCervezaX += posCerX;
			posCervezaY += posCerY;
			posCervezaZ += posCerZ;

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

	//--------------------------Animacion OVNI---------------
	if (animOvni) {
		if (rec1UFO) { // recorrido que sube el ovni
			posUFOY += 5.0f;
			if (posUFOY > 230.0f)
			{
				rec1UFO = false;
				rec2UFO = true; 
			}
		}
		if (rec2UFO) {
			//movimiento(+) en Z incremental
			posUFOZ += 1.0;
			//movimiento en X de acuerdo a la expresión de la circunferencia
			posUFOX = sqrt(90000 - (posUFOZ * posUFOZ));
			printf("(%.2f,%.2f)\n", posUFOX, posUFOZ);
			//el limite antes del valor del radio para evitar raices de cero o negativas
			if (posUFOZ >= 299.0f) { 
				rec2UFO = false;
				rec3UFO = true;
			}
		}
		if (rec3UFO) {
			//movimiento(-) en Z incremental
			posUFOZ -= 1.0;
			//movimiento en X de acuerdo a la expresión de la circunferencia
			posUFOX = -sqrt(90000 - (posUFOZ * posUFOZ));
			printf("(%.2f,%.2f)\n", posUFOX, posUFOZ);
			//el limite antes del valor del radio para evitar raices de cero o negativas
			if (posUFOZ <= -299.0f) {
				rec3UFO = false;
				//rec4UFO = true;
			}
		}
		
	}
	
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
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");				//--------------------Skybox shader
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	
	vector<std::string> faces												    //--------------------Skybox1
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	vector<std::string> faces2												    //--------------------Skybox1
	{
		"resources/skybox2/right2.jpg",
		"resources/skybox2/left2.jpg",
		"resources/skybox2/top2.jpg",
		"resources/skybox2/bottom2.jpg",
		"resources/skybox2/front2.jpg",
		"resources/skybox2/back2.jpg"
	};

	Skybox skybox = Skybox(faces);												//--------------------Skybox1
	Skybox skybox2 = Skybox(faces2);
	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);											//--------------------Skybox shader

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
	Model cerveza("resources/objects/cerveza/cerveza.obj");

	//Modelo aspiradora
	Model aspiradora("resources/objects/cleaner/cleaner.obj");

	//homeroAnimate keyframes
	Model cabeza1("resources/objects/homero/cabeza1.obj");
	Model brazoDer1("resources/objects/homero/brazoDer1.obj");
	Model torso1("resources/objects/homero/torso1.obj");
	Model brazoIzq1("resources/objects/homero/brazoIzq1.obj");
	Model piernaDer1("resources/objects/homero/piernaDer1.obj");
	Model piernaIzq1("resources/objects/homero/piernaIzq1.obj");

	//Modelo UFO
	Model ufo("resources/objects/UFO/Low_poly_UFO.obj");

	//Inicialización de KeyFrames
	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}*/
	//---------KEYFRAMES HOMERO CERVEZA
	KeyFrame[0].posX = 0;
	KeyFrame[0].posY = 0;
	KeyFrame[0].posZ = 0;
	KeyFrame[0].rotRodIzq = 0;
	KeyFrame[0].rotRodDer = 0;
	KeyFrame[0].rotBraIzq = 0;
	KeyFrame[0].rotBraDer = 0;
	KeyFrame[0].giroMonito = 0;

	KeyFrame[1].posX = 0;
	KeyFrame[1].posY = 0;
	KeyFrame[1].posZ = 1;
	KeyFrame[1].rotRodIzq = 5;
	KeyFrame[1].rotRodDer = -5;
	KeyFrame[1].rotBraIzq = 5;
	KeyFrame[1].rotBraDer = -5;
	KeyFrame[1].giroMonito = 0;

	KeyFrame[2].posX = 0;
	KeyFrame[2].posY = 0;
	KeyFrame[2].posZ = 2;
	KeyFrame[2].rotRodIzq = 10;
	KeyFrame[2].rotRodDer = -10;
	KeyFrame[2].rotBraIzq = 10;
	KeyFrame[2].rotBraDer = -10;
	KeyFrame[2].giroMonito = 0;

	KeyFrame[3].posX = 0;
	KeyFrame[3].posY = 0;
	KeyFrame[3].posZ = 3;
	KeyFrame[3].rotRodIzq = 20;
	KeyFrame[3].rotRodDer = -20;
	KeyFrame[3].rotBraIzq = 20;
	KeyFrame[3].rotBraDer = -20;
	KeyFrame[3].giroMonito = 0;

	KeyFrame[4].posX = 0;
	KeyFrame[4].posY = 0;
	KeyFrame[4].posZ = 4;
	KeyFrame[4].rotRodIzq = 25;
	KeyFrame[4].rotRodDer = -25;
	KeyFrame[4].rotBraIzq = 25;
	KeyFrame[4].rotBraDer = -25;
	KeyFrame[4].giroMonito = 0;

	KeyFrame[5].posX = 0;
	KeyFrame[5].posY = 0;
	KeyFrame[5].posZ = 5;
	KeyFrame[5].rotRodIzq = 30;
	KeyFrame[5].rotRodDer = -30;
	KeyFrame[5].rotBraIzq = 30;
	KeyFrame[5].rotBraDer = -30;
	KeyFrame[5].giroMonito = 0;

	KeyFrame[6].posX = 0;
	KeyFrame[6].posY = 0;
	KeyFrame[6].posZ = 6;
	KeyFrame[6].rotRodIzq = 35;
	KeyFrame[6].rotRodDer = -35;
	KeyFrame[6].rotBraIzq = 35;
	KeyFrame[6].rotBraDer = -35;
	KeyFrame[6].giroMonito = 0;

	KeyFrame[7].posX = 0;
	KeyFrame[7].posY = 0;
	KeyFrame[7].posZ = 7;
	KeyFrame[7].rotRodIzq = 40;
	KeyFrame[7].rotRodDer = -40;
	KeyFrame[7].rotBraIzq = 40;
	KeyFrame[7].rotBraDer = -40;
	KeyFrame[7].giroMonito = 0;

	KeyFrame[8].posX = 0;
	KeyFrame[8].posY = 0;
	KeyFrame[8].posZ = 8;
	KeyFrame[8].rotRodIzq = 45;
	KeyFrame[8].rotRodDer = -45;
	KeyFrame[8].rotBraIzq = 45;
	KeyFrame[8].rotBraDer = -45;
	KeyFrame[8].giroMonito = 0;

	KeyFrame[9].posX = 0;
	KeyFrame[9].posY = 0;
	KeyFrame[9].posZ = 9;
	KeyFrame[9].rotRodIzq = 40;
	KeyFrame[9].rotRodDer = -40;
	KeyFrame[9].rotBraIzq = 40;
	KeyFrame[9].rotBraDer = -40;
	KeyFrame[9].giroMonito = 0;

	KeyFrame[10].posX = 0;
	KeyFrame[10].posY = 0;
	KeyFrame[10].posZ = 10;
	KeyFrame[10].rotRodIzq = 35;
	KeyFrame[10].rotRodDer = -35;
	KeyFrame[10].rotBraIzq = 35;
	KeyFrame[10].rotBraDer = -35;
	KeyFrame[10].giroMonito = 0;

	KeyFrame[11].posX = 0;
	KeyFrame[11].posY = 0;
	KeyFrame[11].posZ = 11;
	KeyFrame[11].rotRodIzq = 30;
	KeyFrame[11].rotRodDer = -30;
	KeyFrame[11].rotBraIzq = 30;
	KeyFrame[11].rotBraDer = -30;
	KeyFrame[11].giroMonito = 0;

	KeyFrame[12].posX = 0;
	KeyFrame[12].posY = 0;
	KeyFrame[12].posZ = 12;
	KeyFrame[12].rotRodIzq = 25;
	KeyFrame[12].rotRodDer = -25;
	KeyFrame[12].rotBraIzq = 25;
	KeyFrame[12].rotBraDer = -25;
	KeyFrame[12].giroMonito = 0;

	KeyFrame[13].posX = 0;
	KeyFrame[13].posY = 0;
	KeyFrame[13].posZ = 13;
	KeyFrame[13].rotRodIzq = 20;
	KeyFrame[13].rotRodDer = -20;
	KeyFrame[13].rotBraIzq = 20;
	KeyFrame[13].rotBraDer = -20;
	KeyFrame[13].giroMonito = 0;

	KeyFrame[14].posX = 0;
	KeyFrame[14].posY = 0;
	KeyFrame[14].posZ = 14;
	KeyFrame[14].rotRodIzq = 15;
	KeyFrame[14].rotRodDer = -15;
	KeyFrame[14].rotBraIzq = 15;
	KeyFrame[14].rotBraDer = -15;
	KeyFrame[14].giroMonito = 0;

	KeyFrame[15].posX = 0;
	KeyFrame[15].posY = 0;
	KeyFrame[15].posZ = 15;
	KeyFrame[15].rotRodIzq = 10;
	KeyFrame[15].rotRodDer = -10;
	KeyFrame[15].rotBraIzq = 10;
	KeyFrame[15].rotBraDer = -10;
	KeyFrame[15].giroMonito = 0;

	KeyFrame[16].posX = 0;
	KeyFrame[16].posY = 0;
	KeyFrame[16].posZ = 16;
	KeyFrame[16].rotRodIzq = 5;
	KeyFrame[16].rotRodDer = -5;
	KeyFrame[16].rotBraIzq = 5;
	KeyFrame[16].rotBraDer = -5;
	KeyFrame[16].giroMonito = 0;

	KeyFrame[17].posX = 0;
	KeyFrame[17].posY = 0;
	KeyFrame[17].posZ = 17;
	KeyFrame[17].rotRodIzq = 0;
	KeyFrame[17].rotRodDer = 0;
	KeyFrame[17].rotBraIzq = 0;
	KeyFrame[17].rotBraDer = 0;
	KeyFrame[17].giroMonito = 0;

	KeyFrame[18].posX = 0;
	KeyFrame[18].posY = 0;
	KeyFrame[18].posZ = 18;
	KeyFrame[18].rotRodIzq = -5;
	KeyFrame[18].rotRodDer = 5;
	KeyFrame[18].rotBraIzq = -5;
	KeyFrame[18].rotBraDer = 5;
	KeyFrame[18].giroMonito = 0;

	KeyFrame[19].posX = 0;
	KeyFrame[19].posY = 0;
	KeyFrame[19].posZ = 19;
	KeyFrame[19].rotRodIzq = -10;
	KeyFrame[19].rotRodDer = 10;
	KeyFrame[19].rotBraIzq = -10;
	KeyFrame[19].rotBraDer = 10;
	KeyFrame[19].giroMonito = 0;

	KeyFrame[20].posX = 0;
	KeyFrame[20].posY = 0;
	KeyFrame[20].posZ = 20;
	KeyFrame[20].rotRodIzq = -20;
	KeyFrame[20].rotRodDer = 20;
	KeyFrame[20].rotBraIzq = -20;
	KeyFrame[20].rotBraDer = 20;
	KeyFrame[20].giroMonito = 0;

	KeyFrame[21].posX = 0;
	KeyFrame[21].posY = 0;
	KeyFrame[21].posZ = 21;
	KeyFrame[21].rotRodIzq = -25;
	KeyFrame[21].rotRodDer = 25;
	KeyFrame[21].rotBraIzq = -25;
	KeyFrame[21].rotBraDer = 25;
	KeyFrame[21].giroMonito = 0;

	KeyFrame[22].posX = 0;
	KeyFrame[22].posY = 0;
	KeyFrame[22].posZ = 22;
	KeyFrame[22].rotRodIzq = -30;
	KeyFrame[22].rotRodDer = 30;
	KeyFrame[22].rotBraIzq = -30;
	KeyFrame[22].rotBraDer = 30;
	KeyFrame[22].giroMonito = 0;

	KeyFrame[23].posX = 0;
	KeyFrame[23].posY = 0;
	KeyFrame[23].posZ = 23;
	KeyFrame[23].rotRodIzq = -35;
	KeyFrame[23].rotRodDer = 35;
	KeyFrame[23].rotBraIzq = -35;
	KeyFrame[23].rotBraDer = 35;
	KeyFrame[23].giroMonito = 0;

	KeyFrame[24].posX = 0;
	KeyFrame[24].posY = 0;
	KeyFrame[24].posZ = 24;
	KeyFrame[24].rotRodIzq = -40;
	KeyFrame[24].rotRodDer = 40;
	KeyFrame[24].rotBraIzq = -40;
	KeyFrame[24].rotBraDer = 40;
	KeyFrame[24].giroMonito = 0;

	KeyFrame[25].posX = 0;
	KeyFrame[25].posY = 0;
	KeyFrame[25].posZ = 25;
	KeyFrame[25].rotRodIzq = -45;
	KeyFrame[25].rotRodDer = 45;
	KeyFrame[25].rotBraIzq = -45;
	KeyFrame[25].rotBraDer = 45;
	KeyFrame[25].giroMonito = 0;

	KeyFrame[26].posX = 0;
	KeyFrame[26].posY = 0;
	KeyFrame[26].posZ = 26;
	KeyFrame[26].rotRodIzq = -40;
	KeyFrame[26].rotRodDer = 40;
	KeyFrame[26].rotBraIzq = -40;
	KeyFrame[26].rotBraDer = 40;
	KeyFrame[26].giroMonito = 0;

	KeyFrame[27].posX = 0;
	KeyFrame[27].posY = 0;
	KeyFrame[27].posZ = 27;
	KeyFrame[27].rotRodIzq = -35;
	KeyFrame[27].rotRodDer = 35;
	KeyFrame[27].rotBraIzq = -35;
	KeyFrame[27].rotBraDer = 35;
	KeyFrame[27].giroMonito = 0;

	KeyFrame[28].posX = 0;
	KeyFrame[28].posY = 0;
	KeyFrame[28].posZ = 28;
	KeyFrame[28].rotRodIzq = -30;
	KeyFrame[28].rotRodDer = 30;
	KeyFrame[28].rotBraIzq = -30;
	KeyFrame[28].rotBraDer = 30;
	KeyFrame[28].giroMonito = 0;

	KeyFrame[29].posX = 0;
	KeyFrame[29].posY = 0;
	KeyFrame[29].posZ = 29;
	KeyFrame[29].rotRodIzq = -25;
	KeyFrame[29].rotRodDer = 25;
	KeyFrame[29].rotBraIzq = -25;
	KeyFrame[29].rotBraDer = 25;
	KeyFrame[29].giroMonito = 0;

	KeyFrame[30].posX = 0;
	KeyFrame[30].posY = 0;
	KeyFrame[30].posZ = 30;
	KeyFrame[30].rotRodIzq = -20;
	KeyFrame[30].rotRodDer = 20;
	KeyFrame[30].rotBraIzq = -20;
	KeyFrame[30].rotBraDer = 20;
	KeyFrame[30].giroMonito = 0;

	KeyFrame[31].posX = 0;
	KeyFrame[31].posY = 0;
	KeyFrame[31].posZ = 31;
	KeyFrame[31].rotRodIzq = -15;
	KeyFrame[31].rotRodDer = 15;
	KeyFrame[31].rotBraIzq = -15;
	KeyFrame[31].rotBraDer = 15;
	KeyFrame[31].giroMonito = 0;

	KeyFrame[32].posX = 0;
	KeyFrame[32].posY = 0;
	KeyFrame[32].posZ = 32;
	KeyFrame[32].rotRodIzq = -10;
	KeyFrame[32].rotRodDer = 10;
	KeyFrame[32].rotBraIzq = -10;
	KeyFrame[32].rotBraDer = 10;
	KeyFrame[32].giroMonito = 0;

	KeyFrame[33].posX = 0;
	KeyFrame[33].posY = 0;
	KeyFrame[33].posZ = 33;
	KeyFrame[33].rotRodIzq = -5;
	KeyFrame[33].rotRodDer = 5;
	KeyFrame[33].rotBraIzq = -5;
	KeyFrame[33].rotBraDer = 5;
	KeyFrame[33].giroMonito = 0;

	KeyFrame[34].posX = 0;
	KeyFrame[34].posY = 0;
	KeyFrame[34].posZ = 34;
	KeyFrame[34].rotRodIzq = 0;
	KeyFrame[34].rotRodDer = 0;
	KeyFrame[34].rotBraIzq = 0;
	KeyFrame[34].rotBraDer = 0;
	KeyFrame[34].giroMonito = 0;

	KeyFrame[35].posX = 0;
	KeyFrame[35].posY = 0;
	KeyFrame[35].posZ = 35;
	KeyFrame[35].rotRodIzq = 5;
	KeyFrame[35].rotRodDer = -5;
	KeyFrame[35].rotBraIzq = 5;
	KeyFrame[35].rotBraDer = -5;
	KeyFrame[35].giroMonito = 0;

	KeyFrame[36].posX = 0;
	KeyFrame[36].posY = 0;
	KeyFrame[36].posZ = 36;
	KeyFrame[36].rotRodIzq = 10;
	KeyFrame[36].rotRodDer = -10;
	KeyFrame[36].rotBraIzq = 10;
	KeyFrame[36].rotBraDer = -10;
	KeyFrame[36].giroMonito = 0;

	KeyFrame[37].posX = 0;
	KeyFrame[37].posY = 0;
	KeyFrame[37].posZ = 37;
	KeyFrame[37].rotRodIzq = 20;
	KeyFrame[37].rotRodDer = -20;
	KeyFrame[37].rotBraIzq = 20;
	KeyFrame[37].rotBraDer = -20;
	KeyFrame[37].giroMonito = 0;

	KeyFrame[38].posX = 0;
	KeyFrame[38].posY = 0;
	KeyFrame[38].posZ = 38;
	KeyFrame[38].rotRodIzq = 25;
	KeyFrame[38].rotRodDer = -25;
	KeyFrame[38].rotBraIzq = 25;
	KeyFrame[38].rotBraDer = -25;
	KeyFrame[38].giroMonito = 0;

	KeyFrame[39].posX = 0;
	KeyFrame[39].posY = 0;
	KeyFrame[39].posZ = 39;
	KeyFrame[39].rotRodIzq = 30;
	KeyFrame[39].rotRodDer = -30;
	KeyFrame[39].rotBraIzq = 30;
	KeyFrame[39].rotBraDer = -30;
	KeyFrame[39].giroMonito = 0;

	KeyFrame[40].posX = 0;
	KeyFrame[40].posY = 0;
	KeyFrame[40].posZ = 40;
	KeyFrame[40].rotRodIzq = 35;
	KeyFrame[40].rotRodDer = -35;
	KeyFrame[40].rotBraIzq = 35;
	KeyFrame[40].rotBraDer = -35;
	KeyFrame[40].giroMonito = 0;

	KeyFrame[41].posX = 0;
	KeyFrame[41].posY = 0;
	KeyFrame[41].posZ = 41;
	KeyFrame[41].rotRodIzq = 40;
	KeyFrame[41].rotRodDer = -40;
	KeyFrame[41].rotBraIzq = 40;
	KeyFrame[41].rotBraDer = -40;
	KeyFrame[41].giroMonito = 0;

	KeyFrame[42].posX = 0;
	KeyFrame[42].posY = 0;
	KeyFrame[42].posZ = 42;
	KeyFrame[42].rotRodIzq = 45;
	KeyFrame[42].rotRodDer = -45;
	KeyFrame[42].rotBraIzq = 45;
	KeyFrame[42].rotBraDer = -45;
	KeyFrame[42].giroMonito = 0;

	KeyFrame[43].posX = 0;
	KeyFrame[43].posY = 0;
	KeyFrame[43].posZ = 43;
	KeyFrame[43].rotRodIzq = 40;
	KeyFrame[43].rotRodDer = -40;
	KeyFrame[43].rotBraIzq = 40;
	KeyFrame[43].rotBraDer = -40;
	KeyFrame[43].giroMonito = 0;

	KeyFrame[44].posX = 0;
	KeyFrame[44].posY = 0;
	KeyFrame[44].posZ = 44;
	KeyFrame[44].rotRodIzq = 35;
	KeyFrame[44].rotRodDer = -35;
	KeyFrame[44].rotBraIzq = 35;
	KeyFrame[44].rotBraDer = -35;
	KeyFrame[44].giroMonito = 0;

	KeyFrame[45].posX = 0;
	KeyFrame[45].posY = 0;
	KeyFrame[45].posZ = 45;
	KeyFrame[45].rotRodIzq = 30;
	KeyFrame[45].rotRodDer = -30;
	KeyFrame[45].rotBraIzq = 30;
	KeyFrame[45].rotBraDer = -30;
	KeyFrame[45].giroMonito = 0;

	KeyFrame[46].posX = 0;
	KeyFrame[46].posY = 0;
	KeyFrame[46].posZ = 46;
	KeyFrame[46].rotRodIzq = 25;
	KeyFrame[46].rotRodDer = -25;
	KeyFrame[46].rotBraIzq = 25;
	KeyFrame[46].rotBraDer = -25;
	KeyFrame[46].giroMonito = 0;

	KeyFrame[47].posX = 0;
	KeyFrame[47].posY = 0;
	KeyFrame[47].posZ = 47;
	KeyFrame[47].rotRodIzq = 20;
	KeyFrame[47].rotRodDer = -20;
	KeyFrame[47].rotBraIzq = 20;
	KeyFrame[47].rotBraDer = -20;
	KeyFrame[47].giroMonito = 0;

	KeyFrame[48].posX = 0;
	KeyFrame[48].posY = 0;
	KeyFrame[48].posZ = 48;
	KeyFrame[48].rotRodIzq = 15;
	KeyFrame[48].rotRodDer = -15;
	KeyFrame[48].rotBraIzq = 15;
	KeyFrame[48].rotBraDer = -15;
	KeyFrame[48].giroMonito = 0;

	KeyFrame[49].posX = 0;
	KeyFrame[49].posY = 0;
	KeyFrame[49].posZ = 49;
	KeyFrame[49].rotRodIzq = 10;
	KeyFrame[49].rotRodDer = -10;
	KeyFrame[49].rotBraIzq = 10;
	KeyFrame[49].rotBraDer = -10;
	KeyFrame[49].giroMonito = 0;

	KeyFrame[50].posX = 0;
	KeyFrame[50].posY = 0;
	KeyFrame[50].posZ = 50;
	KeyFrame[50].rotRodIzq = 5;
	KeyFrame[50].rotRodDer = -5;
	KeyFrame[50].rotBraIzq = 5;
	KeyFrame[50].rotBraDer = -5;
	KeyFrame[50].giroMonito = 0;

	KeyFrame[51].posX = 0;
	KeyFrame[51].posY = 0;
	KeyFrame[51].posZ = 51;
	KeyFrame[51].rotRodIzq = 0;
	KeyFrame[51].rotRodDer = 0;
	KeyFrame[51].rotBraIzq = 0;
	KeyFrame[51].rotBraDer = 0;
	KeyFrame[51].giroMonito = 0;

	KeyFrame[52].posX = 0;
	KeyFrame[52].posY = 0;
	KeyFrame[52].posZ = 52;
	KeyFrame[52].rotRodIzq = -5;
	KeyFrame[52].rotRodDer = 5;
	KeyFrame[52].rotBraIzq = -5;
	KeyFrame[52].rotBraDer = 5;
	KeyFrame[52].giroMonito = 0;

	KeyFrame[53].posX = 0;
	KeyFrame[53].posY = 0;
	KeyFrame[53].posZ = 53;
	KeyFrame[53].rotRodIzq = -10;
	KeyFrame[53].rotRodDer = 10;
	KeyFrame[53].rotBraIzq = -10;
	KeyFrame[53].rotBraDer = 10;
	KeyFrame[53].giroMonito = 0;

	KeyFrame[54].posX = 0;
	KeyFrame[54].posY = 0;
	KeyFrame[54].posZ = 54;
	KeyFrame[54].rotRodIzq = -20;
	KeyFrame[54].rotRodDer = 20;
	KeyFrame[54].rotBraIzq = -20;
	KeyFrame[54].rotBraDer = 20;
	KeyFrame[54].giroMonito = 0;

	KeyFrame[55].posX = 0;
	KeyFrame[55].posY = 0;
	KeyFrame[55].posZ = 55;
	KeyFrame[55].rotRodIzq = -25;
	KeyFrame[55].rotRodDer = 25;
	KeyFrame[55].rotBraIzq = -25;
	KeyFrame[55].rotBraDer = 25;
	KeyFrame[55].giroMonito = 0;

	KeyFrame[56].posX = 0;
	KeyFrame[56].posY = 0;
	KeyFrame[56].posZ = 56;
	KeyFrame[56].rotRodIzq = -30;
	KeyFrame[56].rotRodDer = 30;
	KeyFrame[56].rotBraIzq = -30;
	KeyFrame[56].rotBraDer = 30;
	KeyFrame[56].giroMonito = 0;

	KeyFrame[57].posX = 0;
	KeyFrame[57].posY = 0;
	KeyFrame[57].posZ = 57;
	KeyFrame[57].rotRodIzq = -35;
	KeyFrame[57].rotRodDer = 35;
	KeyFrame[57].rotBraIzq = -35;
	KeyFrame[57].rotBraDer = 35;
	KeyFrame[57].giroMonito = 0;

	KeyFrame[58].posX = 0;
	KeyFrame[58].posY = 0;
	KeyFrame[58].posZ = 58;
	KeyFrame[58].rotRodIzq = -40;
	KeyFrame[58].rotRodDer = 40;
	KeyFrame[58].rotBraIzq = -40;
	KeyFrame[58].rotBraDer = 40;
	KeyFrame[58].giroMonito = 0;

	KeyFrame[59].posX = 0;
	KeyFrame[59].posY = 0;
	KeyFrame[59].posZ = 59;
	KeyFrame[59].rotRodIzq = -45;
	KeyFrame[59].rotRodDer = 45;
	KeyFrame[59].rotBraIzq = -45;
	KeyFrame[59].rotBraDer = 45;
	KeyFrame[59].giroMonito = 0;

	KeyFrame[60].posX = 0;
	KeyFrame[60].posY = 0;
	KeyFrame[60].posZ = 60;
	KeyFrame[60].rotRodIzq = -40;
	KeyFrame[60].rotRodDer = 40;
	KeyFrame[60].rotBraIzq = -40;
	KeyFrame[60].rotBraDer = 40;
	KeyFrame[60].giroMonito = 0;

	KeyFrame[61].posX = 0;
	KeyFrame[61].posY = 0;
	KeyFrame[61].posZ = 61;
	KeyFrame[61].rotRodIzq = -35;
	KeyFrame[61].rotRodDer = 35;
	KeyFrame[61].rotBraIzq = -35;
	KeyFrame[61].rotBraDer = 35;
	KeyFrame[61].giroMonito = 0;

	KeyFrame[62].posX = 0;
	KeyFrame[62].posY = 0;
	KeyFrame[62].posZ = 62;
	KeyFrame[62].rotRodIzq = -30;
	KeyFrame[62].rotRodDer = 30;
	KeyFrame[62].rotBraIzq = -30;
	KeyFrame[62].rotBraDer = 30;
	KeyFrame[62].giroMonito = 0;

	KeyFrame[63].posX = 0;
	KeyFrame[63].posY = 0;
	KeyFrame[63].posZ = 63;
	KeyFrame[63].rotRodIzq = -25;
	KeyFrame[63].rotRodDer = 25;
	KeyFrame[63].rotBraIzq = -25;
	KeyFrame[63].rotBraDer = 25;
	KeyFrame[63].giroMonito = 0;

	KeyFrame[64].posX = 0;
	KeyFrame[64].posY = 0;
	KeyFrame[64].posZ = 64;
	KeyFrame[64].rotRodIzq = -20;
	KeyFrame[64].rotRodDer = 20;
	KeyFrame[64].rotBraIzq = -20;
	KeyFrame[64].rotBraDer = 20;
	KeyFrame[64].giroMonito = 0;

	KeyFrame[65].posX = 0;
	KeyFrame[65].posY = 0;
	KeyFrame[65].posZ = 65;
	KeyFrame[65].rotRodIzq = -15;
	KeyFrame[65].rotRodDer = 15;
	KeyFrame[65].rotBraIzq = -15;
	KeyFrame[65].rotBraDer = 15;
	KeyFrame[65].giroMonito = 0;

	KeyFrame[66].posX = 0;
	KeyFrame[66].posY = 0;
	KeyFrame[66].posZ = 66;
	KeyFrame[66].rotRodIzq = -10;
	KeyFrame[66].rotRodDer = 10;
	KeyFrame[66].rotBraIzq = -10;
	KeyFrame[66].rotBraDer = 10;
	KeyFrame[66].giroMonito = 0;

	KeyFrame[67].posX = 0;
	KeyFrame[67].posY = 0;
	KeyFrame[67].posZ = 67;
	KeyFrame[67].rotRodIzq = -5;
	KeyFrame[67].rotRodDer = 5;
	KeyFrame[67].rotBraIzq = -5;
	KeyFrame[67].rotBraDer = 5;
	KeyFrame[67].giroMonito = 0;

	KeyFrame[68].posX = 0;
	KeyFrame[68].posY = 0;
	KeyFrame[68].posZ = 68;
	KeyFrame[68].rotRodIzq = 0;
	KeyFrame[68].rotRodDer = 0;
	KeyFrame[68].rotBraIzq = 0;
	KeyFrame[68].rotBraDer = 0;
	KeyFrame[68].giroMonito = 0;

	KeyFrame[69].posX = 1;
	KeyFrame[69].posY = 0;
	KeyFrame[69].posZ = 69;
	KeyFrame[69].rotRodIzq = 5;
	KeyFrame[69].rotRodDer = -5;
	KeyFrame[69].rotBraIzq = 5;
	KeyFrame[69].rotBraDer = -5;
	KeyFrame[69].giroMonito = 5;

	KeyFrame[70].posX = 2;
	KeyFrame[70].posY = 0;
	KeyFrame[70].posZ = 70;
	KeyFrame[70].rotRodIzq = 10;
	KeyFrame[70].rotRodDer = -10;
	KeyFrame[70].rotBraIzq = 10;
	KeyFrame[70].rotBraDer = -10;
	KeyFrame[70].giroMonito = 10;

	KeyFrame[71].posX = 3;
	KeyFrame[71].posY = 0;
	KeyFrame[71].posZ = 71;
	KeyFrame[71].rotRodIzq = 20;
	KeyFrame[71].rotRodDer = -20;
	KeyFrame[71].rotBraIzq = 20;
	KeyFrame[71].rotBraDer = -20;
	KeyFrame[71].giroMonito = 15;

	KeyFrame[72].posX = 4;
	KeyFrame[72].posY = 0;
	KeyFrame[72].posZ = 72;
	KeyFrame[72].rotRodIzq = 25;
	KeyFrame[72].rotRodDer = -25;
	KeyFrame[72].rotBraIzq = 25;
	KeyFrame[72].rotBraDer = -25;
	KeyFrame[72].giroMonito = 20;

	KeyFrame[73].posX = 5;
	KeyFrame[73].posY = 0;
	KeyFrame[73].posZ = 73;
	KeyFrame[73].rotRodIzq = 30;
	KeyFrame[73].rotRodDer = -30;
	KeyFrame[73].rotBraIzq = 30;
	KeyFrame[73].rotBraDer = -30;
	KeyFrame[73].giroMonito = 25;

	KeyFrame[74].posX = 6;
	KeyFrame[74].posY = 0;
	KeyFrame[74].posZ = 74;
	KeyFrame[74].rotRodIzq = 35;
	KeyFrame[74].rotRodDer = -35;
	KeyFrame[74].rotBraIzq = 35;
	KeyFrame[74].rotBraDer = -35;
	KeyFrame[74].giroMonito = 30;

	KeyFrame[75].posX = 7;
	KeyFrame[75].posY = 0;
	KeyFrame[75].posZ = 75;
	KeyFrame[75].rotRodIzq = 40;
	KeyFrame[75].rotRodDer = -40;
	KeyFrame[75].rotBraIzq = 40;
	KeyFrame[75].rotBraDer = -40;
	KeyFrame[75].giroMonito = 35;

	KeyFrame[76].posX = 8;
	KeyFrame[76].posY = 0;
	KeyFrame[76].posZ = 76;
	KeyFrame[76].rotRodIzq = 45;
	KeyFrame[76].rotRodDer = -45;
	KeyFrame[76].rotBraIzq = 45;
	KeyFrame[76].rotBraDer = -45;
	KeyFrame[76].giroMonito = 40;

	KeyFrame[77].posX = 9;
	KeyFrame[77].posY = 0;
	KeyFrame[77].posZ = 77;
	KeyFrame[77].rotRodIzq = 40;
	KeyFrame[77].rotRodDer = -40;
	KeyFrame[77].rotBraIzq = 40;
	KeyFrame[77].rotBraDer = -40;
	KeyFrame[77].giroMonito = 45;

	KeyFrame[78].posX = 10;
	KeyFrame[78].posY = 0;
	KeyFrame[78].posZ = 78;
	KeyFrame[78].rotRodIzq = 35;
	KeyFrame[78].rotRodDer = -35;
	KeyFrame[78].rotBraIzq = 35;
	KeyFrame[78].rotBraDer = -35;
	KeyFrame[78].giroMonito = 45;

	KeyFrame[79].posX = 11;
	KeyFrame[79].posY = 0;
	KeyFrame[79].posZ = 79;
	KeyFrame[79].rotRodIzq = 30;
	KeyFrame[79].rotRodDer = -30;
	KeyFrame[79].rotBraIzq = 30;
	KeyFrame[79].rotBraDer = -30;
	KeyFrame[79].giroMonito = 45;

	KeyFrame[80].posX = 12;
	KeyFrame[80].posY = 0;
	KeyFrame[80].posZ = 80;
	KeyFrame[80].rotRodIzq = 25;
	KeyFrame[80].rotRodDer = -25;
	KeyFrame[80].rotBraIzq = 25;
	KeyFrame[80].rotBraDer = -25;
	KeyFrame[80].giroMonito = 45;

	KeyFrame[81].posX = 13;
	KeyFrame[81].posY = 0;
	KeyFrame[81].posZ = 81;
	KeyFrame[81].rotRodIzq = 20;
	KeyFrame[81].rotRodDer = -20;
	KeyFrame[81].rotBraIzq = 20;
	KeyFrame[81].rotBraDer = -20;
	KeyFrame[81].giroMonito = 45;

	KeyFrame[82].posX = 14;
	KeyFrame[82].posY = 0;
	KeyFrame[82].posZ = 82;
	KeyFrame[82].rotRodIzq = 15;
	KeyFrame[82].rotRodDer = -15;
	KeyFrame[82].rotBraIzq = 15;
	KeyFrame[82].rotBraDer = -15;
	KeyFrame[82].giroMonito = 45;

	KeyFrame[83].posX = 15;
	KeyFrame[83].posY = 0;
	KeyFrame[83].posZ = 83;
	KeyFrame[83].rotRodIzq = 10;
	KeyFrame[83].rotRodDer = -10;
	KeyFrame[83].rotBraIzq = 10;
	KeyFrame[83].rotBraDer = -10;
	KeyFrame[83].giroMonito = 45;

	KeyFrame[84].posX = 16;
	KeyFrame[84].posY = 0;
	KeyFrame[84].posZ = 84;
	KeyFrame[84].rotRodIzq = 5;
	KeyFrame[84].rotRodDer = -5;
	KeyFrame[84].rotBraIzq = 5;
	KeyFrame[84].rotBraDer = -5;
	KeyFrame[84].giroMonito = 45;

	KeyFrame[85].posX = 17;
	KeyFrame[85].posY = 0;
	KeyFrame[85].posZ = 85;
	KeyFrame[85].rotRodIzq = 0;
	KeyFrame[85].rotRodDer = 0;
	KeyFrame[85].rotBraIzq = 0;
	KeyFrame[85].rotBraDer = 0;
	KeyFrame[85].giroMonito = 45;

	KeyFrame[86].posX = 18;
	KeyFrame[86].posY = 0;
	KeyFrame[86].posZ = 86;
	KeyFrame[86].rotRodIzq = -5;
	KeyFrame[86].rotRodDer = 5;
	KeyFrame[86].rotBraIzq = -5;
	KeyFrame[86].rotBraDer = 5;
	KeyFrame[86].giroMonito = 45;

	KeyFrame[87].posX = 19;
	KeyFrame[87].posY = 0;
	KeyFrame[87].posZ = 87;
	KeyFrame[87].rotRodIzq = -10;
	KeyFrame[87].rotRodDer = 10;
	KeyFrame[87].rotBraIzq = -10;
	KeyFrame[87].rotBraDer = 10;
	KeyFrame[87].giroMonito = 45;

	KeyFrame[88].posX = 20;
	KeyFrame[88].posY = 0;
	KeyFrame[88].posZ = 88;
	KeyFrame[88].rotRodIzq = -20;
	KeyFrame[88].rotRodDer = 20;
	KeyFrame[88].rotBraIzq = -20;
	KeyFrame[88].rotBraDer = 20;
	KeyFrame[88].giroMonito = 45;

	KeyFrame[89].posX = 21;
	KeyFrame[89].posY = 0;
	KeyFrame[89].posZ = 89;
	KeyFrame[89].rotRodIzq = -25;
	KeyFrame[89].rotRodDer = 25;
	KeyFrame[89].rotBraIzq = -25;
	KeyFrame[89].rotBraDer = 25;
	KeyFrame[89].giroMonito = 45;

	KeyFrame[90].posX = 22;
	KeyFrame[90].posY = 0;
	KeyFrame[90].posZ = 90;
	KeyFrame[90].rotRodIzq = -30;
	KeyFrame[90].rotRodDer = 30;
	KeyFrame[90].rotBraIzq = -30;
	KeyFrame[90].rotBraDer = 30;
	KeyFrame[90].giroMonito = 45;

	KeyFrame[91].posX = 23;
	KeyFrame[91].posY = 0;
	KeyFrame[91].posZ = 91;
	KeyFrame[91].rotRodIzq = -35;
	KeyFrame[91].rotRodDer = 35;
	KeyFrame[91].rotBraIzq = -35;
	KeyFrame[91].rotBraDer = 35;
	KeyFrame[91].giroMonito = 45;

	KeyFrame[92].posX = 24;
	KeyFrame[92].posY = 0;
	KeyFrame[92].posZ = 92;
	KeyFrame[92].rotRodIzq = -40;
	KeyFrame[92].rotRodDer = 40;
	KeyFrame[92].rotBraIzq = -40;
	KeyFrame[92].rotBraDer = 40;
	KeyFrame[92].giroMonito = 45;

	KeyFrame[93].posX = 25;
	KeyFrame[93].posY = 0;
	KeyFrame[93].posZ = 93;
	KeyFrame[93].rotRodIzq = -45;
	KeyFrame[93].rotRodDer = 45;
	KeyFrame[93].rotBraIzq = -45;
	KeyFrame[93].rotBraDer = 45;
	KeyFrame[93].giroMonito = 45;

	KeyFrame[94].posX = 26;
	KeyFrame[94].posY = 0;
	KeyFrame[94].posZ = 94;
	KeyFrame[94].rotRodIzq = -40;
	KeyFrame[94].rotRodDer = 40;
	KeyFrame[94].rotBraIzq = -40;
	KeyFrame[94].rotBraDer = 40;
	KeyFrame[94].giroMonito = 45;

	KeyFrame[95].posX = 27;
	KeyFrame[95].posY = 0;
	KeyFrame[95].posZ = 95;
	KeyFrame[95].rotRodIzq = -35;
	KeyFrame[95].rotRodDer = 35;
	KeyFrame[95].rotBraIzq = -35;
	KeyFrame[95].rotBraDer = 35;
	KeyFrame[95].giroMonito = 45;

	KeyFrame[96].posX = 28;
	KeyFrame[96].posY = 0;
	KeyFrame[96].posZ = 96;
	KeyFrame[96].rotRodIzq = -30;
	KeyFrame[96].rotRodDer = 30;
	KeyFrame[96].rotBraIzq = -30;
	KeyFrame[96].rotBraDer = 30;
	KeyFrame[96].giroMonito = 45;

	KeyFrame[97].posX = 29;
	KeyFrame[97].posY = 0;
	KeyFrame[97].posZ = 97;
	KeyFrame[97].rotRodIzq = -25;
	KeyFrame[97].rotRodDer = 25;
	KeyFrame[97].rotBraIzq = -25;
	KeyFrame[97].rotBraDer = 25;
	KeyFrame[97].giroMonito = 45;

	KeyFrame[98].posX = 30;
	KeyFrame[98].posY = 0;
	KeyFrame[98].posZ = 98;
	KeyFrame[98].rotRodIzq = -20;
	KeyFrame[98].rotRodDer = 20;
	KeyFrame[98].rotBraIzq = -20;
	KeyFrame[98].rotBraDer = 20;
	KeyFrame[98].giroMonito = 45;

	KeyFrame[99].posX = 31;
	KeyFrame[99].posY = 0;
	KeyFrame[99].posZ = 99;
	KeyFrame[99].rotRodIzq = -15;
	KeyFrame[99].rotRodDer = 15;
	KeyFrame[99].rotBraIzq = -15;
	KeyFrame[99].rotBraDer = 15;
	KeyFrame[99].giroMonito = 45;

	KeyFrame[100].posX = 32;
	KeyFrame[100].posY = 0;
	KeyFrame[100].posZ = 100;
	KeyFrame[100].rotRodIzq = -10;
	KeyFrame[100].rotRodDer = 10;
	KeyFrame[100].rotBraIzq = -10;
	KeyFrame[100].rotBraDer = 10;
	KeyFrame[100].giroMonito = 45;

	KeyFrame[101].posX = 33;
	KeyFrame[101].posY = 0;
	KeyFrame[101].posZ = 101;
	KeyFrame[101].rotRodIzq = -5;
	KeyFrame[101].rotRodDer = 5;
	KeyFrame[101].rotBraIzq = -5;
	KeyFrame[101].rotBraDer = 5;
	KeyFrame[101].giroMonito = 45;

	KeyFrame[102].posX = 34;
	KeyFrame[102].posY = 0;
	KeyFrame[102].posZ = 102;
	KeyFrame[102].rotRodIzq = 0;
	KeyFrame[102].rotRodDer = 0;
	KeyFrame[102].rotBraIzq = 0;
	KeyFrame[102].rotBraDer = 0;
	KeyFrame[102].giroMonito = 45;

	KeyFrame[103].posX = 35;
	KeyFrame[103].posY = 0;
	KeyFrame[103].posZ = 103;
	KeyFrame[103].rotRodIzq = 5;
	KeyFrame[103].rotRodDer = -5;
	KeyFrame[103].rotBraIzq = 5;
	KeyFrame[103].rotBraDer = -5;
	KeyFrame[103].giroMonito = 45;

	KeyFrame[104].posX = 36;
	KeyFrame[104].posY = 0;
	KeyFrame[104].posZ = 104;
	KeyFrame[104].rotRodIzq = 10;
	KeyFrame[104].rotRodDer = -10;
	KeyFrame[104].rotBraIzq = 10;
	KeyFrame[104].rotBraDer = -10;
	KeyFrame[104].giroMonito = 45;

	KeyFrame[105].posX = 37;
	KeyFrame[105].posY = 0;
	KeyFrame[105].posZ = 105;
	KeyFrame[105].rotRodIzq = 20;
	KeyFrame[105].rotRodDer = -20;
	KeyFrame[105].rotBraIzq = 20;
	KeyFrame[105].rotBraDer = -20;
	KeyFrame[105].giroMonito = 45;

	KeyFrame[106].posX = 38;
	KeyFrame[106].posY = 0;
	KeyFrame[106].posZ = 106;
	KeyFrame[106].rotRodIzq = 25;
	KeyFrame[106].rotRodDer = -25;
	KeyFrame[106].rotBraIzq = 25;
	KeyFrame[106].rotBraDer = -25;
	KeyFrame[106].giroMonito = 45;

	KeyFrame[107].posX = 39;
	KeyFrame[107].posY = 0;
	KeyFrame[107].posZ = 107;
	KeyFrame[107].rotRodIzq = 30;
	KeyFrame[107].rotRodDer = -30;
	KeyFrame[107].rotBraIzq = 30;
	KeyFrame[107].rotBraDer = -30;
	KeyFrame[107].giroMonito = 45;

	KeyFrame[108].posX = 40;
	KeyFrame[108].posY = 0;
	KeyFrame[108].posZ = 108;
	KeyFrame[108].rotRodIzq = 35;
	KeyFrame[108].rotRodDer = -35;
	KeyFrame[108].rotBraIzq = 35;
	KeyFrame[108].rotBraDer = -35;
	KeyFrame[108].giroMonito = 45;

	KeyFrame[109].posX = 41;
	KeyFrame[109].posY = 0;
	KeyFrame[109].posZ = 109;
	KeyFrame[109].rotRodIzq = 40;
	KeyFrame[109].rotRodDer = -40;
	KeyFrame[109].rotBraIzq = 40;
	KeyFrame[109].rotBraDer = -40;
	KeyFrame[109].giroMonito = 45;

	KeyFrame[110].posX = 42;
	KeyFrame[110].posY = 0;
	KeyFrame[110].posZ = 110;
	KeyFrame[110].rotRodIzq = 45;
	KeyFrame[110].rotRodDer = -45;
	KeyFrame[110].rotBraIzq = 45;
	KeyFrame[110].rotBraDer = -45;
	KeyFrame[110].giroMonito = 45;

	KeyFrame[111].posX = 43;
	KeyFrame[111].posY = 0;
	KeyFrame[111].posZ = 111;
	KeyFrame[111].rotRodIzq = 40;
	KeyFrame[111].rotRodDer = -40;
	KeyFrame[111].rotBraIzq = 40;
	KeyFrame[111].rotBraDer = -40;
	KeyFrame[111].giroMonito = 45;

	KeyFrame[112].posX = 44;
	KeyFrame[112].posY = 0;
	KeyFrame[112].posZ = 112;
	KeyFrame[112].rotRodIzq = 35;
	KeyFrame[112].rotRodDer = -35;
	KeyFrame[112].rotBraIzq = 35;
	KeyFrame[112].rotBraDer = -35;
	KeyFrame[112].giroMonito = 45;

	KeyFrame[113].posX = 45;
	KeyFrame[113].posY = 0;
	KeyFrame[113].posZ = 113;
	KeyFrame[113].rotRodIzq = 30;
	KeyFrame[113].rotRodDer = -30;
	KeyFrame[113].rotBraIzq = 30;
	KeyFrame[113].rotBraDer = -30;
	KeyFrame[113].giroMonito = 45;

	KeyFrame[114].posX = 46;
	KeyFrame[114].posY = 0;
	KeyFrame[114].posZ = 114;
	KeyFrame[114].rotRodIzq = 25;
	KeyFrame[114].rotRodDer = -25;
	KeyFrame[114].rotBraIzq = 25;
	KeyFrame[114].rotBraDer = -25;
	KeyFrame[114].giroMonito = 45;

	KeyFrame[115].posX = 47;
	KeyFrame[115].posY = 0;
	KeyFrame[115].posZ = 115;
	KeyFrame[115].rotRodIzq = 20;
	KeyFrame[115].rotRodDer = -20;
	KeyFrame[115].rotBraIzq = 20;
	KeyFrame[115].rotBraDer = -20;
	KeyFrame[115].giroMonito = 45;

	KeyFrame[116].posX = 48;
	KeyFrame[116].posY = 0;
	KeyFrame[116].posZ = 116;
	KeyFrame[116].rotRodIzq = 15;
	KeyFrame[116].rotRodDer = -15;
	KeyFrame[116].rotBraIzq = 15;
	KeyFrame[116].rotBraDer = -15;
	KeyFrame[116].giroMonito = 45;

	KeyFrame[117].posX = 49;
	KeyFrame[117].posY = 0;
	KeyFrame[117].posZ = 117;
	KeyFrame[117].rotRodIzq = 10;
	KeyFrame[117].rotRodDer = -10;
	KeyFrame[117].rotBraIzq = 10;
	KeyFrame[117].rotBraDer = -10;
	KeyFrame[117].giroMonito = 45;

	KeyFrame[118].posX = 50;
	KeyFrame[118].posY = 0;
	KeyFrame[118].posZ = 118;
	KeyFrame[118].rotRodIzq = 5;
	KeyFrame[118].rotRodDer = -5;
	KeyFrame[118].rotBraIzq = 5;
	KeyFrame[118].rotBraDer = -5;
	KeyFrame[118].giroMonito = 45;

	KeyFrame[119].posX = 51;
	KeyFrame[119].posY = 0;
	KeyFrame[119].posZ = 119;
	KeyFrame[119].rotRodIzq = 0;
	KeyFrame[119].rotRodDer = 0;
	KeyFrame[119].rotBraIzq = 0;
	KeyFrame[119].rotBraDer = 0;
	KeyFrame[119].giroMonito = 45;

	KeyFrame[120].posX = 52;
	KeyFrame[120].posY = 0;
	KeyFrame[120].posZ = 120;
	KeyFrame[120].rotRodIzq = -5;
	KeyFrame[120].rotRodDer = 5;
	KeyFrame[120].rotBraIzq = -5;
	KeyFrame[120].rotBraDer = 5;
	KeyFrame[120].giroMonito = 45;

	KeyFrame[121].posX = 53;
	KeyFrame[121].posY = 0;
	KeyFrame[121].posZ = 121;
	KeyFrame[121].rotRodIzq = -10;
	KeyFrame[121].rotRodDer = 10;
	KeyFrame[121].rotBraIzq = -10;
	KeyFrame[121].rotBraDer = 10;
	KeyFrame[121].giroMonito = 45;

	KeyFrame[122].posX = 54;
	KeyFrame[122].posY = 0;
	KeyFrame[122].posZ = 122;
	KeyFrame[122].rotRodIzq = -20;
	KeyFrame[122].rotRodDer = 20;
	KeyFrame[122].rotBraIzq = -20;
	KeyFrame[122].rotBraDer = 20;
	KeyFrame[122].giroMonito = 45;

	KeyFrame[123].posX = 55;
	KeyFrame[123].posY = 0;
	KeyFrame[123].posZ = 123;
	KeyFrame[123].rotRodIzq = -25;
	KeyFrame[123].rotRodDer = 25;
	KeyFrame[123].rotBraIzq = -25;
	KeyFrame[123].rotBraDer = 25;
	KeyFrame[123].giroMonito = 45;

	KeyFrame[124].posX = 56;
	KeyFrame[124].posY = 0;
	KeyFrame[124].posZ = 124;
	KeyFrame[124].rotRodIzq = -30;
	KeyFrame[124].rotRodDer = 30;
	KeyFrame[124].rotBraIzq = -30;
	KeyFrame[124].rotBraDer = 30;
	KeyFrame[124].giroMonito = 45;

	KeyFrame[125].posX = 57;
	KeyFrame[125].posY = 0;
	KeyFrame[125].posZ = 125;
	KeyFrame[125].rotRodIzq = -35;
	KeyFrame[125].rotRodDer = 35;
	KeyFrame[125].rotBraIzq = -35;
	KeyFrame[125].rotBraDer = 35;
	KeyFrame[125].giroMonito = 45;

	KeyFrame[126].posX = 58;
	KeyFrame[126].posY = 0;
	KeyFrame[126].posZ = 126;
	KeyFrame[126].rotRodIzq = -40;
	KeyFrame[126].rotRodDer = 40;
	KeyFrame[126].rotBraIzq = -40;
	KeyFrame[126].rotBraDer = 40;
	KeyFrame[126].giroMonito = 45;

	KeyFrame[127].posX = 59;
	KeyFrame[127].posY = 0;
	KeyFrame[127].posZ = 127;
	KeyFrame[127].rotRodIzq = -45;
	KeyFrame[127].rotRodDer = 45;
	KeyFrame[127].rotBraIzq = -45;
	KeyFrame[127].rotBraDer = 45;
	KeyFrame[127].giroMonito = 45;

	KeyFrame[128].posX = 60;
	KeyFrame[128].posY = 0;
	KeyFrame[128].posZ = 128;
	KeyFrame[128].rotRodIzq = -40;
	KeyFrame[128].rotRodDer = 40;
	KeyFrame[128].rotBraIzq = -40;
	KeyFrame[128].rotBraDer = 40;
	KeyFrame[128].giroMonito = 45;

	KeyFrame[129].posX = 61;
	KeyFrame[129].posY = 0;
	KeyFrame[129].posZ = 129;
	KeyFrame[129].rotRodIzq = -35;
	KeyFrame[129].rotRodDer = 35;
	KeyFrame[129].rotBraIzq = -35;
	KeyFrame[129].rotBraDer = 35;
	KeyFrame[129].giroMonito = 45;

	KeyFrame[130].posX = 62;
	KeyFrame[130].posY = 0;
	KeyFrame[130].posZ = 130;
	KeyFrame[130].rotRodIzq = -30;
	KeyFrame[130].rotRodDer = 30;
	KeyFrame[130].rotBraIzq = -30;
	KeyFrame[130].rotBraDer = 30;
	KeyFrame[130].giroMonito = 45;

	KeyFrame[131].posX = 63;
	KeyFrame[131].posY = 0;
	KeyFrame[131].posZ = 131;
	KeyFrame[131].rotRodIzq = -25;
	KeyFrame[131].rotRodDer = 25;
	KeyFrame[131].rotBraIzq = -25;
	KeyFrame[131].rotBraDer = 25;
	KeyFrame[131].giroMonito = 45;

	KeyFrame[132].posX = 65;
	KeyFrame[132].posY = 0;
	KeyFrame[132].posZ = 133;
	KeyFrame[132].rotRodIzq = -20;
	KeyFrame[132].rotRodDer = 20;
	KeyFrame[132].rotBraIzq = -20;
	KeyFrame[132].rotBraDer = 20;
	KeyFrame[132].giroMonito = 45;

	KeyFrame[133].posX = 67;
	KeyFrame[133].posY = 0;
	KeyFrame[133].posZ = 135;
	KeyFrame[133].rotRodIzq = -15;
	KeyFrame[133].rotRodDer = 15;
	KeyFrame[133].rotBraIzq = -15;
	KeyFrame[133].rotBraDer = 15;
	KeyFrame[133].giroMonito = 45;

	KeyFrame[134].posX = 69;
	KeyFrame[134].posY = 0;
	KeyFrame[134].posZ = 137;
	KeyFrame[134].rotRodIzq = -10;
	KeyFrame[134].rotRodDer = 10;
	KeyFrame[134].rotBraIzq = -10;
	KeyFrame[134].rotBraDer = 10;
	KeyFrame[134].giroMonito = 45;
	KeyFrame[134].giroMonito = 45;

	KeyFrame[135].posX = 72;
	KeyFrame[135].posY = 0;
	KeyFrame[135].posZ = 140;
	KeyFrame[135].rotRodIzq = -5;
	KeyFrame[135].rotRodDer = 5;
	KeyFrame[135].rotBraIzq = -5;
	KeyFrame[135].rotBraDer = 5;
	KeyFrame[135].giroMonito = 45;

	KeyFrame[136].posX = 73;
	KeyFrame[136].posY = 0;
	KeyFrame[136].posZ = 142;
	KeyFrame[136].rotRodIzq = 0;
	KeyFrame[136].rotRodDer = 0;
	KeyFrame[136].rotBraIzq = 0;
	KeyFrame[136].rotBraDer = 0;
	KeyFrame[136].giroMonito = 90;

	KeyFrame[137].posX = 74;
	KeyFrame[137].posY = 0;
	KeyFrame[137].posZ = 142;
	KeyFrame[137].rotRodIzq = 5;
	KeyFrame[137].rotRodDer = -5;
	KeyFrame[137].rotBraIzq = 5;
	KeyFrame[137].rotBraDer = -5;
	KeyFrame[137].giroMonito = 90;

	KeyFrame[138].posX = 75;
	KeyFrame[138].posY = 0.0;
	KeyFrame[138].posZ = 142;
	KeyFrame[138].rotRodIzq = 10;
	KeyFrame[138].rotRodDer = -10;
	KeyFrame[138].rotBraIzq = 10;
	KeyFrame[138].rotBraDer = -10;
	KeyFrame[138].giroMonito = 90;

	KeyFrame[139].posX = 76;
	KeyFrame[139].posY = 0;
	KeyFrame[139].posZ = 142;
	KeyFrame[139].rotRodIzq = 20;
	KeyFrame[139].rotRodDer = -20;
	KeyFrame[139].rotBraIzq = 20;
	KeyFrame[139].rotBraDer = -20;
	KeyFrame[139].giroMonito = 90;

	KeyFrame[140].posX = 77;
	KeyFrame[140].posY = 0;
	KeyFrame[140].posZ = 142;
	KeyFrame[140].rotRodIzq = 25;
	KeyFrame[140].rotRodDer = -25;
	KeyFrame[140].rotBraIzq = 25;
	KeyFrame[140].rotBraDer = -25;
	KeyFrame[140].giroMonito = 90;

	KeyFrame[141].posX = 78;
	KeyFrame[141].posY = 0;
	KeyFrame[141].posZ = 142;
	KeyFrame[141].rotRodIzq = 30;
	KeyFrame[141].rotRodDer = -30;
	KeyFrame[141].rotBraIzq = 30;
	KeyFrame[141].rotBraDer = -30;
	KeyFrame[141].giroMonito = 90;

	KeyFrame[142].posX = 79;
	KeyFrame[142].posY = 0;
	KeyFrame[142].posZ = 142;
	KeyFrame[142].rotRodIzq = 35;
	KeyFrame[142].rotRodDer = -35;
	KeyFrame[142].rotBraIzq = 35;
	KeyFrame[142].rotBraDer = -35;
	KeyFrame[142].giroMonito = 90;

	KeyFrame[143].posX = 80;
	KeyFrame[143].posY = 0;
	KeyFrame[143].posZ = 142;
	KeyFrame[143].rotRodIzq = 40;
	KeyFrame[143].rotRodDer = -40;
	KeyFrame[143].rotBraIzq = 40;
	KeyFrame[143].rotBraDer = -40;
	KeyFrame[143].giroMonito = 90;

	KeyFrame[144].posX = 81;
	KeyFrame[144].posY = 0;
	KeyFrame[144].posZ = 142;
	KeyFrame[144].rotRodIzq = 45;
	KeyFrame[144].rotRodDer = -45;
	KeyFrame[144].rotBraIzq = 45;
	KeyFrame[144].rotBraDer = -45;
	KeyFrame[144].giroMonito = 90;

	KeyFrame[145].posX = 82;
	KeyFrame[145].posY = 0;
	KeyFrame[145].posZ = 142;
	KeyFrame[145].rotRodIzq = 40;
	KeyFrame[145].rotRodDer = -40;
	KeyFrame[145].rotBraIzq = 40;
	KeyFrame[145].rotBraDer = -40;
	KeyFrame[145].giroMonito = 90;

	KeyFrame[146].posX = 83;
	KeyFrame[146].posY = 0;
	KeyFrame[146].posZ = 142;
	KeyFrame[146].rotRodIzq = 35;
	KeyFrame[146].rotRodDer = -35;
	KeyFrame[146].rotBraIzq = 35;
	KeyFrame[146].rotBraDer = -35;
	KeyFrame[146].giroMonito = 90;

	KeyFrame[147].posX = 84;
	KeyFrame[147].posY = 0;
	KeyFrame[147].posZ = 142;
	KeyFrame[147].rotRodIzq = 30;
	KeyFrame[147].rotRodDer = -30;
	KeyFrame[147].rotBraIzq = 30;
	KeyFrame[147].rotBraDer = -30;
	KeyFrame[147].giroMonito = 90;

	KeyFrame[148].posX = 85;
	KeyFrame[148].posY = 0;
	KeyFrame[148].posZ = 142;
	KeyFrame[148].rotRodIzq = 25;
	KeyFrame[148].rotRodDer = -25;
	KeyFrame[148].rotBraIzq = 25;
	KeyFrame[148].rotBraDer = -25;
	KeyFrame[148].giroMonito = 90;

	KeyFrame[149].posX = 86;
	KeyFrame[149].posY = 0;
	KeyFrame[149].posZ = 142;
	KeyFrame[149].rotRodIzq = 20;
	KeyFrame[149].rotRodDer = -20;
	KeyFrame[149].rotBraIzq = 20;
	KeyFrame[149].rotBraDer = -20;
	KeyFrame[149].giroMonito = 90;

	KeyFrame[150].posX = 87;
	KeyFrame[150].posY = 0;
	KeyFrame[150].posZ = 142;
	KeyFrame[150].rotRodIzq = 15;
	KeyFrame[150].rotRodDer = -15;
	KeyFrame[150].rotBraIzq = 15;
	KeyFrame[150].rotBraDer = -15;
	KeyFrame[150].giroMonito = 90;

	KeyFrame[151].posX = 88;
	KeyFrame[151].posY = 0;
	KeyFrame[151].posZ = 142;
	KeyFrame[151].rotRodIzq = 10;
	KeyFrame[151].rotRodDer = -10;
	KeyFrame[151].rotBraIzq = 10;
	KeyFrame[151].rotBraDer = -10;
	KeyFrame[151].giroMonito = 90;

	KeyFrame[152].posX = 89;
	KeyFrame[152].posY = 0;
	KeyFrame[152].posZ = 142;
	KeyFrame[152].rotRodIzq = 5;
	KeyFrame[152].rotRodDer = -5;
	KeyFrame[152].rotBraIzq = 5;
	KeyFrame[152].rotBraDer = -5;
	KeyFrame[152].giroMonito = 90;

	KeyFrame[153].posX = 90;
	KeyFrame[153].posY = 0;
	KeyFrame[153].posZ = 142;
	KeyFrame[153].rotRodIzq = 0;
	KeyFrame[153].rotRodDer = 0;
	KeyFrame[153].rotBraIzq = 0;
	KeyFrame[153].rotBraDer = 0;
	KeyFrame[153].giroMonito = 90;
	//caminar
	KeyFrame[154].posX = 90;
	KeyFrame[154].posY = 0;
	KeyFrame[154].posZ = 142;
	KeyFrame[154].rotRodIzq = 0;
	KeyFrame[154].rotRodDer = 0;
	KeyFrame[154].rotBraIzq = 10;
	KeyFrame[154].rotBraDer = 10;
	KeyFrame[154].rotBraIzqY = 4;
	KeyFrame[154].rotBraDerY = -2;
	KeyFrame[154].giroMonito = 90;

	KeyFrame[155].posX = 90;
	KeyFrame[155].posY = 0;
	KeyFrame[155].posZ = 142;
	KeyFrame[155].rotRodIzq = 0;
	KeyFrame[155].rotRodDer = 0;
	KeyFrame[155].rotBraIzq = 20;
	KeyFrame[155].rotBraDer = 20;
	KeyFrame[155].rotBraIzqY = 9;
	KeyFrame[155].rotBraDerY = -7;
	KeyFrame[155].giroMonito = 90;

	KeyFrame[156].posX = 90;
	KeyFrame[156].posY = 0;
	KeyFrame[156].posZ = 142;
	KeyFrame[156].rotRodIzq = 0;
	KeyFrame[156].rotRodDer = 0;
	KeyFrame[156].rotBraIzq = 30;
	KeyFrame[156].rotBraDer = 30;
	KeyFrame[156].rotBraIzqY = 14;
	KeyFrame[156].rotBraDerY = -12;
	KeyFrame[156].giroMonito = 90;

	KeyFrame[157].posX = 90;
	KeyFrame[157].posY = 0;
	KeyFrame[157].posZ = 142;
	KeyFrame[157].rotRodIzq = 0;
	KeyFrame[157].rotRodDer = 0;
	KeyFrame[157].rotBraIzq = 45;
	KeyFrame[157].rotBraDer = 45;
	KeyFrame[157].rotBraIzqY = 19;
	KeyFrame[157].rotBraDerY = -17;
	KeyFrame[157].giroMonito = 90;

	KeyFrame[158].posX = 90;
	KeyFrame[158].posY = 0;
	KeyFrame[158].posZ = 142;
	KeyFrame[158].rotRodIzq = 0;
	KeyFrame[158].rotRodDer = 0;
	KeyFrame[158].rotBraIzq = 60;
	KeyFrame[158].rotBraDer = 60;
	KeyFrame[158].rotBraIzqY = 24;
	KeyFrame[158].rotBraDerY = -22;
	KeyFrame[158].giroMonito = 90;

	KeyFrame[159].posX = 90;
	KeyFrame[159].posY = 0;
	KeyFrame[159].posZ = 142;
	KeyFrame[159].rotRodIzq = 0;
	KeyFrame[159].rotRodDer = 0;
	KeyFrame[159].rotBraIzq = 70;
	KeyFrame[159].rotBraDer = 70;
	KeyFrame[159].rotBraIzqY = 29;
	KeyFrame[159].rotBraDerY = -27;
	KeyFrame[159].giroMonito = 90;

	KeyFrame[160].posX = 90;
	KeyFrame[160].posY = 0;
	KeyFrame[160].posZ = 142;
	KeyFrame[160].rotRodIzq = 0;
	KeyFrame[160].rotRodDer = 0;
	KeyFrame[160].rotBraIzq = 80;
	KeyFrame[160].rotBraDer = 80;
	KeyFrame[160].rotBraIzqY = 32;
	KeyFrame[160].rotBraDerY = -30;
	KeyFrame[160].giroMonito = 90;

	KeyFrame[161].posX = 90;
	KeyFrame[161].posY = 0;
	KeyFrame[161].posZ = 142;
	KeyFrame[161].rotRodIzq = 0;
	KeyFrame[161].rotRodDer = 0;
	KeyFrame[161].rotBraIzq = 90;
	KeyFrame[161].rotBraDer = 90;
	KeyFrame[161].rotBraIzqY = 34;
	KeyFrame[161].rotBraDerY = -32;
	KeyFrame[161].giroMonito = 90;
	KeyFrame[161].rotCerveza = 0;
	KeyFrame[161].posCervezaX = 0;
	KeyFrame[161].posCervezaY = 0;
	KeyFrame[161].posCervezaZ = 0;

	KeyFrame[162].posX = 90;
	KeyFrame[162].posY = 0;
	KeyFrame[162].posZ = 142;
	KeyFrame[162].rotRodIzq = 0;
	KeyFrame[162].rotRodDer = 0;
	KeyFrame[162].rotBraIzq = 105;
	KeyFrame[162].rotBraDer = 105;
	KeyFrame[162].rotBraIzqY = 34;
	KeyFrame[162].rotBraDerY = -32;
	KeyFrame[162].giroMonito = 90;
	KeyFrame[162].rotCerveza = -10;
	KeyFrame[162].posCervezaX = -0.02;
	KeyFrame[162].posCervezaY = 1;
	KeyFrame[162].posCervezaZ = 0;

	KeyFrame[163].posX = 90;
	KeyFrame[163].posY = 0;
	KeyFrame[163].posZ = 142;
	KeyFrame[163].rotRodIzq = 0;
	KeyFrame[163].rotRodDer = 0;
	KeyFrame[163].rotBraIzq = 115;
	KeyFrame[163].rotBraDer = 115;
	KeyFrame[163].rotBraIzqY = 34;
	KeyFrame[163].rotBraDerY = -32;
	KeyFrame[163].giroMonito = 90;
	KeyFrame[163].rotCerveza = -20;
	KeyFrame[163].posCervezaX = -0.04;
	KeyFrame[163].posCervezaY = 2.5;
	KeyFrame[163].posCervezaZ = 0;

	KeyFrame[164].posX = 90;
	KeyFrame[164].posY = 0;
	KeyFrame[164].posZ = 142;
	KeyFrame[164].rotRodIzq = 0;
	KeyFrame[164].rotRodDer = 0;
	KeyFrame[164].rotBraIzq = 135;
	KeyFrame[164].rotBraDer = 135;
	KeyFrame[164].rotBraIzqY = 28;
	KeyFrame[164].rotBraDerY = -25;
	KeyFrame[164].giroMonito = 90;
	KeyFrame[164].rotCerveza = -30;
	KeyFrame[164].posCervezaX = -0.06;
	KeyFrame[164].posCervezaY = 3.5;
	KeyFrame[164].posCervezaZ = 0;

	KeyFrame[165].posX = 90;
	KeyFrame[165].posY = 0;
	KeyFrame[165].posZ = 142;
	KeyFrame[165].rotRodIzq = 0;
	KeyFrame[165].rotRodDer = 0;
	KeyFrame[165].rotBraIzq = 115;
	KeyFrame[165].rotBraDer = 115;
	KeyFrame[165].rotBraIzqY = 34;
	KeyFrame[165].rotBraDerY = -32;
	KeyFrame[165].giroMonito = 90;
	KeyFrame[165].rotCerveza = -20;
	KeyFrame[165].posCervezaX = -0.04;
	KeyFrame[165].posCervezaY = 2.5;
	KeyFrame[165].posCervezaZ = 0;

	KeyFrame[166].posX = 90;
	KeyFrame[166].posY = 0;
	KeyFrame[166].posZ = 142;
	KeyFrame[166].rotRodIzq = 0;
	KeyFrame[166].rotRodDer = 0;
	KeyFrame[166].rotBraIzq = 105;
	KeyFrame[166].rotBraDer = 105;
	KeyFrame[166].rotBraIzqY = 34;
	KeyFrame[166].rotBraDerY = -32;
	KeyFrame[166].giroMonito = 90;
	KeyFrame[166].rotCerveza = -10;
	KeyFrame[166].posCervezaX = -0.02;
	KeyFrame[166].posCervezaY = 1;
	KeyFrame[166].posCervezaZ = 0;

	KeyFrame[167].posX = 90;
	KeyFrame[167].posY = 0;
	KeyFrame[167].posZ = 142;
	KeyFrame[167].rotRodIzq = 0;
	KeyFrame[167].rotRodDer = 0;
	KeyFrame[167].rotBraIzq = 90;
	KeyFrame[167].rotBraDer = 90;
	KeyFrame[167].rotBraIzqY = 34;
	KeyFrame[167].rotBraDerY = -32;
	KeyFrame[167].giroMonito = 90;
	KeyFrame[167].rotCerveza = 0;
	KeyFrame[167].posCervezaX = 0;
	KeyFrame[167].posCervezaY = 0;
	KeyFrame[167].posCervezaZ = 0;

	KeyFrame[168].posX = 90;
	KeyFrame[168].posY = 0;
	KeyFrame[168].posZ = 142;
	KeyFrame[168].rotRodIzq = 0;
	KeyFrame[168].rotRodDer = 0;
	KeyFrame[168].rotBraIzq = 105;
	KeyFrame[168].rotBraDer = 105;
	KeyFrame[168].rotBraIzqY = 34;
	KeyFrame[168].rotBraDerY = -32;
	KeyFrame[168].giroMonito = 90;
	KeyFrame[168].rotCerveza = -10;
	KeyFrame[168].posCervezaX = -0.02;
	KeyFrame[168].posCervezaY = 1;
	KeyFrame[168].posCervezaZ = 0;

	KeyFrame[169].posX = 90;
	KeyFrame[169].posY = 0;
	KeyFrame[169].posZ = 142;
	KeyFrame[169].rotRodIzq = 0;
	KeyFrame[169].rotRodDer = 0;
	KeyFrame[169].rotBraIzq = 115;
	KeyFrame[169].rotBraDer = 115;
	KeyFrame[169].rotBraIzqY = 34;
	KeyFrame[169].rotBraDerY = -32;
	KeyFrame[169].giroMonito = 90;
	KeyFrame[169].rotCerveza = -20;
	KeyFrame[169].posCervezaX = -0.04;
	KeyFrame[169].posCervezaY = 2.5;
	KeyFrame[169].posCervezaZ = 0;

	KeyFrame[170].posX = 90;
	KeyFrame[170].posY = 0;
	KeyFrame[170].posZ = 142;
	KeyFrame[170].rotRodIzq = 0;
	KeyFrame[170].rotRodDer = 0;
	KeyFrame[170].rotBraIzq = 135;
	KeyFrame[170].rotBraDer = 135;
	KeyFrame[170].rotBraIzqY = 28;
	KeyFrame[170].rotBraDerY = -25;
	KeyFrame[170].giroMonito = 90;
	KeyFrame[170].rotCerveza = -30;
	KeyFrame[170].posCervezaX = -0.06;
	KeyFrame[170].posCervezaY = 3.5;
	KeyFrame[170].posCervezaZ = 0;

	KeyFrame[171].posX = 90;
	KeyFrame[171].posY = 0;
	KeyFrame[171].posZ = 142;
	KeyFrame[171].rotRodIzq = 0;
	KeyFrame[171].rotRodDer = 0;
	KeyFrame[171].rotBraIzq = 115;
	KeyFrame[171].rotBraDer = 115;
	KeyFrame[171].rotBraIzqY = 34;
	KeyFrame[171].rotBraDerY = -32;
	KeyFrame[171].giroMonito = 90;
	KeyFrame[171].rotCerveza = -20;
	KeyFrame[171].posCervezaX = -0.04;
	KeyFrame[171].posCervezaY = 2.5;
	KeyFrame[171].posCervezaZ = 0;

	KeyFrame[172].posX = 90;
	KeyFrame[172].posY = 0;
	KeyFrame[172].posZ = 142;
	KeyFrame[172].rotRodIzq = 0;
	KeyFrame[172].rotRodDer = 0;
	KeyFrame[172].rotBraIzq = 105;
	KeyFrame[172].rotBraDer = 105;
	KeyFrame[172].rotBraIzqY = 34;
	KeyFrame[172].rotBraDerY = -32;
	KeyFrame[172].giroMonito = 90;
	KeyFrame[172].rotCerveza = -10;
	KeyFrame[172].posCervezaX = -0.02;
	KeyFrame[172].posCervezaY = 1;
	KeyFrame[172].posCervezaZ = 0;

	KeyFrame[173].posX = 90;
	KeyFrame[173].posY = 0;
	KeyFrame[173].posZ = 142;
	KeyFrame[173].rotRodIzq = 0;
	KeyFrame[173].rotRodDer = 0;
	KeyFrame[173].rotBraIzq = 90;
	KeyFrame[173].rotBraDer = 90;
	KeyFrame[173].rotBraIzqY = 34;
	KeyFrame[173].rotBraDerY = -32;
	KeyFrame[173].giroMonito = 90;
	KeyFrame[173].rotCerveza = 0;
	KeyFrame[173].posCervezaX = 0;
	KeyFrame[173].posCervezaY = 0;
	KeyFrame[173].posCervezaZ = 0;

	KeyFrame[174].posX = 90;
	KeyFrame[174].posY = 0;
	KeyFrame[174].posZ = 142;
	KeyFrame[174].rotRodIzq = 0;
	KeyFrame[174].rotRodDer = 0;
	KeyFrame[174].rotBraIzq = 105;
	KeyFrame[174].rotBraDer = 105;
	KeyFrame[174].rotBraIzqY = 34;
	KeyFrame[174].rotBraDerY = -32;
	KeyFrame[174].giroMonito = 90;
	KeyFrame[174].rotCerveza = -10;
	KeyFrame[174].posCervezaX = -0.01;
	KeyFrame[174].posCervezaY = 3.0;
	KeyFrame[174].posCervezaZ = 0;

	KeyFrame[175].posX = 90;
	KeyFrame[175].posY = 0;
	KeyFrame[175].posZ = 142;
	KeyFrame[175].rotRodIzq = 0;
	KeyFrame[175].rotRodDer = 0;
	KeyFrame[175].rotBraIzq = 115;
	KeyFrame[175].rotBraDer = 115;
	KeyFrame[175].rotBraIzqY = 34;
	KeyFrame[175].rotBraDerY = -32;
	KeyFrame[175].giroMonito = 90;
	KeyFrame[175].rotCerveza = -30;
	KeyFrame[175].posCervezaX = -0.02;
	KeyFrame[175].posCervezaY = 5.0;
	KeyFrame[175].posCervezaZ = 0;

	KeyFrame[176].posX = 90;
	KeyFrame[176].posY = 0;
	KeyFrame[176].posZ = 142;
	KeyFrame[176].rotRodIzq = 0;
	KeyFrame[176].rotRodDer = 0;
	KeyFrame[176].rotBraIzq = 135;
	KeyFrame[176].rotBraDer = 135;
	KeyFrame[176].rotBraIzqY = 28;
	KeyFrame[176].rotBraDerY = -25;
	KeyFrame[176].giroMonito = 90;
	KeyFrame[176].rotCerveza = -115;
	KeyFrame[176].posCervezaX = 1.0;
	KeyFrame[176].posCervezaY = 9.0;
	KeyFrame[176].posCervezaZ = 0;
	KeyFrame[176].posCabeza = -0.8;
	KeyFrame[176].rotCabeza = 30;

	KeyFrame[177].posX = 90;
	KeyFrame[177].posY = 0;
	KeyFrame[177].posZ = 142;
	KeyFrame[177].rotRodIzq = 0;
	KeyFrame[177].rotRodDer = 0;
	KeyFrame[177].rotBraIzq = 135;
	KeyFrame[177].rotBraDer = 135;
	KeyFrame[177].rotBraIzqY = 28;
	KeyFrame[177].rotBraDerY = -25;
	KeyFrame[177].giroMonito = 90;
	KeyFrame[177].rotCerveza = -115;
	KeyFrame[177].posCervezaX = 1.0;
	KeyFrame[177].posCervezaY = 9.0;
	KeyFrame[177].posCervezaZ = 0;
	KeyFrame[177].posCabeza = -0.8;
	KeyFrame[177].rotCabeza = 30;

	KeyFrame[178].posX = 90;
	KeyFrame[178].posY = 0;
	KeyFrame[178].posZ = 142;
	KeyFrame[178].rotRodIzq = 0;
	KeyFrame[178].rotRodDer = 0;
	KeyFrame[178].rotBraIzq = 135;
	KeyFrame[178].rotBraDer = 135;
	KeyFrame[178].rotBraIzqY = 28;
	KeyFrame[178].rotBraDerY = -25;
	KeyFrame[178].giroMonito = 90;
	KeyFrame[178].rotCerveza = -115;
	KeyFrame[178].posCervezaX = 1.0;
	KeyFrame[178].posCervezaY = 9.0;
	KeyFrame[178].posCervezaZ = 0;
	KeyFrame[178].posCabeza = -0.8;
	KeyFrame[178].rotCabeza = 30;

	KeyFrame[179].posX = 90;
	KeyFrame[179].posY = 0;
	KeyFrame[179].posZ = 142;
	KeyFrame[179].rotRodIzq = 0;
	KeyFrame[179].rotRodDer = 0;
	KeyFrame[179].rotBraIzq = 135;
	KeyFrame[179].rotBraDer = 135;
	KeyFrame[179].rotBraIzqY = 28;
	KeyFrame[179].rotBraDerY = -25;
	KeyFrame[179].giroMonito = 90;
	KeyFrame[179].rotCerveza = -115;
	KeyFrame[179].posCervezaX = 1.0;
	KeyFrame[179].posCervezaY = 9.0;
	KeyFrame[179].posCervezaZ = 0;
	KeyFrame[179].posCabeza = -0.8;
	KeyFrame[179].rotCabeza = 30;


	KeyFrame[180].posX = 90;
	KeyFrame[180].posY = 0;
	KeyFrame[180].posZ = 142;
	KeyFrame[180].rotRodIzq = 0;
	KeyFrame[180].rotRodDer = 0;
	KeyFrame[180].rotBraIzq = 115;
	KeyFrame[180].rotBraDer = 115;
	KeyFrame[180].rotBraIzqY = 34;
	KeyFrame[180].rotBraDerY = -32;
	KeyFrame[180].giroMonito = 90;
	KeyFrame[180].rotCerveza = -20;
	KeyFrame[180].posCervezaX = -0.04;
	KeyFrame[180].posCervezaY = 2.5;
	KeyFrame[180].posCervezaZ = 0;

	KeyFrame[181].posX = 90;
	KeyFrame[181].posY = 0;
	KeyFrame[181].posZ = 142;
	KeyFrame[181].rotRodIzq = 0;
	KeyFrame[181].rotRodDer = 0;
	KeyFrame[181].rotBraIzq = 105;
	KeyFrame[181].rotBraDer = 105;
	KeyFrame[181].rotBraIzqY = 34;
	KeyFrame[181].rotBraDerY = -32;
	KeyFrame[181].giroMonito = 90;
	KeyFrame[181].rotCerveza = -10;
	KeyFrame[181].posCervezaX = -0.02;
	KeyFrame[181].posCervezaY = 1;
	KeyFrame[181].posCervezaZ = 0;


	KeyFrame[182].posX = 90;
	KeyFrame[182].posY = 0;
	KeyFrame[182].posZ = 142;
	KeyFrame[182].rotRodIzq = 0;
	KeyFrame[182].rotRodDer = 0;
	KeyFrame[182].rotBraIzq = 90;
	KeyFrame[182].rotBraDer = 90;
	KeyFrame[182].rotBraIzqY = 34;
	KeyFrame[182].rotBraDerY = -32;
	KeyFrame[182].giroMonito = 90;
	KeyFrame[182].rotCerveza = 0;
	KeyFrame[182].posCervezaX = 0;
	KeyFrame[182].posCervezaY = 0;
	KeyFrame[182].posCervezaZ = 0;

	KeyFrame[183].posX = 90;
	KeyFrame[183].posY = 0;
	KeyFrame[183].posZ = 142;
	KeyFrame[183].rotRodIzq = 0;
	KeyFrame[183].rotRodDer = 0;
	KeyFrame[183].rotBraIzq = 90;
	KeyFrame[183].rotBraDer = 105;
	KeyFrame[183].rotBraIzqY = 34;
	KeyFrame[183].rotBraDerY = -32;
	KeyFrame[183].giroMonito = 90;
	KeyFrame[183].rotCerveza = -10;
	KeyFrame[183].posCervezaX = -0.01;
	KeyFrame[183].posCervezaY = 3.0;
	KeyFrame[183].posCervezaZ = 0;

	KeyFrame[184].posX = 90;
	KeyFrame[184].posY = 0;
	KeyFrame[184].posZ = 142;
	KeyFrame[184].rotRodIzq = 0;
	KeyFrame[184].rotRodDer = 0;
	KeyFrame[184].rotBraIzq = 90;
	KeyFrame[184].rotBraDer = 115;
	KeyFrame[184].rotBraIzqY = 34;
	KeyFrame[184].rotBraDerY = -32;
	KeyFrame[184].giroMonito = 90;
	KeyFrame[184].rotCerveza = -30;
	KeyFrame[184].posCervezaX = -0.02;
	KeyFrame[184].posCervezaY = 5.0;
	KeyFrame[184].posCervezaZ = 0;

	KeyFrame[185].posX = 90;
	KeyFrame[185].posY = 0;
	KeyFrame[185].posZ = 142;
	KeyFrame[185].rotRodIzq = 0;
	KeyFrame[185].rotRodDer = 0;
	KeyFrame[185].rotBraIzq = 90;
	KeyFrame[185].rotBraDer = 135;
	KeyFrame[185].rotBraIzqY = 28;
	KeyFrame[185].rotBraDerY = -25;
	KeyFrame[185].giroMonito = 90;
	KeyFrame[185].rotCerveza = -30;
	KeyFrame[185].posCervezaX = 0.1;
	KeyFrame[185].posCervezaY = 7.0;
	KeyFrame[185].posCervezaZ = 0;
	KeyFrame[185].posCabeza = 0;
	KeyFrame[185].rotCabeza = 0;


	KeyFrame[186].posX = 90;
	KeyFrame[186].posY = 0;
	KeyFrame[186].posZ = 142;
	KeyFrame[186].rotRodIzq = 0;
	KeyFrame[186].rotRodDer = 0;
	KeyFrame[186].rotBraIzq = 90;
	KeyFrame[186].rotBraDer = 115;
	KeyFrame[186].rotBraIzqY = 34;
	KeyFrame[186].rotBraDerY = -32;
	KeyFrame[186].giroMonito = 90;
	KeyFrame[186].rotCerveza = -20;
	KeyFrame[186].posCervezaX = 10;
	KeyFrame[186].posCervezaY = 8;
	KeyFrame[186].posCervezaZ = 0;

	KeyFrame[187].posX = 90;
	KeyFrame[187].posY = 0;
	KeyFrame[187].posZ = 142;
	KeyFrame[187].rotRodIzq = 0;
	KeyFrame[187].rotRodDer = 0;
	KeyFrame[187].rotBraIzq = 90;
	KeyFrame[187].rotBraDer = 105;
	KeyFrame[187].rotBraIzqY = 34;
	KeyFrame[187].rotBraDerY = -32;
	KeyFrame[187].giroMonito = 90;
	KeyFrame[187].rotCerveza = -10;
	KeyFrame[187].posCervezaX = 20;
	KeyFrame[187].posCervezaY = 14;
	KeyFrame[187].posCervezaZ = 0;

	KeyFrame[188].posX = 90;
	KeyFrame[188].posY = 0;
	KeyFrame[188].posZ = 142;
	KeyFrame[188].rotRodIzq = 0;
	KeyFrame[188].rotRodDer = 0;
	KeyFrame[188].rotBraIzq = 90;
	KeyFrame[188].rotBraDer = 90;
	KeyFrame[188].rotBraIzqY = 34;
	KeyFrame[188].rotBraDerY = -32;
	KeyFrame[188].giroMonito = 90;
	KeyFrame[188].rotCerveza = 30;
	KeyFrame[188].posCervezaX = 27;
	KeyFrame[188].posCervezaY = 17;
	KeyFrame[188].posCervezaZ = 0;

	KeyFrame[189].posX = 90;
	KeyFrame[189].posY = 0;
	KeyFrame[189].posZ = 142;
	KeyFrame[189].rotRodIzq = 0;
	KeyFrame[189].rotRodDer = 0;
	KeyFrame[189].rotBraIzq = 90;
	KeyFrame[189].rotBraDer = 90;
	KeyFrame[189].rotBraIzqY = 34;
	KeyFrame[189].rotBraDerY = -32;
	KeyFrame[189].giroMonito = 90;
	KeyFrame[189].rotCerveza = 60;
	KeyFrame[189].posCervezaX = 30;
	KeyFrame[189].posCervezaY = 17;
	KeyFrame[189].posCervezaZ = 0;

	KeyFrame[190].posX = 90;
	KeyFrame[190].posY = 0;
	KeyFrame[190].posZ = 142;
	KeyFrame[190].rotRodIzq = 0;
	KeyFrame[190].rotRodDer = 0;
	KeyFrame[190].rotBraIzq = 90;
	KeyFrame[190].rotBraDer = 90;
	KeyFrame[190].rotBraIzqY = 34;
	KeyFrame[190].rotBraDerY = -32;
	KeyFrame[190].giroMonito = 90;
	KeyFrame[190].rotCerveza = 90;
	KeyFrame[190].posCervezaX = 40;
	KeyFrame[190].posCervezaY = 15;
	KeyFrame[190].posCervezaZ = 0;

	KeyFrame[191].posX = 90;
	KeyFrame[191].posY = 0;
	KeyFrame[191].posZ = 142;
	KeyFrame[191].rotRodIzq = 0;
	KeyFrame[191].rotRodDer = 0;
	KeyFrame[191].rotBraIzq = 90;
	KeyFrame[191].rotBraDer = 90;
	KeyFrame[191].rotBraIzqY = 34;
	KeyFrame[191].rotBraDerY = -32;
	KeyFrame[191].giroMonito = 90;
	KeyFrame[191].rotCerveza = 130;
	KeyFrame[191].posCervezaX = 60;
	KeyFrame[191].posCervezaY = 10;
	KeyFrame[191].posCervezaZ = 0;

	KeyFrame[192].posX = 90;
	KeyFrame[192].posY = 0;
	KeyFrame[192].posZ = 142;
	KeyFrame[192].rotRodIzq = 0;
	KeyFrame[192].rotRodDer = 0;
	KeyFrame[192].rotBraIzq = 90;
	KeyFrame[192].rotBraDer = 90;
	KeyFrame[192].rotBraIzqY = 34;
	KeyFrame[192].rotBraDerY = -32;
	KeyFrame[192].giroMonito = 90;
	KeyFrame[192].rotCerveza = 150;
	KeyFrame[192].posCervezaX = 70;
	KeyFrame[192].posCervezaY = 5;
	KeyFrame[192].posCervezaZ = 0;

	KeyFrame[193].posX = 90;
	KeyFrame[193].posY = 0;
	KeyFrame[193].posZ = 142;
	KeyFrame[193].rotRodIzq = 0;
	KeyFrame[193].rotRodDer = 0;
	KeyFrame[193].rotBraIzq = 90;
	KeyFrame[193].rotBraDer = 90;
	KeyFrame[193].rotBraIzqY = 34;
	KeyFrame[193].rotBraDerY = -32;
	KeyFrame[193].giroMonito = 90;
	KeyFrame[193].rotCerveza = 180;
	KeyFrame[193].posCervezaX = 80;
	KeyFrame[193].posCervezaY = 0;
	KeyFrame[193].posCervezaZ = 0;

	KeyFrame[194].posX = 90;
	KeyFrame[194].posY = 0;
	KeyFrame[194].posZ = 142;
	KeyFrame[194].rotRodIzq = 0;
	KeyFrame[194].rotRodDer = 0;
	KeyFrame[194].rotBraIzq = 90;
	KeyFrame[194].rotBraDer = 90;
	KeyFrame[194].rotBraIzqY = 34;
	KeyFrame[194].rotBraDerY = -32;
	KeyFrame[194].giroMonito = 90;
	KeyFrame[194].rotCerveza = 220;
	KeyFrame[194].posCervezaX = 90;
	KeyFrame[194].posCervezaY = -4;
	KeyFrame[194].posCervezaZ = 0;

	KeyFrame[195].posX = 90;
	KeyFrame[195].posY = 0;
	KeyFrame[195].posZ = 142;
	KeyFrame[195].rotRodIzq = 0;
	KeyFrame[195].rotRodDer = 0;
	KeyFrame[195].rotBraIzq = 90;
	KeyFrame[195].rotBraDer = 90;
	KeyFrame[195].rotBraIzqY = 34;
	KeyFrame[195].rotBraDerY = -32;
	KeyFrame[195].giroMonito = 90;
	KeyFrame[195].rotCerveza = 270;
	KeyFrame[195].posCervezaX = 100;
	KeyFrame[195].posCervezaY = -6;
	KeyFrame[195].posCervezaZ = 0;
	//---------TERMINAN KEYFRAMES HOMERO CERVEZA
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

		/*staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		*/
		if (dia) {
			staticShader.setVec3("dirLight.direction", lightDirection);
			staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			staticShader.setVec3("dirLight.direction", lightDirection);
			staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
			staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		}
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
		
		//Homero
		//---------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 12.0f, -150.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		torso1.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(1.0f, -3.0f, -0.5f));
		//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		piernaIzq1.Draw(staticShader);

		//Pierna izq
		model = glm::translate(tmp, glm::vec3(-1.3f, -3.0f, -0.5f));
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		piernaIzq1.Draw(staticShader);


		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(1.6f, 0.95f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraDerY), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		brazoDer1.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(-1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraIzqY), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		brazoIzq1.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, posCabeza));
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		cabeza1.Draw(staticShader);

		//cerveza
		model = glm::translate(glm::mat4(1.0f), glm::vec3(95.0f, 8.5f, -8.0f));
		model = glm::translate(model, glm::vec3(posCervezaX, posCervezaY, posCervezaZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotCerveza), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cerveza.Draw(staticShader);


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
		//----------Modelos jardin globo//
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


		//Aspiradora//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(80.0, 4.5, 70.0));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(3));
		staticShader.setMat4("model", model);
		aspiradora.Draw(staticShader);

		//UFO//
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-200.0 + posUFOX, 0.0 + posUFOY, -160.0 + posUFOZ));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::translate(model, glm::vec3(0.0 + posUFOX, 0.0 + posUFOY, 0.0 + posUFOZ));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
		model = glm::rotate(model, glm::radians(girpollo), glm::vec3(0.0f, 1.0f, 0.0)); //giro natural del ovni
		model = glm::scale(model, glm::vec3(1));
		staticShader.setMat4("model", model);
		ufo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		if (dia) {
			skyboxShader.use();
			skybox.Draw(skyboxShader, view, projection, camera);
		}
		else {
			skyboxShader.use();
			skybox2.Draw(skyboxShader, view, projection, camera);
		}
		
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
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion ^= true;
		dia = false;
	}

	if (key == GLFW_KEY_U && action == GLFW_PRESS)
		animOvni ^= true;
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