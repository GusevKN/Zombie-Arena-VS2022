#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Zombie
{
private:
	// Скорость каждого типа зомби
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	// Очки здоровья каждого типа зомби
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;
	// Создаем небольшое отклонение в скорости для каждого зомби
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;
	// Где находится этот зомби?
	Vector2f m_Position;
	// Спрайт для зомби
	Sprite m_Sprite;
	// Как быстро этот зомби может перемещаться?
	float m_Speed;
	// Сколько очков здоровья у этого зомби?
	float m_Health;
	// Жив ли он?
	bool m_Alive=true;
	// Прототипы публичных функций
public:
	// Обработка попадания пули в зомби
	bool hit();
	// Проверка, жив ли зомби
	bool isAlive();
	// Генерация нового зомби
	void spawn(float startX, float startY, int type, int seed);
	// Возвращаем прямоугольник, представляющий позицию зомби
	FloatRect getPosition();
	// Возвращаем копию спрайта для отрисовки
	Sprite getSprite();
	// Обновляем зомби каждый кадр
	void update(float elapsedTime, Vector2f playerLocation);
};