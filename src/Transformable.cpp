#include "Transformable.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace
{
    constexpr double pi = 3.1415926535897931;
    constexpr double unitRadian = 180 / pi;
    constexpr double unitDegree = pi / 180;
}

Transformable::Transformable(float radians, const glm::vec2& size, const glm::vec2& origin, const glm::vec2& scale,
                             const glm::vec2& position)
    : m_isDirty(true)
    , m_radians(radians)
    , m_size(size)
    , m_origin(origin)
    , m_scale(scale)
    , m_position(position)
    , m_transformation(1.0f)
{
}

void Transformable::setRadians(float radians)
{
    m_radians = radians;
    setDirty();
}

float Transformable::getDegrees() const { return m_radians * unitRadian; }
void Transformable::setDegrees(float degrees) { setRadians(degrees * unitDegree); }

void Transformable::setSize(const glm::vec2& size)
{
    m_size = size;
    setDirty();
}

void Transformable::setOrigin(const glm::vec2& origin)
{
    m_origin = origin;
    setDirty();
}

void Transformable::setScale(const glm::vec2& scale)
{
    m_scale = scale;
    setDirty();
}

void Transformable::setPosition(const glm::vec2& position)
{
    m_position = position;
    setDirty();
}

void Transformable::move(const glm::vec2& offset)
{
    if (offset.x != 0)
    {
        m_position += glm::vec2(1.0f, 0.0f) * offset.x;
        setDirty();
    }
    if (offset.y != 0)
    {
        m_position += glm::vec2(0.0f, 1.0f) * offset.y;
        setDirty();
    }
}

const glm::mat4& Transformable::getTransformation()
{
    if (m_isDirty)
    {
        m_isDirty = false;

        m_transformation = getIdentity();

        const glm::vec3 positionTranslate(-m_position.x, -m_position.y, 0.0f);
        m_transformation = glm::translate(m_transformation, positionTranslate);

        m_transformation = glm::rotate(m_transformation, m_radians, glm::vec3(0.0f, 0.0f, 1.0f));

        const glm::vec3 originTranslate(m_origin.x * m_size.x, m_origin.y * m_size.y, 0.0f);
        m_transformation = glm::translate(m_transformation, originTranslate);

        const glm::vec3 scaleTranslate(m_size.x * m_scale.x, m_size.y * m_scale.y, 0.0f);
        m_transformation = glm::scale(m_transformation, scaleTranslate);
    }
    return m_transformation;
}

void Transformable::setDirty() { m_isDirty = true; }
