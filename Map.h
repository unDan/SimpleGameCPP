#pragma once

class Map
{
private:

	float height_multiplier; //множитель высоты карты
	float width_multiplier; //множитель ширины карты
	int width; //ширина карты
	int height; //высота карты
	sf::Texture texture; //картинка карты
	sf::Sprite sprite; //спрайты карты

public:
	
	Map(int w, int h) //конструктор класса
	{
		width = w;
		height = h;
		texture.loadFromFile("Sprites/background.png");
		width_multiplier = (float)width / (float)texture.getSize().x;
		height_multiplier = (float)height / (float)texture.getSize().y;
	}

	void create() //создание карты
	{
		sprite.setTexture(texture);
		sprite.setScale(width_multiplier, height_multiplier);
	}

	void draw(sf::RenderWindow &window) //рисовка карты
	{
		sprite.setPosition(0, 0);
		window.draw(sprite);
	}

	sf::Vector2i getSize()
	{
		return sf::Vector2i(width, height);
	}
};