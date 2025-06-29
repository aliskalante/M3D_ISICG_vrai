#include "lab_work_5.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/shaders/";

	LabWork5::~LabWork5()
	{
		glDeleteProgram( _program );
		_bunny.cleanGL();
	}

	bool LabWork5::init()
	{
		std::cout << "Initializing Lab Work 5..." << std::endl;

		// Load shaders
		const std::string vertexShaderSource   = readFile( _shaderFolder + "meshtexture.vert" );
		const std::string fragmentShaderSource = readFile( _shaderFolder + "meshtexture.frag" );

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

		// Link shaders into a program
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

		glUseProgram( _program );

		_initCamera();

		_bunny.load( "Bunny", "data/models/sponza/sponza.obj" );
		_bunny._transformation = glm::scale( _bunny._transformation, glm::vec3( 0.003f ) );

		// Uniform locations
		_locationMVP		  = glGetUniformLocation( _program, "MVPMatrix" );
		_locationNormalMatrix = glGetUniformLocation( _program, "NormalMatrix" );
		_locationViewMatrix	  = glGetUniformLocation( _program, "ViewMatrix" );
		_locationModelMatrix  = glGetUniformLocation( _program, "ModelMatrix" );
		_locationCameraPos	  = glGetUniformLocation( _program, "Camerapos" );

		glDeleteShader( _vertexShader );
		glDeleteShader( _fragmentShader );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork5::animate( const float deltaTime ) {}

	void LabWork5::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable( GL_DEPTH_TEST );

		// Calculate matrices
		_MVP = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _bunny._transformation;
		Mat3f normalMatrix
			= Mat3f( glm::transpose( glm::inverse( _camera.getViewMatrix() * _bunny._transformation ) ) );

		// Update uniforms
		glProgramUniformMatrix4fv( _program, _locationMVP, 1, GL_FALSE, glm::value_ptr( _MVP ) );
		glProgramUniformMatrix3fv( _program, _locationNormalMatrix, 1, GL_FALSE, glm::value_ptr( normalMatrix ) );
		glProgramUniform3fv( _program,
							 glGetUniformLocation( _program, "lightPos" ),
							 1, 
							 glm::value_ptr( _camera.getPosition() ) );
		glProgramUniformMatrix4fv(
			_program, _locationViewMatrix, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
		glProgramUniformMatrix4fv(
			_program, _locationModelMatrix, 1, GL_FALSE, glm::value_ptr( _bunny._transformation ) );
		glProgramUniform3fv( _program, _locationCameraPos, 1, glm::value_ptr( _camera.getPosition() ) );

		_bunny.render( _program );
	}

	void LabWork5::displayUI()
	{
		ImGui::Begin( "Settings Lab Work 5" );
		ImGui::Text( "No settings available!" );
		ImGui::End();
	}

	void LabWork5::_initCamera()
	{
		_camera.setPosition( Vec3f( 1.f, 2.f, 0.f ) );
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setLookAt( Vec3f( 0.f, 0.f, 0.f ) );
	}

	void LabWork5::_updateViewMatrix()
	{
		glProgramUniformMatrix4fv(
			_program, _locationViewMatrix, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
	}

	void LabWork5::handleEvents( const SDL_Event & event )
	{
		if ( event.type == SDL_KEYDOWN )
		{
			switch ( event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W:
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S:
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A:
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D:
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R:
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F:
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		if ( event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( event.motion.xrel * _cameraSensitivity, event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}
} // namespace M3D_ISICG
