#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
using namespace sf;
int main()
{
	// Здесь создается экземпляр TextureHolder
	TextureHolder holder;
	// Игра всегда находится в одном из четырех состояний
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Начинаем с состояния GAME_OVER
	State state = State::GAME_OVER;
	// Получаем разрешение экрана и создаем окно SFML
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(
		VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);
	// Создаем объект View библиотеки SFML для основного действия
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	// Здесь находится наш таймер для отслеживания времени
	Clock clock;
	// Сколько времени прошло в состоянии PLAYING
	Time gameTimeTotal;
	// Где находится указатель мыши относительно глобальных координат
	Vector2f mouseWorldPosition;
	// Где находится указатель мыши относительно координат на экране
	Vector2i mouseScreenPosition;
	// Создаем экземпляр класса Player
	Player player;
	// Границы арены
	IntRect arena;
	// Создаем фон
	VertexArray background;
	// Загружаем текстуру для нашего массива вершин фона
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet.png");
	// Готовимся к орде зомби
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;
	// 100 пуль должно хватить
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// Когда в последний раз была нажата кнопка выстрела?
	Time lastPressed;
	// Скрываем указатель мыши и заменяем его прицелом
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	// Создаем пару бонусных предметов
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	// Об игре
	int score = 0;
	int hiScore = 0;
	// Для главного экрана и экрана завершения игры
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	// Создаем область отображения для HUD
	 View hudView(sf::FloatRect(0, 0, 1920,1080));
// Создаем спрайт для иконки патронов
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture(
		"graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	// Загружаем шрифт
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");
	// Пауза
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
	// Игра окончена
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");
	// Повышение уровня
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<//Повышение скорострельности
		"\n2- Increasing the clip size (on the next reload)" <<//Повышение размера обоймы (при следующей перезарядке)
		"\n3- Increasing the maximum level of health" <<//Повышение максимального уровня здоровья
		"\n4- Increased movement speed" <<//Повышение скорости передвижения
		"\n5- First Aid Kit Improvement" <<//Улучшение аптечки
		"\n6- Ammo improvement";//Улучшение боеприпасов
	levelUpText.setString(levelUpStream.str());
	// Патроны
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	// Счет
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// Загружаем рекорд из текстового файла
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		// >> Считываем данные
		inputFile >> hiScore;
		inputFile.close();
	}
	// Рекорд
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());
	// Оставшиеся зомби
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	// Номер волны
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
	// Шкала здоровья
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
	// Когда мы в последний раз обновляли HUD?
	int framesSinceLastHUDUpdate = 0;
	// Как часто (в кадрах) мы должны обновлять HUD
	int fpsMeasurementFrameInterval = 1000;

	// Основной игровой цикл
	while (window.isOpen())
	{
/*
***************
Обработка ввода
***************
*/
		// Обработка событий через опрос
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Пауза в игре во время PLAYING
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				// Возобновление игры во время PAUSED
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Сброс таймера, чтобы избежать скачка кадра
					clock.restart();
				}
				// Начало новой игры в состоянии GAME_OVER
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;
					// Подготавливаем оружие и патроны для новой игры
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;
					// Сбрасываем статистику игрока
					player.resetPlayerStats();
				}
				if (state == State::PLAYING)
				{
					// Перезарядка
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// Много патронов. Перезаряжаем.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							// Осталось немного патронов
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							// Здесь будет больше кода позже
						}
					}
				
				}
			}
		}// Завершение опроса событий
		// Обработка выхода из игры
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		// Оработка WASD во время игры
		if (state == State::PLAYING)
		{
			// Обработка нажатия и отпускания клавиш WASD
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			// Обработка выстрела
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip > 0)
				{
					// Передаем центр игрового персонажа и прицела в функцию shoot
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}// Конец обработки выстрела
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip > 0)
				{
					// Передаем центр игрового персонажа и прицела в функцию shoot
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}
		}// Завершение обработки WASD во время игры
		// Обработка состояния повышения уровня
		if (state == State::LEVELING_UP)
		{
			// Обработка повышения уровня игрока
			if (event.key.code == Keyboard::Num1)
			{
				// Увеличиваем скорострельность
				fireRate++;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2)
			{
				// Увеличиваем размер обоймы
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3)
			{
				// Увеличиваем показатели здоровья
				player.upgradeHealth();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4)
			{
				// Увеличиваем скорость передвижения
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5)
			{
				// Улучшаем аптечку
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6)
			{
				// Улучшаем боеприпасы
				ammoPickup.upgrade();
				state = State::PLAYING;
			}
			if (state == State::PLAYING)
			{
				// Увеличиваем номер волны
				wave++;
				// Подготовка уровня
				// Мы изменим следующие две строки позже
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;
				// Передаем массив вершин по ссылке
				// в функцию createBackground
				int tileSize = createBackground(background, arena);
				// Мы изменим эту строку кода позже
				//int tileSize = 50;

				// Появление игрока в центре арены
				player.spawn(arena, resolution, tileSize);
				// Настраиваем предметы
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);
				// Создаем орду зомби
				numZombies = 5 * wave;
				// Освобождаем ранее выделенную память (если она существует)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;
				// Сброс таймера, чтобы избежать скачка кадра
				clock.restart();
			}
		}// Завершение обработки повышения уровня
		/*
****************
Обновление кадра
****************
*/
		if (state == State::PLAYING)
		{
			// Обновление delta time
			Time dt = clock.restart();
			// Обновляем общее игровое время
			gameTimeTotal += dt;
			// Преобразуем delta time в дробь
			float dtAsSeconds = dt.asSeconds();
			// Получаем текущее положение указателя мыши
			mouseScreenPosition = Mouse::getPosition();
			// Преобразуем положение указателя мыши в глобальные
			// координаты относительно mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);
			// Устанавливаем прицел
			spriteCrosshair.setPosition(mouseWorldPosition);
			// Обновляем игрового персонажа
			player.update(dtAsSeconds, Mouse::getPosition());
			// Сохраняем новое положение персонажа
			Vector2f playerPosition(player.getCenter());
			// Центрируем вид вокруг игрового персонажа
			mainView.setCenter(player.getCenter());
			// Перебираем всех зомби и обновляем их
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			// Обновляем все пули, которые находятся в полете
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}
			// Обновляем бонусные предметы
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
			// Обнаружение коллизий
// Был ли зомби подстрелен?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
						(zombies[j].getPosition()))
						{
							// Останавливаем пулю
							bullets[i].stop();
							// Регистрируем попадание и проверяем, было ли это убийством
							if (zombies[j].hit())
							{
								// Не просто попадание, но и убийство
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;
								// Когда все зомби мертвы (снова)
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			}// Конец обработки подстреленного зомби
			/*// Был ли игрок задет зомби
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						// Здесь будет больше кода позже
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						std::ofstream outputFile("gamedata/scores.txt");
						// << Записываем данные
							outputFile << hiScore;
						outputFile.close();
					}
				}
			}// Конец обработки касания игрока*/
			// Коснулся ли герой аптечки
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}
			// Коснулся ли герой боеприпасов
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
			}
			// Изменяем размер шкалы здоровья
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			// Увеличиваем количество кадров с момента последнего обновления
			framesSinceLastHUDUpdate++;
			// Пересчитываем каждые fpsMeasurementFrameInterval кадров
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				// Обновляем текст HUD
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Обновляем текст, отображающий количество оставшихся патронов
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				// Обновляем текст, показывающий счет
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				// Обновляем текст, показывающий рекорд
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				// Обновляем текст, показывающий волну
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				// Обновляем текст, отражающий количество оставшихся зомби
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}// Конец обновления HUD
		}// Завершение обновления сцены
		/*
***************
Отрисовка сцены
***************
*/
		if (state == State::PLAYING)
		{
			window.clear();
			// Устанавливаем mainView для отображения в окне
			// и отрисовываем все элементы, связанные с ним
			window.setView(mainView);
			// Отрисовываем фон
			window.draw(background, &textureBackground);
			// Отрисовываем зомби
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}
			// Отрисовываем игрового персонажа
			window.draw(player.getSprite());
			// Отрисовываем предметы, если они сгенерированы
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			// Отрисовываем прицел
			window.draw(spriteCrosshair);
			// Переключаемся на HUD
			window.setView(hudView);
			// Отрисовываем все элементы HUD
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);

		}
		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
	}// Конец игрового цикла
	// Очищаем ранее выделенную память (если она существует)
	delete[] zombies;
	return 0;
}