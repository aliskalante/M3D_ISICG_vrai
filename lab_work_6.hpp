#ifndef __LAB_WORK_6_HPP__
#define __LAB_WORK_6_HPP__

#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "common/models/triangle_mesh_model.hpp"

namespace M3D_ISICG
{
	class LabWork6 : public BaseLabWork
	{
	  public:
		LabWork6() : BaseLabWork() {}
		~LabWork6() override;

		// M�thodes principales
		bool init() override;	// Initialise tout (G-Buffer, shaders, etc.)
		void render() override; // Appel de la Geometry Pass + Shading Pass
		void displayUI() override;

		// Gestion d'�v�nements et d'animation (optionnel)
		void handleEvents( const SDL_Event & p_event ) override {}
		void animate( const float p_deltaTime ) override {}

	  private:
		// --------------------------------------------------------------------
		// Pass g�om�trique
		// --------------------------------------------------------------------
		void prepareGBuffer();
		void _geometryPass();

		// --------------------------------------------------------------------
		// Pass d'ombrage
		// --------------------------------------------------------------------
		void initShadingPass();
		void initFullScreenQuad();
		void _shadingPass();

		// --------------------------------------------------------------------
		// Contr�les et utilitaires
		// --------------------------------------------------------------------
		GLuint _compileShaders( const std::string & vertexSource, const std::string & fragmentSource );
		void   checkShaderCompilation( GLuint shader );
		void   checkProgramLinking( GLuint program );

	  private:
		// Camera (pour les matrices projection/view)
		Camera _camera;

		// Mod�le 3D (bunny)
		TriangleMeshModel _bunnyModel;

		// Programmes (shaders)
		GLuint _geometryPassProgram = 0;
		GLuint _shadingPassProgram	= 0;

		// G-Buffer : Framebuffer et textures
		GLuint _gBufferFBO			 = 0;
		GLuint _gBufferTextures[ 6 ] = { 0, 0, 0, 0, 0, 0 };

		// Indice de la texture G-Buffer affich�e
		GLuint _selectedTextureIndex = 0;

		// Param�tres de mat�riau
		glm::vec3 _ambientColor	 = glm::vec3( 0.1f, 0.1f, 0.1f );
		glm::vec3 _diffuseColor	 = glm::vec3( 1.0f, 1.0f, 1.0f );
		glm::vec3 _specularColor = glm::vec3( 1.0f, 1.0f, 1.0f );
		float	  _shininess	 = 32.0f;

		// Quad plein �cran pour le shading pass
		GLuint _quadVAO = 0;
		GLuint _quadVBO = 0;
		GLuint _quadEBO = 0;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_6_HPP__
