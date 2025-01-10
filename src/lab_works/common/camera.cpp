#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace M3D_ISICG
{
	Camera::Camera()
	{
		_computeViewMatrix();
		_computeProjectionMatrix();
	}

	const glm::mat4 & Camera::getViewMatrix() const { return _viewMatrix; }

	const glm::mat4 & Camera::getProjectionMatrix() const { return _projectionMatrix; }

	void Camera::setPosition( const glm::vec3 & p_position )
	{
		_position = p_position;
		_computeViewMatrix();
	}

	void Camera::setLookAt( const glm::vec3 & p_lookAt )
	{
		_invDirection = glm::normalize( _position - p_lookAt );
		_pitch		  = glm::clamp( glm::degrees( glm::asin( _invDirection.y ) ), -89.f, 89.f );
		_yaw		  = glm::degrees( glm::atan( _invDirection.z, _invDirection.x ) );
		_updateVectors();
	}

	void Camera::setFovy( float p_fovy )
	{
		_fovy = p_fovy;
		_computeProjectionMatrix();
	}

	void Camera::setScreenSize( int p_width, int p_height )
	{
		_screenWidth  = p_width;
		_screenHeight = p_height;
		_aspectRatio  = static_cast<float>( _screenWidth ) / _screenHeight;
		_computeProjectionMatrix();
	}

	void Camera::moveFront( float p_delta )
	{
		_position -= _invDirection * p_delta;
		_computeViewMatrix();
	}

	void Camera::moveRight( float p_delta )
	{
		_position += _right * p_delta;
		_computeViewMatrix();
	}

	void Camera::moveUp( float p_delta )
	{
		_position += _up * p_delta;
		_computeViewMatrix();
	}

	void Camera::rotate( float p_yaw, float p_pitch )
	{
		_yaw   = glm::mod( _yaw + p_yaw, 360.f );
		_pitch = glm::clamp( _pitch + p_pitch, -89.f, 89.f );
		_updateVectors();
	}

	void Camera::print() const
	{
		std::cout << "======== Camera ========" << std::endl
				  << "Position: " << glm::to_string( _position ) << std::endl
				  << "View direction: " << glm::to_string( -_invDirection ) << std::endl
				  << "Right: " << glm::to_string( _right ) << std::endl
				  << "Up: " << glm::to_string( _up ) << std::endl
				  << "Yaw: " << _yaw << std::endl
				  << "Pitch: " << _pitch << std::endl
				  << "========================" << std::endl;
	}

	void Camera::_computeViewMatrix()
	{
		_viewMatrix = glm::lookAt( _position,				  // Camera position
								   _position - _invDirection, // Target point
								   _up						  // Up direction
		);
	}

	void Camera::_computeProjectionMatrix()
	{
		_projectionMatrix = glm::perspective( glm::radians( _fovy ), // Vertical field of view
											  _aspectRatio,			 // Aspect ratio
											  0.1f,					 // Near plane
											  100.0f				 // Far plane
		);
	}

	void Camera::_updateVectors()
	{
		const float yaw	  = glm::radians( _yaw );
		const float pitch = glm::radians( _pitch );

		_invDirection = glm::normalize(
			glm::vec3( glm::cos( yaw ) * glm::cos( pitch ), glm::sin( pitch ), glm::sin( yaw ) * glm::cos( pitch ) ) );

		_right = glm::normalize( glm::cross( glm::vec3( 0.f, 1.f, 0.f ), _invDirection ) ); // Y-axis as "up".
		_up	   = glm::normalize( glm::cross( _invDirection, _right ) );

		_computeViewMatrix();
	}

} // namespace M3D_ISICG
