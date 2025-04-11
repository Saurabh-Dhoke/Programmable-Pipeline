// Assignment - 3 moving lights
/******************************************* 
	Three Moving lights on a sphere - Per Fragment ...Per vertex Lighting  - 
	
	Date : 09.04.2024 

	Base Code - 
				Single moving Light 
				two Steady lights
*******************************************/

// Common Header Files
#include<windows.h>
#include<stdio.h> 
#include<stdlib.h> // for exit() 

#include"OGL.h"

// opengl header Files
#include<gl/glew.h> // this must be before gl/GL.h
#include<gl/GL.h>

#include"vmath.h"
using namespace vmath;

// For Sphere
#include"Sphere.h"

// Macrose
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// link with OpenGL library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")     // for Sphere 

// openGl related global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLuint shaderProgramObject_PV  = 0;
GLuint shaderProgramObject_PF  = 0;
char choosenShadder = 'v';    // for toggling of PV - PF

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR ,   
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_NORMAL    
};

GLuint vao = 0;
GLuint vbo_position = 0;
GLuint vbo_element = 0;
GLuint vbo_normal = 0; 
GLuint vbo_texcoord = 0;

// for per vertex 
GLuint modelMatrixUniform_PV = 0;
GLuint viewMatrixUniform_PV = 0;
GLuint projectionMatrixUniform_PV = 0;

GLuint lightDiffuseUniform_PV[3];
GLuint lightAmbiantUniform_PV[3];
GLuint lightSpecularUniform_PV[3];
GLuint lightPositionUniform_PV[3];

GLuint materialAmbiantUniform_PV = 0;
GLuint materialDiffuseUniform_PV = 0;
GLuint materialSpecularUniform_PV = 0;
GLuint materialShinninesUniform_PV = 0;

// for per fragment 
GLuint modelMatrixUniform_PF = 0;
GLuint viewMatrixUniform_PF = 0;
GLuint projectionMatrixUniform_PF = 0;

GLuint lightDiffuseUniform_PF[3];
GLuint lightAmbiantUniform_PF[3];
GLuint lightSpecularUniform_PF[3];
GLuint lightPositionUniform_PF[3];

GLuint materialAmbiantUniform_PF = 0;
GLuint materialDiffuseUniform_PF = 0;
GLuint materialSpecularUniform_PF = 0;
GLuint materialShinninesUniform_PF = 0;

GLuint keyPressedUniform = 0;

BOOL bLightingEnabled = FALSE;

GLfloat lightAmbiant[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 0.0f }; 

GLfloat materialAmbiant[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat materialShinniness =  128.0f ;

// Variables for light 
BOOL bLight = FALSE;

mat4 PerspectiveProjectionMatrix; // mat4 in vmath.h 

//--------
GLfloat PI = 22.0f / 7.0f;
GLfloat lightAngle_X = 0.0f;
GLfloat lightAngle_Y = 0.0f;
GLfloat lightAngle_Z = 0.0f;

struct Light
{
	vec3 ambiant;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};
struct Light light[3];

//--------

unsigned int NumVertices = 0;
unsigned int numSphereElements = 0;

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable declarations
HWND ghwnd = NULL;  
DWORD dwStyle = 0;

WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
BOOL gbfullscreen = FALSE; 

FILE* gpFILE = NULL;
BOOL gbActive = FALSE;


// Entry point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{ 
	// Function Declarations 
	int initialize(void);
	void uninitialize(void);
	void display(void); 
	void update(void);

	// local variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SVDWindow"); 
	int iResult = 0;  // for base code
	BOOL bDone = FALSE; 

	// Local Variable for finding the center of the window
	int SW = 0;   // Screen width
	int SH = 0;   // Screen Height

	float x = 0.0f;
	float y = 0.0f;

	// Code
	//gpFILE = fopen("log.txt","w");
	//if (gpFILE == NULL)
	
	if(fopen_s(&gpFILE, "log.txt", "w") != 0) // success zal tr 0 return krt nhitr non zero return krt 
	{
		MessageBox(NULL, TEXT("Log File cannot be opend..."), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(gpFILE, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	}

	fprintf(gpFILE , "Program Started Successfully... \n");
	fprintf(gpFILE, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");


	// WNDCLASSEX initialization  
	// structer ch inline initialization

	wndclass.cbSize			= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wndclass.cbWndExtra		= 0;
	wndclass.cbClsExtra		= 0;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.hInstance		= hInstance;
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor		= LoadCursor(NULL,IDC_ARROW);
	wndclass.lpszClassName	= szAppName;
	wndclass.lpszMenuName	= NULL;
	wndclass.hIconSm		= LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));


	// Register WNDCLASSEX 
	RegisterClassEx(&wndclass);

	// Centering of window code
	SW = GetSystemMetrics(SM_CXSCREEN); 
	SH = GetSystemMetrics(SM_CYSCREEN); 

	x = (SW / 2.0) - (WIN_WIDTH / 2.0); 
	y = (SH / 2.0) - (WIN_HEIGHT / 2.0); 


	// Create Window
	hwnd = CreateWindowEx( WS_EX_APPWINDOW,
						 szAppName,
						 TEXT("Saurabh Dhoke"),
						 WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, 
						 x,
				   		 y,
		                 WIN_WIDTH,  
		                 WIN_HEIGHT,  
						 NULL,
						 NULL,
						 hInstance,
						 NULL );

	// gloabal handel madhi value takli 
	ghwnd = hwnd; 

	//initialization
	iResult = initialize();

	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed..."), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);

		//uninitialize();
		//exit(0);	//  yala call kela tri chalel
	}

	// Show the Window
	ShowWindow(hwnd,iCmdShow); 
	
	SetForegroundWindow(hwnd); 
	SetFocus(hwnd); 

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE)  // setfocus mdhi true zalelay he gbActive 
			{
				// Render
				display();

				// Update 
				update();				
			}
		}
	}

	// uninitialization
	uninitialize();
	
	return((int)msg.wParam);  
}

// CALLBACK function
LRESULT CALLBACK WndProc(HWND   hwnd, UINT   iMsg, WPARAM wParam, LPARAM lParam)
{
	// Function declarations
	void ToggleFullscreen(void);
	void resize(int,int); 


	// Code

	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE; 
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE; 
		break;

	case WM_SIZE: //  Width           Height
		resize(LOWORD(lParam), HIWORD(lParam)); 
		break;

	case WM_ERASEBKGND:  // os ch WS_Paint vaparayach nahiy mhanun he vapralay 
		return(0);

	case WM_KEYDOWN :
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE :
			if (gbfullscreen == FALSE)
			{
				ToggleFullscreen();
				gbfullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				gbfullscreen = FALSE;
			}
			break;
		}
		break;
	
	case WM_CHAR :
		switch (LOWORD(wParam))
		{
		case 'L':
		case 'l':
			if (bLightingEnabled == TRUE)
			{
				bLightingEnabled = FALSE;
			}
			else
			{
				bLightingEnabled = TRUE;
			}
			break;

		case 'F':
		case 'f':
			choosenShadder = 'f';
			break;

		case 'V':
		case 'v':
			choosenShadder = 'v';
			break;

		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_DESTROY : 
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	default :
		break;
	}

	return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}
void ToggleFullscreen(void)
{
	// Local Variable Declarations
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
	if (gbfullscreen == FALSE)
	{	
		dwStyle = GetWindowLong(ghwnd,GWL_STYLE);
		
		if (dwStyle & WS_OVERLAPPEDWINDOW) 
		{	
			if (GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{    
				SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW); // dwStyle shodh aani WS_OVERLAPPEDWINDOW kadhun tak (~ remove karto)                             
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);			
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); // window placement allredy set zali first line la mhanun 0 dilet
		ShowCursor(TRUE); 
	}
}
int initialize(void)
{
	// Function Declarations
	void printGLInfo(void);
	void uninitialize();
	void resize(int, int);

	// Code
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	
	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

	// step 1 - initialization of pixel format descriptor
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;  /// ???????????? PFD_RGBA
	pfd.cColorBits = 32;
	pfd.cRedBits   = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits  = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// step 2 - get the DC (device context)
	ghdc = GetDC(ghwnd);
	if (ghdc == 0)
	{
		fprintf(gpFILE, "GetDC() failed.. \n\n");
		return -1;
	}

	// step 3
	iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFILE, "ChoosePixelFormat() failed.. \n\n");
		return -2;
	}

	// Step 4
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat() failed.. \n\n");
		return -3;
	}

	// tell wgl library to give opengl compatable device context to this device context
	// create opengl context from device context 
	ghrc = wglCreateContext(ghdc);
	if (ghrc == 0)
	{
		fprintf(gpFILE, "wglCreateContext() failed.. \n\n");
		return -4;
	}

	// Make rendering Context current 
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent() failed.. \n\n");
		return -5;
	}

	//----------------------------------------------------
	// PP cha code ya shivay eligible hot nahi 
	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInint() failed.. \n\n");
		return -6;
	}

	//-----------------------------------
	// Print OpenGL info 
	// printGLInfo();
	//-----------------------------------
	
	//====================================================================================================================
	// ---------- Per Vertex Shadders ------------
	 //------------ Vertex Shader --------------- 
	const GLchar* vertexShaderSourceCode_PV =
		"#version 460 core" \
		"\n " \
		"in vec4 aPosition; " \
		"in vec3 aNormal; " \
		"in vec4 aColor; " \
		"out vec4 oColor; " \
		"uniform mat4 uModelMatrix; " \
		"uniform mat4 uViewMatrix; " \
		"uniform mat4 uProjectionMatrix; " \
		"uniform vec3 uLightAmbiant[3]; " \
		"uniform vec3 uLightDiffuse[3]; " \
		"uniform vec3 uLightSpecular[3]; " \
		"uniform vec4 ulightPosition[3]; " \
		"uniform vec3 uMaterialAmbiant; " \
		"uniform vec3 uMaterialDiffuse; " \
		"uniform vec3 uMaterialSpecular; " \
		"uniform float uMaterialShinniness; " \
		"uniform int uKeyPressed; " \
		"out vec3 oPhong_ADS_Light; " \
		"void main(void) " \
		"{ " \
			"oPhong_ADS_Light = vec3(0.0f, 0.0f, 0.0f); " \
			"if(uKeyPressed == 1) " \
			"{ " \
				"vec4 iCoordinates       = uViewMatrix * uModelMatrix * aPosition; " \
				"vec3 transformedNormals = normalize(mat3( uViewMatrix * uModelMatrix) * aNormal);" \
				"vec3 ambiantLight[3]; " \
				"vec3 lightDirection[3]; " \
				"vec3 diffuseLight[3]; " \
				"vec3 reflectionVector[3]; " \
				"vec3 lightSpecular[3]; " \
				"for(int i = 0; i < 3; i++)" \
				"{" \
					"ambiantLight[i]       = uLightAmbiant[i] * uMaterialAmbiant;" \
					"lightDirection[i]     = normalize(vec3(ulightPosition[i] - iCoordinates));" \
					"diffuseLight[i]       = uLightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i],transformedNormals),0.0); " \
					"reflectionVector[i]   = reflect(-lightDirection[i],transformedNormals); " \
					"vec3 viewerVector     = normalize(-iCoordinates.xyz); " \
					"lightSpecular[i]      = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],viewerVector), 0.0), uMaterialShinniness ); " \
					"oPhong_ADS_Light      = oPhong_ADS_Light +  ambiantLight[i] + diffuseLight[i] + lightSpecular[i]; " \
				"} " \
			"} " \
			"else " \
			"{ " \
				"oPhong_ADS_Light = vec3(0.0, 0.0, 0.0); " \
			"} " \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition; " \
		"} ";

	// 2: Create vertex shadder object
	GLuint vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

	// 3: give vertex shader source code to opengl
	glShaderSource(vertexShaderObject_PV, 1, (const GLchar**)&vertexShaderSourceCode_PV, NULL);

	// 4: let opengl compile vertex shader
	glCompileShader(vertexShaderObject_PV);

	// 5.c
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	// 5.a
	glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{	// 5.b
		glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{   // 5.c
			szInfoLog = (GLchar*)malloc(infoLogLength + 1);

			if (szInfoLog != NULL)
			{   // 5.d
				glGetShaderInfoLog(vertexShaderObject_PV, infoLogLength + 1, NULL, szInfoLog);
				// 5.e
				fprintf(gpFILE, "vertex shadder Compilation Error Log : %s \n", szInfoLog);
				// 5.f
				free(szInfoLog);
				szInfoLog = NULL;
				// 5.g
			}
		}
		uninitialize();
	}

	//-------------- Fragment Shader ---------------
	const GLchar* fragmentShaderSourceCode_PV =
		"#version 460 core" \
		"\n" \
		"in vec4 oColor;" \
		"in vec3 oPhong_ADS_Light;" \
		"out vec4 FragColor;" \
		"uniform int uKeyPressed;" \
		"void main(void)" \
		"{" \
			"if(uKeyPressed == 1)" \
			"{" \
				"FragColor = vec4(oPhong_ADS_Light, 1.0);" \
			"}" \
			"else" \
			"{" \
				"FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);" \
			"}" \
		"}";

	GLuint fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject_PV, 1, (const GLchar**)&fragmentShaderSourceCode_PV, NULL);
	glCompileShader(fragmentShaderObject_PV);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (infoLogLength != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Fragment shader Compilation Error Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}

	//------------ Shader Progarm --------------
	// 7: create shader program
	shaderProgramObject_PV = glCreateProgram();

	// 8 : Attach both shadders to this program
	glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);
	glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

	// 9 : bind attribute locations with the shader program objects
	glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_COLOR, "aColor");
	glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_NORMAL, "aNormal");

	// 10: link the shader program
	glLinkProgram(shaderProgramObject_PV);

	// 11:
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (infoLogLength != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}

	// get shader uniform locations 
   //                      kunakadun gheu , kontya uniform location ch gheu  
  // mvpMatrixUniform = glGetUniformLocation(shaderProgramObject_PV, "uMVPMatrix");
	modelMatrixUniform_PV       = glGetUniformLocation(shaderProgramObject_PV, "uModelMatrix");
	viewMatrixUniform_PV        = glGetUniformLocation(shaderProgramObject_PV, "uViewMatrix");
	projectionMatrixUniform_PV  = glGetUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");
	
	lightAmbiantUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbiant[0]");
	lightDiffuseUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[0]");
	lightSpecularUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[0]");
	lightPositionUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[0]");
	
	lightAmbiantUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbiant[1]");
	lightDiffuseUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[1]");
	lightSpecularUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[1]");
	lightPositionUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[1]");

	lightAmbiantUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbiant[2]");
	lightDiffuseUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[2]");
	lightSpecularUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[2]");
	lightPositionUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[2]");
	
	materialAmbiantUniform_PV   = glGetUniformLocation(shaderProgramObject_PV, "uMaterialAmbiant");
	materialDiffuseUniform_PV   = glGetUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");
	materialSpecularUniform_PV  = glGetUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");
	materialShinninesUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialShinniness");
	
	keyPressedUniform        = glGetUniformLocation(shaderProgramObject_PV, "uKeyPressed");

	
	//====================================================================================================================
	
	//====================================================================================================================
	//---------- Per Fragment Shadders ------------
	 //------------ Vertex Shader --------------- 
	 const GLchar* vertexShaderSourceCode_PF =
		"#version 460 core" \
		"\n " \
		"in vec4 aPosition; " \
		"in vec3 aNormal; " \
		"in vec4 aColor; " \
		"out vec4 oColor; " \
		"uniform mat4 uModelMatrix; " \
		"uniform mat4 uViewMatrix; " \
		"uniform mat4 uProjectionMatrix; " \
		"uniform vec4 ulightPosition[3]; " \
		"uniform int uKeyPressed; " \
		"out vec3 oTransformedNormals; " \
		"out vec3 oLightDirection[3]; " \
		"out vec3 oViewerVector; " \
		"void main(void) " \
		"{ " \
			"if(uKeyPressed == 1) " \
			"{ " \
				"vec4 iCoordinates       = uViewMatrix * uModelMatrix * aPosition; " \
				"oTransformedNormals = normalize(mat3( uViewMatrix * uModelMatrix) * aNormal);" \
				"oViewerVector     = normalize(-iCoordinates.xyz); " \
				"for(int i = 0; i < 3; i++)" \
				"{" \
					"oLightDirection[i]= normalize(vec3(ulightPosition[i] - iCoordinates));" \
				"} " \
			"} " \
			"else " \
			"{ " \
				"oTransformedNormals = vec3(0.0, 0.0, 0.0); " \
				"oViewerVector       = vec3(0.0, 0.0, 0.0); " \
				"for(int i = 0; i < 3; i++)" \
				"{" \
					"oLightDirection[i] = vec3(0.0, 0.0, 0.0); " \
				"} " \
			"} " \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition; " \
		"} ";
	 
	 // 2: Create vertex shadder object
	 GLuint vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);
	
	 // 3: give vertex shader source code to opengl
	 glShaderSource(vertexShaderObject_PF,1,(const GLchar**)&vertexShaderSourceCode_PF,NULL);

	 // 4: let opengl compile vertex shader
	 glCompileShader(vertexShaderObject_PF);

	 // 5.c
	 status        = 0;
	 infoLogLength = 0;
	 szInfoLog   = NULL;

	 // 5.a
	 glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &status);
	 if (status == GL_FALSE)
	 {	// 5.b
		 glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {   // 5.c
			 szInfoLog = (GLchar *)malloc(infoLogLength+1);

			 if (szInfoLog != NULL)
			 {   // 5.d
				 glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength+1, NULL, szInfoLog);
				 // 5.e
				 fprintf(gpFILE, "vertex shadder Compilation Error Log : %s \n", szInfoLog);
				 // 5.f
				 free(szInfoLog);
				 szInfoLog = NULL;
				 // 5.g
			 }
		 }
		 uninitialize();
	 }

	 //-------------- Fragment Shader ---------------
	const GLchar* fragmentShaderSourceCode_PF =
		 "#version 460 core" \
		 "\n" \
		 "in vec4 oColor;" \
		 "in vec3 oTransformedNormals; " \
		 "in vec3 oLightDirection[3]; " \
		 "in vec3 oViewerVector; " \
		 "uniform vec3 uLightAmbiant[3]; " \
		 "uniform vec3 uLightDiffuse[3]; " \
		 "uniform vec3 uLightSpecular[3]; " \
		 "uniform vec3 uMaterialAmbiant; " \
		 "uniform vec3 uMaterialDiffuse; " \
		 "uniform vec3 uMaterialSpecular; " \
		 "uniform float uMaterialShinniness; " \
		 "out vec4 FragColor;" \
		 "uniform int uKeyPressed;" \
		 "void main(void)" \
		 "{" \
			 "vec3 Phong_ADS_Light;" \
			 "if(uKeyPressed == 1)" \
			 "{" \
				 "vec3 ambiantLight[3]; " \
				 "vec3 diffuseLight[3]; " \
				 "vec3 reflectionVector[3]; " \
				 "vec3 lightSpecular[3]; " \
		 		 "\n" \
				 "vec3 normalisedLightDirection[3] ;" \
				 "for(int i = 0; i < 3; i++)" \
				 "{" \
					 "normalisedLightDirection[i]     =  normalize(oLightDirection[i]); " \
				 "}" \
				 "vec3 normalisedTransformedNormals =  normalize(oTransformedNormals); " \
				 "vec3 normalisedViewerVector       =  normalize(oViewerVector); " \
				 "\n" \
				 "for(int i = 0; i < 3; i++)" \
				 "{" \
					 "ambiantLight[i]       = uLightAmbiant[i] * uMaterialAmbiant;" \
					 "diffuseLight[i]       = uLightDiffuse[i] * uMaterialDiffuse * max(dot(normalisedLightDirection[i],normalisedTransformedNormals),0.0); " \
					 "reflectionVector[i]   = reflect(-normalisedLightDirection[i],normalisedTransformedNormals); " \
					 "lightSpecular[i]      = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],normalisedViewerVector), 0.0), uMaterialShinniness ); " \
					 "Phong_ADS_Light       = Phong_ADS_Light +  ambiantLight[i] + diffuseLight[i] + lightSpecular[i]; " \
				 "} " \
				 "FragColor = vec4(Phong_ADS_Light, 1.0);" \
			 "}" \
			 "else" \
			 "{" \
				 "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);" \
			 "}" \
		 "}";

	 GLuint fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);
	 glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);
	 glCompileShader(fragmentShaderObject_PF);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &status);
	 if (status == GL_FALSE)
	 {
		 glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (infoLogLength != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength, NULL,szInfoLog);
				 fprintf(gpFILE,"Fragment shader Compilation Error Log : %s \n", szInfoLog); 

				 free(szInfoLog);
				 szInfoLog = NULL;
			 }
		 }
		 uninitialize();
	 }


	 //------------ Shader Progarm --------------
	 // 7: create shader program
	 shaderProgramObject_PF = glCreateProgram();

	 // 8 : Attach both shadders to this program
	 glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
	 glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

	 // 9 : bind attribute locations with the shader program objects
	 glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_POSITION,"aPosition");
	 glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_COLOR, "aColor");
	 glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_NORMAL, "aNormal");

	 // 10: link the shader program
	 glLinkProgram(shaderProgramObject_PF);

	 // 11:
	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_PF,GL_LINK_STATUS,&status);
	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_PF,GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (infoLogLength != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				 free(szInfoLog);
				 szInfoLog = NULL; 
			 }
		 }
		 uninitialize();
	 }

	  // get shader uniform locations 
	 //                      kunakadun gheu , kontya uniform location ch gheu  
	// mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
	 modelMatrixUniform_PF       = glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");
	 viewMatrixUniform_PF        = glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");
	 projectionMatrixUniform_PF  = glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");
	
	 lightAmbiantUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbiant[0]");
	 lightDiffuseUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[0]");
	 lightSpecularUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[0]");
	 lightPositionUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[0]");
	
	
	 lightAmbiantUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbiant[1]");
	 lightDiffuseUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[1]");
	 lightSpecularUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[1]");
	 lightPositionUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[1]");
	
	
	 lightAmbiantUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbiant[2]");
	 lightDiffuseUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[2]");
	 lightSpecularUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[2]");
	 lightPositionUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[2]");
	
	 materialAmbiantUniform_PF   = glGetUniformLocation(shaderProgramObject_PF, "uMaterialAmbiant");
	 materialDiffuseUniform_PF   = glGetUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");
	 materialSpecularUniform_PF  = glGetUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");
	 materialShinninesUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialShinniness");
	 
	 keyPressedUniform = glGetUniformLocation(shaderProgramObject_PF, "uKeyPressed");
	
	 //====================================================================================================================
	
	 // For Sphere 
	 float sphere_positions[1146];
	 float sphere_normals[1146];
	 float sphere_texcoords[764];
	 unsigned short sphere_elements[2280];

	 getSphereVertexData(sphere_positions, sphere_normals, sphere_texcoords, sphere_elements);
	 NumVertices = getNumberOfSphereVertices();
	 numSphereElements = getNumberOfSphereElements();


	 //----------
	 // Cube
	 // 13 : VAO - create vertex array object 
	 glGenVertexArrays(1, &vao);

	 // 14: bind with vao
	 glBindVertexArray(vao);

	 // 15: VBO - vertex buffer object for postion 
	 glGenBuffers(1, &vbo_position);
	 // 16: bind with vbo_position
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_positions), sphere_positions, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);	 // unbind VBO

	 // for vbo normal 
	 glGenBuffers(1, &vbo_normal);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);  	 // unbind VBO

	 // vbo for texcoord  
	 glGenBuffers(1, &vbo_texcoord);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_texcoords), sphere_texcoords, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);  	 // unbind VBO

	 // vbo for elements 
	 glGenBuffers(1, &vbo_element);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	 glBindVertexArray(0); // unbind vao
	//--------------------------------------------------------------------------------
	
	// Enabling Depth
	glClearDepth(1.0f);							// compulsary 
	glEnable(GL_DEPTH_TEST);					// compulsary
	glDepthFunc(GL_LEQUAL);						// compulsary

	// here openGl starts
	// Set the clear color of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // blue color de as sangital ct ith color ith nhi dila jat

	// for light 
	light[0].ambiant = vec3(0.0f, 0.0f, 0.0f);
	light[0].diffuse = vec3(1.0f, 0.0f, 0.0f);
	light[0].specular = vec3(0.0f, 0.0f, 1.0f);
	//light[0].position = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	light[1].ambiant = vec3(0.0f, 0.0f, 0.0f);
	light[1].diffuse = vec3(0.0f, 1.0f, 0.0f);
	light[1].specular = vec3(0.0f, 0.0f, 1.0f);
	//light[1].position = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	light[2].ambiant = vec3(0.0f, 0.0f, 0.0f);
	light[2].diffuse = vec3(0.0f, 0.0f, 1.0f);
	light[2].specular = vec3(0.0f, 0.0f, 1.0f);

	// initialize orthograficprojection matrix
	PerspectiveProjectionMatrix = vmath::mat4::identity();

	// warm up resize call
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void printGLInfo(void)
{
	// Local Variable declarations 
	GLint Num_Extensions;  // As a Array Use krtoy aapan ith
	GLint i = 0;

	// Code 
	fprintf(gpFILE, "OpenGL Vendor   : %s \n", glGetString(GL_VENDOR));
	fprintf(gpFILE, "OpenGL Rendorer : %s \n", glGetString(GL_RENDERER)); 
	fprintf(gpFILE, "OpenGL Version  : %s \n", glGetString(GL_VERSION)); 
	fprintf(gpFILE, "GLSL Version    : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));   // Grafic library shading language

	// Listing of Supported Extensions 
	glGetIntegerv(GL_EXTENSIONS, &Num_Extensions); 

	for (i = 0; i < Num_Extensions; i++)
	{
		fprintf(gpFILE, "%s \n", glGetStringi(GL_EXTENSIONS, i));  
	}

}

void resize(int width, int height)
{
	// Code

	if (height <= 0) // precausion (height na division krnar y pudh )
		height = 1;

	// set Perspective projection matrix
	PerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // glViewport = binocular - durbin
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Blue color hya line mul set hoto

	// 1: use shader program 

	if (choosenShadder == 'v')
		{
			glUseProgram(shaderProgramObject_PV);
		}
	else if (choosenShadder == 'f')
		{
			glUseProgram(shaderProgramObject_PF);
		}


	//--------------------------------------
	// Cube 
	mat4 translationMatrix = vmath::mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	mat4 scaleMatrix = vmath::mat4::identity();
	mat4 modelMatrix = translationMatrix;
	mat4 viewMatrix  = vmath::mat4::identity();

	// Transformation 

	// Push above MVP into vertex shaders mvpuniform 
	glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, PerspectiveProjectionMatrix);

	if (bLightingEnabled == TRUE && choosenShadder == 'v')
	{
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, PerspectiveProjectionMatrix);

		glUniform1i(keyPressedUniform, 1);
	
		glUniform3fv(lightDiffuseUniform_PV[0], 1, light[0].diffuse);
		glUniform3fv(lightAmbiantUniform_PV[0], 1, light[0].ambiant);
		glUniform3fv(lightSpecularUniform_PV[0], 1, light[0].specular);
		glUniform4fv(lightPositionUniform_PV[0], 1, light[0].position);

		glUniform3fv(lightDiffuseUniform_PV[1], 1, light[1].diffuse);
		glUniform3fv(lightAmbiantUniform_PV[1], 1, light[1].ambiant);
		glUniform3fv(lightSpecularUniform_PV[1], 1, light[1].specular);
		glUniform4fv(lightPositionUniform_PV[1], 1, light[1].position);

		glUniform3fv(lightDiffuseUniform_PV[2], 1, light[2].diffuse);
		glUniform3fv(lightAmbiantUniform_PV[2], 1, light[2].ambiant);
		glUniform3fv(lightSpecularUniform_PV[2], 1, light[2].specular);
		glUniform4fv(lightPositionUniform_PV[2], 1, light[2].position);

		glUniform3fv(materialDiffuseUniform_PV, 1, materialDiffuse);
		glUniform3fv(materialAmbiantUniform_PV, 1, materialAmbiant);
		glUniform3fv(materialSpecularUniform_PV, 1, materialSpecular);
		glUniform1f(materialShinninesUniform_PV, materialShinniness);
	}
	else if (bLightingEnabled == TRUE && choosenShadder == 'f')
	{
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, PerspectiveProjectionMatrix);


		glUniform1i(keyPressedUniform, 1);
		
		glUniform3fv(lightDiffuseUniform_PF[0], 1, light[0].diffuse);
		glUniform3fv(lightAmbiantUniform_PF[0], 1, light[0].ambiant);
		glUniform3fv(lightSpecularUniform_PF[0], 1, light[0].specular);
		glUniform4fv(lightPositionUniform_PF[0], 1, light[0].position);

		glUniform3fv(lightDiffuseUniform_PF[1], 1, light[1].diffuse);
		glUniform3fv(lightAmbiantUniform_PF[1], 1, light[1].ambiant);
		glUniform3fv(lightSpecularUniform_PF[1], 1, light[1].specular);
		glUniform4fv(lightPositionUniform_PF[1], 1, light[1].position);
		
		glUniform3fv(lightDiffuseUniform_PF[2], 1, light[2].diffuse);
		glUniform3fv(lightAmbiantUniform_PF[2], 1, light[2].ambiant);
		glUniform3fv(lightSpecularUniform_PF[2], 1, light[2].specular);
		glUniform4fv(lightPositionUniform_PF[2], 1, light[2].position);

		glUniform3fv(materialDiffuseUniform_PF, 1, materialDiffuse);
		glUniform3fv(materialAmbiantUniform_PF, 1, materialAmbiant);
		glUniform3fv(materialSpecularUniform_PF, 1, materialSpecular);
		glUniform1f(materialShinninesUniform_PF, materialShinniness);
	}
	else
	{
		glUniform1i(keyPressedUniform, 0);
	}


	// 2 : bind with vao
	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	//             kay draw karu , kiti elements draw karu, elements cha type kuthalay , kuthun draw karu 
	glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);


	// 4 : unbind vao
	glBindVertexArray(vao);

	//--------------------------------------

	// 5 : unuse the shader program
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code

	if (bLightingEnabled == TRUE)
	{
		//------------ light 0 ----------------
		light[0].position[0] = 0.0;
		light[0].position[1] = 10.0 * (GLfloat)sin(lightAngle_X);
		light[0].position[2] = 10.0 * (GLfloat)cos(lightAngle_X);
		light[0].position[3] = 100.0f;

		if (lightAngle_X < 2 * PI)
		{
			lightAngle_X = lightAngle_X + 0.01f;
		}
		else
		{
			lightAngle_X = 0.0f;
		}

		//--------- light 1 ---------------------
		light[1].position[0] = 10.0 * (GLfloat)sin(lightAngle_Y);
		light[1].position[1] = 0.0f;
		light[1].position[2] = 10.0 * (GLfloat)cos(lightAngle_Y);
		light[1].position[3] = 100.0f;

		if (lightAngle_Y < 2 * PI)
		{
			lightAngle_Y = lightAngle_Y + 0.01f;
		}
		else
		{
			lightAngle_Y = 0.0f;
		}

		//--------- light 2 -----------------
		light[2].position[0] = 10.0 * (GLfloat)sin(lightAngle_Z);
		light[2].position[1] = 10.0 * (GLfloat)cos(lightAngle_Z);
		light[2].position[2] = 0.0;
		light[2].position[3] = 100.0f;

		if (lightAngle_Z < 2 * PI)
		{
			lightAngle_Z = lightAngle_Z + 0.01f;
		}
		else
		{
			lightAngle_Z = 0.0f;
		}
	
	}


}

void uninitialize(void)
{
	// Function Declarations
	void ToggleFullscreen(void);
	
	// Code
	// Shader program uninitialize
	if (shaderProgramObject_PF)
	{
		glUseProgram(shaderProgramObject_PF);
		
		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &numShaders);
		
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_PF, numShaders, NULL, pShaders);
				
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_PF, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				// 6
				free(pShaders);
				pShaders = NULL;
			}
		}
		// 7
		glUseProgram(0);
		//8
		glDeleteProgram(shaderProgramObject_PF);
		shaderProgramObject_PF = 0;
	 }

	//------------------
	// delete vbo_normal 
	if (vbo_normal)
	{
		glDeleteBuffers(1, &vbo_normal);
		vbo_normal = 0;
	}

	// delete vbo_element
	if (vbo_element)
	{
		glDeleteBuffers(1, &vbo_element);
		vbo_element = 0;
	}

	// delte vbo_position
	if (vbo_position)
	{
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
	}

	// delte vbo_texcoord
	if (vbo_texcoord)
	{
		glDeleteBuffers(1, &vbo_texcoord);
		vbo_texcoord = 0;
	}

	// delete vao
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	//--------
	//--------------------
	// if application is exiting in fullDcreen
	if (gbfullscreen == TRUE)    
	{
		ToggleFullscreen();     // jar kuni madhich esc kel tr window small kr fullscreen asl tr an mng close kr 
		gbfullscreen = FALSE; 
	}

	// Make the hdc as the current dc
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// destroy / delte rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// release the HDC 
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// Destroy Window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	
	// Close the log file 
	if (gpFILE)
	{	
		fprintf(gpFILE, "\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
		fprintf(gpFILE, "Program Ended Successfully...\n"); 
		fprintf(gpFILE, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}

}

                                                                                                        



