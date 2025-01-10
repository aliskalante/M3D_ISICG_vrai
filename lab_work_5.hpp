#ifndef __LAB_WORK_5_HPP__
#define __LAB_WORK_5_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork5 : public BaseLabWork
	{
	  public:
		LabWork5() : BaseLabWork() {}
		~LabWork5();

		bool init() override;
		void animate( const float deltaTime ) override;
		void render() override;
		void handleEvents( const SDL_Event & event ) override;
		void displayUI() override;

	  private:
		// Scene data
		Camera			  _camera;
		TriangleMeshModel _bunny;

		// Initialization functions
		void _initCamera();
		void _updateViewMatrix();

		// Camera settings
		float _fov				 = 60.0f;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;

		// Shader data
		GLuint _vertexShader, _fragmentShader;
		GLuint _program;

		// Uniform locations
		GLint _locationMVP;
		GLint _locationNormalMatrix;
		GLint _locationViewMatrix;
		GLint _locationModelMatrix;
		GLint _locationCameraPos;

		// Transformation matrices
		Mat4f _MVP;

		// Background color
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f );

		// Shader folder
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_5_HPP__
