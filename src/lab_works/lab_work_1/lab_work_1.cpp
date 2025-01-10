#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1()
	{
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		glDeleteProgram( Program );
		glDeleteBuffers( 1, &VBO );
		glDisableVertexArrayAttrib( VAO, 0 );
		glDeleteVertexArrays( 1, &VAO );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		// Load shaders
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );

		vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER );
		const char * VSrc = vertexShaderStr.c_str();
		const char * FSrc = fragmentShaderStr.c_str();
		glShaderSource( vertexShader, 1, &VSrc, NULL );
		glShaderSource( fragmentShader, 1, &FSrc, NULL );
		glCompileShader( vertexShader );
		glCompileShader( fragmentShader );

		// Check compilation vertex shader
		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader: " << log << std::endl;
			return false;
		}

		// Check compilation fragment shader
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling fragment shader: " << log << std::endl;
			return false;
		}

		// créer et linker le program
		Program = glCreateProgram();
		glAttachShader( Program, vertexShader );
		glAttachShader( Program, fragmentShader );
		glLinkProgram( Program );

		GLint linked;
		glGetProgramiv( Program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( Program, sizeof( log ), NULL, log );
			std::cerr << "Error linking program: " << log << std::endl;
			return false;
		}

		// Set up vertex data
		Position_sommets = { Vec2f( -0.5, 0.5 ), Vec2f( 0.5, 0.5 ), Vec2f( 0.5, -0.5 ) };

		glCreateBuffers( 1, &VBO );
		glNamedBufferData( VBO, Position_sommets.size() * sizeof( Vec2f ), Position_sommets.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &VAO );
		glEnableVertexArrayAttrib( VAO, 0 );
		glVertexArrayAttribFormat( VAO, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( VAO, 0, VBO, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( VAO, 0, 0 );

		glUseProgram( Program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
