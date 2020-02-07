#include "pch.h"
#include <SFML/Graphics.hpp>
#include <clocale>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include "Parameters.h"
#include "Map.h"
#include "Ammo.h"
#include "Player.h"
#include "Enemies.h"
#define WIDTH_1 1920
#define HEIGHT_1 1080
#define WIDTH_2 1600
#define HEIGHT_2 900
#define WIDTH_3 1366
#define HEIGHT_3 768
#define WIDTH_4 1280
#define HEIGHT_4 720
#define WIDTH_5 800
#define HEIGHT_5 600
#define FPS 600

using namespace std;

enum options {FIRST_OPTION = 1, SECOND_OPTION, THIRD_OPTION, FOURTH_OPTION, FIFTH_OPTION, LAST_OPTION };
void Settings(sf::Vector2i &resolution, bool &fullscreen); //меню настроек
void Menu(int wave_number, int &points, Parameters &parameter, sf::RenderWindow &window); //меню улучшений
void SpawnEnemy(Enemy *&first, Enemy *&last, Enemy *&current, Parameters &parameter); //создать врагов
void AddEnemy(Enemy *&first, Enemy *&last, Enemy *&current, Parameters &parameter); //добавить врага
void DeleteEnemy(Enemy *&first, Enemy *&last, Enemy *&current); //удаление врагов
void SetParameters(Parameters &parameter, sf::Vector2i resolution, int wave_number); //установка параметров
sf::Vector2f SpawnPoint(Parameters &parameter); //рассчёт координат появления врагов

const int enemies_on_screen_amount = 5; //количество врагов которое может быть на экране одновременно

int main()
{
	setlocale(LC_ALL, "russian");
	srand(time(NULL));

	sf::Vector2i resolution; //разрешение экрана
	sf::RenderWindow window; //окно
	sf::Font font; //шрифт
	sf::Text points_text; //текст для отображения очков
	sf::Text wave_text; //текст для отображения номера волны

	int points = STARTING_POINTS_AMOUNT; //очки
	int wave_number = 1; //номер волны

	bool fullscreen = false; //полноэкранный режим?
	Settings(resolution, fullscreen);

	if (fullscreen)
		window.create(sf::VideoMode(resolution.x, resolution.y), "Game", sf::Style::Fullscreen);
	else if (!fullscreen)
		window.create(sf::VideoMode(resolution.x, resolution.y), "Game", sf::Style::Default);

	Parameters parameter;
	SetParameters(parameter, resolution, wave_number);

	Map map(resolution.x, resolution.y);
	map.create();

	Player *player = new Player(map.getSize(), parameter);

	sf::Clock clock; //для времени
	float last_iteration_time = 0.0f; //запоминает время на каждой итерации цикла (для оптимизации рисования объектов)
	parameter.current_enemy_on_screen_amount = enemies_on_screen_amount; 
	parameter.current_enemy_amount = parameter.enemy_for_wave_amount; 

	bool wave_ended = true; //волна окончена?
	bool gameover = false; //игра окончена?

	Ammo *first_ammo = NULL; //head 
	Ammo *last_ammo = NULL; //tail
	Ammo *current_ammo = NULL; //p
	Enemy *first_enemy = NULL;
	Enemy *last_enemy = NULL;
	Enemy *current_enemy = NULL;

	font.loadFromFile("Fonts/arial.ttf"); //загрузка шрифта для текста
	points_text.setFont(font); //установка шрифта для текста
	points_text.setFillColor(sf::Color::Red); //установка цвета для текста
	points_text.setCharacterSize(25); //установка размера букв
	wave_text.setFont(font);
	wave_text.setFillColor(sf::Color::Red);
	wave_text.setCharacterSize(25);


	player->Update(0.0f, map.getSize(), first_ammo, last_ammo, parameter);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); //взятие времени
		clock.restart();
		time /= FPS;
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (wave_ended && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
		{
			wave_ended = false;
			SpawnEnemy(first_enemy, last_enemy, current_enemy, parameter);
		}

		if (wave_ended && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.setVisible(false); //скрывает окно
			delete player; //удаление игрока (для обновления параметров)
			Menu(wave_number, points, parameter, window); //открывает меню
			player = new Player(map.getSize(), parameter); //заново создаёт игрока, обновив параметры
			player->Update(time, map.getSize(), first_ammo, last_ammo, parameter);
			window.setVisible(true); //раскрывает окно
		}

		if (!wave_ended)
		{
			//если количество врагов на экране в данный момент меньше, чем должно быть
			if (parameter.current_enemy_on_screen_amount < enemies_on_screen_amount)
			{
				//если количество врагов на эту волну больше, чем количество, которое должно быть на экране
				if (parameter.current_enemy_amount >= enemies_on_screen_amount)
				{
					AddEnemy(first_enemy, last_enemy, current_enemy, parameter);
					parameter.current_enemy_on_screen_amount++;
				}
			}
		}
	
		if (time != last_iteration_time)
		{
			if (!wave_ended)
			{
				player->Update(time, map.getSize(), first_ammo, last_ammo, parameter); //Обработка движения игрока

				current_ammo = first_ammo;
				while (current_ammo != NULL)
				{
					current_ammo->Update(time, first_ammo, current_ammo, last_ammo); //Обработка движения снарядов
					if (current_ammo != NULL)
						current_ammo = current_ammo->next;

				}

				current_enemy = first_enemy;
				while (current_enemy != NULL)
				{
					current_enemy->Update(time, map.getSize(), first_ammo, current_ammo, last_ammo, first_enemy, last_enemy, current_enemy, gameover, parameter, points); //Обработка движения врагов
					if (current_enemy != NULL)
						current_enemy = current_enemy->next;
				}
			}

			points_text.setString("Points earned: " + to_string(points));
			points_text.setPosition(sf::Vector2f(0, 0));
			wave_text.setString("Wave: " + to_string(wave_number));
			wave_text.setPosition(sf::Vector2f(0, 0 + 1.5f * wave_text.getCharacterSize()));

			if (parameter.current_enemy_amount == 0) //количество врагов 0 = конец волны
			{
				wave_ended = true;
				wave_number++;
				SetParameters(parameter, resolution, wave_number);
			}

			window.clear();

			map.draw(window); //рисование карты
			player->draw(window); //рисование игрока

			current_ammo = first_ammo;
			while (current_ammo != NULL)
			{
				current_ammo->draw(window); //рисование снарядов
				current_ammo = current_ammo->next;
			}

			current_enemy = first_enemy;
			while (current_enemy != NULL)
			{
				current_enemy->draw(window); //рисование врагов
				current_enemy = current_enemy->next;
			}

			window.draw(points_text);
			window.draw(wave_text);

			window.display();
		}

		last_iteration_time = time;

		if (gameover)
		{
			//если какой-то из самолётов достиг низа экрана
			DeleteEnemy(first_enemy, last_enemy, current_enemy);
			system("cls");
			cout << "GAME OVER!\n" << "Волн пройдено: " << wave_number << endl << endl;
			window.close();
		}
		else if (wave_number == 30)
		{
			//если продержаться 30 волн
			DeleteEnemy(first_enemy, last_enemy, current_enemy);
			system("cls");
			cout << "CONGRATULATIONS!!! Вы продержались все 30 волн!" << endl << endl;
			window.close();
		}

	}

	player->ReleaseMemory(first_ammo);
	delete player;

	return 0;
}

void Settings(sf::Vector2i &resolution, bool &fullscreen)
{
	int choice; //выбор пользователя
	do 
	{
		cout << "Выберите разрешение экрана:";
		cout << "\n1) " << WIDTH_1 << "x" << HEIGHT_1;
		cout << "\n2) " << WIDTH_2 << "x" << HEIGHT_2;
		cout << "\n3) " << WIDTH_3 << "x" << HEIGHT_3;
		cout << "\n4) " << WIDTH_4 << "x" << HEIGHT_4;
		cout << "\n5) " << WIDTH_5 << "x" << HEIGHT_5;
		cout << "\n6) Полноэкранный режим";
		cout << endl << endl;
		cin >> choice;
	} while (choice < FIRST_OPTION || choice > LAST_OPTION);

	switch(choice)
	{
		case FIRST_OPTION: resolution = sf::Vector2i(WIDTH_1, HEIGHT_1); break;
		case SECOND_OPTION: resolution = sf::Vector2i(WIDTH_2, HEIGHT_2); break;
		case THIRD_OPTION: resolution = sf::Vector2i(WIDTH_3, HEIGHT_3); break;
		case FOURTH_OPTION: resolution = sf::Vector2i(WIDTH_4, HEIGHT_4); break;
		case FIFTH_OPTION: resolution = sf::Vector2i(WIDTH_5, HEIGHT_5); break;
		case LAST_OPTION: resolution = sf::Vector2i(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); fullscreen = true; break;
	}

	system("cls");
	cout << "Нажмите TAB, чтобы начать новую волну.\nНажмите Esc в перерыве между волнами, чтобы открыть меню.\n\n";
	system("pause");
}

void Menu(int wave_number, int &points, Parameters &parameter, sf::RenderWindow &window)
{
	int answer; //выбор игрока
	static int ammo_damage_cost = AMMO_DAMAGE_STARTING_COST; //стоимость первого улучшения урона снаряда 
	static int ammo_speed_cost = AMMO_SPEED_STARTING_COST; //стоимость первого улучшения скорости снаряда
	static int reloading_time_cost = RELOADING_TIME_STARTING_COST; //стоимость первого улучшения времени перезарядки
	static int player_speed_cost = PLAYER_SPEED_STARTING_COST; //стоимость первого улучшения скорости движения установки

	do
	{
		do
		{
			system("cls");
			cout << "				Меню улучшений:";
			cout << "\nВаше текущее количество очков: " << points;
			cout << "\nТекущий урон снаряда: " << parameter.ammo_damage << " единиц";
			cout << "\nТекущая скорость снаряда: " << parameter.ammo_speed << " пикс./cек.";
			cout << "\nТекущее время перезарядки: " << parameter.reloading_time << " секунды";
			cout << "\nТекущая скорость установки: " << parameter.player_speed << " пикс./сек.";
			cout << "\n";

			if (parameter.ammo_damage_tier < MAX_TIER)
				cout << "\n1) Улучшить урон снаряда (" << ammo_damage_cost << ")";
			if (parameter.ammo_speed_tier < MAX_TIER)
				cout << "\n2) Улучшить скорость снаряда (" << ammo_speed_cost << ")";
			if (parameter.reloading_time_tier < MAX_TIER)
				cout << "\n3) Улучшить время перезарядки (" << reloading_time_cost << ")";
			if (parameter.player_speed_tier < MAX_TIER)
				cout << "\n4) Улучшить скорость установки (" << player_speed_cost << ")";
			cout << "\n5) Продолжить игру";
			cout << "\n6) Выйти из игры\n\n";

			cin >> answer;
		} while (answer < FIRST_OPTION || answer > LAST_OPTION);

		switch (answer)
		{
		case FIRST_OPTION:
		{
			if (points >= ammo_damage_cost)
			{
				points -= ammo_damage_cost; //снимает потраченные очки
				parameter.ammo_damage += DAMAGE_UPGRADE_VALUE; //производит улучшение
				parameter.ammo_damage_tier++; //повышает уровень
				parameter.ammo_texture.loadFromFile("../Sprites/rocketT" + to_string(parameter.ammo_damage_tier) + ".png");
				parameter.player_texture.loadFromFile("../Sprites/gunT" + to_string(parameter.ammo_damage_tier) + ".png");
				ammo_damage_cost += COST_INCREASE_VALUE; //повышает цену
			}
			else
				cout << "\n\nУ вас недостаточно очков!\n\n";
			break;
		}
		case SECOND_OPTION:
		{
			if (points >= ammo_speed_cost)
			{
				points -= ammo_speed_cost;
				parameter.ammo_speed += SPEED_UPGRADE_VALUE;
				parameter.ammo_speed_tier++;
				ammo_speed_cost += COST_INCREASE_VALUE;
			}
			else
				cout << "\n\nУ вас недостаточно очков!\n\n";
			break;
		}
		case THIRD_OPTION:
		{
			if (points >= reloading_time_cost)
			{
				points -= reloading_time_cost;
				parameter.reloading_time -= RELOAD_UPGRADE_VALUE;
				parameter.reloading_time_tier++;
				reloading_time_cost += COST_INCREASE_VALUE;
			}
			else
				cout << "\n\nУ вас недостаточно очков!\n\n";
			break;
		}
		case FOURTH_OPTION:
		{
			if (points >= player_speed_cost)
			{
				points -= player_speed_cost;
				parameter.player_speed += SPEED_UPGRADE_VALUE;
				parameter.player_speed_tier++;
				player_speed_cost += COST_INCREASE_VALUE;
			}
			else
				cout << "\n\nУ вас недостаточно очков!\n\n";
			break;
		}
		case FIFTH_OPTION: break;
		case LAST_OPTION: window.close(); break;
		}

		system("pause");
	} while (answer != FIFTH_OPTION && answer != LAST_OPTION);
}

void SetParameters(Parameters &parameter, sf::Vector2i resolution, int wave_number)
{
	if (wave_number == 1) //установка всех изначальных параметров
	{
		parameter.player_texture.loadFromFile("Sprites/gunT1.png");
		parameter.ammo_texture.loadFromFile("Sprites/rocketT1.png");
		parameter.map_size = resolution;
		parameter.player_speed = PLAYER_START_SPEED;
		parameter.reloading_time = RELOADING_START_TIME;
		parameter.ammo_speed = AMMO_START_SPEED;
		parameter.ammo_damage = AMMO_START_DAMAGE;
		parameter.enemy_speed = ENEMY_START_SPEED;
		parameter.enemy_for_wave_amount = ENEMY_START_AMOUNT;
		parameter.enemy_health = ENEMY_START_HEALTH;
		parameter.ammo_damage_tier = 1;
		parameter.player_speed_tier = 1;
		parameter.ammo_speed_tier = 1;
		parameter.reloading_time_tier = 1;
		parameter.enemy_tier = 1;
	}
	else
	{	
		parameter.enemy_for_wave_amount++;
		parameter.current_enemy_amount = parameter.enemy_for_wave_amount;
		parameter.current_enemy_on_screen_amount = enemies_on_screen_amount;
		if (parameter.enemy_for_wave_amount % 5 == 0)
		{
			if (parameter.enemy_for_wave_amount <= 20)
			{
				parameter.enemy_tier++;
				parameter.enemy_health += HEALTH_UPGRADE_VALUE;
			} 
			parameter.enemy_speed += ENEMY_SPEED_UPGRADE_VALUE;
		}
	}

	parameter.enemy_texture.loadFromFile("Sprites/enemyT" + to_string(parameter.enemy_tier) + ".png");
}

void SpawnEnemy(Enemy *&first, Enemy *&last, Enemy *&current, Parameters &parameter)
{
	int current_enemy_amount = 0; //текущее количество врагов

	//создание списка из врагов
	current = first;
	while (current_enemy_amount < enemies_on_screen_amount)
	{
		if (first == NULL)
		{
			first = new Enemy(SpawnPoint(parameter), parameter);
			last = first;
			current = first;
		}
		else
		{
			last = new Enemy(SpawnPoint(parameter), parameter);
			current->next = last;
			last->prev = current;
			current = last;
		}
		current_enemy_amount++;
	}
}

void AddEnemy(Enemy *&first, Enemy *&last, Enemy *&current, Parameters &parameter)
{
	//добавление врага в конец списка
	current = last;
	last = new Enemy(SpawnPoint(parameter), parameter);
	current->next = last;
	last->prev = current;
	current = last;
}

void DeleteEnemy(Enemy *&first, Enemy *&last, Enemy *&current)
{
	//удаление списка врагов
	current = first;
	last = current;

	while (current != NULL)
	{
		last = current->next;
		delete current;
		current = last;
	}
	current = NULL;
	first = NULL;
	last = NULL;
}

sf::Vector2f SpawnPoint(Parameters &parameter)
{
	//вычисление координат появления самолёта
	float y = 0.0f - parameter.enemy_texture.getSize().y;
	float x = (rand() % (parameter.map_size.x - parameter.enemy_texture.getSize().x)) + parameter.enemy_texture.getSize().x / 2;
	return sf::Vector2f(x, y);
}