#include <Ludum/Graphics/Camera.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	static const float MaxVerticalAngle = 85.0f; // must be less than 90 to avoid gimbal lock
}

namespace gb
{
	Camera::Camera()
		:m_position(0.0f, 0.0f, 1.0f)
		,m_horizontalAngle(180.0f)
		,m_verticalAngle(0.0f)
		,m_rollAngle(0)
		,m_fieldOfView(50.0f)
		,m_nearPlane(0.1f)
		,m_farPlane(100.0f)
		,m_viewportAspectRatio(4.0f / 3.0f)

	{
	}

	const glm::vec3& Camera::getPosition() const
	{
		return m_position;
	}

	Camera& Camera::setPosition(const glm::vec3& position)
	{
		m_position = position;

		return *this;
	}

	Camera& Camera::move(const glm::vec3& offset)
	{
		m_position += offset;

		return *this;
	}

	float Camera::fieldOfView() const
	{
		return m_fieldOfView;
	}

	Camera& Camera::setFieldOfView(float fieldOfView)
	{
		assert(fieldOfView > 0.0f && fieldOfView < 180.0f);
		m_fieldOfView = fieldOfView;

		return *this;
	}

	float Camera::nearPlane() const
	{
		return m_nearPlane;
	}

	float Camera::farPlane() const
	{
		return m_farPlane;
	}

	Camera& Camera::setNearAndFarPlanes(float nearPlane, float farPlane)
	{
		assert(nearPlane > 0.0f);
		assert(farPlane > nearPlane);
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;

		return *this;
	}

	glm::mat4 Camera::orientation() const
	{
		glm::mat4 orientation;
		orientation = glm::rotate(orientation, m_verticalAngle, glm::vec3(1, 0, 0));
		orientation = glm::rotate(orientation, m_horizontalAngle, glm::vec3(0, 1, 0));

		glm::vec4 f = glm::inverse(orientation) * glm::vec4(0, 0, 1, 1);
		orientation = glm::rotate(orientation, m_rollAngle, glm::vec3(f));

		return orientation;
	}

	float Camera::getHorizontalAngle() const
	{
		return m_horizontalAngle;
	}

	float Camera::getVerticalAngle() const
	{
		return m_verticalAngle;
	}

	Camera& Camera::offsetOrientation(float upAngle, float rightAngle)
	{
		m_horizontalAngle += rightAngle;
		m_verticalAngle += upAngle;
		normalizeAngles();

		return *this;
	}

	Camera& Camera::setRollAngle(float angle)
	{
		m_rollAngle = angle;

		return *this;
	}

	float Camera::getRollAngle() const
	{
		return m_rollAngle;
	}

	Camera& Camera::lookAt(glm::vec3 position)
	{
		assert(position != m_position);
		glm::vec3 direction = glm::normalize(position - m_position);
		m_verticalAngle = glm::radians(asinf(-direction.y));
		m_horizontalAngle = -glm::radians(atan2f(-direction.x, -direction.z));
		normalizeAngles();

		return *this;
	}

	float Camera::viewportAspectRatio() const
	{
		return m_viewportAspectRatio;
	}

	Camera& Camera::setViewportAspectRatio(float viewportAspectRatio)
	{
		assert(viewportAspectRatio > 0.0);
		m_viewportAspectRatio = viewportAspectRatio;

		return *this;
	}

	glm::vec3 Camera::forward() const
	{
		glm::vec4 forward = glm::inverse(orientation()) * glm::vec4(0, 0, -1, 1);
		return glm::vec3(forward);
	}

	glm::vec3 Camera::backward() const
	{
		glm::vec4 backward = glm::inverse(orientation()) * glm::vec4(0, 0, 1, 1);
		return glm::vec3(backward);
	}

	glm::vec3 Camera::right() const
	{
		glm::vec4 right = glm::inverse(orientation()) * glm::vec4(1, 0, 0, 1);
		return glm::vec3(right);
	}

	glm::vec3 Camera::left() const
	{
		glm::vec4 left = glm::inverse(orientation()) * glm::vec4(-1, 0, 0, 1);
		return glm::vec3(left);
	}

	glm::vec3 Camera::up() const
	{
		glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0, 1, 0, 1);
		return glm::vec3(up);
	}

	glm::vec3 Camera::down() const
	{
		glm::vec4 down = glm::inverse(orientation()) * glm::vec4(0, -1, 0, 1);
		return glm::vec3(down);
	}


	glm::mat4 Camera::matrix() const
	{
		return projection() * view();
	}

	glm::mat4 Camera::projection() const
	{
		return glm::perspective(m_fieldOfView, m_viewportAspectRatio, m_nearPlane, m_farPlane);
	}

	glm::mat4 Camera::view() const
	{
		return orientation() * glm::translate(glm::mat4(), -m_position);
	}

	void Camera::normalizeAngles()
	{
		m_horizontalAngle = fmodf(m_horizontalAngle, 360.0f);
		m_rollAngle = fmodf(m_rollAngle, 360.0f);
		if (m_horizontalAngle < 0.0f)
			m_horizontalAngle += 360.0f;

		if (m_rollAngle < 0.0f)
			m_rollAngle += 360.0f;


		if (m_verticalAngle > MaxVerticalAngle)
			m_verticalAngle = MaxVerticalAngle;
		else if (m_verticalAngle < -MaxVerticalAngle)
			m_verticalAngle = -MaxVerticalAngle;
	}
}

