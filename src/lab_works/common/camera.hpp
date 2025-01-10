#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

namespace M3D_ISICG
{
	// Freefly camera class
	class Camera
	{
	  public:
		// Constructor
		Camera();

		// Getters
		const glm::mat4 &		 getViewMatrix() const;
		const glm::mat4 &		 getProjectionMatrix() const;
		inline const glm::vec3 & getPosition() const { return _position; }
		int						 getScreenWidth() const { return _screenWidth; }
		int						 getScreenHeight() const { return _screenHeight; }

		// Setters
		void setPosition( const glm::vec3 & p_position );
		void setLookAt( const glm::vec3 & p_lookAt );
		void setFovy( float p_fovy );
		void setScreenSize( int p_width, int p_height );

		// Movement
		void moveFront( float p_delta );
		void moveRight( float p_delta );
		void moveUp( float p_delta );

		// Rotation
		void rotate( float p_yaw, float p_pitch );

		// Debug
		void print() const;

	  private:
		// Helper functions
		void _computeViewMatrix();
		void _computeProjectionMatrix();
		void _updateVectors();

		// Camera attributes
		glm::vec3 _position		= glm::vec3( 0.0f, 0.0f, 0.0f );
		glm::vec3 _invDirection = glm::vec3( 0.0f, 0.0f, -1.0f );
		glm::vec3 _right		= glm::vec3( 1.0f, 0.0f, 0.0f );
		glm::vec3 _up			= glm::vec3( 0.0f, 1.0f, 0.0f );

		// Angles and view attributes
		float _yaw		   = -90.0f; // Rotation angle around Y-axis
		float _pitch	   = 0.0f;	 // Rotation angle around X-axis
		float _fovy		   = 45.0f;	 // Field of view (vertical)
		float _aspectRatio = 16.0f / 9.0f;

		// Screen dimensions
		int _screenWidth  = 1280;
		int _screenHeight = 720;

		// Matrices
		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
	};
} // namespace M3D_ISICG

#endif // CAMERA_HPP
