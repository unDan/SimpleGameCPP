#pragma once

class Player
{
private:
	sf::Texture texture; //текстура установки
	sf::Sprite sprite; //сама установка
	float x, y; //координаты установки
	float speed; //скорость установки
	float reloading_time; //скорость перезарядки 
	sf::Clock reload_timer; 
	sf::Vector2f barrel_pos; //позиция откуда вылетает снаряд

	void AddToList(Ammo *&first_ammo, Ammo *&last_ammo, Parameters &parameter)
	{
		static Ammo *current;
		if (first_ammo == NULL)
		{
			first_ammo = new Ammo(barrel_pos, parameter);
			last_ammo = first_ammo;
			current = last_ammo;			
		}
		else
		{
			last_ammo = new Ammo(barrel_pos, parameter);
			last_ammo->prev = current;
			current->next = last_ammo;
			current = last_ammo;
		}
	}

public:

	Player(sf::Vector2i map_size, Parameters &parameter) //конструктор по умолчанию
	{
		texture = parameter.player_texture;
		sprite.setTexture(texture);

		sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

		x = map_size.x / 2; //изначальный Х установки
		y = map_size.y - (texture.getSize().y / 2); //изначальный У установки

		speed = parameter.player_speed;
		reloading_time = parameter.reloading_time;
	}

	void Update(float time, sf::Vector2i mapSize, Ammo *&first_ammo, Ammo *&last_ammo, Parameters &parameter)
	{
		float dx = 0; //текущая скорость
		float reloading = reload_timer.getElapsedTime().asSeconds(); //текущее время перезарядки

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && x < (mapSize.x - texture.getSize().x / 2)) //движение вправо
			dx = speed; 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && x > texture.getSize().x / 2) //движение влево
			dx = -speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && reloading >= reloading_time) //стрельба
		{
			reload_timer.restart(); //перезарядка
			AddToList(first_ammo, last_ammo, parameter);
		}

		x += dx * time; //движение установки вправо или влево
		barrel_pos = sf::Vector2f(x, y - (texture.getSize().y / 2));
		dx = 0;
		sprite.setPosition(x,y);
	}

	void draw(sf::RenderWindow &window) //рисование установки
	{
		window.draw(sprite);
	}

	sf::Vector2f getBarrelPosition()
	{
		return sf::Vector2f(x, y - (texture.getSize().y/2));
	}

	void ReleaseMemory(Ammo *&first_ammo)
	{
		Ammo *current = first_ammo;
		while (current != NULL)
		{
			current = current->next;
			delete current->prev;
		}
	}
};
