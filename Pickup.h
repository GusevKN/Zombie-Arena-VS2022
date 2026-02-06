#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Pickup
{
private:
	// Начальное значение для предмета
	static const int HEALTH_START_VALUE = 50;
	static const int AMMO_START_VALUE = 12;
	static const int START_WAIT_TIME = 10;
	static const int START_SECONDS_TO_LIVE = 5;
	// Спрайт, представляющий этот предмет
	Sprite m_Sprite;
	// Арена, на которой существует предмет
	IntRect m_Arena;
	// Ценность предмета
	int m_Value;
	// Тип предмета или бонуса (1 – здоровье, 2 – боеприпасы)
	// 1 = health, 2 = ammo
	int m_Type;
	// Обработка появления и исчезновения
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;
	// Здесь начинаются прототипы публичных функций
public:
	Pickup(int type);
	// Подготавливаем новый предмет
	void setArena(IntRect arena);
	void spawn();
	// Проверяем позицию предмета
	FloatRect getPosition();
	// Получаем спрайт для отрисовки
	Sprite getSprite();
	// Обновляем предмет каждый кадр
	void update(float elapsedTime);
	// Этот предмет сейчас на арене?
	bool isSpawned();
	// Считываем полезность предмета
	int gotIt();
	// Улучшаем каждый предмет
	void upgrade();
};