#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Bullet
{
private:
	// Где находится пуля?
	Vector2f m_Position;
	// Как выглядит каждая пуля?
	RectangleShape m_BulletShape;
	// Летит ли эта пуля в данный момент?
	bool m_InFlight = false;
	// С какой скоростью летит пуля?
	float m_BulletSpeed = 1000;
	// Какую долю пикселя пуля проходит
	// по горизонтали и вертикали за каждый кадр?
	// Эти значения будут вычислены на основе m_BulletSpeed
	float m_BulletDistanceX;
	float m_BulletDistanceY;
	// Определяем границы, чтобы пуля не летела бесконечно
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;
	// Здесь начинаются прототипы публичных функций
public:
	// Конструктор
	Bullet();
	// Останавливает пулю
	void stop();
	// Возвращает значение m_InFlight
	bool isInFlight();
	// Запускает новую пулю
	void shoot(float startX, float startY,
		float xTarget, float yTarget);
	// Сообщает вызывающему коду, где находится пуля
	FloatRect getPosition();
	// Возвращает фактическую форму (для отрисовки)
	RectangleShape getShape();
	// Обновляет пулю каждый кадр
	void update(float elapsedTime);
};