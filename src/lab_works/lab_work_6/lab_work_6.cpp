#include "lab_work_6.hpp"
#include "utils/read_file.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>

namespace M3D_ISICG
{
	LabWork6::~LabWork6()
	{
		glDeleteFramebuffers( 1, &_gBufferFBO );
		glDeleteTextures( 6, _gBufferTextures );
		glDeleteProgram( _geometryPassProgram );
		glDeleteProgram( _shadingPassProgram );
	}

	bool LabWork6::init()
	{
		std::cout << "Initializing LabWork6..." << std::endl;

		// Load and compile shaders for geometry pass
		const std::string vertexShaderSource   = readFile( "src/lab_works/lab_work_6/shaders/geometry_pass.vert" );
		const std::string fragmentShaderSource = readFile( "src/lab_works/lab_work_6/shaders/geometry_pass.frag" );
		_geometryPassProgram				   = _compileShaders( vertexShaderSource, fragmentShaderSource );

		// Prepare G-buffer
		prepareGBuffer();

		// Load and setup model
		_bunnyModel.load( "bunny", "data/models/bunny.obj" );
		_bunnyModel.setupGL();

		// Initialize shading pass
		initShadingPass();

		// Initialize fullscreen quad for shading
		initFullScreenQuad();

		glEnable( GL_DEPTH_TEST );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork6::render()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, _gBufferFBO );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		_geometryPass();
		_shadingPass();
	}

	void LabWork6::prepareGBuffer()
	{
		glGenFramebuffers( 1, &_gBufferFBO );
		glBindFramebuffer( GL_FRAMEBUFFER, _gBufferFBO );

		glGenTextures( 6, _gBufferTextures );

		GLenum formats[ 6 ]		= { GL_RGB16F, GL_RGB16F, GL_RGB, GL_RGB, GL_RGBA16F, GL_DEPTH_COMPONENT };
		GLenum attachments[ 5 ] = {
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4
		};

		for ( int i = 0; i < 5; ++i )
		{
			glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ i ] );
			glTexImage2D( GL_TEXTURE_2D,
						  0,
						  formats[ i ],
						  _camera.getScreenWidth(),
						  _camera.getScreenHeight(),
						  0,
						  GL_RGB,
						  GL_FLOAT,
						  nullptr );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glFramebufferTexture2D( GL_FRAMEBUFFER, attachments[ i ], GL_TEXTURE_2D, _gBufferTextures[ i ], 0 );
		}

		glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ 5 ] );
		glTexImage2D( GL_TEXTURE_2D,
					  0,
					  GL_DEPTH_COMPONENT,
					  _camera.getScreenWidth(),
					  _camera.getScreenHeight(),
					  0,
					  GL_DEPTH_COMPONENT,
					  GL_FLOAT,
					  nullptr );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _gBufferTextures[ 5 ], 0 );

		GLenum drawBuffers[ 5 ] = {
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4
		};
		glDrawBuffers( 5, drawBuffers );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		{
			std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void LabWork6::_geometryPass()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, _gBufferFBO );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( _geometryPassProgram );

		glm::mat4 viewMatrix	   = _camera.getViewMatrix();
		glm::mat4 projectionMatrix = _camera.getProjectionMatrix();
		glm::mat4 modelMatrix	   = glm::mat4( 1.0f );
		glm::mat4 mvpMatrix		   = projectionMatrix * viewMatrix * modelMatrix;

		glUniformMatrix4fv(
			glGetUniformLocation( _geometryPassProgram, "uMVPMatrix" ), 1, GL_FALSE, glm::value_ptr( mvpMatrix ) );
		glUniformMatrix4fv(
			glGetUniformLocation( _geometryPassProgram, "uModelMatrix" ), 1, GL_FALSE, glm::value_ptr( modelMatrix ) );

		_bunnyModel.render( _geometryPassProgram );

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void LabWork6::_shadingPass()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( _shadingPassProgram );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ 0 ] );
		glUniform1i( glGetUniformLocation( _shadingPassProgram, "gPositionTex" ), 0 );

		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ 1 ] );
		glUniform1i( glGetUniformLocation( _shadingPassProgram, "gNormalTex" ), 1 );

		glActiveTexture( GL_TEXTURE2 );
		glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ 2 ] );
		glUniform1i( glGetUniformLocation( _shadingPassProgram, "gAmbientTex" ), 2 );

		glActiveTexture( GL_TEXTURE3 );
		glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ 3 ] );
		glUniform1i( glGetUniformLocation( _shadingPassProgram, "gDiffuseTex" ), 3 );

		glActiveTexture( GL_TEXTURE4 );
		glBindTexture( GL_TEXTURE_2D, _gBufferTextures[ 4 ] );
		glUniform1i( glGetUniformLocation( _shadingPassProgram, "gSpecularTex" ), 4 );

		glBindVertexArray( _quadVAO );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork6::initShadingPass()
	{
		const std::string vertexShaderCode	 = readFile( "src/lab_works/lab_work_6/shaders/shading_pass.vert" );
		const std::string fragmentShaderCode = readFile( "src/lab_works/lab_work_6/shaders/shading_pass.frag" );
		_shadingPassProgram					 = _compileShaders( vertexShaderCode, fragmentShaderCode );
	}

	void LabWork6::initFullScreenQuad()
	{
		float quadVertices[]
			= { // positions        // texCoords
				-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f
			  };

		unsigned int quadIndices[] = { 0, 1, 2, 2, 3, 0 };

		glGenVertexArrays( 1, &_quadVAO );
		glGenBuffers( 1, &_quadVBO );
		glGenBuffers( 1, &_quadEBO );

		glBindVertexArray( _quadVAO );

		glBindBuffer( GL_ARRAY_BUFFER, _quadVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertices ), quadVertices, GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _quadEBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( quadIndices ), quadIndices, GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void *)0 );
		glEnableVertexAttribArray( 0 );

		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void *)( 2 * sizeof( float ) ) );
		glEnableVertexAttribArray( 1 );

		glBindVertexArray( 0 );
	}

	void LabWork6::displayUI()
	{
		ImGui::Begin( "Deferred Shading UI" );
		const char * items[] = { "Position", "Normal", "Ambient", "Diffuse", "Specular" };
		ImGui::Combo( "G-Buffer Texture", (int *)&_selectedTextureIndex, items, IM_ARRAYSIZE( items ) );
		ImGui::End();
	}

	GLuint LabWork6::_compileShaders( const std::string & vertexSource, const std::string & fragmentSource )
	{
		GLuint program		  = glCreateProgram();
		GLuint vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		const char * vSource = vertexSource.c_str();
		glShaderSource( vertexShader, 1, &vSource, nullptr );
		glCompileShader( vertexShader );
		checkShaderCompilation( vertexShader );

		const char * fSource = fragmentSource.c_str();
		glShaderSource( fragmentShader, 1, &fSource, nullptr );
		glCompileShader( fragmentShader );
		checkShaderCompilation( fragmentShader );

		glAttachShader( program, vertexShader );
		glAttachShader( program, fragmentShader );
		glLinkProgram( program );
		checkProgramLinking( program );

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		return program;
	}
} // namespace M3D_ISICG
