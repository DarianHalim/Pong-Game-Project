#include <iostream>
#include "raylib.h"

// Styling
Color courtBG = {20, 160, 133, 255};
Color cpuSide = {0, 255, 0, 102};
// Styling

int screenWidth = 1200;
int screenHeight = 800;

int currentScreenWidth;
int currentScreenHeight;

using namespace std;

// scoring
int playerScore = 0;
int cpuScore = 0;

// Control paddle for player
class Paddle
{
protected:
    void LimitArena()
    {

        // Keep the player's paddle inside the Pong arena
        if (yAxis < 0)
            yAxis = 0;

        if (yAxis + paddleHeigth > screenHeight)
            yAxis = screenHeight - paddleHeigth;
    }

public:
    float xAxis;
    float yAxis;
    float paddleWidth;
    float paddleHeigth;
    int paddleSpeed;

    void DrawPlayerPaddle()
    {
        DrawRectangle(xAxis, yAxis, paddleWidth, paddleHeigth, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_W))
        {
            yAxis = yAxis - paddleSpeed;
        }

        if (IsKeyDown(KEY_S))
        {
            yAxis = yAxis + paddleSpeed;
        }

        // Call LimitArena() to keep the paddle inside the arena
        LimitArena();
    }
};

// Ball movement
class Ball
{
public:
    float x, y;
    int speedX;
    int speedY;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void UpdateBall()
    {
        x += speedX;
        y += speedY;

        if (x + radius >= screenWidth || x - radius <= 0)
        {
            speedX *= -1;
        }

        if (x + radius >= screenWidth)
        {
            cpuScore++;
            ResetBallCenter();
        }

        if (x - radius <= 0)
        {
            playerScore++;
            ResetBallCenter();
        }

        if (y + radius >= screenHeight || y - radius <= 0)
        {
            speedY *= -1;
        }
    }

    void ResetBallCenter()
    { // So player score doesn't keep goin up
        x = screenWidth / 2;
        y = screenHeight / 2;

        // Randomize Speed
        int speedChoices[2] = {-1, 1};
        speedX = 6.5 * speedChoices[GetRandomValue(0, 1)]; // Reset speedX
        speedY = 6.5 * speedChoices[GetRandomValue(0, 1)]; // Reset speedY
    }
};

// Child of Paddle
class cpuPaddle : public Paddle
{
public:
    void update(int ballY)
    {
        // Adjust the cpu paddle's position based on ball's y to the center of paddle's y
        if (yAxis + paddleHeigth / 2 < ballY)
        {
            yAxis += paddleSpeed;
        }
        else if (yAxis + paddleHeigth / 2 > ballY)
        {
            yAxis -= paddleSpeed;
        }

        // Keep the CPU paddle inside the Pong arena
        LimitArena();
    }

    void DrawPlayerPaddle()
    {
        DrawRectangle(xAxis, yAxis, paddleWidth, paddleHeigth, YELLOW);
    }

private: // delay ball spawning
    float resetTime;
    const float resetDelay = 1.5f;
};
// Initialize
Paddle player;
Ball ball;
cpuPaddle cpu;

int main()
{
    cout << "Start Pong" << endl;

    InitWindow(screenWidth, screenHeight, "Pong Video Game");
    SetTargetFPS(144);

    // Paddle player variables
    player.paddleWidth = 30;
    player.paddleHeigth = 130;
    player.xAxis = 20;
    player.yAxis = screenHeight / 2 - player.paddleHeigth / 2;
    player.paddleSpeed = 5;

    // Ball variables
    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speedX = 5.5; // Set initial speedX
    ball.speedY = 5.5;// Set initial speedY

    // cpu variables

    cpu.paddleWidth = 30;
    cpu.paddleHeigth = 130;
    cpu.xAxis = screenWidth - cpu.paddleWidth - 20;
    cpu.yAxis = screenHeight / 2 - cpu.paddleHeigth / 2;
    cpu.paddleSpeed = 4.5;

    while (!WindowShouldClose())
    {
        // Update the screen dimensions dynamically
        currentScreenWidth = GetScreenWidth();
        currentScreenHeight = GetScreenHeight();

        // Update elements
        player.Update();   // Updating player
        ball.UpdateBall(); // Updating ball
        cpu.update(ball.y);

        // Start drawing
        BeginDrawing();
        ClearBackground(courtBG);
        DrawRectangle(screenWidth/2,0,screenWidth/2,screenHeight,cpuSide);
        DrawCircleGradient(screenWidth / 2, screenHeight / 2, screenHeight / 4, YELLOW, WHITE);

        // Draw Score Text
        DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, YELLOW);

        // checking for collisions with  player's paddle
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.xAxis, player.yAxis, player.paddleWidth, player.paddleHeigth}))
        {
            ball.speedX *= -1;
        }

        // checking for collisions with  cpu's paddle
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.xAxis, cpu.yAxis, cpu.paddleWidth, cpu.paddleHeigth}))
        {
            {
                ball.speedX *= -1;
            }
        }
        // Draw ball
        ball.Draw();

        // Draw player paddlew
        player.DrawPlayerPaddle();

        // Draw cpu paddle
        cpu.DrawPlayerPaddle();

        // Draw right rectangle

        // Draw center line
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
