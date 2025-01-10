#include "lab_work_3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3()
	{
		glDeleteShader( _vertexShader );
		glDeleteShader( _fragmentShader );
		glDeleteProgram( _program );
		glDeleteBuffers( 1, &_mesh.VBO_positions );
		glDeleteBuffers( 1, &_mesh.VBO_colors );
		glDeleteVertexArrays( 1, &_mesh.VAO );
		glDeleteBuffers( 1, &_mesh.EBO );
	}

	bool LabWork3::init()
	{
		std::cout << "Initializing Lab Work 3..." << std::endl;

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderSource   = readFile( _shaderFolder + "lw3.vert" );
		const std::string fragmentShaderSource = readFile( _shaderFolder + "lw3.frag" );

		_vertexShader	= glCreateShader( GL_VERTEX_SHADER );
		_fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		const char * vSource = vertexShaderSource.c_str();
		const char * fSource = fragmentShaderSource.c_str();

		glShaderSource( _vertexShader, 1, &vSource, NULL );
		glShaderSource( _fragmentShader, 1, &fSource, NULL );

		glCompileShader( _vertexShader );
		glCompileShader( _fragmentShader );

		GLint compiled;
		glGetShaderiv( _vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( _vertexShader, sizeof( log ), NULL, log );
			std::cerr << "Vertex Shader Compilation Error: " << log << std::endl;
			return false;
		}

		glGetShaderiv( _fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( _fragmentShader, sizeof( log ), NULL, log );
			std::cerr << "Fragment Shader Compilation Error: " << log << std::endl;
			return false;
		}

		_program = glCreateProgram();
		glAttachShader( _program, _vertexShader );
		glAttachShader( _program, _fragmentShader );
		glLinkProgram( _program );

		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std::cerr << "Program Linking Error: " << log << std::endl;
			return false;
		}

		_mesh = _createCube();
		_initBuffers();
		_initCamera();

		_locationMVP = glGetUniformLocation( _program, "MVP" );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork3::animate( const float p_deltaTime )
	{
		_mesh.transformMatrix
			= glm::rotate( _mesh.transformMatrix, glm::radians( p_deltaTime ), glm::vec3( 0.0f, 1.0f, 1.0f ) );
		_MVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _mesh.transformMatrix;

		glProgramUniformMatrix4fv( _program, _locationMVP, 1, GL_FALSE, glm::value_ptr( _MVPMatrix ) );
	}

	void LabWork3::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable( GL_DEPTH_TEST );
		glBindVertexArray( _mesh.VAO );

		glUseProgram( _program );
		_MVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _mesh.transformMatrix;

		glProgramUniformMatrix4fv( _program, _locationMVP, 1, GL_FALSE, glm::value_ptr( _MVPMatrix ) );

		glDrawElements( GL_TRIANGLES, _mesh.indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings Lab Work 3" );

		if ( ImGui::ColorEdit3( "Background Color", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		if ( ImGui::SliderFloat( "FOV", &_fov, 5.0f, 160.0f, "%.1f" ) )
		{
			_camera.setFovy( _fov );
		}

		ImGui::End();
	}

	LabWork3::Mesh LabWork3::_createCube()
	{
		Mesh cube;

		cube.positions = { Vec3f( -0.5f, -0.5f, 0.0f ), Vec3f( -0.5f, 0.5f, 0.0f ),	 Vec3f( 0.5f, 0.5f, 0.0f ),
						   Vec3f( 0.5f, -0.5f, 0.0f ),	Vec3f( -0.5f, -0.5f, 1.0f ), Vec3f( -0.5f, 0.5f, 1.0f ),
						   Vec3f( 0.5f, 0.5f, 1.0f ),	Vec3f( 0.5f, -0.5f, 1.0f ) };

		cube.colors = { getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f(),
						getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f() };

		cube.indices = { 0, 1, 4, 1, 4, 5, 1, 2, 5, 2, 5, 6, 0, 3, 4, 4, 3, 7,
						 2, 3, 7, 2, 6, 7, 0, 3, 1, 1, 2, 3, 5, 4, 7, 5, 6, 7 };

		cube.transformMatrix = glm::mat4( 1.0f );
		cube.transformMatrix = glm::scale( cube.transformMatrix, glm::vec3( 0.8f ) );

		return cube;
	}

	void LabWork3::_initBuffers()
	{
		glCreateBuffers( 1, &_mesh.VBO_positions );
		glNamedBufferData(
			_mesh.VBO_positions, _mesh.positions.size() * sizeof( Vec3f ), _mesh.positions.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &_mesh.VBO_colors );
		glNamedBufferData(
			_mesh.VBO_colors, _mesh.colors.size() * sizeof( Vec3f ), _mesh.colors.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &_mesh.VAO );
		glEnableVertexArrayAttrib( _mesh.VAO, 0 );
		glEnableVertexArrayAttrib( _mesh.VAO, 1 );

		glVertexArrayAttribFormat( _mesh.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _mesh.VAO, 0, _mesh.VBO_positions, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( _mesh.VAO, 0, 0 );

		glVertexArrayAttribFormat( _mesh.VAO, 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _mesh.VAO, 1, _mesh.VBO_colors, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( _mesh.VAO, 1, 1 );

		glCreateBuffers( 1, &_mesh.EBO );
		glNamedBufferData(
			_mesh.EBO, _mesh.indices.size() * sizeof( unsigned int ), _mesh.indices.data(), GL_STATIC_DRAW );

		glVertexArrayElementBuffer( _mesh.VAO, _mesh.EBO );
	}

	void LabWork3::_initCamera()
	{
		_camera.setPosition( Vec3f( 0.0f, 1.0f, 3.0f ) );
		_camera.setScreenSize( _windowWidth, _windowHeight );
	}

	void LabWork3::handleEvents( const SDL_Event & event )
	{
		if ( event.type == SDL_KEYDOWN )
		{
			switch ( event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: _camera.moveFront( _cameraSpeed ); break;
			case SDL_SCANCODE_S: _camera.moveFront( -_cameraSpeed ); break;
			case SDL_SCANCODE_A: _camera.moveRight( -_cameraSpeed ); break;
			case SDL_SCANCODE_D: _camera.moveRight( _cameraSpeed ); break;
			case SDL_SCANCODE_R: _camera.moveUp( _cameraSpeed ); break;
			case SDL_SCANCODE_F: _camera.moveUp( -_cameraSpeed ); break;
			default: break;
			}
		}

		if ( event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( event.motion.xrel * _cameraSensitivity, event.motion.yrel * _cameraSensitivity );
		}
	}
} // namespace M3D_ISICG
