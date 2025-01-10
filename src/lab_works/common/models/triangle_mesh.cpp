#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glUseProgram( p_glProgram );

		// Pass material properties to the shader
		_bindMaterialToShader( p_glProgram );

		// Bind VAO and render
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, static_cast<GLsizei>( _indices.size() ), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );

		// Unbind textures after rendering
		if ( _material._hasDiffuseMap )
		{
			glBindTextureUnit( 1, 0 );
		}

		glUseProgram( 0 );
	}

	void TriangleMesh::_bindMaterialToShader( const GLuint p_glProgram ) const
	{
		// Pass texture availability flag
		glUniform1i( glGetUniformLocation( p_glProgram, "uHasDiffuseMap" ), _material._hasDiffuseMap ? 1 : 0 );

		if ( _material._hasDiffuseMap )
		{
			glBindTextureUnit( 1, _material._diffuseMap._id );
		}

		// Pass diffuse color
		glUniform3fv( glGetUniformLocation( p_glProgram, "uDiffuseColor" ), 1, glm::value_ptr( _material._diffuse ) );

		// Pass light direction (default)
		glm::vec3 lightDirection = glm::normalize( glm::vec3( 0.0f, 0.0f, -1.0f ) );
		glUniform3fv( glGetUniformLocation( p_glProgram, "uLightDirection" ), 1, glm::value_ptr( lightDirection ) );
	}

	void TriangleMesh::cleanGL()
	{
		// Disable attributes and clean up GL resources
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );

		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::setupGL()
	{
		// Generate and bind VAO, VBO, and EBO
		glGenVertexArrays( 1, &_vao );
		glGenBuffers( 1, &_vbo );
		glGenBuffers( 1, &_ebo );

		glBindVertexArray( _vao );

		// Setup VBO
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glBufferData( GL_ARRAY_BUFFER, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );

		// Setup EBO
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ebo );
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );

		// Setup vertex attributes
		_setupVertexAttributes();

		glBindVertexArray( 0 );
	}

	void TriangleMesh::_setupVertexAttributes() const
	{
		// Vertex positions
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void *)offsetof( Vertex, _position ) );

		// Vertex normals
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void *)offsetof( Vertex, _normal ) );

		// Vertex texture coordinates
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void *)offsetof( Vertex, _texCoords ) );
	}

} // namespace M3D_ISICG
