
/*
	Effect    : Julia Set 
	Reference : Super Bibal 7th ediition 
	Base Code : B/W Perspective Triangle 
	Date      : 19.08.2024
*/

#pragma once

#include <cmath>

// Variable Declarations 

GLuint vao = 0;
GLuint palette_texture = 0;
GLuint shaderProgramObject = 0;

GLuint mvpMatrixUniform = 0;
mat4 perspectiveProjectionMatrix; // mat4 in vmath.h 

// uniforms 
GLuint zoomUniform = 0;
GLuint offsetUniform = 0;
GLuint CUniform = 0;


// flag 
bool animation_flag_1 = true;
bool animation_flag_2 = false;



bool paused;
float time_offset;
float zoom;
float x_offset;
float y_offset;


static const unsigned char palette[] =
{
    0xFF, 0x00, 0x00, 0xFF, 0x0E, 0x03, 0xFF, 0x1C,
    0x07, 0xFF, 0x2A, 0x0A, 0xFF, 0x38, 0x0E, 0xFF,
    0x46, 0x12, 0xFF, 0x54, 0x15, 0xFF, 0x62, 0x19,
    0xFF, 0x71, 0x1D, 0xFF, 0x7F, 0x20, 0xFF, 0x88,
    0x22, 0xFF, 0x92, 0x25, 0xFF, 0x9C, 0x27, 0xFF,
    0xA6, 0x2A, 0xFF, 0xB0, 0x2C, 0xFF, 0xBA, 0x2F,
    0xFF, 0xC4, 0x31, 0xFF, 0xCE, 0x34, 0xFF, 0xD8,
    0x36, 0xFF, 0xE2, 0x39, 0xFF, 0xEC, 0x3B, 0xFF,
    0xF6, 0x3E, 0xFF, 0xFF, 0x40, 0xF8, 0xFE, 0x40,
    0xF1, 0xFE, 0x40, 0xEA, 0xFE, 0x41, 0xE3, 0xFD,
    0x41, 0xDC, 0xFD, 0x41, 0xD6, 0xFD, 0x42, 0xCF,
    0xFC, 0x42, 0xC8, 0xFC, 0x42, 0xC1, 0xFC, 0x43,
    0xBA, 0xFB, 0x43, 0xB4, 0xFB, 0x43, 0xAD, 0xFB,
    0x44, 0xA6, 0xFA, 0x44, 0x9F, 0xFA, 0x45, 0x98,
    0xFA, 0x45, 0x92, 0xF9, 0x45, 0x8B, 0xF9, 0x46,
    0x84, 0xF9, 0x46, 0x7D, 0xF8, 0x46, 0x76, 0xF8,
    0x46, 0x6F, 0xF8, 0x47, 0x68, 0xF8, 0x47, 0x61,
    0xF7, 0x47, 0x5A, 0xF7, 0x48, 0x53, 0xF7, 0x48,
    0x4C, 0xF6, 0x48, 0x45, 0xF6, 0x49, 0x3E, 0xF6,
    0x49, 0x37, 0xF6, 0x4A, 0x30, 0xF5, 0x4A, 0x29,
    0xF5, 0x4A, 0x22, 0xF5, 0x4B, 0x1B, 0xF5, 0x4B,
    0x14, 0xF4, 0x4B, 0x0D, 0xF4, 0x4C, 0x06, 0xF4,
    0x4D, 0x04, 0xF1, 0x51, 0x0D, 0xE9, 0x55, 0x16,
    0xE1, 0x59, 0x1F, 0xD9, 0x5D, 0x28, 0xD1, 0x61,
    0x31, 0xC9, 0x65, 0x3A, 0xC1, 0x69, 0x42, 0xB9,
    0x6D, 0x4B, 0xB1, 0x71, 0x54, 0xA9, 0x75, 0x5D,
    0xA1, 0x79, 0x66, 0x99, 0x7D, 0x6F, 0x91, 0x81,
    0x78, 0x89, 0x86, 0x80, 0x81, 0x8A, 0x88, 0x7A,
    0x8E, 0x90, 0x72, 0x92, 0x98, 0x6A, 0x96, 0xA1,
    0x62, 0x9A, 0xA9, 0x5A, 0x9E, 0xB1, 0x52, 0xA2,
    0xBA, 0x4A, 0xA6, 0xC2, 0x42, 0xAA, 0xCA, 0x3A,
    0xAE, 0xD3, 0x32, 0xB2, 0xDB, 0x2A, 0xB6, 0xE3,
    0x22, 0xBA, 0xEB, 0x1A, 0xBE, 0xF4, 0x12, 0xC2,
    0xFC, 0x0A, 0xC6, 0xF5, 0x02, 0xCA, 0xE6, 0x09,
    0xCE, 0xD8, 0x18, 0xD1, 0xCA, 0x27, 0xD5, 0xBB,
    0x36, 0xD8, 0xAD, 0x45, 0xDC, 0x9E, 0x54, 0xE0,
    0x90, 0x62, 0xE3, 0x82, 0x6F, 0xE6, 0x71, 0x7C,
    0xEA, 0x61, 0x89, 0xEE, 0x51, 0x96, 0xF2, 0x40,
    0xA3, 0xF5, 0x30, 0xB0, 0xF9, 0x20, 0xBD, 0xFD,
    0x0F, 0xE3, 0xFF, 0x01, 0xE9, 0xFF, 0x01, 0xEE,
    0xFF, 0x01, 0xF4, 0xFF, 0x00, 0xFA, 0xFF, 0x00,
    0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x0A, 0xFF, 0xFF,
    0x15, 0xFF, 0xFF, 0x20, 0xFF, 0xFF, 0x2B, 0xFF,
    0xFF, 0x36, 0xFF, 0xFF, 0x41, 0xFF, 0xFF, 0x4C,
    0xFF, 0xFF, 0x57, 0xFF, 0xFF, 0x62, 0xFF, 0xFF,
    0x6D, 0xFF, 0xFF, 0x78, 0xFF, 0xFF, 0x81, 0xFF,
    0xFF, 0x8A, 0xFF, 0xFF, 0x92, 0xFF, 0xFF, 0x9A,
    0xFF, 0xFF, 0xA3, 0xFF, 0xFF, 0xAB, 0xFF, 0xFF,
    0xB4, 0xFF, 0xFF, 0xBC, 0xFF, 0xFF, 0xC4, 0xFF,
    0xFF, 0xCD, 0xFF, 0xFF, 0xD5, 0xFF, 0xFF, 0xDD,
    0xFF, 0xFF, 0xE6, 0xFF, 0xFF, 0xEE, 0xFF, 0xFF,
    0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9,
    0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xED, 0xFF, 0xFF,
    0xE7, 0xFF, 0xFF, 0xE1, 0xFF, 0xFF, 0xDB, 0xFF,
    0xFF, 0xD5, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0xCA,
    0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xBE, 0xFF, 0xFF,
    0xB8, 0xFF, 0xFF, 0xB2, 0xFF, 0xFF, 0xAC, 0xFF,
    0xFF, 0xA6, 0xFF, 0xFF, 0xA0, 0xFF, 0xFF, 0x9B,
    0xFF, 0xFF, 0x96, 0xFF, 0xFF, 0x90, 0xFF, 0xFF,
    0x8B, 0xFF, 0xFF, 0x86, 0xFF, 0xFF, 0x81, 0xFF,
    0xFF, 0x7B, 0xFF, 0xFF, 0x76, 0xFF, 0xFF, 0x71,
    0xFF, 0xFF, 0x6B, 0xFF, 0xFF, 0x66, 0xFF, 0xFF,
    0x61, 0xFF, 0xFF, 0x5C, 0xFF, 0xFF, 0x56, 0xFF,
    0xFF, 0x51, 0xFF, 0xFF, 0x4C, 0xFF, 0xFF, 0x47,
    0xFF, 0xFF, 0x41, 0xF9, 0xFF, 0x40, 0xF0, 0xFF,
    0x40, 0xE8, 0xFF, 0x40, 0xDF, 0xFF, 0x40, 0xD7,
    0xFF, 0x40, 0xCF, 0xFF, 0x40, 0xC6, 0xFF, 0x40,
    0xBE, 0xFF, 0x40, 0xB5, 0xFF, 0x40, 0xAD, 0xFF,
    0x40, 0xA4, 0xFF, 0x40, 0x9C, 0xFF, 0x40, 0x95,
    0xFF, 0x40, 0x8D, 0xFF, 0x40, 0x86, 0xFF, 0x40,
    0x7E, 0xFF, 0x40, 0x77, 0xFF, 0x40, 0x6F, 0xFF,
    0x40, 0x68, 0xFF, 0x40, 0x60, 0xFF, 0x40, 0x59,
    0xFF, 0x40, 0x51, 0xFF, 0x40, 0x4A, 0xFA, 0x43,
    0x42, 0xF3, 0x48, 0x3E, 0xED, 0x4E, 0x3D, 0xE6,
    0x53, 0x3B, 0xDF, 0x58, 0x39, 0xD8, 0x5E, 0x37,
    0xD2, 0x63, 0x35, 0xCB, 0x68, 0x34, 0xC4, 0x6D,
    0x32, 0xBD, 0x73, 0x30, 0xB7, 0x78, 0x2E, 0xB0,
    0x7D, 0x2D, 0xA9, 0x83, 0x2B, 0xA2, 0x88, 0x29,
    0x9C, 0x8D, 0x27, 0x95, 0x92, 0x25, 0x8E, 0x98,
    0x24, 0x87, 0x9D, 0x22, 0x81, 0xA2, 0x20, 0x7A,
    0xA6, 0x1E, 0x74, 0xAB, 0x1D, 0x6E, 0xB0, 0x1B,
    0x68, 0xB5, 0x1A, 0x61, 0xB9, 0x18, 0x5B, 0xBE,
    0x17, 0x55, 0xC3, 0x15, 0x4F, 0xC8, 0x13, 0x48,
    0xCD, 0x12, 0x42, 0xD1, 0x10, 0x3C, 0xD6, 0x0F,
    0x36, 0xDB, 0x0D, 0x2F, 0xE0, 0x0C, 0x29, 0xE4,
    0x0A, 0x23, 0xE9, 0x08, 0x1D, 0xEE, 0x07, 0x16,
    0xF3, 0x05, 0x10, 0xF7, 0x04, 0x0A, 0xFC, 0x02,
    0x04, 0xFB, 0x01, 0x04, 0xEF, 0x00, 0x12, 0xE4,
    0x00, 0x1F, 0xD9, 0x00, 0x2D, 0xCE, 0x00, 0x3B,
    0xC2, 0x00, 0x48, 0xB7, 0x00, 0x56, 0xAC, 0x00,
    0x64, 0xA0, 0x00, 0x72, 0x95, 0x00, 0x7F, 0x8A,
    0x00, 0x88, 0x7F, 0x00, 0x92, 0x75, 0x00, 0x9C,
    0x6B, 0x00, 0xA6, 0x61, 0x00, 0xB0, 0x57, 0x00,
    0xBA, 0x4E, 0x00, 0xC4, 0x44, 0x00, 0xCE, 0x3A,
    0x00, 0xD8, 0x30, 0x00, 0xE2, 0x27, 0x00, 0xEC,
    0x1D, 0x00, 0xF6, 0x13, 0x00, 0xFF, 0x09, 0x00,
};

void initialize_julia()
{
	// function declarations 
	void display_julia();
	void uninitialize_julia();

	// init 
		paused      = false;

		zoom        =  0.0f;
		time_offset = -6.414309f;
		x_offset    =  0.009517f;
		y_offset    =  0.006217;

	// code 

	//------------ Vertex Shader --------------- 
	const GLchar* vertexShaderSourceCode =
		"#version 150 core" \
		"\n " \
		"// Zoom factor                                                                         \n" \
		"uniform float zoom;                                                                    \n" \
		"                                                                                       \n" \
		"// Offset vector                                                                       \n" \
		"uniform vec2 offset;                                                                   \n" \
		"                                                                                       \n" \
		"out vec2 initial_z;                                                                    \n" \
		"                                                                                       \n" \
		"void main(void)                                                                        \n" \
		"{                                                                                      \n" \
		"    const vec4 vertices[4] = vec4[4](vec4(-1.0, -1.0, 0.5, 1.0),vec4( 1.0, -1.0, 0.5, 1.0),vec4( 1.0,  1.0, 0.5, 1.0), vec4(-1.0,  1.0, 0.5, 1.0));                      \n" \
		"    initial_z = (vertices[gl_VertexID].xy * zoom) + offset;                            \n" \
		"    gl_Position = vertices[gl_VertexID];                                               \n" \
		"}";

	// 2: Create vertex shadder object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// 3: give vertex shader source code to opengl
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	// 4: let opengl compile vertex shader
	glCompileShader(vertexShaderObject);

	// 5.c
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	// 5.a
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{	// 5.b
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{   // 5.c
			szInfoLog = (GLchar*)malloc(infoLogLength + 1);

			if (szInfoLog != NULL)
			{   // 5.d
				glGetShaderInfoLog(vertexShaderObject, infoLogLength + 1, NULL, szInfoLog);
				// 5.e
				fprintf(gpFILE, "vertex shadder Compilation Error Log : %s \n", szInfoLog);
				// 5.f
				free(szInfoLog);
				szInfoLog = NULL;
				// 5.g
			}
		}
		uninitialize_julia();
	}

	//-------------- Fragment Shader ---------------
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec2 initial_z;                                                                     \n" \
		"                                                                                       \n" \
		"out vec4 color;                                                                        \n" \
		"                                                                                       \n" \
		"uniform sampler1D tex_gradient;                                                        \n" \
		"uniform vec2 C;                                                                        \n" \
		"                                                                                       \n" \
		"void main(void)                                                                        \n" \
		"{                                                                                      \n" \
		"    vec2 Z = initial_z;                                                                \n" \
		"    int iterations = 0;                                                                \n" \
		"    const float threshold_squared = 16.0;                                              \n" \
		"    const int max_iterations = 256;                                                    \n" \
		"    while (iterations < max_iterations && dot(Z, Z) < threshold_squared) {             \n" \
		"        vec2 Z_squared;                                                                \n" \
		"        Z_squared.x = Z.x * Z.x - Z.y * Z.y;                                           \n" \
		"        Z_squared.y = 2.0 * Z.x * Z.y;                                                 \n" \
		"        Z = Z_squared + C;                                                             \n" \
		"        iterations++;                                                                  \n" \
		"    }                                                                                  \n" \
		"    if (iterations == max_iterations)                                                  \n" \
		"        color = vec4(0.0, 0.0, 0.0, 1.0);                                              \n" \
		"    else                                                                               \n" \
		"        color = texture(tex_gradient, float(iterations) / float(max_iterations));      \n" \
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
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Fragment shader Compilation Error Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize_julia();
	}

	//------------ Shader Progarm --------------
	// 7: create shader program
	shaderProgramObject = glCreateProgram();

	// 8 : Attach both shadders to this program
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// 10: link the shader program
	glLinkProgram(shaderProgramObject);

	// 11:

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize_julia();
	}

	// get shader uniform locations 
	//                      kunakadun gheu , kontya uniform location ch gheu  
	zoomUniform = glGetUniformLocation(shaderProgramObject, "zoom");
    offsetUniform = glGetUniformLocation(shaderProgramObject, "offset");
    CUniform = glGetUniformLocation(shaderProgramObject, "C");
   // CUniform = glGetUniformLocation(shaderProgramObject, "tex_gradient");
	

	// 13 : VAO - create vertex array object 
	glGenVertexArrays(1, &vao);

	// 14: bind with vao
	glBindVertexArray(vao);

	glGenTextures(1, &palette_texture);
	glBindTexture(GL_TEXTURE_1D, palette_texture);
	glTexStorage1D(GL_TEXTURE_1D, 8, GL_RGB8, 256);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, palette); 
	glGenerateMipmap(GL_TEXTURE_1D); 

	// unbind vao
	glBindVertexArray(0);

	//--------------------------------------------------------------------------------


}

void display_julia()
{
	// function declarations 
	void update_julia();

	// Variable declarations 
	static float t = 0.0f;
	float        r = 0.0f;

	static float currentTime = 5.0;

	/*if (animation_flag_1)
	{
		if(currentTime < 9.5)
			currentTime = currentTime + 0.001;
		else
		{
			animation_flag_1 = false;
			animation_flag_2 = true;
		}
	}
	else if (animation_flag_2)
	{
		if (currentTime > 5.0)
			currentTime = currentTime - 0.001;
		else
		{
			animation_flag_1 = true;
			animation_flag_2 = false;
		}
	}*/

	// Code 
	
	if (!paused)
	{
		t = (float)currentTime;
	}

	r = t + time_offset;

	float C[2] = { (sinf(r * 0.1f) + cosf(r * 0.23f)) * 0.5f, (cosf(r * 0.13f) + sinf(r * 0.21f)) * 0.5f };
	float offset[2] = { x_offset, y_offset };


	//zoom = (sinf(2.0f) + 1.3f) * 0.7f;

	glUseProgram(shaderProgramObject);    	// use shader program 

	// uniforms 
	glUniform2fv(CUniform, 1, C);
	glUniform2fv(offsetUniform, 1, offset); 
	glUniform1f(zoomUniform, zoom);  

	
	glBindVertexArray(vao);                         // bind with vao
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    	// Draw the Geometry
	glBindVertexArray(0);                  	        // unbind vao

	glUseProgram(0);         	                    // unuse the shader program

	update_julia();
}

void update_julia()
{
	// code 
	

	if (zoom < 0.05f)
	{
		zoom = zoom + 0.00005f;

		if (time_offset < 0.0f)
			time_offset = time_offset + 0.000002f;

	}
	else if (zoom < 1.7f)
	{
		zoom = zoom + 0.0007f;
		
		if (zoom < 0.2f)
			time_offset = time_offset + 0.0001f;
		else if(zoom < 0.8)
			time_offset = time_offset - 0.00005f;
		else 
			time_offset = time_offset + 0.0001f;

	}

}

void uninitialize_julia()
{

	fprintf(gpFILE, "Zoom  : %f \n", zoom);
	fprintf(gpFILE, "time_offset  : %f \n", time_offset);
	fprintf(gpFILE, "x_offset  : %f \n", x_offset);
	fprintf(gpFILE, "y_offset  : %f \n", y_offset);




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

	// delete texture
	if (palette_texture)
	{
		glDeleteTextures(1, &palette_texture);
		palette_texture = 0;
	}

	// delete vao
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

}
