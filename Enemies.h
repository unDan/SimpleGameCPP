#pragma once

class Enemy
{
private:
	sf::Texture texture; //�������� �����
	sf::Sprite sprite; //����
	float speed; //�������� 
	float explode_time = 0;
	float health; //�������� �����
	float x, y; //������� ���������� 

	bool CollisionDetection(Ammo *&current_ammo, Ammo *&first_ammo) //�������� ������������ �����-���� �� ����� � ������� ��������
	{
		current_ammo = first_ammo;
		while (current_ammo != NULL)
		{
			if ((current_ammo->getPosition().x >= this->x - (texture.getSize().x)) && (current_ammo->getPosition().x <= this->x + (texture.getSize().x)))
			{
				if (current_ammo->getPosition().y <= this->y + (texture.getSize().y / 2.0f))
				{
					return true;
				}
			}
			current_ammo = current_ammo->next;
		}

		return false;
	}

public:

	Enemy *next = NULL;
	Enemy *prev = NULL;

	Enemy(sf::Vector2f spawnpoint, Parameters parameter)
	{
		texture = parameter.enemy_texture;
		sprite.setTexture(texture);
		sprite.scale(1, -1); //��������� ������� �� �����������
		sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f); //��������� ������ �������

		x = spawnpoint.x;
		y = spawnpoint.y;

		speed = parameter.enemy_speed + (((float)((rand() % 21) - 50) / 1000.0f));
		health = parameter.enemy_health;
	}

	void Update(float time, sf::Vector2i mapSize, Ammo *&first_ammo, Ammo *&current_ammo, Ammo *&last_ammo, Enemy *&first_enemy, Enemy *&last_enemy, Enemy *&current_enemy, bool &gameover, Parameters &parameter, int &points)
	{
		if (y < mapSize.y - texture.getSize().y/2 && !CollisionDetection(current_ammo, first_ammo)) //������� �������� �������
			y += time * speed; //�������� ������� ���� �����
		else if (CollisionDetection(current_ammo, first_ammo)) //���� � ������ ����� ������
		{
			health -= current_ammo->damage; //���������� �������� ��� ��������� ������
			current_ammo->remove(first_ammo, current_ammo, last_ammo);
			if (health <= 0)
			{
				//���� ������ ���������
				speed = 0;
				parameter.current_enemy_amount--;
				parameter.current_enemy_on_screen_amount--;
				points += 100;
				this->remove(first_enemy, current_enemy, last_enemy);
			}
		}
		else //���� ������ ����� �� ������ ������� �����
			gameover = true;

		sprite.setPosition(x, y);
	}

	void remove(Enemy *&first, Enemy *&current, Enemy *&last)
	{
		if (this == first) //���� ��������� ������ ������� ������
		{
			if (this->next == NULL)
			{
				first = NULL;
				last = NULL;
				current = NULL;
				delete this;
			}
			else
			{
				first = this->next;
				current = first;
				current->prev = NULL;
				delete this;
			}
		}
		else if (this == last) //���� ��������� ��������� ������� ������
		{
			last = this->prev;
			last->next = NULL;
			current = last;
			delete this;
		}
		else
		{
			current = this->next;
			(this->prev)->next = current;
			current->prev = this->prev;
			delete this;
		}
	}

	void draw(sf::RenderWindow &window)
	{
		window.draw(sprite);
	}


};