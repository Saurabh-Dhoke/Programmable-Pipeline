
/******************************************* 
	Cube Map Texture 
	Date : 20 - 06 -2024 
*******************************************/

// Common Header Files
#include<windows.h>
#include<stdio.h> 
#include<stdlib.h> // for exit() 

#include<iostream>
#include <vector>
using namespace std;

#include"OGL.h"

// for PlaySound() api
#include <mmsystem.h>	

// opengl header Files
#include<gl/glew.h> // this must be before gl/GL.h
#include<gl/GL.h>

#include"vmath.h"
using namespace vmath;

// for image loading ..(png, bmp, jpeg, jpg)
#define STB_IMAGE_IMPLEMENTATION

#include"stb_image.h"

// Macrose
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// link with OpenGL library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")

// openGl related global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLuint shaderProgramObject  = 0;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR  ,
	AMC_ATTRIBUTE_TEXCOORD
};

GLuint vao_skybox = 0;
GLuint vbo_position_Skybox = 0;
//GLuint vbo_Texcoord_Cube = 0;

GLuint mvpMatrixUniform = 0;
GLuint textureSamplerUniform = 0;

mat4 PerspectiveProjectionMatrix; // mat4 in vmath.h 


// Texture Object/ Variable
GLuint Texture_Sky = 0;

GLfloat angle_Skybox = 0.0;

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable declarations
HWND ghwnd = NULL;  
DWORD dwStyle = 0;

WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
BOOL gbfullscreen = FALSE; 

FILE* gpFILE = NULL;
BOOL gbActive = FALSE;

//------------------------------------------
// for sound play 
HINSTANCE hInst = NULL;
//------------------------------------------

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
			DestroyWindow(hwnd);
			break;
		}
		break;
	
	case WM_CHAR :
		switch (LOWORD(wParam))
		{
		case 'F' :
		case 'f' :
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
	void uninitialize();
	void resize(int, int);
	
	GLuint loadSkyboxTexture(vector<std::string> faces);  

	// Code
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	BOOL bResult; // for texture

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

	 //------------ Vertex Shader --------------- 
	const GLchar* vertexShaderSourceCode =
		"#version 460 core" \
		"\n " \
		"in vec3 aPosition; " \
		"out vec3 oTexCoord; " \
		"uniform mat4 uMVPMatrix;" \
		"void main(void)" \
		"{" \
			"gl_Position = uMVPMatrix * vec4(aPosition, 1.0) ; " \
			"oTexCoord = aPosition ; " \
		"}";
	 
	 // 2: Create vertex shadder object
	 GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	 // 3: give vertex shader source code to opengl
	 glShaderSource(vertexShaderObject,1,(const GLchar**)&vertexShaderSourceCode,NULL);

	 // 4: let opengl compile vertex shader
	 glCompileShader(vertexShaderObject);

	 // 5.c
	 GLint status        = 0;
	 GLint infoLogLength = 0;
	 GLchar * szInfoLog   = NULL;

	 // 5.a
	 glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	 if (status == GL_FALSE)
	 {	// 5.b
		 glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength); 

		 if (infoLogLength > 0)
		 {   // 5.c
			 szInfoLog = (GLchar *)malloc(infoLogLength+1);

			 if (szInfoLog != NULL)
			 {   // 5.d
				 glGetShaderInfoLog(vertexShaderObject, infoLogLength+1, NULL, szInfoLog);  
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
	 const GLchar* fragmentShaderSourceCode =
		 "#version 460 core" \
		 "\n" \
		 "in vec3 oTexCoord;" \
		 "uniform samplerCube  uSkybox;" \
		 "out vec4 FragColor;" \
		 "void main(void)" \
		 "{" \
			 "FragColor = texture(uSkybox, oTexCoord); " \
		 "}";

	 GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	 glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	 glCompileShader(fragmentShaderObject);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	 if (status == GL_FALSE)
	 {
		 glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (infoLogLength != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL,szInfoLog);
				 fprintf(gpFILE,"Fragment shader Compilation Error Log : %s \n", szInfoLog); 

				 free(szInfoLog);
				 szInfoLog = NULL;
			 }
		 }
		 uninitialize();
	 }

	 //------------ Shader Progarm --------------
	 // 7: create shader program
	 shaderProgramObject = glCreateProgram();

	 // 8 : Attach both shadders to this program
	 glAttachShader(shaderProgramObject, vertexShaderObject);
	 glAttachShader(shaderProgramObject, fragmentShaderObject);

	 // 9 : bind attribute locations with the shader program objects
	 glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION,"aPosition");

	 // 10: link the shader program
	 glLinkProgram(shaderProgramObject);

	 // 11:

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject,GL_LINK_STATUS,&status); 
	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject,GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (infoLogLength != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				 free(szInfoLog);
				 szInfoLog = NULL; 
			 }
		 }
		 uninitialize();
	 }

	  // get shader uniform locations 
	 //                      kunakadun gheu , kontya uniform location ch gheu  
	 mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
	 textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uSkybox");


	 // declare position and color arrays
	 const GLfloat skybox_position[] =
	  {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	  };


	 //----------
	 // Cube
	 // 13 : VAO - create vertex array object 
	 glGenVertexArrays(1, &vao_skybox);

	 // 14: bind with vao
	 glBindVertexArray(vao_skybox);

	 // 15: VBO - vertex buffer object for postion 
	 glGenBuffers(1, &vbo_position_Skybox);

	 // 16: bind with vbo_position
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_position_Skybox);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_position), skybox_position, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);	 // unbind VBO

	 glBindVertexArray(0);

	//--------------------------------------------------------------------------------
	 vector<std::string> faces
	 {
		 "skybox/right.jpg",
		 "skybox/left.jpg",
		 "skybox/top.jpg",
		 "skybox/bottom.jpg",
		 "skybox/front.jpg",
		 "skybox/back.jpg"
	 };

	 Texture_Sky = loadSkyboxTexture(faces);
	
	//--------------------------------------------------------------------------------
	
	// Enabling Depth
	glClearDepth(1.0f);							// compulsary 
	glEnable(GL_DEPTH_TEST);					// compulsary
	glDepthFunc(GL_LEQUAL);						// compulsary

	// here openGl starts
	// Set the clear color of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // blue color de as sangital ct ith color ith nhi dila jat

	//-----------------------------
	PlaySound(TEXT("soundName"), hInst, SND_RESOURCE | SND_ASYNC);
	//-----------------------------

	// tell opengl to enable the texture (step 4)
	glEnable(GL_TEXTURE_2D);
	//------------

	// initialize orthographicprojection matrix
	PerspectiveProjectionMatrix = vmath::mat4::identity();

	// warm up resize call
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}


GLuint loadSkyboxTexture(vector<std::string> faces)
{
	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	
	// Variable declarations 
	int width;
	int height;
	int components;
	int i;

	// code 
	GLuint texture_id;  
	glGenTextures(1, &texture_id); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	for (i = 0; i < faces.size(); i++)
	{
		unsigned char* imageData = stbi_load(faces[i].c_str(), &width, &height, &components, 0);

		if (imageData)
		{
			// 1- target ,2-mipmap level , 6 - border ahe ka , 3- image format , 7 - opengl format donhi samech ast 99% , 8- datatype image cha , 9 - image cha data 
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			stbi_image_free(imageData);
		}
		else
		{
			fprintf(gpFILE, "Skybox texture failed to load at patha : %s \n", faces[i].c_str());
			stbi_image_free(imageData);
		}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

	return texture_id;
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
	glUseProgram(shaderProgramObject);

	//--------------------------------------
	// Cube 
	mat4 translationMatrix = vmath::mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

	mat4 scaleMatrix = vmath::mat4::identity();
	scaleMatrix = vmath::scale(6.0f, 6.0f, 6.0f);

	mat4 rotationMatrix = vmath::mat4::identity(); 
	rotationMatrix = vmath::rotate(angle_Skybox, 0.0f, 1.0f, 0.0f);
	
	mat4 modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// Transformation 
	mat4 modelViewProjectionMatrix = PerspectiveProjectionMatrix * modelViewMatrix; // order of multiplication is VIMP

	// Push above MVP into vertex shaders mvpuniform 
	// 2-kiti matrices dhakalaychet , 3-tu dilelya matrix la transpose karu ka , 4-konta matrix dhkalu 
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glDepthMask(GL_FALSE); 

	// for texture 
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_CUBE_MAP , Texture_Sky); 
	glUniform1i(textureSamplerUniform, 0);

	// 2 : bind with vao
	glBindVertexArray(vao_skybox);

		// 3 : Draw the Geometry 
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// 4 : unbind vao
	glBindVertexArray(0);

	// unbind with texture 
	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthMask(GL_TRUE);
	//--------------------------------------

	// 5 : unuse the shader program
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code

	angle_Skybox = angle_Skybox + 0.05;
	if (angle_Skybox >= 360.0)
	{
		angle_Skybox = angle_Skybox - 360;
	}
}

void uninitialize(void)
{
	// Function Declarations
	void ToggleFullscreen(void);
	
	// Code

	// Shader program uninitialize
	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);
		
		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
		
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
				
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject, pShaders[i]);
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
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	 }

	//------------------
	// Cube
	// 9: delete vbo of color 
	
	// 10: delete vbo of position 
	if (vbo_position_Skybox)
	{
		glDeleteBuffers(1, &vbo_position_Skybox);
		vbo_position_Skybox = 0;
	}

	// 11: delete vao
	if (vao_skybox)
	{
		glDeleteVertexArrays(1, &vao_skybox);
		vao_skybox = 0;
	}
	//-------------

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
	
	// texture uninitialize
	if (Texture_Sky)
	{
		glDeleteTextures(1, &Texture_Sky);
		Texture_Sky = 0;
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

                                                                                                        



