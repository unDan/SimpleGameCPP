#pragma once

class Map
{
private:

	float height_multiplier; //��������� ������ �����
	float width_multiplier; //��������� ������ �����
	int width; //������ �����
	int height; //������ �����
	sf::Texture texture; //�������� �����
	sf::Sprite sprite; //������� �����

public:
	
	Map(int w, int h) //����������� ������
	{
		width = w;
		height = h;
		texture.loadFromFile("Sprites/background.png");
		width_multiplier = (float)width / (float)texture.getSize().x;
		height_multiplier = (float)height / (float)texture.getSize().y;
	}

	void create() //�������� �����
	{
		sprite.setTexture(texture);
		sprite.setScale(width_multiplier, height_multiplier);
	}

	void draw(sf::RenderWindow &window) //������� �����
	{
		sprite.setPosition(0, 0);
		window.draw(sprite);
	}

	sf::Vector2i getSize()
	{
		return sf::Vector2i(width, height);
	}
};