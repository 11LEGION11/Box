// Проект Сафаров Бокс.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>

const int MAX_WIDTH = 800;
const int MAX_HEIGHT = 600;
const Rectangle ground = { 0,MAX_HEIGHT - 100,MAX_WIDTH,100 };
const Rectangle ground_Stamina = { 0,MAX_HEIGHT - 100,MAX_WIDTH,100 };
int Frames_Counter = 0;
int D1Frames_Counter = 0;
int D2Frames_Counter = 0;

const char* sky_filename = "./Assets/cloud2.png";
const char* ground_filename = "./Assets/moss2.png";
const char* boxer1_filename = "./Assets/Sprite/James.png";
const char* boxer2_filename = "./Assets/Sprite/James.png";
const char* Block_filename = "./Assets/Sprite/James.png";
const char* Block2_filename = "./Assets/Sprite/James.png";
const char* L_Attack_filename = "./Assets/Sprite/James.png";
const char* R_Attack_filename = "./Assets/Sprite/James.png";
const char* Dodge_filename = "./Assets/Sprite/James.png";
const char* Lose_filename = "./Assets/Sprite/James.png";
const char* Appercot_filename = "./Assets/Sprite/James.png";
Texture2D Ground_Texture = { 0 };
Texture2D Sky_Texture = { 0 };


struct Boxer {
	Vector2 Position;
	Vector2 Speed;
	Vector2 L_Hand_Start_Position;
	Vector2 R_Hand_Start_Position;
	Vector2 Block_Position;
	bool Block_Activate;
	bool L_Attack;
	bool R_Attack;
	int Max_Health;
	int Health;
	int Attack;
	int Stamina;
	int Max_Stamina;
	Rectangle Body;
	Rectangle L_Hand;
	Rectangle R_Hand;
	Rectangle Block;
	Color Hands_Color;

	Texture2D Body_Texture;
	Texture2D Block_Texture;
	Texture2D L_Attack_Texture;
	Texture2D R_Attack_Texture;
	Texture2D Appercot_Texture;
	Texture2D Lose;
	bool Lose_Activate;
	bool Dodge_Activate;
	Texture2D Dodge_Texture;

	Boxer(Vector2 position, bool direction) :Position(position) {
		if (direction)
			Speed = { 10, -5 };
		else
			Speed = { -10,-5 };
		Body = { position.x,position.y,120,MAX_HEIGHT / 2 };

		Vector2 l_hand_pos; 
		Vector2 r_hand_pos;
		if(direction)
		{
			l_hand_pos = { Body.x + Body.width + 30,Body.y + Body.height / 2 };
			r_hand_pos = { Body.x + Body.width + 30,Body.y + Body.height / 3 };
		}
		else {
			l_hand_pos = { Body.x -200  + Body.width + 30,Body.y + Body.height / 2 };
			r_hand_pos = { Body.x -200 + Body.width + 30,Body.y + Body.height / 3 };
		}
		if (direction)
			Hands_Color = RED; 
		else
			Hands_Color = BLUE; 
		L_Hand_Start_Position = l_hand_pos;
		R_Hand_Start_Position = r_hand_pos;
		L_Hand = { l_hand_pos.x,l_hand_pos.y,40,40 };
		R_Hand = { r_hand_pos.x,r_hand_pos.y,40,40 };
		Block = { r_hand_pos.x,r_hand_pos.y,20,70 };
		L_Attack = false;
		R_Attack = false;
		Health = 100;
		Max_Health = 100;
		Attack = 10;
		Stamina = 100;
		Max_Stamina = 100;
		Block_Position = r_hand_pos;
		Block = { r_hand_pos.x,r_hand_pos.y,20,70 };
		Block_Activate = false;
		Dodge_Activate = false;
		Lose_Activate = false;
		Image image = LoadImage(boxer1_filename);
		if (image.data != NULL)
		{
			ImageCrop(&image, { 0,4,36,66 });
			ImageResize(&image, Body.width, Body.height);
			if(!direction)
				ImageFlipHorizontal(&image);
			
			Body_Texture = LoadTextureFromImage(image);
			UnloadImage(image);
		}
		image = LoadImage(Block_filename);
		if (image.data != NULL)
		{
			//290,150,25*50
			ImageCrop(&image, { 0,140,34,65 });
			ImageResize(&image, Body.width, Body.height);
			if(!direction)
				ImageFlipHorizontal(&image);
			Block_Texture = LoadTextureFromImage(image);
			UnloadImage(image);
		}
		image = LoadImage(L_Attack_filename);
		if (image.data != NULL)
		{
			ImageCrop(&image, { 154,73,46,65 });
			ImageResize(&image, 220, Body.height);
			if (!direction)
				ImageFlipHorizontal(&image);
			L_Attack_Texture = LoadTextureFromImage(image);
			UnloadImage(image);
		}
		image = LoadImage(R_Attack_filename);
		if (image.data != NULL)
		{
			ImageCrop(&image, { 150,1,50,69 });
			ImageResize(&image, 220, Body.height);
			if (!direction)
				ImageFlipHorizontal(&image);
			R_Attack_Texture = LoadTextureFromImage(image);
			UnloadImage(image);
		}
		image = LoadImage(Appercot_filename);

		if (image.data != NULL)
		{
			ImageCrop(&image, { 70,70,43,67 });
			ImageResize(&image, 220, Body.height); 
			if (!direction)
			ImageFlipHorizontal(&image);
			Appercot_Texture = LoadTextureFromImage(image);
			UnloadImage(image);
		}
		image = LoadImage(Lose_filename);
		{
			ImageCrop(&image, { 173,243,65,33 });
			ImageResize(&image, 220, Body.height);
			if (!direction)
				ImageFlipHorizontal(&image);
			Lose = LoadTextureFromImage(image);
			UnloadImage(image);
		}
		image = LoadImage(Dodge_filename);

		if (image.data != NULL)
		{
			ImageCrop(&image, { 133, 140, 37, 60 });
			ImageResize(&image, Body.width, Body.height); if (!direction)
				ImageFlipHorizontal(&image);
			Dodge_Texture = LoadTextureFromImage(image);
			UnloadImage(image);
		}
	}
};

std::vector<Boxer> boxers = {};
struct Wall {
	Vector2 position;
	float width;
	float height;
};

bool GameOver = false;

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
void InitGame()
{
	Image image = LoadImage(sky_filename);

	if (image.data != NULL)
	{
		ImageResize(&image, MAX_WIDTH, MAX_HEIGHT);
		Sky_Texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(ground_filename);

	if (image.data != NULL)
	{
		ImageResize(&image, MAX_WIDTH, MAX_HEIGHT);
		Ground_Texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}

	boxers.clear();
	GameOver = false;

	Vector2 position = { MAX_WIDTH / 6,MAX_HEIGHT / 2 - 90 };
	Boxer boxer1 = {position,true};
	boxers.push_back(boxer1);

	Vector2 position2 = { MAX_WIDTH / 1.5,MAX_HEIGHT / 2 - 90 };
	Boxer boxer2 = {position2,false};
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

bool DoHit(Boxer& boxer) {
	if (boxer.L_Attack)
	{
		boxer.L_Hand.x += boxer.Speed.x;
		return true;

	}
	if (boxer.R_Attack)
	{
		boxer.R_Hand.x += boxer.Speed.x;
		return true;
	}
	if (boxer.R_Attack)
	{
		boxer.R_Hand.x += boxer.Speed.x;
		return true;
	}
	return false;
}

void UpdateGame() {
	Frames_Counter++;
	if (boxers[0].Stamina > 100) {
		boxers[0].Stamina = 100;
	}
	if (boxers[1].Stamina > 100) {
		boxers[1].Stamina = 100;
	}

	if (Frames_Counter % 10 == 0) {
		if (boxers[0].Stamina < 100)
		{
			boxers[0].Stamina += 5;
		}
		if (boxers[1].Stamina < 100)
		{
			boxers[1].Stamina += 5;
		}
	}
	if (GameOver == true) {
		if (IsKeyPressed(KEY_ENTER))
		{
			InitGame();
			GameOver = false;
		}
		return;
	}
	if (IsKeyPressed(KEY_Q)) {
		if (boxers[0].Stamina > 0 && !boxers[0].Block_Activate && !boxers[0].Dodge_Activate) {
			boxers[0].L_Attack = true;
			boxers[0].Attack = 10;
			boxers[0].Stamina -= 20;
		}
	}
	if (IsKeyPressed(KEY_E)) {
		if (boxers[0].Stamina > 0 && !boxers[0].Block_Activate && !boxers[0].Dodge_Activate) {
			boxers[0].R_Attack = true;
			boxers[0].Attack = 10;
			boxers[0].Stamina -= 20;
		}
	}
	if (IsKeyReleased(KEY_ONE)) {
		if (boxers[0].Stamina > 0 && !boxers[0].Block_Activate && !boxers[0].Dodge_Activate) {
			boxers[0].R_Attack = true;
			boxers[0].Attack = 20;
			boxers[0].Stamina -= 25;
		}
	}
	bool hit1 = DoHit(boxers[1]);

	if (IsKeyPressed(KEY_TWO)) {
		boxers[0].Block_Activate = true;
	}
	if (IsKeyReleased(KEY_TWO)) {
		boxers[0].Block_Activate = false;
	}

	if (IsKeyPressed(KEY_THREE)) {
		if (hit1) {
			boxers[0].Stamina += 20;
		}
		boxers[0].Dodge_Activate = true;
	}
	if (boxers[0].Dodge_Activate) {
		D1Frames_Counter++;
	}
	if (D1Frames_Counter > 120) {
		boxers[0].Dodge_Activate = false;
		D1Frames_Counter = 0;
	}
	if (IsKeyPressed(KEY_I)) {
		if (boxers[1].Stamina > 0 && !boxers[1].Block_Activate && !boxers[1].Dodge_Activate) {
			boxers[1].L_Attack = true;
			boxers[1].Attack = 10;
			boxers[1].Stamina -= 20;
		}
	}
	if (IsKeyPressed(KEY_P)) {
		if (boxers[1].Stamina > 0 && !boxers[1].Block_Activate && !boxers[1].Dodge_Activate) {
			boxers[1].R_Attack = true;
			boxers[1].Attack = 10;
			boxers[1].Stamina -= 20;
		}
	}
	bool hit0 = DoHit(boxers[0]);
	if (IsKeyPressed(KEY_ZERO)) {
		boxers[1].Block_Activate = true;
	}
	if (IsKeyReleased(KEY_ZERO)) {
		boxers[1].Block_Activate = false;
	}
	if (IsKeyReleased(KEY_EIGHT)) {
		if (boxers[1].Stamina > 0 && !boxers[1].Block_Activate && !boxers[1].Dodge_Activate) {
			boxers[1].R_Attack = true;
			boxers[1].Attack = 20;
			boxers[1].Stamina -= 25;
		}
	}
	if (IsKeyPressed(KEY_NINE)) {
		if (hit0) {
			boxers[1].Stamina += 20;
		}
		boxers[1].Dodge_Activate = true;
	}
	if (boxers[1].Dodge_Activate) {
		D2Frames_Counter++;
	}
	if (D2Frames_Counter > 120) {
		boxers[1].Dodge_Activate = false;
		D2Frames_Counter = 0;
	}

	if (HandleHit(boxers[0].Body, boxers[1]) && !boxers[0].Dodge_Activate) {
		if (boxers[0].Block_Activate) {

			boxers[0].Health -= boxers[1].Attack / 2;
		}
		else
		{
			if (boxers[1].R_Attack)
			{
				boxers[0].Health -= boxers[1].Attack * 2;
			}
			else
			{
				boxers[0].Health -= boxers[1].Attack;
			}
		}
	}
	if (HandleHit(boxers[1].Body, boxers[0]) && !boxers[1].Dodge_Activate) {
		if (boxers[1].Block_Activate)
		{
			boxers[1].Health -= boxers[0].Attack / 2;
		}
		else
		{
			if (boxers[0].R_Attack) {

				boxers[1].Health -= boxers[0].Attack * 2;
			}
			else {

				boxers[1].Health -= boxers[0].Attack;
			}
		}
	}

	if (boxers[0].Health <= 0) {
		GameOver = true;
		boxers[0].Lose_Activate = true;
	}
	if (boxers[1].Health <= 0) {
		GameOver = true;
		boxers[1].Lose_Activate = true;
	}
}
void DrawHealthBar(const int& Hp, const int& Max_Health, const int& counter) {
	if (counter == 0)
	{
		DrawRectangle(0, 0, Max_Health, 40, BLACK);
		DrawRectangle(0, 0 + 5, Hp, 30, RED);
	}
	else {
		DrawRectangle(MAX_WIDTH - Max_Health, 0, Max_Health, 40, BLACK);
		DrawRectangle(MAX_WIDTH - Hp, 0 + 5, Hp, 30, RED);
	}
}

void DrawStaminaBar(const int& Stamina, const int& Max_Stamina, const int& counter) {
	if (counter == 0)
	{
		DrawRectangle(0, 30, Max_Stamina, 40, BLACK);
		DrawRectangle(0, 30 + 5, Stamina, 30, GREEN);
	}
	else
	{
		DrawRectangle(MAX_WIDTH - Max_Stamina, 30, Max_Stamina, 40, BLACK);
		DrawRectangle(MAX_WIDTH - Stamina, 30 + 5, Stamina, 30, GREEN);
	}
}
/// <summary>
/// Рисуем боксеров
/// </summary>
/// <param name="boxer"></param>
void DrawBoxer(const Boxer& boxer, int counter) {
	//DrawRectangleRec(boxer.Body, GRAY);

	DrawHealthBar(boxer.Health, boxer.Max_Health, counter);
	DrawStaminaBar(boxer.Stamina, boxer.Max_Stamina, counter);
	if (boxer.Block_Activate) {
		DrawRectangleRec(boxer.Block, boxer.Hands_Color);
		DrawTexture(boxer.Block_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
		//DrawTexture(boxer.l_hand_Texture, boxer.L_Hand.x, boxer.Position.y, RAYWHITE);
		//DrawTexture(boxer.r_hand_Texture, boxer.R_Hand.x, boxer.Position.y, RAYWHITE);
	}
	else if (boxer.Dodge_Activate) {
		DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
		DrawTexture(boxer.Dodge_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
	}
	else if (boxer.L_Attack)
	{
		DrawRectangleRec(boxer.L_Hand, boxer.Hands_Color);
		if (counter % 2 == 1)
			DrawTexture(boxer.L_Attack_Texture, boxer.Position.x - 120, boxer.Position.y, RAYWHITE);
		else
			DrawTexture(boxer.L_Attack_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
	}
	else if (boxer.R_Attack && boxer.Attack == 10)
	{
		DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
		if (counter % 2 == 1)
			DrawTexture(boxer.R_Attack_Texture, boxer.Position.x - 100, boxer.Position.y, RAYWHITE);
		else
			DrawTexture(boxer.R_Attack_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
	}
	else if (boxer.R_Attack && boxer.Attack == 20)
	{
		DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
		if (counter % 2 == 1)
			DrawTexture(boxer.Appercot_Texture, boxer.Position.x - 100, boxer.Position.y, RAYWHITE);
		else
			DrawTexture(boxer.Appercot_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
	}
	else if (boxer.Lose_Activate) {
		DrawTexture(boxer.Lose, boxer.Position.x, boxer.Position.y + 100, RAYWHITE);
	}
	else {
		DrawTexture(boxer.Body_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
		DrawRectangleRec(boxer.L_Hand, boxer.Hands_Color);
		DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
		//DrawTexture(boxer.l_hand_Texture, boxer.L_Hand.x, boxer.L_Hand.y, RAYWHITE);
		//DrawTexture(boxer.r_hand_Texture, boxer.R_Hand.x, boxer.R_Hand.y, RAYWHITE);
	}
}
void DrawGame() {
	BeginDrawing();
	if (GameOver) {
		ClearBackground(RED);
		DrawRectangleRec(ground, BROWN);
		DrawTexture(Sky_Texture, 0, 0, RAYWHITE);
		DrawTexture(Ground_Texture, ground.x, ground.y, GREEN);
		DrawText("GameOver", 300, 70, 20, YELLOW);
		for (int i = 0; i < boxers.size(); i++)
		{
			DrawBoxer(boxers[i], i);
		}
		EndDrawing();
		return;
	}
	ClearBackground(RED);
	DrawRectangleRec(ground, BROWN);
	DrawTexture(Sky_Texture, 0, 0, RAYWHITE);
	DrawTexture(Ground_Texture, ground.x, ground.y, GREEN);
	for (int i = 0; i < boxers.size(); i++)
	{
		DrawBoxer(boxers[i], i);
	}

	EndDrawing();
}
