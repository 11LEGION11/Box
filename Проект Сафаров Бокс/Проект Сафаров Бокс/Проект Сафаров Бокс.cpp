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
	Vector2 Speed;
	Vector2 L_Hand_Start_Position;
	Vector2 R_Hand_Start_Position;
	Vector2 Block_Position;
	bool Block_Activate;
	bool L_Attack;
	bool R_Attack;

	Rectangle Body;
	Rectangle L_Hand;
	Rectangle R_Hand;
	Rectangle Block;
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

	Vector2 Block_Position = r_hand_pos;
	Rectangle Block = { r_hand_pos.x,r_hand_pos.y,20,70 };
	bool Block_Activate = false;
	bool first_L_attack = false;
	bool first_R_attack = false;
	Boxer boxer1 = {
		position,speed1,l_hand_pos,r_hand_pos,Block_Position, Block_Activate,first_L_attack,first_R_attack,Body,L_Hand,R_Hand,Block,Hands_color
	};
	boxers.push_back(boxer1);

	Vector2 speed2 = { -20,-5 };
	Vector2 position2 = { MAX_WIDTH / 2,MAX_HEIGHT / 2 - 90 };
	Rectangle Body2 = { position2.x,position2.y,50,MAX_HEIGHT / 2 };
	Vector2 l_hand_pos2 = { Body2.x + Body2.width - 75,Body2.y + Body2.height / 2 };
	Rectangle L_Hand2 = { l_hand_pos2.x,l_hand_pos2.y, 20, 20 };
	Vector2 r_hand_pos2 = { Body2.x + Body2.width - 80,Body2.y + Body2.height / 2 - 50 };
	Rectangle R_Hand2 = { r_hand_pos2.x, r_hand_pos2.y,20,20 };
	Color Hands_color2 = DARKBLUE;

	Vector2 Block_Position2 = r_hand_pos2;
	Rectangle Block2 = { r_hand_pos2.x,r_hand_pos2.y,20,70 };
	bool Block_Activate2 = false;
	bool second_L_attack = false;
	bool second_R_attack = false;
	Boxer boxer2 = {
		position2,speed2,l_hand_pos2,r_hand_pos2,Block_Position2, Block_Activate2,second_L_attack,second_R_attack,Body2,L_Hand2,R_Hand2,Block2,Hands_color2
	};
	boxers.push_back(boxer2);
}
bool HandleHit(const Rectangle& body, Boxer& boxer) {
	if (CheckCollisionRecs(body, boxer.L_Hand))
	{
		boxer.L_Hand.x = boxer.L_Hand_Start_Position.x;
		boxer.L_Hand.y = boxer.L_Hand_Start_Position.y;
		boxer.L_Attack = false;
		return true;
	}
	if (CheckCollisionRecs(body, boxer.R_Hand))
	{
		boxer.R_Hand.x = boxer.R_Hand_Start_Position.x;
		boxer.R_Hand.y = boxer.R_Hand_Start_Position.y;
		boxer.R_Attack = false;
		return true;
	}
	return false;
}

void DoHit(Boxer& boxer) {
	if (boxer.L_Attack) boxer.L_Hand.x += boxer.Speed.x;
	if (boxer.R_Attack) boxer.R_Hand.x += boxer.Speed.x;
}

void UpdateGame() {
	if (IsKeyPressed(KEY_Q)) {
		boxers[0].L_Attack = true;
	}
	if (IsKeyPressed(KEY_W)) {
		boxers[0].R_Attack = true;
	}
	if (IsKeyPressed(KEY_TWO)) {
		boxers[0].Block_Activate = true;
	}
	if (IsKeyReleased(KEY_TWO)) {
		boxers[0].Block_Activate = false;
	}
	if (IsKeyPressed(KEY_O)) {
		boxers[1].L_Attack = true;
	}
	if (IsKeyPressed(KEY_P)) {
		boxers[1].R_Attack = true;
	}
	if (IsKeyPressed(KEY_ZERO)) {
		boxers[1].Block_Activate = true;
	}
	if (IsKeyReleased(KEY_ZERO)) {
		boxers[1].Block_Activate = false;
	}

	DoHit(boxers[0]);
	DoHit(boxers[1]);

	HandleHit(boxers[0].Body, boxers[1]);
	HandleHit(boxers[1].Body, boxers[0]);
	if (boxers[0].Block_Activate)
	{
		HandleHit(boxers[0].Block, boxers[1]);
	}
	if (boxers[1].Block_Activate)
	{
		HandleHit(boxers[1].Block, boxers[0]);
	}


}
/// <summary>
/// Рисуем боксеров
/// </summary>
/// <param name="boxer"></param>
void DrawBoxer(const Boxer& boxer) {
	DrawRectangleRec(boxer.Body, GRAY);
	if (boxer.Block_Activate) {
		DrawRectangleRec(boxer.Block, boxer.Hands_Color);
	}
	else {
		DrawRectangleRec(boxer.L_Hand, boxer.Hands_Color);
		DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
	}
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
