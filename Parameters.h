#pragma once
#define MAX_TIER 4
#define AMMO_DAMAGE_STARTING_COST 1600
#define AMMO_SPEED_STARTING_COST 800
#define RELOADING_TIME_STARTING_COST 1200
#define PLAYER_SPEED_STARTING_COST 300
#define HEALTH_UPGRADE_VALUE 50
#define ENEMY_SPEED_UPGRADE_VALUE 0.01f
#define DAMAGE_UPGRADE_VALUE 75
#define SPEED_UPGRADE_VALUE 0.05f
#define RELOAD_UPGRADE_VALUE 0.05f
#define COST_INCREASE_VALUE 1000
#define PLAYER_START_SPEED 0.6f
#define RELOADING_START_TIME 0.45f
#define AMMO_START_SPEED 0.25f
#define AMMO_START_DAMAGE 50
#define ENEMY_START_SPEED 0.1f
#define ENEMY_START_AMOUNT 5
#define ENEMY_START_HEALTH 100
#define STARTING_POINTS_AMOUNT 0;

struct Parameters
{
	float ammo_speed; //скорость снаряда
	float enemy_speed; //скорость врага
	float player_speed; //скорость игрока
	float reloading_time; //время перезарядки
	int ammo_speed_tier; //уровень улучшения скорости снаряда
	int player_speed_tier; //уровень улучшения скорости игрока
	int reloading_time_tier; //уровень улучшения скорости перезарядки
	int ammo_damage_tier; //уровент улучшения урона снаряда
	int enemy_tier; //уровень врагов
	int enemy_for_wave_amount; //количество врагов на волну
	int current_enemy_amount; //текущее количество врагов
	int current_enemy_on_screen_amount; //текущее количество врагов на экране
	int enemy_health; //хп врага
	int ammo_damage; //урон снаряда
	int wave_number; //номер волны
	sf::Vector2i map_size; //размер карты
	sf::Texture player_texture;
	sf::Texture ammo_texture;
	sf::Texture enemy_texture;
};