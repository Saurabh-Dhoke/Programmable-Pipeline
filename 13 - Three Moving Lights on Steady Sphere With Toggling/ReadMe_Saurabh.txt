
/* 
	Program - Three Moving Lights (Assignment)
	Date    - 09.04.2024

	Base Code : 
				- Toggling of PV - PF  Program
				- Two Steady Lights Program 

*/

-----------------------------------------------------------------------------
Globally : 
	// new Variables add kele lights move krnya sathi 
		GLfloat PI = 22.0f / 7.0f;
		GLfloat lightAngle_X = 0.0f;
		GLfloat lightAngle_Y = 0.0f;
		GLfloat lightAngle_Z = 0.0f;

	// two steady madhun light ch struct gheun 3 cha array kela struct cha 
		struct Light
		{
			vec3 ambiant;
			vec3 diffuse;
			vec3 specular;
			vec4 position;
		};
		struct Light light[3];

	// jashe two steady madhi 2 che arrya hote te 3 che kele 
	// PV and PF 2ghanaa pn 
		GLuint lightDiffuseUniform_PV[3];
		GLuint lightAmbiantUniform_PV[3];
		GLuint lightSpecularUniform_PV[3];
		GLuint lightPositionUniform_PV[3];
		
		GLuint lightDiffuseUniform_PF[3];
		GLuint lightAmbiantUniform_PF[3];
		GLuint lightSpecularUniform_PF[3];
		GLuint lightPositionUniform_PF[3];


-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
# Update() : 
	
	- jevha lightEnabled asal tevach lights he update houn move zale pahije mhanun 
	  if chya condition madhi ghetal sagal 
	- aapalyala lights move karaychet mahunu position change zali pahije mhanun sin cos vaparalay 
	- jya axis bhovati move kraychay ti position chi value 0 dyaychi 
	- aani jover angle chi value less than 2PI mhanje 360 asal tover angle increse kraycha an 360 zala ki 0 karaycha 
	- sin an cos chi return value double madhi return hoti mhanun type cast karun ghetali float madhi 
	- an asach 3 nhi lights sathi kelay 0 for x axis , 1 for y , and 2 for z axis 


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

-----------------------------------------------------------------------------










































