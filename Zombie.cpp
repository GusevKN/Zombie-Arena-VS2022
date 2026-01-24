#include "zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>
using namespace std;
void Zombie::spawn(float startX, float startY, int type, int seed)
{
	m_Alive = true;
	switch (type)
	{
	case 0:
		// Толстяк
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;
	case 1:
		// Охотник
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;
	case 2:
		// Ползун
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	}
	// Изменяем скорость, чтобы сделать зомби уникальным
	// Каждый зомби уникален. Создаем модификатор скорости
	srand((int)time(0)* seed);
	// Где-то между 80 и 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	// Выражаем это как дробное число от 0 до 1
	modifier /= 100; // Теперь значение между 0.7 и 1
	m_Speed *= modifier;
	// Инициализируем его местоположение
	m_Position.x = startX;
	m_Position.y = startY;
	// Устанавливаем начало координат спрайта в его центр
	m_Sprite.setOrigin(25, 25);
	// Устанавливаем его позицию
	m_Sprite.setPosition(m_Position);
}
bool Zombie::hit()
{
	m_Health--;
	if (m_Health < 0)
	{
		// Мертв
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	// Ранен, но еще жив
	return false;
}
bool Zombie::isAlive()
{
	return m_Alive;
}
FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}
Sprite Zombie::getSprite()
{
	return m_Sprite;
}
void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;
	// Обновляем переменные, отвечающие за позиции зомби
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x + m_Speed * elapsedTime;
	}
	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y + m_Speed * elapsedTime;
	}
	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x - m_Speed * elapsedTime;
	}
	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}
	// Перемещаем спрайт
	m_Sprite.setPosition(m_Position);
	// Поворачиваем спрайт в правильном направлении
	float angle = (atan2(playerY - m_Position.y, playerX -	m_Position.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}

