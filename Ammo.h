#pragma once

class Ammo
{
private:
	sf::Texture texture; //�������� ���������
	sf::Sprite sprite; //���� ���������
	float speed; //�������� ������ �������
	float x, y; //������� ���������� ������ �������

public:
	Ammo *next = NULL;
	Ammo *prev = NULL;
	float damage; //���� ��������� ����� ��������

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
			y -= speed * time; //�������� ������� ����� �����
		else
			this->remove(first_ammo, current_ammo, last_ammo); //�������� ������� �������
		
		sprite.setPosition(x, y);
	}

	void remove(Ammo *&first_ammo, Ammo *&current_ammo, Ammo *&last_ammo)
	{
		if (this == first_ammo) //���� ��������� ������ ������� ������
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
		else if (this == last_ammo) //���� ��������� ��������� ������� ������
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