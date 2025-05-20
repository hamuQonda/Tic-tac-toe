#include <raylib.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define BOARD_SIZE 3
#define CELL_SIZE (SCREEN_HEIGHT/(BOARD_SIZE+1))
#define BD_POS CELL_SIZE/2

typedef enum { EMPTY, PLAYER_X, PLAYER_O } Cell;

Cell board[BOARD_SIZE][BOARD_SIZE];
Cell currentPlayer;
int winner;
int moves;

static int checkWinLine() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) {
            DrawRectangle(BD_POS + 0 * CELL_SIZE + 1, BD_POS + i * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
            DrawRectangle(BD_POS + 1 * CELL_SIZE + 1, BD_POS + i * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
            DrawRectangle(BD_POS + 2 * CELL_SIZE + 1, BD_POS + i * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
            return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY) {
            DrawRectangle(BD_POS + i * CELL_SIZE + 1, BD_POS + 0 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
            DrawRectangle(BD_POS + i * CELL_SIZE + 1, BD_POS + 1 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
            DrawRectangle(BD_POS + i * CELL_SIZE + 1, BD_POS + 2 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
            return board[0][i];
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        DrawRectangle(BD_POS + 0 * CELL_SIZE + 1, BD_POS + 0 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
        DrawRectangle(BD_POS + 1 * CELL_SIZE + 1, BD_POS + 1 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
        DrawRectangle(BD_POS + 2 * CELL_SIZE + 1, BD_POS + 2 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        DrawRectangle(BD_POS + 2 * CELL_SIZE + 1, BD_POS + 0 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
        DrawRectangle(BD_POS + 1 * CELL_SIZE + 1, BD_POS + 1 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
        DrawRectangle(BD_POS + 0 * CELL_SIZE + 1, BD_POS + 2 * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, YELLOW);
        return board[0][2];
    }
    return EMPTY;
}

static void DrawBoard() {
    DrawRectangle(BD_POS - 10, BD_POS - 10, CELL_SIZE * BOARD_SIZE + 20, CELL_SIZE * BOARD_SIZE + 20, WHITE);
    for (int y = 0; y <= BOARD_SIZE; y++) {
        DrawLine(BD_POS, BD_POS + y * CELL_SIZE, BD_POS + BOARD_SIZE * CELL_SIZE, BD_POS + y * CELL_SIZE, BLACK);
        DrawLine(BD_POS + y * CELL_SIZE, BD_POS, BD_POS + y * CELL_SIZE, BD_POS + BOARD_SIZE * CELL_SIZE, BLACK);
    }
    checkWinLine();
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == PLAYER_X) {
                DrawText("X", BD_POS + x * CELL_SIZE + CELL_SIZE / 5, BD_POS + y * CELL_SIZE + CELL_SIZE / 15, CELL_SIZE, RED);
            }
            else if (board[y][x] == PLAYER_O) {
                DrawText("O", BD_POS + x * CELL_SIZE + CELL_SIZE / 5, BD_POS + y * CELL_SIZE + CELL_SIZE / 15, CELL_SIZE, BLUE);
            }
        }
    }
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
                winner = checkWinLine();
                currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
            }
        }
    }
    else if (IsKeyPressed(KEY_R)) {
        InitGame();
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawBoard();
    DrawCellCursor(cellX, cellY);

    if (winner != EMPTY) {
        DrawText(winner == PLAYER_X ? "X wins!" : "O wins!", BD_POS * 1.5 + BOARD_SIZE * CELL_SIZE, BD_POS + CELL_SIZE, 50, GREEN);
        DrawText("Press 'R' to Retry", BD_POS * 1.5 + BOARD_SIZE * CELL_SIZE, BD_POS + CELL_SIZE * 2, 20, DARKGRAY);
    }
    else if (moves == BOARD_SIZE * BOARD_SIZE) {
        DrawText("It's a draw!", BD_POS * 1.5 + BOARD_SIZE * CELL_SIZE, BD_POS + CELL_SIZE, 36, GREEN);
        DrawText("Press 'R' to Retry", BD_POS * 1.5 + BOARD_SIZE * CELL_SIZE, BD_POS + CELL_SIZE * 2, 20, DARKGRAY);
    }

    EndDrawing();
}


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");
    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();

    return 0;
}