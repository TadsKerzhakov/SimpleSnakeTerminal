#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>   //  usleep (microsleep activation)
#include <ncurses.h>

#define WIDTH 20
#define HEIGHT 10
#define INITIAL_LENGTH 3
#define MAX_LENGTH (WIDTH * HEIGHT)

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

struct Snake {
    struct Point body[MAX_LENGTH];
    int length;
    enum Direction dir;
};

struct Snake snake;
struct Point fruit;
int score;
int gameOver;

void setup() {
    snake.length = INITIAL_LENGTH;
    snake.body[0].x = WIDTH / 2;
    snake.body[0].y = HEIGHT / 2;
    snake.dir = RIGHT;
    score = 0;
    gameOver = 0;

    srand(time(NULL));
    fruit.x = rand() % WIDTH;
    fruit.y = rand() % HEIGHT;
}

void draw() {
    clear();

    // Top Wall
    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");
    printw("\n");

    // Middle Wall
    for (int i = 0; i < HEIGHT; i++) {
        printw("#");
        for (int j = 0; j < WIDTH; j++) {
            int isSnakeBody = 0;
            for (int k = 0; k < snake.length; k++) {
                if (snake.body[k].x == j && snake.body[k].y == i) {
                    printw("o");
                    isSnakeBody = 1;
                    break;
                }
            }
            if (!isSnakeBody && fruit.x == j && fruit.y == i)
                printw("F");
            else if (!isSnakeBody)
                printw(" ");
        }
        printw("#\n");
    }

    // Bottom Wall
    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");
    printw("\n");

    // Score
    printw("Score: %d\n", score);

    refresh(); // Refresh screen
}

void input() {
    // Timeout for non-blocking input
    timeout(100);

    // Input
    int c = getch();
    switch (c) {
        case 'w':
            if (snake.dir != DOWN) snake.dir = UP;
            break;
        case 's':
            if (snake.dir != UP) snake.dir = DOWN;
            break;
        case 'a':
            if (snake.dir != RIGHT) snake.dir = LEFT;
            break;
        case 'd':
            if (snake.dir != LEFT) snake.dir = RIGHT;
            break;
        case 'x':
            gameOver = 1;
            break;
    }
}

void logic() {
    // Move Body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    // Move Head
    switch (snake.dir) {
        case UP:
            snake.body[0].y -= 1;
            break;
        case DOWN:
            snake.body[0].y += 1;
            break;
        case LEFT:
            snake.body[0].x -= 1;
            break;
        case RIGHT:
            snake.body[0].x += 1;
            break;
    }

    // Collision
    if (snake.body[0].x < 0 || snake.body[0].x >= WIDTH ||
        snake.body[0].y < 0 || snake.body[0].y >= HEIGHT)
        gameOver = 1;

    // Self Collision
    for (int i = 1; i < snake.length; i++) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y)
            gameOver = 1;
    }

    // Fruit collision
    if (snake.body[0].x == fruit.x && snake.body[0].y == fruit.y) {
        // Increase snakwe length
        snake.length++;
        score += 10;

        // New fruit position
        fruit.x = rand() % WIDTH;
        fruit.y = rand() % HEIGHT;
    }
}

int main() {
    initscr(); // Initialize ncurses
    noecho(); // Dont echo input characters
    curs_set(0); // Hide cursor

    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        usleep(100000); // Sleep for 100 milliseconds
    }

    endwin(); // End ncurses

    printf("Game Over! Score: %d\n", score);

    return 0;
}
