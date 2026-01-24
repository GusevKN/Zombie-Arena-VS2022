#include "bullet.h"
//  онструктор
Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}
void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	// ќтслеживаем пулю
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;
	// ¬ычисл€ем уклон траектории полета
	float gradient = (startX - targetX) / (startY - targetY);
	// ƒелаем отрицательным любое значение уклона меньше нул€
	if (gradient < 0)
	{
		gradient *= -1;
	}
	// ¬ычисл€ем соотношение между x и y
	float ratioXY = m_BulletSpeed / (1 + gradient);
	// ”станавливаем "скорость" по горизонтали и вертикали
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;
	// «адаем пуле правильное направление
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}
	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}
	// ”станавливаем максимальную дистанцию 1000 пикселей в любом направлении
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	// –азмещаем пулю в начальной позиции дл€ отрисовки
	m_BulletShape.setPosition(m_Position);
}
void Bullet::stop()
{
	m_InFlight = false;
}
bool Bullet::isInFlight()
{
	return m_InFlight;
}
FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}
RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}
void Bullet::update(float elapsedTime)
{
	// ќбновл€ем переменные позиции пули
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;
	// ѕеремещаем пулю
	m_BulletShape.setPosition(m_Position);
	// ѕул€ вышла за пределы диапазона?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}