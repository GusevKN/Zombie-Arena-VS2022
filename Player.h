#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	// Где находится игровой персонаж
	Vector2f m_Position;
	// Спрайт
	Sprite m_Sprite;
	// Текстура
// !!Внимание — здесь скоро будут изменения!!
	Texture m_Texture;
	// Разрешение экрана
	Vector2f m_Resolution;
	// Размер текущей арены
	IntRect m_Arena;
	// Размер каждой плитки арены
	int m_TileSize;
	// Определение направления движения персонажа
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	// Количество очков здоровья
	int m_Health;
	// Максимально возможное количество очков здоровья у героя
	int m_MaxHealth;
	// Когда герой был последний раз ранен
	Time m_LastHit;
	// Скорость в пикселях в секунду
	float m_Speed;
	// Далее будут все наши публичные функции
public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	// Вызывается в конце каждой игры
	void resetPlayerStats();
	// Обработка удара зомби по игровому персонажу
	bool hit(Time timeHit);
	// Когда персонаж был последний раз ранен
	Time getLastHitTime();
	// Где находится персонаж
	FloatRect getPosition();
	// Где находится центр персонажа
	Vector2f getCenter();
	// В каком направлении смотрит герой
	float getRotation();
	// Отправляем копию спрайта в основную функцию
	Sprite getSprite();
	// Следующие четыре функции отвечают за перемещение персонажа
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	// Останавливают движение персонажа
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	// Эта функция будет вызываться каждый кадр
	void update(float elapsedTime, Vector2i mousePosition);
	// Дает игровому персонажу ускорение
	void upgradeSpeed();
	// Дает здоровье
	void upgradeHealth();
	// Увеличивает максимально возможное количество здоровья персонажа
	void increaseHealthLevel(int amount);
	// Сколько здоровья у героя в текущий момент?
	int getHealth();


};