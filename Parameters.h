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
	float ammo_speed; //�������� �������
	float enemy_speed; //�������� �����
	float player_speed; //�������� ������
	float reloading_time; //����� �����������
	int ammo_speed_tier; //������� ��������� �������� �������
	int player_speed_tier; //������� ��������� �������� ������
	int reloading_time_tier; //������� ��������� �������� �����������
	int ammo_damage_tier; //������� ��������� ����� �������
	int enemy_tier; //������� ������
	int enemy_for_wave_amount; //���������� ������ �� �����
	int current_enemy_amount; //������� ���������� ������
	int current_enemy_on_screen_amount; //������� ���������� ������ �� ������
	int enemy_health; //�� �����
	int ammo_damage; //���� �������
	int wave_number; //����� �����
	sf::Vector2i map_size; //������ �����
	sf::Texture player_texture;
	sf::Texture ammo_texture;
	sf::Texture enemy_texture;
};