#ifndef Ludum_Graphics_Transformable_hpp
#define Ludum_Graphics_Transformable_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gb
{
	class Transformable
	{
	public:
		struct Rotation
		{
			float angle;
			glm::vec3 axis;

			Rotation()
				:angle(0)
				,axis(0, 1, 0)
			{
			}
			Rotation(float angle, const glm::vec3& axis)
				:angle(angle)
				,axis(glm::normalize(axis))
			{
			}
		};

		Transformable();
		virtual ~Transformable();

		Transformable& setPosition(const glm::vec3& position);
		Transformable& setPosition(float x, float y, float z);

		Transformable& setRotation(const Transformable::Rotation& rotation);
		Transformable& setRotation(float angle, const glm::vec3& axis);
		Transformable& setRotation(float angle, float x, float y, float z);

		Transformable& setScale(const glm::vec3& scale);
		Transformable& setScale(float x, float y, float z);

		Transformable& setOrigin(const glm::vec3& position);
		Transformable& setOrigin(float x, float y, float z);

		const glm::vec3& getPosition() const;
		const Rotation&  getRotation() const;
		const glm::vec3& getScale()    const;
		const glm::vec3& getOrigin()   const;

		Transformable& move(const glm::vec3 offset);
		Transformable& move(float x, float y, float z);

		const glm::mat4& getTransform() const;
		const glm::mat4& getInverseTransform() const;

	private:
		glm::vec3 m_origin;
		glm::vec3 m_position;
		Rotation  m_rotation;
		glm::vec3 m_scale;

		mutable glm::mat4 m_transform;
		mutable bool      m_transformNeedUpdate;
		mutable glm::mat4 m_inverseTransform;
		mutable bool      m_inverseTransformNeedUpdate;
	};
}

#endif
