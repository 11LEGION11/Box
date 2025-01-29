// Проект Сафаров Бокс.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>

const int MAX_WIDTH = 800;
const int MAX_HEIGHT = 600;
const Rectangle ground = { 0,MAX_HEIGHT - 100,MAX_WIDTH,100 };

struct Boxer {
	Vector2 Position;
	Vector2 speed;
	Vector2 L_Hand_Start_Position;
	Vector2 R_Hand_Start_Position;

	Rectangle Body;
	Rectangle L_Hand;
	Rectangle R_Hand;
	Color Hands_Color;
};

std::vector<Boxer> boxers = {};

struct Wall {
	Vector2 position;
	float width;
	float height;
};

void InitGame();
/// <summary>
/// Обновление игры
/// </summary>
void UpdateGame();
/// <summary>
/// Отрисовка игры
/// </summary>
void DrawGame();

bool first_L_attack = false;
bool first_R_attack = false;
bool second_L_attack = false;
bool second_R_attack = false;

int main()
{
	InitWindow(MAX_WIDTH, MAX_HEIGHT, "Бокс");
	//Инициализация
	InitGame();
	SetTargetFPS(60);
	//Обновление кадра
	while (!WindowShouldClose()) {
		//	Обновить позицию объектов игры
		UpdateGame();
		//  Обновить графическое отображение игры
		DrawGame();
	}
	CloseWindow();

}
/// <summary>
/// Реализация игры
/// </summary>
void InitGame() {
	Vector2 speed1 = { 20,-5 };
	Vector2 position = { MAX_WIDTH / 4,MAX_HEIGHT / 2 - 90 };
	Rectangle Body = { position.x,position.y,50,MAX_HEIGHT / 2 };
	Vector2 l_hand_pos = { Body.x + Body.width + 5,Body.y + Body.height / 2 };
	Rectangle L_Hand = { l_hand_pos.x,l_hand_pos.y,20,20 };
	Vector2 r_hand_pos = { Body.x + Body.width + 10,Body.y + Body.height / 2 - 50 };
	Rectangle R_Hand = { r_hand_pos.x,r_hand_pos.y,20,20 };
	Color Hands_color = RED;

	Boxer boxer1 = {
		position,speed1,l_hand_pos,r_hand_pos,Body,L_Hand,R_Hand,Hands_color
	};
	boxers.push_back(boxer1);

	Vector2 speed2 = { 20,-5 };
	Vector2 position2 = { MAX_WIDTH / 2,MAX_HEIGHT / 2 - 90 };
	Rectangle Body2 = { position2.x,position2.y,50,MAX_HEIGHT / 2 };
	Vector2 l_hand_pos2 = { Body2.x + Body2.width - 75,Body2.y + Body2.height / 2 };
	Rectangle L_Hand2 = { l_hand_pos2.x,l_hand_pos2.y, 20, 20 };
	Vector2 r_hand_pos2 = { Body2.x + Body2.width - 80,Body2.y + Body2.height / 2 - 50 };
	Rectangle R_Hand2 = { r_hand_pos2.x, r_hand_pos2.y,20,20 };
	Color Hands_color2 = DARKBLUE;

	Boxer boxer2 = {
		position2,speed2,l_hand_pos2,r_hand_pos2,Body2,L_Hand2,R_Hand2,Hands_color2
	};
	boxers.push_back(boxer2);
}
void UpdateGame() {
	if (IsKeyPressed(KEY_Q)) {
		first_L_attack = true;
	}
	if (IsKeyPressed(KEY_W)) {
		first_R_attack = true;
	}
	if (IsKeyPressed(KEY_O)) {
		second_L_attack = true;
	}
	if (IsKeyPressed(KEY_P)) {
		second_R_attack = true;
	}
	if (first_L_attack) boxers[0].L_Hand.x += boxers[0].speed.x;
	if (first_R_attack) boxers[0].R_Hand.x += boxers[0].speed.x;
	if (second_L_attack) boxers[1].L_Hand.x -= boxers[1].speed.x;
	if (second_R_attack) 
		boxers[1].R_Hand.x -= boxers[1].speed.x;

	if (CheckCollisionRecs(boxers[0].Body, boxers[1].L_Hand))
	{
		boxers[1].L_Hand.x = boxers[1].L_Hand_Start_Position.x;
		boxers[1].L_Hand.y = boxers[1].L_Hand_Start_Position.y;
		second_L_attack = false;
	}

	if (CheckCollisionRecs(boxers[0].Body, boxers[1].R_Hand))
	{
		boxers[1].R_Hand.x = boxers[1].R_Hand_Start_Position.x;
		boxers[1].R_Hand.y = boxers[1].R_Hand_Start_Position.y;
		second_R_attack = false;
	}
	if (CheckCollisionRecs(boxers[1].Body, boxers[0].L_Hand))
	{
		boxers[0].L_Hand.x = boxers[0].L_Hand_Start_Position.x;
		boxers[0].L_Hand.y = boxers[0].L_Hand_Start_Position.y;
		first_L_attack = false;
	}
	if (CheckCollisionRecs(boxers[1].Body, boxers[0].R_Hand))
	{
		boxers[0].R_Hand.x = boxers[0].R_Hand_Start_Position.x;
		boxers[0].R_Hand.y = boxers[0].R_Hand_Start_Position.y;
		first_R_attack = false;
	}
}
/// <summary>
/// Рисуем боксеров
/// </summary>
/// <param name="boxer"></param>
void DrawBoxer(const Boxer& boxer) {
	DrawRectangleRec(boxer.Body, GRAY);
	DrawRectangleRec(boxer.L_Hand, boxer.Hands_Color);
	DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
}
void DrawGame() {
	BeginDrawing();
	ClearBackground(SKYBLUE);
	DrawRectangleRec(ground, BROWN);
	for (const auto& boxer : boxers) {
		DrawBoxer(boxer);
	}
	EndDrawing();
}
