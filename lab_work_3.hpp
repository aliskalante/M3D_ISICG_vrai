#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void handleEvents( const SDL_Event & p_event ) override;

		void displayUI() override;

	  private:
		struct Mesh
		{
			std::vector<Vec3f>		  positions;						   // Positions des sommets
			std::vector<Vec3f>		  colors;							   // Couleurs des sommets
			std::vector<unsigned int> indices;							   // Indices des sommets
			Mat4f					  transformMatrix;					   // Matrice de transformation
			GLuint					  VBO_positions, VBO_colors, VAO, EBO; // Buffers et VAO
		};
		Mesh _mesh;
		Mesh _createCube();
		void _initBuffers();
		void _updateViewMatrix();
		void _updateProjectionMatrix();
		void _initCamera();

		Camera _camera;

		Mat4f _MVPMatrix;
		float _fov				 = 60.0f;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;

		GLuint _vertexShader, _fragmentShader;
		GLuint _program;
		GLint  _locationMVP;

		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f );

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
