/* Assignment 
   Program : per Fragment to per vertex lighting toggling on keys 
   Date : 08.04.2024

   Keys :   
            L : enable disable lighting 
            v : per vertex lighting (by default)
            f : per fragment lighting 
            Q : Quit 
            Esc : Full screen toggling 

    Execution : 
                starting la white sphere 
                Press L enable lighting by default per vertex lighting disal 
                Press F.... to toggle per vertex to per fragment toggling 

    Base Code : 
                perVertex Lighting on sphere 
                perFragment Lighting on sphere 

*/

//---------------------------------------------------------------------------
                             Changes :
 //---------------------------------------------------------------------------
 #Globbaly 
       - PerVertex sathi   : _PV suffix lavla last la 
       - PerFragment sathi : _PF suffix lavla last la 
       - Pratek thikani 
      
      1) Shader program che 2 object kele globallay PV and PF sathi 
      2) choosenShadder navach globally ek variable ghetal toggling sathi,
         by default perVertex sathi intialize 'v' na kel .
      3) globally aslele sagale uniforms double kele PV and PF sathi 

@Code :-
    GLuint shaderProgramObject_PV  = 0;
    GLuint shaderProgramObject_PF  = 0;
    char choosenShadder = 'v';   

    // for per vertex 
    GLuint modelMatrixUniform_PV = 0;
    GLuint viewMatrixUniform_PV = 0;
    GLuint projectionMatrixUniform_PV = 0;

    GLuint lightDiffuseUniform_PV = 0;
    GLuint lightAmbiantUniform_PV = 0;
    GLuint lightSpecularUniform_PV = 0;
    GLuint lightPositionUniform_PV = 0;

    GLuint materialAmbiantUniform_PV = 0;
    GLuint materialDiffuseUniform_PV = 0;
    GLuint materialSpecularUniform_PV = 0;
    GLuint materialShinninesUniform_PV = 0;

    // for per fragment 
    GLuint modelMatrixUniform_PF = 0;
    GLuint viewMatrixUniform_PF = 0;
    GLuint projectionMatrixUniform_PF = 0;

    GLuint lightDiffuseUniform_PF = 0;
    GLuint lightAmbiantUniform_PF = 0;
    GLuint lightSpecularUniform_PF = 0;
    GLuint lightPositionUniform_PF = 0;

    GLuint materialAmbiantUniform_PF = 0;
    GLuint materialDiffuseUniform_PF = 0;
    GLuint materialSpecularUniform_PF = 0;
    GLuint materialShinninesUniform_PF = 0;


//------------------------------------------------------------------------------
#WndProc() :- 
     - yamadhi thode changes kelet 
     - L : enable disable lighting 
     - v : per vertex lighting (by default)
     - f : per fragment lighting 
     - Q : Quit 
     - Esc : Full screen toggling 

@Code :- 
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
			fprintf(gpFILE, "1\n");

			DestroyWindow(hwnd);
			break;
	
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#initialize():-
	- pervertex and perFragment sathi mhanun aapan pratekache 2..2 ashe 4 shader write kele 
	- aani pervertex sathiche je 2 yet tyanna _PV suffix laval an ,
	  perfragment sathiche je 2 yet tyana _PF suffix lavla 
	- code :
		1) vertexShaderSourceCode_PV
		2) fragmentShaderSourceCode_PV
		3) vertexShaderSourceCode_PF
		4) fragmentShaderSourceCode_PF
	- asach tya tya shadders madhi corresponding changes kele 
	- shaderProgramObject pn aapan 2 kelet var globally ya doghansathi,
	  vegvegale mng tyat pn corresspondingly changes kele


//------------------------------------------------------------------------------
#Display() : 
    
    // toggling sathi PNV to PF as kel 
	if (choosenShadder == 'v')
		{
			glUseProgram(shaderProgramObject_PV);
		}
	else if (choosenShadder == 'f')
		{
			glUseProgram(shaderProgramObject_PF);
		}

	// shadders madhi jevha toggling hoil,
	// tevha he uniforms pn change zalele astil tya mul as kel pn change hotiil
    
	glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, PerspectiveProjectionMatrix);

	if (bLightingEnabled == TRUE && choosenShadder == 'v')
	{
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, PerspectiveProjectionMatrix);

		glUniform1i(keyPressedUniform, 1);
		glUniform3fv(lightDiffuseUniform_PV,1, lightDiffuse);
		glUniform3fv(lightAmbiantUniform_PV,1, lightAmbiant);
		glUniform3fv(lightSpecularUniform_PV,1, lightSpecular);
		glUniform4fv(lightPositionUniform_PV, 1, lightPosition);

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
		glUniform3fv(lightDiffuseUniform_PF, 1, lightDiffuse);
		glUniform3fv(lightAmbiantUniform_PF, 1, lightAmbiant);
		glUniform3fv(lightSpecularUniform_PF, 1, lightSpecular);
		glUniform4fv(lightPositionUniform_PF, 1, lightPosition);

		glUniform3fv(materialDiffuseUniform_PF, 1, materialDiffuse);
		glUniform3fv(materialAmbiantUniform_PF, 1, materialAmbiant);
		glUniform3fv(materialSpecularUniform_PF, 1, materialSpecular);
		glUniform1f(materialShinninesUniform_PF, materialShinniness);
	}
	else
	{
		glUniform1i(keyPressedUniform, 0);
	}



//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#uninitialize():-
    - shaderProgramObject che aata aplya kd 2 object yet 
        1) shaderProgramObject_PF
        2) shaderProgramObject_PV
    - mhanun ith 2 nda he uninitialize kele
//------------------------------------------------------------------------------















