#include <raylib.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BOARD_SIZE 3
#define CELL_SIZE 120
#define BD_POS 45

typedef enum { EMPTY, PLAYER_X, PLAYER_O } Cell;

Cell board[BOARD_SIZE][BOARD_SIZE];
Cell currentPlayer;
int winner;
int moves;

static void DrawBoard() {
    for (int y = 0; y <= BOARD_SIZE; y++) {
        DrawLine(BD_POS, BD_POS + y * CELL_SIZE, BD_POS + BOARD_SIZE * CELL_SIZE, BD_POS + y * CELL_SIZE, BLACK);
        DrawLine(BD_POS + y * CELL_SIZE, BD_POS, BD_POS + y * CELL_SIZE, BD_POS + BOARD_SIZE * CELL_SIZE, BLACK);
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == PLAYER_X) {
                DrawText("X", BD_POS + 35 + x * CELL_SIZE, BD_POS + 30 + y * CELL_SIZE, 80, RED);
            }
            else if (board[y][x] == PLAYER_O) {
                DrawText("O", BD_POS + 35 + x * CELL_SIZE, BD_POS + 30 + y * CELL_SIZE, 80, BLUE);
            }
        }
    }
}


static int CheckWinner() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY) return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) return board[0][2];
    return EMPTY;
}

static void InitGame() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = EMPTY;
        }
    }
    currentPlayer = PLAYER_X;
    winner = EMPTY;
    moves = 0;
}

static void DrawCellCursor(int& cellX, int& cellY) {
    if (cellX >= 0 && cellX < BOARD_SIZE && cellY >= 0 && cellY < BOARD_SIZE && board[cellY][cellX] == EMPTY) {
        DrawRectangle(BD_POS + cellX * CELL_SIZE + 1, BD_POS + cellY * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, RAYWHITE);
    }
}

static void UpdateDrawFrame()
{
    Vector2 mousePoint = GetMousePosition();
    int cellX = (mousePoint.x - BD_POS) > 0 ? (mousePoint.x - BD_POS) / CELL_SIZE : -1;
    int cellY = (mousePoint.y - BD_POS) > 0 ? (mousePoint.y - BD_POS) / CELL_SIZE : -1;

    if (winner == EMPTY && moves < BOARD_SIZE * BOARD_SIZE) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if (cellX >= 0 && cellX < BOARD_SIZE && cellY >= 0 && cellY < BOARD_SIZE && board[cellY][cellX] == EMPTY) {
                board[cellY][cellX] = currentPlayer;
                moves++;
                winner = CheckWinner();
                currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
            }
        }
    }
    else if (IsKeyPressed(KEY_R)) {
        InitGame();
    }

    BeginDrawing();
    ClearBackground(WHITE);
    DrawBoard();
    DrawCellCursor(cellX, cellY);

    if (winner != EMPTY) {
        DrawText(winner == PLAYER_X ? "X wins!" : "O wins!", BD_POS + BOARD_SIZE * CELL_SIZE + 10, BD_POS + CELL_SIZE, 50, GREEN);
        DrawText("Press 'R' to Retry", BD_POS + BOARD_SIZE * CELL_SIZE + 10, BD_POS + CELL_SIZE * 2, 20, DARKGRAY);
    }
    else if (moves == BOARD_SIZE * BOARD_SIZE) {
        DrawText("It's a draw!", BD_POS + BOARD_SIZE * CELL_SIZE + 10, BD_POS + CELL_SIZE, 36, GREEN);
        DrawText("Press 'R' to Retry", BD_POS + BOARD_SIZE * CELL_SIZE + 10, BD_POS + CELL_SIZE * 2, 20, DARKGRAY);
    }

    EndDrawing();
}


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");
    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();

    return 0;
}