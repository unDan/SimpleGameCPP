#pragma once

class Ammo
{
private:
	sf::Texture texture; //текстура установки
	sf::Sprite sprite; //сама установка
	float speed; //скорость одного снаряда
	float x, y; //текущие координаты одного снаряда

public:
	Ammo *next = NULL;
	Ammo *prev = NULL;
	float damage; //урон наносимый одним снарядом

	Ammo(sf::Vector2f barrel_pos, Parameters &parameter)
	{
		texture = parameter.ammo_texture;
		sprite.setTexture(texture);
		sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);

		x = barrel_pos.x;
		y = barrel_pos.y - texture.getSize().y / 2;

		speed = parameter.ammo_speed;
		damage = parameter.ammo_damage;
	}

	void Update(float time, Ammo *&first_ammo, Ammo *&current_ammo, Ammo *&last_ammo)
	{
		if (y > 0.0f - texture.getSize().y / 2)
			y -= speed * time; //движение снаряда вверх карты
		else
			this->remove(first_ammo, current_ammo, last_ammo); //удаление данного снаряда
		
		sprite.setPosition(x, y);
	}

	void remove(Ammo *&first_ammo, Ammo *&current_ammo, Ammo *&last_ammo)
	{
		if (this == first_ammo) //если удаляется первый элемент списка
		{
			if (this->next == NULL)
			{
				first_ammo = NULL;
				last_ammo = NULL;
				current_ammo = NULL;
				delete this;
			}
			else
			{
				first_ammo = this->next;
				current_ammo = first_ammo;
				current_ammo->prev = NULL;
				delete this;
			}
		}
		else if (this == last_ammo) //если удаляется последний элемент списка
		{
			last_ammo = this->prev;
			last_ammo->next = NULL;
			current_ammo = last_ammo;
			delete this;
		}
		else
		{
			current_ammo = this->next;
			(this->prev)->next = current_ammo;
			current_ammo->prev = this->prev;
			delete this;
		}
	}

	sf::Vector2f getPosition()
	{
		return sf::Vector2f(x, y);
	}

	void draw(sf::RenderWindow &window)
	{	
		window.draw(sprite);
	}
};