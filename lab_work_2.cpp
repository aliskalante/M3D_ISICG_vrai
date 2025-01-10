#include "lab_work_2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		glDeleteProgram( Program );
		glDeleteBuffers( 1, &VBO );
		glDisableVertexArrayAttrib( VAO, 0 );
		glDeleteVertexArrays( 1, &VAO );
		glDeleteBuffers( 1, &VBO_color );
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 2..." << std::endl;
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw2.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );

		vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		fragmentShader	  = glCreateShader( GL_FRAGMENT_SHADER );
		const char * VSrc = vertexShaderStr.c_str();
		const char * FSrc = fragmentShaderStr.c_str();

		glShaderSource( vertexShader, 1, &VSrc, NULL );
		glShaderSource( fragmentShader, 1, &FSrc, NULL );
		glCompileShader( vertexShader );
		glCompileShader( fragmentShader );

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

		Positionsommets = { Vec2f( -0.5, 0.5 ), Vec2f( 0.5, 0.5 ), Vec2f( 0.5, -0.5 ), Vec2f( -0.5, -0.5 ) };

		indexessommets = { 0, 1, 2, 0, 3, 2 };
		color_sommets
			= { Vec3f( 1.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ), Vec3f( 0.f, 0.f, 1.f ), Vec3f( 1.f, 0.f, 1.f ) };

		glCreateBuffers( 1, &VBO );
		glNamedBufferData( VBO, Positionsommets.size() * sizeof( Vec2f ), Positionsommets.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &VBO_color );
		glNamedBufferData( VBO_color, color_sommets.size() * sizeof( Vec3f ), color_sommets.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &VAO );
		glEnableVertexArrayAttrib( VAO, 0 );
		glVertexArrayAttribFormat( VAO, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( VAO, 0, VBO, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( VAO, 0, 0 );

		glEnableVertexArrayAttrib( VAO, 1 );
		glVertexArrayVertexBuffer( VAO, 1, VBO_color, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( VAO, 1, 1 );

		glCreateBuffers( 1, &EBO );
		glNamedBufferData( EBO, indexessommets.size() * sizeof( int ), indexessommets.data(), GL_STATIC_DRAW );
		glVertexArrayElementBuffer( VAO, EBO );

		loc_translation = glGetUniformLocation( Program, "uTranslationX" );
		loc_luminosité	= glGetUniformLocation( Program, "c_luminosite" );

		glProgramUniform1f( Program, loc_luminosité, Luminosité );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::animate( const float p_deltaTime )
	{
		_time += p_deltaTime;
		double valeur = glm::sin( _time );
		glProgramUniform1f( Program, loc_translation, valeur );
	}

	void LabWork2::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram( Program );
		glBindVertexArray( VAO );
		glDrawElements( GL_TRIANGLES, indexessommets.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		if ( ImGui::SliderFloat( "Luminosite", &Luminosité, 0.f, 1.f ) )
		{
			glProgramUniform1f( Program, loc_luminosité, Luminosité );
		}

		if ( ImGui::ColorEdit3( "couleur background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		ImGui::Begin( "Settings lab work 2" );
		ImGui::Text( "No setting available" );
		ImGui::End();
	}

} // namespace M3D_ISICG
