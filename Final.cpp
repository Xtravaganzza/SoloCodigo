/*---------------------------------------------------------*/
/* ----------------  PROYECTO FINAL             -----------*/
/*-----------------    2022-1   ---------------------------*/
/*------------- Alumno: MORENO DEL CASTILLO ARACELI -------*/
/*------------- No. Cuenta 302053300        ---------------*/
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

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;

//VARIABLES ANIMACION TREN
float	movTren_x = 0.0f,
		movTren_y = 0.0f,
		movTren_z = 0.0f,
		movCampana = 0.0f,
		movBarra = 0.0f,
		giroRuedas = 0.0f,
		orientaTren = 0.0f;

bool	animacionTren = false,
		recorridoTren1 = true,
		recorridoTren2 = false,
		recorridoTren3 = false,
		recorridoTren4 = false,
		recorridoTren5 = false,
		recorridoTren6 = false,
		recorridoTren7 = false;

//VARIABLES ANIMACION LEGO
float	movLego_x = 0.0f,
		movLego_y = 0.0f,
		movLego_z = 0.0f,
		movPieDer = 0.0f,
		movPieIzq = 0.0f,
		movBrazoDer = 0.0f,
		movBrazoIzq = 0.0f,
		movCabeza = 0.0f,
		movTorzo = 0.0f,
		orientaLego = 0.0f;

bool	animacionLego = false,
		recorridoLego1 = true;

//VARIABLES ANIMACION LEGO2
float	movLego2_x = 0.0f,
		movLego2_y = 0.0f,
		movLego2_z = 0.0f,
		movPieDer2 = 0.0f,
		movPieIzq2 = 0.0f,
		movBrazoDer2 = 0.0f,
		movBrazoIzq2 = 0.0f,
		movCabeza2 = 0.0f,
		movTorzo2 = 0.0f,
		orientaLego2 = 0.0f;

bool	animacionLego2 = false,
		recorrido2Lego1 = true,
		recorrido2Lego2 = false,
		recorrido2Lego3 = false,
		recorrido2Lego4 = false,
		recorrido2Lego5 = false,
		recorrido2Lego6 = false,
		recorrido2Lego7 = false,
		recorrido2Lego8 = false;

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

	//////////////////ANIMACIONES PROY
	//ANIMACION TREN
	if (animacionTren)
	{
		if (recorridoTren1)
		{
			movTren_x += 1.0f;
			orientaTren = 0.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas += 1.0f;
			if (movTren_x == 150.0f)
			{
				recorridoTren1 = false;
				recorridoTren2 = true;
			}
		}
		if (recorridoTren2)
		{
			movTren_z += 1.0f;
			//orientaTren = -90.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas = 1.0f;
			if (movTren_z == 150.0f)
			{
				recorridoTren2 = false;
				recorridoTren3 = true;
			}
		}
		if (recorridoTren3)
		{
			movTren_x -= 1.0f;
			//orientaTren = 180.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas = 1.0f;
			if (movTren_z == 0.0f)
			{
				recorridoTren3 = false;
				recorridoTren4 = true;
			}
		}
		if (recorridoTren4)
		{
			movTren_z -= 1.0f;
			//orientaTren = 90.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas = 1.0f;
			if (movTren_z == 0.0f)
			{
				recorridoTren4 = false;
				recorridoTren5 = true;
			}
		}
		if (recorridoTren5)
		{
			movTren_x += 1.5f;
			movTren_z += 1.5f;
			//orientaTren = -45.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas = 1.0f;
			if (movTren_x >= 150.0f && movTren_z >= 150.0f)
			{
				recorridoTren5 = false;
				recorridoTren6 = true;
			}
		}
		if (recorridoTren6)
		{
			movTren_x -= 1.0f;
			//orientaTren = 180.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas = 1.0f;
			if (movTren_z = 0.0f)
			{
				recorridoTren6 = false;
				recorridoTren7 = true;
			}
		}
		if (recorridoTren7)
		{
			movTren_z -= 1.0f;
			//orientaTren = 90.0f;
			//cout << "Posicion x: " << movTren_x << endl;
			//cout << "Posicion y: " << movTren_y << endl;
			//cout << "Posicion z: " << movTren_z << endl;
			//giroRuedas = 1.0f;
			if (movTren_z = 0.0f)
			{
				recorridoTren7 = false;
				animacionTren = false;
			}
		}
	}

	if (animacionLego) 
	{
		if (recorridoLego1) 
		{
			movLego_z += 1.0f;
			//cout << "Posicion x: " << movLego_x << endl; 
			//cout << "Posicion y: " << movLego_y << endl;
			//cout << "Posicion z: " << movLego_z << endl;
			movPieDer -= 5.0f;
			movPieIzq += 5.0f;
			movBrazoDer += 5.0;
			movBrazoIzq -= 5.0f;
			if (movPieDer == -40.0f && movPieIzq == 40.f && movBrazoDer == 40.f && movBrazoIzq == -40.0f) 
			{
				movPieDer += 5.0f;
			}
			else if (movPieDer == 40.0f && movPieIzq == -40.f && movBrazoDer == -40.f && movBrazoIzq == 40.0f)
			{
				movPieDer -= 5.0;
			}

			if (movLego_z == 200.0f)
			{
				recorridoLego1 = false;
				movPieDer = 0.0f;
				movPieIzq = 0.0f;
				movBrazoDer = 0.0f;
				movBrazoIzq = 0.0f;
				animacionLego = false;
			}
		}
	}

	if (animacionLego2)
	{
		if (recorrido2Lego1)
		{
			movBrazoDer2 -= 3.0f;
			if (movBrazoDer2 == -60.f)
			{
				recorrido2Lego1 = false;
				recorrido2Lego2 = true;
			}
		}
		if (recorrido2Lego2)
		{
			movBrazoIzq2 -= 3.0f;
			if (movBrazoIzq2 == -60.f)
			{
				recorrido2Lego2 = false;
				recorrido2Lego3 = true;
			}
		}
		if (recorrido2Lego3)
		{
			movBrazoDer2 -= 3.0f;
			if (movBrazoDer2 == -120.f)
			{
				recorrido2Lego3 = false;
				recorrido2Lego4 = true;
			}
		}
		if (recorrido2Lego4)
		{
			movBrazoIzq2 -= 3.0f;
			if (movBrazoIzq2 == -120.f)
			{
				recorrido2Lego4 = false;
				recorrido2Lego5 = true;
			}
		}
		if (recorrido2Lego5)
		{
			movBrazoDer2 += 3.0f;
			if (movBrazoDer2 == -60.f)
			{
				recorrido2Lego5 = false;
				recorrido2Lego6 = true;
			}
		}
		if (recorrido2Lego6)
		{
			movBrazoIzq2 += 3.0f;
			if (movBrazoIzq2 == -60.f)
			{
				recorrido2Lego6 = false;
				recorrido2Lego7 = true;
			}
		}
		if (recorrido2Lego7)
		{
			movBrazoDer2 += 3.0f;
			if (movBrazoDer2 == 40.f)
			{
				recorrido2Lego7 = false;
				recorrido2Lego8 = true;
			}
		}
		if (recorrido2Lego8)
		{
			movBrazoIzq2 += 3.0f;
			if (movBrazoIzq2 == 40.f)
			{
				recorrido2Lego6 = false;
				animacionLego2 = false;
			}
		}

	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
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
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

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

	// CONFIGURACION DEL SHADER
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// CARGA DE MODELOS
	// -----------
	//****** BLOQUE 1 (PISO CASA)
	Model piso("resources/objects/piso/piso.obj");
	Model casa("resources/objects/Casa/CasaFinal.obj");

	//****** BLOQUE 2 (SALA-COMEDOR-COCINA)
	//Model sala("resources/objects/Casa/SalaFinal.obj");
	//Model comedor("resources/objects/Casa/ComedorFinal.obj");
	//Model arbol("resources/objects/Casa/ArbolFinal.obj"); 
	//Model cocina("resources/objects/Casa/CocinaFinal.obj");
	//Model cocinadeta("resources/objects/Casa/CocinaDetaFinal.obj");

	//****** BLOQUE 3 (RECAMARA, BAÑO)
	//Model cuartoPrinci("resources/objects/Casa/CuartoFinal.obj");
	//Model TVcuarto("resources/objects/Casa/TVCuarto.obj");
	//Model banioPb("resources/objects/Casa/BanioPBFinal.obj");

	//****** BLOQUE 4 (CUARTO ENTRETENIMIENTO, BAÑO PA, CUARTO BB)
	//Model cuartoEntrete1("resources/objects/Casa/CuartoEnfinal.obj");
	//Model cuartoEntrete2("resources/objects/Casa/CuartoEn2Final.obj");
	//Model cuartoEntrete3("resources/objects/Casa/CuartoEn3Final.obj");
	//Model banioPa("resources/objects/Casa/BanioPAFinal.obj");
	//Model cuartoBB("resources/objects/Casa/CuartoBBFinal.obj");

	//****** BLOQUE 5 (Exterior)
	Model exterior1("resources/objects/Exterior/Exterior.obj");

	//MODELOS PARA ANIMAR
	//TREN
	Model trenEstructura("resources/objects/Tren/TrenEstructura.obj");
	Model trenCampana("resources/objects/Tren/TrenCampana.obj");
	Model trenBarraI("resources/objects/Tren/TrenBarraIzq.obj");
	Model trenBarraD("resources/objects/Tren/TrenBarraDer.obj");
	Model trenRuedaI1("resources/objects/Tren/TrenRuedaIzq1.obj");
	Model trenRuedaI2("resources/objects/Tren/TrenRuedaIzq2.obj");
	Model trenRuedaI3("resources/objects/Tren/TrenRuedaIzq3.obj");
	Model trenRuedaI4("resources/objects/Tren/TrenRuedaIzq4.obj");
	Model trenRuedaD1("resources/objects/Tren/TrenRuedaDer1.obj");
	Model trenRuedaD2("resources/objects/Tren/TrenRuedaDer2.obj");
	Model trenRuedaD3("resources/objects/Tren/TrenRuedaDer3.obj");
	Model trenRuedaD4("resources/objects/Tren/TrenRuedaDer4.obj");

	//LEGO
	Model legocabeza("resources/objects/Lego/LegoCabeza.obj");
	Model legocuerpo("resources/objects/Lego/LegoCuero.obj");
	Model legobrazoizq("resources/objects/Lego/LegoBrazoIzq.obj");
	Model legobrazoder("resources/objects/Lego/LegoBrazoDer.obj");
	Model legopieizq("resources/objects/Lego/LegoPieIzq.obj");
	Model legopieder("resources/objects/Lego/LegoPieDer.obj");

	//LEG
	Model legocabeza2("resources/objects/Leg/LegCabeza.obj");
	Model legocuerpo2("resources/objects/Leg/LegCuerpo.obj");
	Model legobrazoizq2("resources/objects/Leg/LegBrazoIzq.obj");
	Model legobrazoder2("resources/objects/Leg/LegBrazoDer.obj");
	Model legopieizq2("resources/objects/Leg/LegPieIzq.obj");
	Model legopieder2("resources/objects/Leg/LegPieDer.obj");

	//Reloj
	//Model relojBase("resources/objects/Reloj/RelojBase.obj");
	//Model relojEstruc("resources/objects/Reloj/RelojEstructura.obj");
	//Model relojCamp1("resources/objects/Reloj/RelojCampana1.obj");
	//Model relojCamp2("resources/objects/Reloj/RelojCampana2.obj");
	//Model relojMane1("resources/objects/Reloj/RelojManecilla1.obj");
	//Model relojMane2("resources/objects/Reloj/RelojManecilla2.obj");

	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	//ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	//ninja.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
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
		//Setup Advanced Lights LUCES
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.09f);
		staticShader.setFloat("pointLight[1].quadratic", 0.32f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelTren = glm::mat4(1.0f);
		glm::mat4 modelLeg = glm::mat4(1.0f);
		glm::mat4 tmp1 = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);
		glm::mat4 tmp4 = glm::mat4(1.0f);
		glm::mat4 tmp5 = glm::mat4(1.0f);
		glm::mat4 tmp6 = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

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

		//****** BLOQUE 1 (PISO, CASA)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Elementos Casa
		// -------------------------------------------------------------------------------------------------------------------------
		//****** BLOQUE 1 (PISO, CASA)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		//****** BLOQUE 2 (SALA-COMEDOR-COCINA)
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//sala.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//comedor.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//arbol.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cocina.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cocinadeta.Draw(staticShader);

		//****** BLOQUE 3 (RECAMARA, BAÑO)
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cuartoPrinci.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//TVcuarto.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//banioPb.Draw(staticShader);

		//****** BLOQUE 4 (CUARTO ENTRETENIMIENTO, BAÑO PA, CUARTO BB)
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cuartoEntrete1.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cuartoEntrete2.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cuartoEntrete3.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//banioPa.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//cuartoBB.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Elementos Exteriores
		// -------------------------------------------------------------------------------------------------------------------------
		//****** BLOQUE 5 (Exterior)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		exterior1.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Elementos Animaciones
		// -------------------------------------------------------------------------------------------------------------------------
		//Tren
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movTren_x, movTren_y, movTren_z));
		tmp1 = model = glm::rotate(model, glm::radians(orientaTren), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		trenEstructura.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movCampana), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		trenCampana.Draw(staticShader); //Movimiento Campana en su eje

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBarra), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		trenBarraI.Draw(staticShader); //Movimiento Barra adelante atras

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBarra), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		trenBarraD.Draw(staticShader);   
		
		//Giro de la rueda INICIAN
		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaI1.Draw(staticShader); 

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaI2.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaI3.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaI4.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaD1.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaD2.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaD3.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroRuedas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		trenRuedaD4.Draw(staticShader);

		//Lego
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movLego_x, movLego_y, movLego_z));
		tmp2 = model = glm::rotate(model, glm::radians(orientaLego), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movTorzo), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legocuerpo.Draw(staticShader);

		model = glm::translate(tmp2, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		legocabeza.Draw(staticShader); //Define el movimiento de la cabeza

		model = glm::translate(tmp2, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legobrazoizq.Draw(staticShader); //Define el movimiento de las extremidades

		model = glm::translate(tmp2, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legobrazoder.Draw(staticShader); //Define el movimiento de las extremidades

		model = glm::translate(tmp2, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPieIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legopieizq.Draw(staticShader); //Define el movimiento de las extremidades

		model = glm::translate(tmp2, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPieDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legopieder.Draw(staticShader); //Define el movimiento de las extremidades

		//Lego2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movLego2_x, movLego2_y, movLego2_z));
		tmp3 = model = glm::rotate(model, glm::radians(orientaLego2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movTorzo2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legocuerpo2.Draw(staticShader);

		model = glm::translate(tmp4, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movCabeza2), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		legocabeza2.Draw(staticShader); //Define el movimiento de la cabeza

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBrazoIzq2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legobrazoizq2.Draw(staticShader); //Define el movimiento de las extremidades

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBrazoDer2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legobrazoder2.Draw(staticShader); //Define el movimiento de las extremidades

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPieIzq2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legopieizq2.Draw(staticShader); //Define el movimiento de las extremidades

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPieDer2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		legopieder2.Draw(staticShader);//Define el movimiento de las extremidades

		//Reloj
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//relojBase.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//relojEstruc.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//relojCamp1.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//relojCamp2.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//relojMane1.Draw(staticShader);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		//relojMane2.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
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
	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	//	posZ++;
	//if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	//	posZ--;
	//if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	//	posX--;
	//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	//	posX++;
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	//	rotRodIzq--;
	//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	//	rotRodIzq++;
	//if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	//	giroMonito--;
	//if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	//	giroMonito++;
	//if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	//	lightPosition.x++;
	//if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	//	lightPosition.x--;

	//ACTIVAR ANIMACIONES
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	//	animacion ^= true;
	//Animacion Tren
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		animacionTren ^= true;

	//Regresar a su posicion el tren
	if (key == GLFW_KEY_2 & action == GLFW_PRESS)
	{
		movTren_x = 0.0f;
		movTren_y = 0.0f;
		movTren_z = 0.0f;
		movCampana = 0.0f;
		movBarra = 0.0f;
		giroRuedas = 0.0f;
		orientaTren = 0.0f;
		animacionTren = false;
		recorridoTren1 = true;
		recorridoTren2 = false;
		recorridoTren3 = false;
		recorridoTren4 = false;
		recorridoTren5 = false;
		recorridoTren6 = false;
		recorridoTren7 = false;
	}

	//Animacion Lego
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		animacionLego ^= true;

	//Regresar Lego
	if (key == GLFW_KEY_4 & action == GLFW_PRESS && animacionTren == false)
	{
		movLego_x = 0.0f;
		movLego_y = 0.0f;
		movLego_z = 0.0f;
		movPieDer = 0.0f;
		movPieIzq = 0.0f;
		movBrazoDer = 0.0f;
		movBrazoIzq = 0.0f;
		movCabeza = 0.0f;
		movTorzo = 0.0f;
		orientaLego = 0.0f;
		animacionLego = false;
		recorridoLego1 = true;
	}

	//Animacion Lego2
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		animacionLego2 ^= true;

	//Regresar Lego
	if (key == GLFW_KEY_6 & action == GLFW_PRESS && animacionTren == false)
	{
		movLego2_x = 0.0f;
		movLego2_y = 0.0f;
		movLego2_z = 0.0f;
		movPieDer2 = 0.0f;
		movPieIzq2 = 0.0f;
		movBrazoDer2 = 0.0f;
		movBrazoIzq2 = 0.0f;
		movCabeza2 = 0.0f;
		movTorzo2 = 0.0f;
		orientaLego2 = 0.0f;
		animacionLego2 = false;
		recorrido2Lego1 = true;
		recorrido2Lego2 = false;
		recorrido2Lego3 = false;
		recorrido2Lego4 = false;
		recorrido2Lego5 = false;
		recorrido2Lego6 = false;
		recorrido2Lego7 = false;
		recorrido2Lego8 = false;
	}

	//Animacion Reloj

	//Animacion 

	//Animacion 

	//ANIMAR POR KEY FRAMES
	//if (key == GLFW_KEY_P && action == GLFW_PRESS)
	//{
	//	if (play == false && (FrameIndex > 1))
	//	{
	//		std::cout << "Play animation" << std::endl;
	//		resetElements();
	//		//First Interpolation				
	//		interpolation();

	//		play = true;
	//		playIndex = 0;
	//		i_curr_steps = 0;
	//	}
	//	else
	//	{
	//		play = false;
	//		std::cout << "Not enough Key Frames" << std::endl;
	//	}
	//}

	//SALVAR LOS KEY FRAMES
	//if (key == GLFW_KEY_L && action == GLFW_PRESS)
	//{
	//	if (FrameIndex < MAX_FRAMES)
	//	{
	//		saveFrame();
	//	}
	//}
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