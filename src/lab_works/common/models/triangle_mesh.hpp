#ifndef __TRIANGLE_MESH_HPP__
#define __TRIANGLE_MESH_HPP__

#include "GL/gl3w.h"
#include "define.hpp"
#include <string>
#include <vector>

namespace M3D_ISICG
{
	struct Vertex
	{
		Vec3f _position;  // Position of the vertex
		Vec3f _normal;	  // Normal of the vertex
		Vec2f _texCoords; // Texture coordinates
		Vec3f _tangent;	  // Tangent vector
		Vec3f _bitangent; // Bitangent vector
	};

	struct Texture
	{
		unsigned int _id;	// Texture ID
		std::string	 _type; // Texture type (e.g., diffuse, specular)
		std::string	 _path; // File path
	};

	struct Material
	{
		Vec3f _ambient	 = VEC3F_ZERO; // Ambient color
		Vec3f _diffuse	 = VEC3F_ZERO; // Diffuse color
		Vec3f _specular	 = VEC3F_ZERO; // Specular color
		float _shininess = 0.f;		   // Shininess factor

		bool _hasAmbientMap	  = false;
		bool _hasDiffuseMap	  = false;
		bool _hasSpecularMap  = false;
		bool _hasShininessMap = false;

		Texture _ambientMap;
		Texture _diffuseMap;
		Texture _specularMap;
		Texture _shininessMap;
	};

	class TriangleMesh
	{
	  public:
		TriangleMesh() = delete;

		TriangleMesh( const std::string &				p_name,
					  const std::vector<Vertex> &		p_vertices,
					  const std::vector<unsigned int> & p_indices,
					  const Material &					p_material );

		~TriangleMesh() = default;

		// Renders the mesh
		void render( const GLuint p_glProgram ) const;

		// Cleans up GL resources
		void cleanGL();

		// Sets up GL resources
		void setupGL();

	  private:
		// Binds material properties to the shader
		void _bindMaterialToShader( const GLuint p_glProgram ) const;

		// Sets up vertex attributes for the VAO
		void _setupVertexAttributes() const;

	  public:
		std::string _name = "Unknown";

		// Geometric data
		std::vector<Vertex>		  _vertices;
		std::vector<unsigned int> _indices;

		// Material data
		Material _material;

		// GL data
		GLuint _vao = GL_INVALID_INDEX; // Vertex Array Object
		GLuint _vbo = GL_INVALID_INDEX; // Vertex Buffer Object
		GLuint _ebo = GL_INVALID_INDEX; // Element Buffer Object
	};
} // namespace M3D_ISICG

#endif // __TRIANGLE_MESH_HPP__
