#include <stdbool.h>
#include <stdio.h>

#define SIZE 3

void PrintBoard(int rows, int cols, char board[rows][cols]);
bool CheckWin(int rows, int cols, char board[rows][cols], char player);
bool IsDraw(int rows, int cols, char board[rows][cols]);

int main(void) {
    char gameboard[SIZE][SIZE] = {
        {'-', '-', '-'},
        {'-', '-', '-'},
        {'-', '-', '-'}
    };

    int rows = SIZE;
    int cols = SIZE;
    char player = 'X';
    int moveCount = 0;

    while (true) {
        PrintBoard(rows, cols, gameboard);
        printf("Player '%c', enter your move (column row): ", player);
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) {
            printf("Invalid input. Please enter two integers.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        if (x < 0 || x >= cols || y < 0 || y >= rows) {
            printf("Coordinates out of bounds. Try again.\n");
            continue;
        }
        if (gameboard[y][x] != '-') {
            printf("Cell already occupied. Try again.\n");
            continue;
        }

        gameboard[y][x] = player;
        moveCount++;

        if (CheckWin(rows, cols, gameboard, player)) {
            PrintBoard(rows, cols, gameboard);
            printf("Player '%c' wins!\n", player);
            break;
        }

        if (IsDraw(rows, cols, gameboard)) {
            PrintBoard(rows, cols, gameboard);
            printf("It's a draw!\n");
            break;
        }

        // Switch player
        player = (player == 'X') ? 'O' : 'X';
    }

    return 0;
}

void PrintBoard(int rows, int cols, char board[rows][cols]) {
    printf("  ");
    for (int i = 0; i < cols; i++) {
        printf(" %d", i);
    }
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%d ", i);
        for (int j = 0; j < cols; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

bool CheckWin(int rows, int cols, char board[rows][cols], char player) {
    // Check rows
    for (int i = 0; i < rows; i++) {
        bool win = true;
        for (int j = 0; j < cols; j++) {
            if (board[i][j] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    // Check columns
    for (int j = 0; j < cols; j++) {
        bool win = true;
        for (int i = 0; i < rows; i++) {
            if (board[i][j] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    // Check main diagonal
    bool win = true;
    for (int i = 0; i < rows; i++) {
        if (board[i][i] != player) {
            win = false;
            break;
        }
    }
    if (win) return true;

    // Check anti-diagonal
    win = true;
    for (int i = 0; i < rows; i++) {
        if (board[i][cols - i - 1] != player) {
            win = false;
            break;
        }
    }
    if (win) return true;

    return false;
}

bool IsDraw(int rows, int cols, char board[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}