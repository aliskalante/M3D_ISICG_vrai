#ifndef __TRIANGLE_MESH_MODEL_HPP__
#define __TRIANGLE_MESH_MODEL_HPP__

#include "base_model.hpp"
#include "triangle_mesh.hpp"
#include "utils/file_path.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace M3D_ISICG
{
	class TriangleMeshModel : public BaseModel
	{
	  public:
		TriangleMeshModel()			 = default;
		virtual ~TriangleMeshModel() = default;

		void load( const std::string & p_name, const FilePath & p_filePath );
		void setupGL();
		void render( const GLuint p_glProgram ) const override;
		void cleanGL() override;

	  private:
		void					  _loadMeshes( const aiScene * scene );
		void					  _loadMesh( const aiMesh * const p_mesh, const aiScene * const p_scene );
		std::vector<Vertex>		  _extractVertices( const aiMesh * const p_mesh ) const;
		std::vector<unsigned int> _extractIndices( const aiMesh * const p_mesh ) const;
		Material				  _extractMaterial( const aiMaterial * const p_mtl ) const;
		void					  _loadMaterialComponent( const aiMaterial * const p_mtl,
														  aiTextureType			   type,
														  const char *			   key,
														  glm::vec3 &			   outColor,
														  Texture &				   outTexture,
														  bool &				   hasTexture ) const;
		Texture					  _loadTexture( const aiString & p_path, const std::string & p_type );

	  public:
		std::vector<TriangleMesh> _meshes;
		std::vector<Texture>	  _loadedTextures;

		int _nbTriangles = 0;
		int _nbVertices	 = 0;

		FilePath _dirPath = "";
	};
} // namespace M3D_ISICG

#endif // __TRIANGLE_MESH_MODEL_HPP__
