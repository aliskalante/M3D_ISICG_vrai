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

		void	cleanGL() override;
		Texture _loadTexture( const aiString & p_path, const std::string & p_type );

	  private:
		void	 _loadMesh( const aiMesh * const p_mesh, const aiScene * const p_scene );
		Material _loadMaterial( const aiMaterial * const p_mtl );

	  public:
		std::vector<TriangleMesh> _meshes;		   
		std::vector<Texture>	  _loadedTextures;
		
		int _nbTriangles = 0;
		int _nbVertices	 = 0;

		FilePath _dirPath = "";
	};
} // namespace M3D_ISICG

#endif // __TRIANGLE_MESH_MODEL_HPP__
