#include <iostream>
#include <raylib.h>
#include <cmath>
using namespace std;



const int SH = 900;
const int SW = 1500;
const float GRAVITY = 0.5f;
const float JUMP = -13.0f;
float RESPAWN_TIME = 1.0f;





struct Bullet
{
    float x, y;
    float dirX, dirY;
    bool active;
    Rectangle collider;
};

struct Enemy
{
    float x, y;
    float speed;
    bool active;
    float rT;
    Rectangle collider;
};

void ShowMenu(bool& isMenu, bool& isPlaying, bool& isControls, Texture2D Page)
{
    while (isMenu)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(Page, 0, 0, WHITE);
        DrawText("METAL SLUG X", SW / 2 - 150, SH / 3 - 50, 50, LIGHTGRAY);
        DrawText("PRESS P TO PLAY", SW / 2 - 100, SH / 2, 30, LIGHTGRAY);
        DrawText("PRESS C FOR CONTROLS", SW / 2 - 150, SH / 2 + 50, 30, LIGHTGRAY);
        DrawText("PRESS E TO EXIT", SW / 2 - 100, SH / 2 + 100, 30, LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_P))
        {
            isMenu = false;
            isPlaying = true;
        }
        if (IsKeyPressed(KEY_C))
        {
            isMenu = false;
            isControls = true;

        }
        if (IsKeyPressed(KEY_E))
        {
            CloseWindow();
            exit(0);
        }
    }
}

void HandlePlayerMovement(float& chX, float& chY, float& vy, bool& isJ, bool& R, float speed)
{
    if (IsKeyDown(KEY_RIGHT))
    {
        chX += speed;
        R = true;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        chX -= speed;
        R = false;
    }

    if (chX < 0) chX = 0;
    if (chX > SW - 100) chX = SW - 100;

    if (IsKeyPressed(KEY_SPACE) && !isJ)
    {
        vy = JUMP;
        isJ = true;
    }

    vy += GRAVITY;
    chY += vy;

    if (chY >= SH - 250)
    {
        chY = SH - 250;
        vy = 0;
        isJ = false;
    }
}

void HandleBullets(Bullet bullets[], int mB, float chX, float chY, bool R, Sound beep, Enemy enemies[], int maxEnemies, int& Score)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mP = GetMousePosition();
        for (int i = 0; i < mB; i++)
        {
            if (!bullets[i].active)
            {
                bullets[i].active = true;
                if (R)
                {
                    bullets[i].x = chX + 90;
                }
                else
                {
                    bullets[i].x = chX - 10;
                }
                bullets[i].y = chY + 65;
                float deltaX = mP.x - bullets[i].x;
                float deltaY = mP.y - bullets[i].y;
                float length = sqrt(deltaX * deltaX + deltaY * deltaY);
                bullets[i].dirX = (deltaX / length) * 7;
                bullets[i].dirY = (deltaY / length) * 7;
                bullets[i].collider = { bullets[i].x, bullets[i].y, 40, 20 };
                PlaySound(beep);
                break;
            }
        }
    }

    for (int i = 0; i < mB; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].dirX;
            bullets[i].y += bullets[i].dirY;
            if (bullets[i].x < 0 || bullets[i].x > SW || bullets[i].y < 0 || bullets[i].y > SH)
                bullets[i].active = false;

            bullets[i].collider.x = bullets[i].x;
            bullets[i].collider.y = bullets[i].y;

            for (int j = 0; j < maxEnemies; j++)
            {
                if (enemies[j].active && CheckCollisionRecs(bullets[i].collider, enemies[j].collider))
                {
                    enemies[j].active = false;
                    enemies[j].rT = RESPAWN_TIME;
                    bullets[i].active = false;
                    Score += 5;
                    break;
                }
            }
        }
    }
}

void HandleEnemies(Enemy enemies[], int maxEnemies, float enemyWidth)
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].rT -= GetFrameTime();
            if (enemies[i].rT <= 0)
            {
                enemies[i].active = true;
                enemies[i].x = SW + 100;
            }
        }
        else

        {
            enemies[i].x -= enemies[i].speed;
            enemies[i].collider.x = enemies[i].x;
            if (enemies[i].x < -enemyWidth)
            {
                enemies[i].x = SW + 100;
            }
        }
    }
}

void HandleCollisions(float chX, float chY, Enemy enemies[], int maxEnemies, bool& isPlay, bool& isGameOver)
{
    Rectangle playerCollider = { chX, chY, 100, 150 };

    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemies[i].active && CheckCollisionRecs(playerCollider, enemies[i].collider))
        {
            isPlay = false;
            isGameOver = true;
            break;
        }
    }
}


void DrawGame(Texture2D img1, Texture2D chRight, Texture2D chLeft, float chX, float chY, bool R,
    Texture2D enemy, Enemy enemies[], int maxEnemies, Texture2D Bull, Bullet bullets[], int mB, int& Score)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(img1, 0, 0, WHITE);

    DrawText(TextFormat("Score is %d", Score), 0, 50, 50, RAYWHITE);

    if (R)
    {
        DrawTexture(chRight, chX, chY, WHITE);
    }
    else
    {
        DrawTexture(chLeft, chX, chY, WHITE);
    }


    for (int i = 0; i < maxEnemies; i++)
        if (enemies[i].active)
            DrawTexture(enemy, enemies[i].x, enemies[i].y, WHITE);

    for (int i = 0; i < mB; i++)
        if (bullets[i].active)
            DrawTexture(Bull, bullets[i].x, bullets[i].y, WHITE);

    EndDrawing();
}

void level_1(int& Score)
{
    InitAudioDevice();
    InitWindow(SW, SH, "Metal Slug X");
    Texture2D img1 = LoadTexture("1.png");
    Texture2D chRight = LoadTexture("ch.png");
    Texture2D chLeft = LoadTexture("ch1.png");
    Texture2D Bull = LoadTexture("Bullet.png");
    Texture2D enemy = LoadTexture("enmy.png");
    Texture2D Loading = LoadTexture("Loading.png");
    Texture2D Loading2 = LoadTexture("Loading2.png");
    Texture2D Loading3 = LoadTexture("Loading3.png");
    Texture2D Page = LoadTexture("A1.png");
    Texture2D Page1 = LoadTexture("A1.png");


    bool sMode = true;


    Page1.height = SH;
    Page1.width = SW;
    Page.height = SH;
    Page.width = SW;
    img1.height = SH;
    img1.width = SW;
    chRight.height = chLeft.height = 150;
    chRight.width = chLeft.width = 100;
    Bull.height = 20;
    Bull.width = 40;
    enemy.height = 150;
    enemy.width = 100;




    while (sMode)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(Page1, 0, 0, WHITE);
        DrawText("SELECT MODE", SW / 2 - 50, SH / 2 - 250, 30, LIGHTGRAY);
        DrawText("--------------------- E _ EASY --------------------- ", SW / 2 - 250 - 100, SH / 2 - 180, 30, GREEN);
        DrawText("--------------------- M _ MEDIUM -------------------", SW / 2 - 250 - 100, SH / 2 - 100, 30, GOLD);
        DrawText("--------------------- H _ HARD ---------------------", SW / 2 - 250 - 100, SH / 2 - 20, 30, RED);

        EndDrawing();

        if (IsKeyPressed(KEY_E))
        {
            RESPAWN_TIME = 1.0f;
            sMode = false;
        }
        else if (IsKeyPressed(KEY_M))
        {
            RESPAWN_TIME = 0.6f;
            sMode = false;
        }
        else if (IsKeyPressed(KEY_H))
        {
            RESPAWN_TIME = 0.2f;
            sMode = false;
        }
    }
    UnloadTexture(Page1);




    float chX = SW / 2 - 100;
    float chY = SH - 250;
    float speed = 5;
    float vy = 0;
    bool isJ = false;
    bool R = true;
    const int mB = 10;
    Bullet bullets[mB] = {};

    Sound beep = LoadSound("beep.wav");
    Music b = LoadMusicStream("Sd.mp3");
    float loadingTime = 0.0f;
    bool isLoading = true;
    bool isMenu = true;
    bool isPlay = false;
    bool isControls = false;
    bool isGameOver = false;
    const int maxEnemies = 3;
    Enemy enemies[maxEnemies] = {};

    for (int i = 0; i < maxEnemies; i++)
    {
        enemies[i].x = SW + (i * 400);
        enemies[i].y = SH - 250;
        enemies[i].speed = 2 + (i * 0.5f);
        enemies[i].active = true;
        enemies[i].rT = 0.0f;
        enemies[i].collider = { enemies[i].x, enemies[i].y, (float)enemy.width, (float)enemy.height };
    }


    void (*playerMovement)(float&, float&, float&, bool&, bool&, float) = HandlePlayerMovement;
    void (*bulletHandler)(Bullet[], int, float, float, bool, Sound, Enemy[], int, int&) = HandleBullets;
    void (*enemyHandler)(Enemy[], int, float) = HandleEnemies;
    void (*collisionHandler)(float, float, Enemy[], int, bool&, bool&) = HandleCollisions;
    void (*menuHandler)(bool&, bool&, bool&, Texture2D) = ShowMenu;
    void (*gameDrawer)(Texture2D, Texture2D, Texture2D, float, float, bool, Texture2D, Enemy[], int, Texture2D, Bullet[], int, int&) = DrawGame;

    SetTargetFPS(60);
    PlayMusicStream(b);

    while (!WindowShouldClose())
    {
        if (Score > 100)
        {
            break;
        }
        UpdateMusicStream(b);

        if (isLoading)
        {
            loadingTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (loadingTime < 1.5)
                DrawTexture(Loading, SW / 2 - Loading.width / 2, SH / 2 - Loading.height / 2, WHITE);
            else if (loadingTime < 2.7)
                DrawTexture(Loading2, SW / 2 - Loading.width / 2, SH / 2 - Loading.height / 2, WHITE);
            else if (loadingTime < 3.0)
                DrawTexture(Loading3, SW / 2 - Loading.width / 2, SH / 2 - Loading.height / 2, WHITE);
            EndDrawing();
            if (loadingTime > 3.0f)
            {
                isLoading = false;
                loadingTime = 0.0f;
            }
        }
        else if (isMenu)
        {
            menuHandler(isMenu, isPlay, isControls, Page);
        }
        else if (isGameOver)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("YOU LOSE!", SW / 2 - 150, SH / 2 - 50, 50, RED);
            DrawText("Press R to Restart or M for Menu", SW / 2 - 250, SH / 2 + 50, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_R))
            {
                isGameOver = false;
                isPlay = true;
                chX = SW / 2 - 100;
                chY = SH - 250;
                Score = 0;

                for (int i = 0; i < maxEnemies; i++)
                {
                    enemies[i].x = SW + (i * 400);
                    enemies[i].active = true;
                }
            }
            else if (IsKeyPressed(KEY_M))
            {
                isGameOver = false;
                isMenu = true;
            }
        }
        else if (isPlay)
        {
            playerMovement(chX, chY, vy, isJ, R, speed);
            bulletHandler(bullets, mB, chX, chY, R, beep, enemies, maxEnemies, Score);
            enemyHandler(enemies, maxEnemies, enemy.width);
            collisionHandler(chX, chY, enemies, maxEnemies, isPlay, isGameOver);
            gameDrawer(img1, chRight, chLeft, chX, chY, R, enemy, enemies, maxEnemies, Bull, bullets, mB, Score);
        }
    }
}






void ShowMenu2(bool& isMenu, bool& isPlaying, bool& isControls, Texture2D Page)
{
    while (isMenu)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(Page, 0, 0, WHITE);
        DrawText("METAL SLUG X", SW / 2 - 150, SH / 3 - 50, 50, LIGHTGRAY);
        DrawText("Level 2 started ", SW / 2 - 100, SH / 2, 30, LIGHTGRAY);
        DrawText("PRESS P to continue ", SW / 2 - 150, SH / 2 + 50, 30, LIGHTGRAY);
        DrawText("PRESS E TO EXIT", SW / 2 - 100, SH / 2 + 100, 30, LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_P))
        {
            isMenu = false;
            isPlaying = true;
        }
        /*if (IsKeyPressed(KEY_C))
        {
            isMenu = false;
            isControls = true;

        }*/
        if (IsKeyPressed(KEY_E))
        {
            CloseWindow();
            exit(0);
        }
    }
}





void HandlePlayerMovement2(float& chX, float& chY, float& vy, bool& isJ, bool& R, float speed, float& backgroundOffset, Texture2D img1)
{
    if (IsKeyDown(KEY_RIGHT))
    {
        chX += speed;
        backgroundOffset -= speed;
        R = true;

    }
    if (IsKeyDown(KEY_LEFT))
    {

        chX -= speed;
        if (backgroundOffset < 0)
            backgroundOffset += speed;
        R = false;



    }

    if (chX < 0) chX = 0;
    if (chX > SW / 2 - 100) chX = SW / 2 - 100;

    if (IsKeyPressed(KEY_SPACE) && !isJ)
    {
        vy = JUMP;
        isJ = true;
    }

    vy += GRAVITY;
    chY += vy;

    if (chY >= SH - 250)
    {
        chY = SH - 250;
        vy = 0;
        isJ = false;
    }
}




void HandleBullets2(Bullet bullets[], int mB, float chX, float chY, bool R, Sound beep, Enemy enemies[], int maxEnemies, int& Score, Enemy enemies2[], int maxEnemies2)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mP = GetMousePosition();
        for (int i = 0; i < mB; i++)
        {
            if (!bullets[i].active)
            {
                bullets[i].active = true;
                if (R)
                {
                    bullets[i].x = chX + 90;
                }
                else
                {
                    bullets[i].x = chX - 10;
                }
                bullets[i].y = chY + 65;
                float deltaX = mP.x - bullets[i].x;
                float deltaY = mP.y - bullets[i].y;
                float length = sqrt(deltaX * deltaX + deltaY * deltaY);
                bullets[i].dirX = (deltaX / length) * 7;
                bullets[i].dirY = (deltaY / length) * 7;
                bullets[i].collider = { bullets[i].x, bullets[i].y, 40, 20 };
                PlaySound(beep);
                break;
            }
        }
    }

    for (int i = 0; i < mB; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].dirX;
            bullets[i].y += bullets[i].dirY;
            if (bullets[i].x < 0 || bullets[i].x > SW || bullets[i].y < 0 || bullets[i].y > SH)
                bullets[i].active = false;

            bullets[i].collider.x = bullets[i].x;
            bullets[i].collider.y = bullets[i].y;

            for (int j = 0; j < maxEnemies; j++)
            {
                if (enemies[j].active && CheckCollisionRecs(bullets[i].collider, enemies[j].collider))
                {
                    enemies[j].active = false;
                    enemies[j].rT = RESPAWN_TIME;
                    bullets[i].active = false;
                    Score += 5;
                    break;
                }
            }
            for (int j = 0; j < maxEnemies2; j++)
            {
                if (enemies2[j].active && CheckCollisionRecs(bullets[i].collider, enemies2[j].collider))
                {
                    enemies2[j].active = false;
                    enemies2[j].rT = RESPAWN_TIME;
                    bullets[i].active = false;
                    Score += 5;
                    break;
                }
            }

        }
    }
}

void HandleEnemies2(Enemy enemies[], int maxEnemies, float enemyWidth)
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].rT -= GetFrameTime();
            if (enemies[i].rT <= 0)
            {
                enemies[i].active = true;
                enemies[i].x = SW + 100;
            }
        }
        else

        {
            enemies[i].x -= enemies[i].speed;
            enemies[i].collider.x = enemies[i].x;
            if (enemies[i].x < -enemyWidth)
            {
                enemies[i].x = SW + 100;
            }
        }
    }
}
void HandleEnemies22(Enemy enemies2[], int maxEnemies2, float enemyWidth2)
{
    for (int i = 0; i < maxEnemies2; i++)
    {
        if (!enemies2[i].active)
        {
            enemies2[i].rT -= GetFrameTime();
            if (enemies2[i].rT <= 0)
            {
                enemies2[i].active = true;
                enemies2[i].x = SW + 100;
            }
        }
        else

        {
            enemies2[i].x -= enemies2[i].speed;
            enemies2[i].collider.x = enemies2[i].x;
            if (enemies2[i].x < -enemyWidth2)
            {
                enemies2[i].x = SW + 100;
            }
        }
    }
}
void HandleCollisions2(float chX, float chY, Enemy enemies[], int maxEnemies, bool& isPlay, bool& isGameOver)
{
    Rectangle playerCollider = { chX, chY, 100, 150 };

    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemies[i].active && CheckCollisionRecs(playerCollider, enemies[i].collider))
        {
            isPlay = false;
            isGameOver = true;
            break;
        }
    }
}
void HandleCollisions22(float chX, float chY, Enemy enemies2[], int maxEnemies2, bool& isPlay, bool& isGameOver)
{
    Rectangle playerCollider = { chX, chY, 100, 150 };

    for (int i = 0; i < maxEnemies2; i++)
    {
        if (enemies2[i].active && CheckCollisionRecs(playerCollider, enemies2[i].collider))
        {
            isPlay = false;
            isGameOver = true;
            break;
        }
    }
}
void DrawGame2(Texture2D img1, Texture2D chRight, Texture2D chLeft, float chX, float chY, bool R,
    Texture2D enemy, Enemy enemies[], int maxEnemies, Texture2D Bull, Bullet bullets[], int mB, int& Score, float& backgroundOffset, Texture2D enemy2, Enemy enemies2[], int maxEnemies2)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);


    backgroundOffset = fmod(backgroundOffset, img1.width);



    DrawTexture(img1, backgroundOffset, 0, WHITE);
    DrawTexture(img1, backgroundOffset + img1.width, 0, WHITE);




    //DrawTexture(img1, 0, 0, WHITE);

    DrawText(TextFormat("Score is %d", Score), 0, 50, 50, RAYWHITE);

    if (R)
    {
        DrawTexture(chRight, chX, chY, WHITE);
    }
    else
    {
        DrawTexture(chLeft, chX, chY, WHITE);
    }


    for (int i = 0; i < maxEnemies; i++)
        if (enemies[i].active)
            DrawTexture(enemy, enemies[i].x, enemies[i].y, WHITE);

    for (int i = 0; i < mB; i++)
        if (bullets[i].active)
            DrawTexture(Bull, bullets[i].x, bullets[i].y, WHITE);

    for (int i = 0; i < maxEnemies2; i++)
        if (enemies2[i].active)
            DrawTexture(enemy2, enemies2[i].x, enemies2[i].y, WHITE);


    EndDrawing();
}




void level_2(int& Score)
{



    // InitAudioDevice();
    //InitWindow(SW, SH, "Metal Slug X");
    Texture2D img1 = LoadTexture("m1.png");
    Texture2D chRight = LoadTexture("ch.png");
    Texture2D chLeft = LoadTexture("ch1.png");
    Texture2D Bull = LoadTexture("Bullet.png");
    Texture2D enemy = LoadTexture("tank.png");
    Texture2D enemy2 = LoadTexture("walker.png");
    Texture2D Loading = LoadTexture("level2started1.png");
    Texture2D Loading2 = LoadTexture("level2final.png");
    //  Texture2D Loading3 = LoadTexture("Loading3.png");
    Texture2D Page = LoadTexture("A1.png");


    Page.height = SH;
    Page.width = SW;
    img1.height = SH;
    img1.width = SW + 5000;
    chRight.height = chLeft.height = 150;
    chRight.width = chLeft.width = 100;
    Bull.height = 20;
    Bull.width = 40;
    enemy.height = 150;
    enemy.width = 100;
    enemy2.height = 150;
    enemy2.width = 100;




    float backgroundOffset = 0.0f;

    float chX = SW / 2 - 100;
    float chY = SH - 250;
    float speed = 5;
    float vy = 0;
    bool isJ = false;
    bool R = true;
    const int mB = 10;
    Bullet bullets[mB] = {};

    Sound beep = LoadSound("beep.wav");
    Music b = LoadMusicStream("Sd.mp3");
    float loadingTime = 0.0f;
    bool isLoading = true;
    bool isMenu = true;
    bool isPlay = false;
    bool isControls = false;
    bool isGameOver = false;
    const int maxEnemies = 3;
    const int maxEnemies2 = 3;
    Enemy enemies[maxEnemies] = {};
    Enemy enemies2[maxEnemies] = {};


    for (int i = 0; i < maxEnemies; i++)
    {
        enemies[i].x = SW + (i * 400);
        enemies[i].y = SH - 250;
        enemies[i].speed = 1.6 + (i * 0.5f);
        enemies[i].active = true;
        enemies[i].rT = 0.0f;
        enemies[i].collider = { enemies[i].x, enemies[i].y, (float)enemy.width, (float)enemy.height };
    }
    for (int i = 0; i < maxEnemies2; i++)
    {
        enemies2[i].x = SW + (i * 400);
        enemies2[i].y = SH - 250;
        enemies2[i].speed = 4 + (i * 0.5f);
        enemies2[i].active = true;
        enemies2[i].rT = 0.0f;
        enemies2[i].collider = { enemies2[i].x, enemies2[i].y, (float)enemy2.width, (float)enemy2.height };
    }

    SetTargetFPS(60);
    PlayMusicStream(b);

    while (!WindowShouldClose())
    {
        if (Score > 300)
            break;

        UpdateMusicStream(b);

        if (isLoading)
        {
            loadingTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (loadingTime < 1.5)
                DrawTexture(Loading, SW - 5000, SH - 5000, WHITE);
            else if (loadingTime < 2.7)
                DrawTexture(Loading2, SW, SH, WHITE);

            EndDrawing();
            if (loadingTime > 3.0f)
            {
                isLoading = false;
                loadingTime = 0.0f;
            }
        }
        else if (isMenu)
        {
            ShowMenu2(isMenu, isPlay, isControls, Page);
        }
        else if (isGameOver)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("YOU LOSE!", SW / 2 - 150, SH / 2 - 50, 50, RED);
            DrawText("Press R to Restart or M for Menu", SW / 2 - 250, SH / 2 + 50, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_R))
            {
                isGameOver = false;
                isPlay = true;
                chX = SW / 2 - 100;
                chY = SH - 250;
                Score = 0;


                for (int i = 0; i < maxEnemies; i++)
                {
                    enemies[i].x = SW + (i * 400);
                    enemies[i].active = true;
                }
            }
            else if (IsKeyPressed(KEY_M))
            {

                isGameOver = false;
                isMenu = true;
            }
        }
        else if (isPlay)
        {
            HandlePlayerMovement2(chX, chY, vy, isJ, R, speed, backgroundOffset, img1);
            HandleBullets2(bullets, mB, chX, chY, R, beep, enemies, maxEnemies, Score, enemies2, maxEnemies);
            HandleEnemies2(enemies, maxEnemies, enemy.width);
            HandleEnemies2(enemies2, maxEnemies2, enemy2.width);

            HandleCollisions2(chX, chY, enemies, maxEnemies, isPlay, isGameOver);
            HandleCollisions22(chX, chY, enemies2, maxEnemies2, isPlay, isGameOver);

            DrawGame2(img1, chRight, chLeft, chX, chY, R, enemy, enemies, maxEnemies, Bull, bullets, mB, Score, backgroundOffset, enemy2, enemies2, maxEnemies2);
        }
    }

    //CloseWindow();







}

void ShowMenu3(bool& isMenu, bool& isPlaying, bool& isControls, Texture2D Page)
{
    while (isMenu)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(Page, 0, 0, WHITE);
        DrawText("METAL SLUG X", SW / 2 - 150, SH / 3 - 50, 50, LIGHTGRAY);
        DrawText("Level 3 (Three ) started ", SW / 2 - 100, SH / 2, 30, LIGHTGRAY);
        DrawText("PRESS P to continue ", SW / 2 - 150, SH / 2 + 50, 30, LIGHTGRAY);
        DrawText("PRESS E TO EXIT", SW / 2 - 100, SH / 2 + 100, 30, LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_P))
        {
            isMenu = false;
            isPlaying = true;
        }
        /*if (IsKeyPressed(KEY_C))
        {
            isMenu = false;
            isControls = true;

        }*/
        if (IsKeyPressed(KEY_E))
        {
            CloseWindow();
            exit(0);
        }
    }
}



void HandlePlayerMovement3(float& chX, float& chY, float& vy, bool& isJ, bool& R, float speed, float& backgroundOffset, Texture2D img1)
{
    if (IsKeyDown(KEY_RIGHT))
    {
        chX += speed;
        backgroundOffset -= speed;
        R = true;

    }
    if (IsKeyDown(KEY_LEFT))
    {

        chX -= speed;
        if (backgroundOffset < 0)
            backgroundOffset += speed;
        R = false;



    }

    if (chX < 0) chX = 0;
    if (chX > SW / 2 - 100) chX = SW / 2 - 100;

    if (IsKeyPressed(KEY_SPACE) && !isJ)
    {
        vy = JUMP;
        isJ = true;
    }

    vy += GRAVITY;
    chY += vy;

    if (chY >= SH - 250)
    {
        chY = SH - 250;
        vy = 0;
        isJ = false;
    }
}




void HandleBullets3(Bullet bullets[], int mB, float chX, float chY, bool R, Sound beep, Enemy enemies[], int maxEnemies, int& Score, Enemy enemies2[], int maxEnemies2, Enemy enemies3[], int maxEnemies3)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mP = GetMousePosition();
        for (int i = 0; i < mB; i++)
        {
            if (!bullets[i].active)
            {
                bullets[i].active = true;
                if (R)
                {
                    bullets[i].x = chX + 90;
                }
                else
                {
                    bullets[i].x = chX - 10;
                }
                bullets[i].y = chY + 65;
                float deltaX = mP.x - bullets[i].x;
                float deltaY = mP.y - bullets[i].y;
                float length = sqrt(deltaX * deltaX + deltaY * deltaY);
                bullets[i].dirX = (deltaX / length) * 7;
                bullets[i].dirY = (deltaY / length) * 7;
                bullets[i].collider = { bullets[i].x, bullets[i].y, 40, 20 };
                PlaySound(beep);
                break;
            }
        }
    }

    for (int i = 0; i < mB; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].dirX;
            bullets[i].y += bullets[i].dirY;
            if (bullets[i].x < 0 || bullets[i].x > SW || bullets[i].y < 0 || bullets[i].y > SH)
                bullets[i].active = false;

            bullets[i].collider.x = bullets[i].x;
            bullets[i].collider.y = bullets[i].y;

            for (int j = 0; j < maxEnemies; j++)
            {
                if (enemies[j].active && CheckCollisionRecs(bullets[i].collider, enemies[j].collider))
                {
                    enemies[j].active = false;
                    enemies[j].rT = RESPAWN_TIME;
                    bullets[i].active = false;
                    Score += 5;
                    break;
                }
            }


            for (int j = 0; j < maxEnemies3; j++)
            {


                if (enemies3[j].active && CheckCollisionRecs(bullets[i].collider, enemies3[j].collider))
                {
                    cout << "\n\n\n\ninside check collision\n\n\n\n";
                    enemies3[j].active = false;
                    enemies3[j].rT = RESPAWN_TIME;
                    bullets[i].active = false;
                    Score += 5;
                    break;
                }
            }
        }

    }

    for (int i = 0; i < mB; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].dirX;
            bullets[i].y += bullets[i].dirY;
            if (bullets[i].x < 0 || bullets[i].x > SW || bullets[i].y < 0 || bullets[i].y > SH)
                bullets[i].active = false;

            bullets[i].collider.x = bullets[i].x;
            bullets[i].collider.y = bullets[i].y;

            for (int j = 0; j < maxEnemies2; j++)
            {
                if (enemies2[j].active && CheckCollisionRecs(bullets[i].collider, enemies2[j].collider))
                {
                    enemies2[j].active = false;
                    enemies2[j].rT = RESPAWN_TIME;
                    bullets[i].active = false;
                    Score += 5;
                    break;
                }
            }
        }
    }



}

void HandleEnemies3(Enemy enemies[], int maxEnemies, float enemyWidth)
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].rT -= GetFrameTime();
            if (enemies[i].rT <= 0)
            {
                enemies[i].active = true;
                enemies[i].x = SW + 100;
            }
        }
        else

        {
            enemies[i].x -= enemies[i].speed;
            enemies[i].collider.x = enemies[i].x;
            if (enemies[i].x < -enemyWidth)
            {
                enemies[i].x = SW + 100;
            }
        }
    }
}

void HandleEnemies32(Enemy enemies[], int maxEnemies, float enemyWidth, float enemyHeight)
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].rT -= GetFrameTime();
            if (enemies[i].rT <= 0)
            {
                enemies[i].active = true;
                enemies[i].x = GetRandomValue(100, SW - 100);
                enemies[i].y = -enemyHeight;
            }
        }
        else
        {
            enemies[i].y += enemies[i].speed;

            enemies[i].collider.y = enemies[i].y;


            if (enemies[i].y > SH)
            {
                enemies[i].x = GetRandomValue(100, SW - 100);
                enemies[i].y = -enemyHeight;
            }
        }
    }
}
void HandleEnemies33(Enemy enemies3[], int maxEnemies3, float chX, float chY, float enemy3Width, float enemy3Height) {
    for (int i = 0; i < maxEnemies3; i++) {
        if (!enemies3[i].active) {
            enemies3[i].rT -= GetFrameTime();
            if (enemies3[i].rT <= 0) {
                enemies3[i].active = true;
                enemies3[i].x = GetRandomValue(0, SW - enemy3Width);
                enemies3[i].y = -enemy3Height;
            }
        }
        else {

            float dx = chX - enemies3[i].x;
            float dy = chY - enemies3[i].y;
            float dist = sqrt(dx * dx + dy * dy);
            if (dist > 0) {
                enemies3[i].x += (dx / dist) * enemies3[i].speed;
                enemies3[i].y += (dy / dist) * enemies3[i].speed;
            }


            enemies3[i].collider.x = enemies3[i].x;
            enemies3[i].collider.y = enemies3[i].y;
            if (enemies3[i].y > SH || enemies3[i].x < -enemy3Width || enemies3[i].x > SW + enemy3Width) {
                enemies3[i].active = false;
                enemies3[i].rT = RESPAWN_TIME;
            }
        }
    }
}

void HandleCollisions3(float chX, float chY, Enemy enemies[], int maxEnemies, bool& isPlay, bool& isGameOver)
{
    Rectangle playerCollider = { chX, chY, 100, 150 };

    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemies[i].active && CheckCollisionRecs(playerCollider, enemies[i].collider))
        {
            isPlay = false;
            isGameOver = true;
            break;
        }
    }
}
void HandleCollisions32(float chX, float chY, Enemy enemies2[], int maxEnemies2, bool& isPlay, bool& isGameOver)
{
    Rectangle playerCollider = { chX, chY, 100, 150 };

    for (int i = 0; i < maxEnemies2; i++)
    {
        if (enemies2[i].active && CheckCollisionRecs(playerCollider, enemies2[i].collider))
        {
            isPlay = false;
            isGameOver = true;
            break;
        }
    }
}
void HandleCollisions33(float chX, float chY, Enemy enemies3[], int maxEnemies3, bool& isPlay, bool& isGameOver) {
    Rectangle playerCollider = { chX, chY, 100, 150 };
    for (int i = 0; i < maxEnemies3; i++) {
        if (enemies3[i].active && CheckCollisionRecs(playerCollider, enemies3[i].collider)) {
            isPlay = false;
            isGameOver = true;
            break;
        }
    }
}



void DrawGame3(Texture2D img1, Texture2D chRight, Texture2D chLeft, float chX, float chY, bool R,
    Texture2D enemy, Enemy enemies[], int maxEnemies, Texture2D Bull, Bullet bullets[], int mB, int& Score, float& backgroundOffset, Texture2D enemy2, Enemy enemies2[], int maxEnemies2, Texture2D enemy3, Enemy enemies3[], int maxEnemies3)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);


    backgroundOffset = fmod(backgroundOffset, img1.width);



    DrawTexture(img1, backgroundOffset, 0, WHITE);
    DrawTexture(img1, backgroundOffset + img1.width, 0, WHITE);




    //DrawTexture(img1, 0, 0, WHITE);

    DrawText(TextFormat("Score is %d", Score), 0, 50, 50, RAYWHITE);

    if (R)
    {
        DrawTexture(chRight, chX, chY, WHITE);
    }
    else
    {
        DrawTexture(chLeft, chX, chY, WHITE);
    }


    for (int i = 0; i < maxEnemies; i++)
        if (enemies[i].active)
            DrawTexture(enemy, enemies[i].x, enemies[i].y, WHITE);

    for (int i = 0; i < mB; i++)
        if (bullets[i].active)
            DrawTexture(Bull, bullets[i].x, bullets[i].y, WHITE);


    for (int i = 0; i < maxEnemies2; i++)
        if (enemies2[i].active)
            DrawTexture(enemy2, enemies2[i].x, enemies2[i].y, WHITE);


    for (int i = 0; i < maxEnemies3; i++)
        if (enemies3[i].active)
            DrawTexture(enemy3, enemies3[i].x, enemies3[i].y, WHITE);



    EndDrawing();
}





void level_3(int& Score)
{

    //InitWindow(SW, SH, "Metal Slug X");
    Texture2D img1 = LoadTexture("level3.png");
    Texture2D chRight = LoadTexture("ch.png");
    Texture2D chLeft = LoadTexture("ch1.png");
    Texture2D Bull = LoadTexture("Bullet.png");
    Texture2D enemy = LoadTexture("car1.png");
    Texture2D enemy2 = LoadTexture("hellicopter.png");
    Texture2D enemy3 = LoadTexture("helicopter.png");

    Texture2D Loading = LoadTexture("Level2.png");
    Texture2D Loading2 = LoadTexture("Level2.png");
    Texture2D Page = LoadTexture("A1.png");
    Texture2D winS = LoadTexture("Win.png");


    Page.height = SH;
    Page.width = SW;
    img1.height = SH;
    img1.width = SW + 5000;
    chRight.height = chLeft.height = 150;
    chRight.width = chLeft.width = 100;
    Bull.height = 20;
    Bull.width = 40;
    enemy.height = 150;
    enemy.width = 100;

    enemy2.height = 150;
    enemy2.width = 100;

    enemy3.height = 150;
    enemy3.width = 100;

    winS.height = SH;
    winS.width = SW;


    float backgroundOffset = 0.0f;

    float chX = SW / 2 - 100;
    float chY = SH - 250;
    float speed = 5;
    float vy = 0;
    bool isJ = false;
    bool R = true;
    const int mB = 10;
    Bullet bullets[mB] = {};

    Sound beep = LoadSound("beep.wav");
    Music b = LoadMusicStream("Sd.mp3");
    float loadingTime = 0.0f;
    bool isLoading = true;
    bool isMenu = true;
    bool isPlay = false;
    bool isControls = false;
    bool isGameOver = false;
    const int maxEnemies = 3;
    const int maxEnemies2 = 1;
    const int maxEnemies3 = 2;
    Enemy enemies[maxEnemies] = {};

    Enemy enemies2[maxEnemies2] = {};
    Enemy enemies3[maxEnemies3] = {};
    for (int i = 0; i < maxEnemies; i++)
    {
        enemies[i].x = SW + (i * 400);
        enemies[i].y = SH - 250;
        enemies[i].speed = 2 + (i * 0.5f);
        enemies[i].active = true;
        enemies[i].rT = 0.0f;
        enemies[i].collider = { enemies[i].x, enemies[i].y, (float)enemy.width, (float)enemy.height };
    }

    for (int i = 0; i < maxEnemies2; i++)
    {
        enemies2[i].x = SW + (i * 400);
        enemies2[i].y = SH - 300;
        enemies2[i].speed = 2 + (i * 0.5f);
        enemies2[i].active = true;
        enemies2[i].rT = 0.0f;
        enemies2[i].collider = { enemies2[i].x, enemies2[i].y, (float)enemy2.width, (float)enemy2.height };
    }

    for (int i = 0; i < maxEnemies3; i++)
    {
        enemies3[i].x = GetRandomValue(0, SW - enemy3.width);
        enemies3[i].y = -enemy3.height;
        enemies3[i].speed = 4.0f;
        enemies3[i].active = true;
        enemies3[i].rT = 0.0f;
        enemies3[i].collider = { enemies3[i].x, enemies3[i].y, (float)enemy3.width, (float)enemy3.height };
    }


    SetTargetFPS(60);
    PlayMusicStream(b);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(b);

        if (isLoading)
        {
            loadingTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (loadingTime < 1.5)
                DrawTexture(Loading, SW - 5000, SH - 5000, WHITE);
            else if (loadingTime < 2.7)
                DrawTexture(Loading2, SW, SH, WHITE);

            EndDrawing();
            if (loadingTime > 3.0f)
            {
                isLoading = false;
                loadingTime = 0.0f;
            }
        }
        else if (isMenu)
        {
            ShowMenu3(isMenu, isPlay, isControls, Page);
        }
        else if (isGameOver)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("YOU LOSE!", SW / 2 - 150, SH / 2 - 50, 50, RED);
            DrawText("Press R to Restart or M for Menu", SW / 2 - 250, SH / 2 + 50, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_R))
            {
                isGameOver = false;
                isPlay = true;
                chX = SW / 2 - 100;
                chY = SH - 250;
                Score = 0;


                for (int i = 0; i < maxEnemies2; i++)
                {
                    enemies[i].x = SW + (i * 400);
                    enemies[i].active = true;
                }
                for (int i = 0; i < maxEnemies2; i++)
                {
                    enemies2[i].x = SW + (i * 400);
                    enemies2[i].active = true;
                }
                for (int i = 0; i < maxEnemies3; i++)
                {
                    enemies3[i].x = SW + (i * 400);
                    enemies3[i].active = true;
                }
            }
            else if (IsKeyPressed(KEY_M))
            {

                isGameOver = false;
                isMenu = true;
            }
        }
        else if (isPlay)
        {
            HandlePlayerMovement3(chX, chY, vy, isJ, R, speed, backgroundOffset, img1);
            HandleBullets3(bullets, mB, chX, chY, R, beep, enemies, maxEnemies, Score, enemies2, maxEnemies2, enemies3, maxEnemies3);


            HandleEnemies3(enemies, maxEnemies, enemy.width);
            HandleEnemies32(enemies2, maxEnemies2, enemy2.width, enemy.height);
            HandleEnemies33(enemies3, maxEnemies3, chX, chY, enemy3.width, enemy3.height);

            HandleCollisions3(chX, chY, enemies, maxEnemies, isPlay, isGameOver);
            HandleCollisions32(chX, chY, enemies2, maxEnemies2, isPlay, isGameOver);
            HandleCollisions33(chX, chY, enemies3, maxEnemies3, isPlay, isGameOver);

            DrawGame3(img1, chRight, chLeft, chX, chY, R, enemy, enemies, maxEnemies, Bull, bullets, mB, Score, backgroundOffset, enemy2, enemies2, maxEnemies2, enemy3, enemies3, maxEnemies3);

            if (Score >= 1000)
            {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(winS, 0, 0, WHITE);
                EndDrawing();
                WaitTime(5.0);
                break;
            }


        }
    }

    CloseWindow();
}



int main()
{
    int Score = 0;


    level_1(Score);
    level_2(Score);
    level_3(Score);



    return 0;
}