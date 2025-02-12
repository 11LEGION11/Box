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
	int Max_Health;
	int Health;
	int Attack;

	Rectangle Body;
	Rectangle L_Hand;
	Rectangle R_Hand;
	Rectangle Block;
	Color Hands_Color;

	Texture2D Body_Texture;
	Texture2D Block_Texture;
	Texture2D l_hand_Texture;
	Texture2D r_hand_Texture;
	
};

std::vector<Boxer> boxers = {};

const char* sky_filename = "./Assets/cloud2.png";
const char* ground_filename = "./Assets/moss2.png";
const char* boxer1_filename = "./Assets/maleBase/maleBase/base/base_light.png";
const char* boxer2_filename = "./Assets/maleBase/maleBase/base/base_dark.png";
const char* l_hand_filename = "./Assets/Gloves/glove_L.png";
const char* r_hand_filename = "./Assets/Gloves/glove_R.png";
const char* Block_filename = "./Assets/maleBase/maleBase/base/base_light.png";
const char* Block2_filename = "./Assets/maleBase/maleBase/base/base_dark.png";

Texture2D Ground_Texture = { 0 };
Texture2D Sky_Texture = { 0 };

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
		Sky_Texture= LoadTextureFromImage(image);
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
	Vector2 speed1 = { 20,-5 };
	Vector2 position = { MAX_WIDTH / 6,MAX_HEIGHT / 2 - 90 };
	Rectangle Body = { position.x,position.y,80,MAX_HEIGHT / 2 };
	Vector2 l_hand_pos = { Body.x + Body.width + 5,Body.y + Body.height / 2 };
	Rectangle L_Hand = { l_hand_pos.x,l_hand_pos.y,50,50 };
	Vector2 r_hand_pos = { Body.x + Body.width + 10,Body.y + Body.height / 2 - 50 };
	Rectangle R_Hand = { r_hand_pos.x,r_hand_pos.y,50,50 };
	Color Hands_color = RED;
	int Hp = 100;
	int Max_Health = 100;
	int Attack = 10;
	Vector2 Block_Position = r_hand_pos;
	Rectangle Block = { r_hand_pos.x,r_hand_pos.y,20,70 };
	bool Block_Activate = false;
	bool first_L_attack = false;
	bool first_R_attack = false;
	Texture2D body_texture;
	Texture2D block_texture;
	Texture2D l_hand_texture;
	Texture2D r_hand_texture;

	image = LoadImage(boxer1_filename);
	if (image.data != NULL)
	{
		ImageCrop(&image, { 4,15,22,51 });
		ImageResize(&image, Body.width, Body.height);
		body_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(Block_filename);
	if (image.data != NULL)
	{
		//290,150,25*50
		ImageCrop(&image, { 290,145,24,48 });
		ImageResize(&image, Body.width, Body.height);
		block_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(l_hand_filename);
	if (image.data != NULL)
	{
		ImageRotate(&image, 45);
		ImageResize(&image, L_Hand.width, L_Hand.height);
		l_hand_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(r_hand_filename);
	if (image.data != NULL)
	{
		ImageRotate(&image, 45);
		ImageResize(&image, R_Hand.width, R_Hand.height);
		r_hand_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}

	Boxer boxer1 = {
		position,speed1,
		l_hand_pos,r_hand_pos,
		Block_Position, Block_Activate,
		first_L_attack,first_R_attack,
		Max_Health,Hp,Attack,Body,
		L_Hand,R_Hand,Block,
		Hands_color,
		body_texture,
		block_texture,
		l_hand_texture,
		r_hand_texture
	};
	boxers.push_back(boxer1);

	Vector2 speed2 = { -20,-5 };
	Vector2 position2 = { MAX_WIDTH / 2,MAX_HEIGHT / 2 - 90 };
	Rectangle Body2 = { position2.x,position2.y,80,MAX_HEIGHT / 2 };
	Vector2 l_hand_pos2 = { Body2.x + Body2.width - 110,Body2.y + Body2.height / 2 };
	Rectangle L_Hand2 = { l_hand_pos2.x,l_hand_pos2.y, 50, 50 };
	Vector2 r_hand_pos2 = { Body2.x + Body2.width - 120,Body2.y + Body2.height / 2 - 50 };
	Rectangle R_Hand2 = { r_hand_pos2.x, r_hand_pos2.y,50,50 };
	Color Hands_color2 = DARKBLUE;

	Vector2 Block_Position2 = r_hand_pos2;
	Rectangle Block2 = { r_hand_pos2.x,r_hand_pos2.y,20,70 };
	bool Block_Activate2 = false;
	bool second_L_attack = false;
	bool second_R_attack = false;

	image = LoadImage(boxer2_filename);
	if (image.data != NULL)
	{
		ImageCrop(&image, { 4,15,22,51 });
		ImageResize(&image, Body2.width, Body2.height);
		ImageFlipHorizontal(&image);
		body_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(Block2_filename);
	if (image.data != NULL)
	{
		ImageCrop(&image, { 290,145,24,48 });
		ImageResize(&image, Body2.width, Body2.height);
		ImageFlipHorizontal(&image);
		block_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(l_hand_filename);
	if (image.data != NULL)
	{
		ImageRotate(&image, 45);
		ImageResize(&image, L_Hand.width, L_Hand.height);
		ImageFlipHorizontal(&image);
		l_hand_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	image = LoadImage(r_hand_filename);
	if (image.data != NULL)
	{
		ImageRotate(&image, 45);
		ImageResize(&image, R_Hand.width, R_Hand.height);
		ImageFlipHorizontal(&image);
		r_hand_texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}

	Boxer boxer2 = {
		position2,speed2,
		l_hand_pos2,r_hand_pos2,
		Block_Position2, Block_Activate2,
		second_L_attack,second_R_attack
		,Max_Health,Hp,Attack, Body2,
		L_Hand2,R_Hand2,Block2,
		Hands_color2,
		body_texture,
		block_texture,
		l_hand_texture,
		r_hand_texture
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
	if(GameOver == true){
		if (IsKeyPressed(KEY_ENTER))
		{
			InitGame();
			GameOver = false;
		}
		return;
	}
	if (IsKeyPressed(KEY_Q)) {
		boxers[0].L_Attack = true;
	}
	if (IsKeyPressed(KEY_E)) {
		boxers[0].R_Attack = true;
	}
	if (IsKeyPressed(KEY_TWO)) {
		boxers[0].Block_Activate = true;
	}
	if (IsKeyReleased(KEY_TWO)) {
		boxers[0].Block_Activate = false;
	}
	if (IsKeyPressed(KEY_I)) {
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

	if (HandleHit(boxers[0].Body, boxers[1]) && !boxers[1].Block_Activate) {
		boxers[0].Health -= boxers[1].Attack;
	}
	if (HandleHit(boxers[1].Body, boxers[0]) && !boxers[0].Block_Activate) {
		boxers[1].Health -= boxers[0].Attack;
	}

	if (boxers[0].Block_Activate)
	{
		HandleHit(boxers[0].Block, boxers[1]);
	}
	if (boxers[1].Block_Activate)
	{
		HandleHit(boxers[1].Block, boxers[0]);
	}

	if (boxers[0].Health <= 0) {
		GameOver = true;
	}	
	if (boxers[1].Health <= 0) {
		GameOver = true;
	}
	
}
void DrawHealthBar(const int& Hp,const int& Max_Health,const int& counter) {
	if (counter == 0)
	{
		DrawRectangle(0, 0, Max_Health, 40, BLACK);
		DrawRectangle(0, 0 + 5, Hp, 30, RED);
	}
	else {
		DrawRectangle(MAX_WIDTH  - Max_Health, 0, Max_Health, 40, BLACK);
		DrawRectangle(MAX_WIDTH - Max_Health, 0 + 5, Hp, 30, RED);
	}
}
/// <summary>
/// Рисуем боксеров
/// </summary>
/// <param name="boxer"></param>
void DrawBoxer(const Boxer& boxer, int counter) {
	//DrawRectangleRec(boxer.Body, GRAY);

	DrawHealthBar(boxer.Health, boxer.Max_Health,counter);
	if (boxer.Block_Activate) {
		DrawRectangleRec(boxer.Block, boxer.Hands_Color);
		DrawTexture(boxer.Block_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
	}
	else {
		DrawTexture(boxer.Body_Texture, boxer.Position.x, boxer.Position.y, RAYWHITE);
		DrawRectangleRec(boxer.L_Hand, boxer.Hands_Color);
		DrawRectangleRec(boxer.R_Hand, boxer.Hands_Color);
		DrawTexture(boxer.l_hand_Texture, boxer.L_Hand.x, boxer.L_Hand.y, RAYWHITE);
		DrawTexture(boxer.r_hand_Texture, boxer.R_Hand.x, boxer.R_Hand.y, RAYWHITE);
	}
}
void DrawGame() {
	BeginDrawing();
	if (GameOver) {
		DrawText("GameOver", 300, 70, 20, YELLOW);
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
