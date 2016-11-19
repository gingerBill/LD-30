#ifndef Ludum_Graphics_Camera_hpp
#define Ludum_Graphics_Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace gb
{
	class Camera
	{
	public:
		Camera();

		const glm::vec3& getPosition() const;

		Camera& setPosition(const glm::vec3& position);
		Camera& setPosition(float x, float y, float z) { return setPosition(glm::vec3(x, y, z)); }
		Camera& move(const glm::vec3& offset);
		Camera& move(float x, float y, float z) { return move(glm::vec3(x, y, z)); }

		float fieldOfView() const;
		Camera& setFieldOfView(float fieldOfView);

		float nearPlane() const;
		float farPlane() const;

		Camera& setNearAndFarPlanes(float nearPlane, float farPlane);

		glm::mat4 orientation() const;
		float getHorizontalAngle() const;
		float getVerticalAngle() const;

		Camera& offsetOrientation(float upAngle, float rightAngle);

		Camera& setRollAngle(float angle);
		float getRollAngle() const;

		Camera& lookAt(glm::vec3 position);

		float viewportAspectRatio() const;
		Camera& setViewportAspectRatio(float viewportAspectRatio);

		glm::vec3 forward() const;
		glm::vec3 backward() const;

		glm::vec3 right() const;
		glm::vec3 left() const;

		glm::vec3 up() const;
		glm::vec3 down() const;

		glm::mat4 matrix() const;
		glm::mat4 projection() const;
		glm::mat4 view() const;

	private:
		void normalizeAngles();

		glm::vec3 m_position;
		float m_horizontalAngle;
		float m_verticalAngle;
		float m_rollAngle;
		float m_fieldOfView;
		float m_nearPlane;
		float m_farPlane;
		float m_viewportAspectRatio;
	};
}

#endif
