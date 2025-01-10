#ifndef __LAB_WORK_4_HPP__
#define __LAB_WORK_4_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "common/models/triangle_mesh.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork4 : public BaseLabWork
	{
	  public:
		LabWork4() : BaseLabWork() {}
		~LabWork4();

		bool init() override;
		void animate( const float deltaTime ) override;
		void render() override;
		void handleEvents( const SDL_Event & event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		Camera			  _camera;
		TriangleMeshModel _bunny;

		void _initCamera();
		void _updateViewMatrix();

		float _fov				 = 60.0f;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;

		// ================ GL data.
		GLuint _vertexShader, _fragmentShader;
		GLuint _program;
		GLint  _locationMVP;
		GLint  _locationViewMatrix;
		GLint  _locationModelMatrix;
		GLint  _locationNormalMatrix;
		GLint  _locationCameraPos;

		// ================ Matrices.
		Mat4f _MVP;

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f );

		// ================ Shader folder.
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_4_HPP__
