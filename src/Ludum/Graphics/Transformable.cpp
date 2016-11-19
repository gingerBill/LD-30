#include <Ludum/Graphics/Transformable.hpp>

#include <Ludum/Common.hpp>

#include <cmath>

namespace gb
{
	Transformable::Transformable()
		:m_origin(0, 0, 0)
		,m_position(0, 0, 0)
		,m_rotation()
		,m_scale(1, 1, 1)
		,m_transform()
		,m_transformNeedUpdate(true)
		,m_inverseTransform()
		,m_inverseTransformNeedUpdate(true)
	{

	}

	Transformable::~Transformable()
	{
	}

	Transformable& Transformable::setPosition(const glm::vec3& position)
	{
		m_position = position;
		m_transformNeedUpdate = true;
		m_inverseTransformNeedUpdate = true;

		return *this;
	}

	Transformable& Transformable::setPosition(float x, float y, float z)
	{
		return setPosition(glm::vec3(x, y, z));
	}

	Transformable& Transformable::setRotation(const Transformable::Rotation& rotation)
	{
		m_rotation.angle = static_cast<float>(fmod(rotation.angle, 360));
		if (m_rotation.angle < 0)
			m_rotation.angle += 360.0f;

		m_rotation.axis = glm::normalize(rotation.axis);

		m_transformNeedUpdate = true;
		m_inverseTransformNeedUpdate = true;

		return *this;
	}

	Transformable& Transformable::setRotation(float angle, const glm::vec3& axis)
	{
		return setRotation(Rotation(angle, axis));
	}

	Transformable& Transformable::setRotation(float angle, float x, float y, float z)
	{
		return setRotation(Rotation(angle, glm::vec3(x, y, z)));
	}


	Transformable& Transformable::setScale(const glm::vec3& scale)
	{
		m_scale = scale;
		m_transformNeedUpdate = true;
		m_inverseTransformNeedUpdate = true;

		return *this;
	}

	Transformable& Transformable::setScale(float x, float y, float z)
	{
		return setScale(glm::vec3(x, y, z));
	}

	Transformable& Transformable::setOrigin(const glm::vec3& position)
	{
		m_origin = position;
		m_transformNeedUpdate = true;
		m_inverseTransformNeedUpdate = true;

		return *this;
	}

	Transformable& Transformable::setOrigin(float x, float y, float z)
	{
		return setOrigin(glm::vec3(x, y, z));
	}

	const glm::vec3& Transformable::getPosition() const
	{
		return m_position;
	}

	const Transformable::Rotation& Transformable::getRotation() const
	{
		return m_rotation;
	}

	const glm::vec3& Transformable::getScale()    const
	{
		return m_scale;
	}

	const glm::vec3& Transformable::getOrigin()   const
	{
		return m_origin;
	}

	Transformable& Transformable::move(const glm::vec3 offset)
	{
		return setPosition(getPosition() + offset);
	}

	Transformable& Transformable::move(float x, float y, float z)
	{
		return setPosition(getPosition() + glm::vec3(x, y, z));
	}

	const glm::mat4& Transformable::getTransform() const
	{
		if (m_transformNeedUpdate)
		{
			m_transform  = translate(m_position);
			m_transform *= rotate(m_rotation.angle, m_rotation.axis);
			m_transform *= scale(m_scale);
			m_transform *= translate(-m_origin);

			m_transformNeedUpdate = false;
		}

		return m_transform;
	}

	const glm::mat4& Transformable::getInverseTransform() const
	{
		if (m_inverseTransformNeedUpdate)
		{
			m_inverseTransform = m_transform._inverse();
			m_inverseTransformNeedUpdate = false;
		}

		return m_inverseTransform;
	}
}

