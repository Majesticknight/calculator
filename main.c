#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8
#define MINES 10

typedef struct {
    int isMine;
    int revealed;
    int adjacentMines;
} Cell;

Cell board[SIZE][SIZE];

void initBoard();
void placeMines();
void calculateNumbers();
void printBoard(int revealAll);
void revealCell(int row, int col);
int isValid(int row, int col);
int countMines(int row, int col);
int checkWin();

int main() {
    int row, col;
    int gameOver = 0;

    srand(time(NULL));
    initBoard();
    placeMines();
    calculateNumbers();

    while (!gameOver) {
        printBoard(0);
        printf("\nEnter row and column (0-%d): ", SIZE - 1);
        scanf("%d %d", &row, &col);

        if (!isValid(row, col) || board[row][col].revealed) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        if (board[row][col].isMine) {
            printf("\nGame Over! You hit a mine.\n");
            gameOver = 1;
            printBoard(1);
        } else {
            revealCell(row, col);
            if (checkWin()) {
                printf("\nCongratulations! You won!\n");
                gameOver = 1;
                printBoard(1);
            }
        }
    }

    return 0;
}

void initBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j].isMine = 0;
            board[i][j].revealed = 0;
            board[i][j].adjacentMines = 0;
        }
    }
}

void placeMines() {
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % SIZE;
        int c = rand() % SIZE;
        if (!board[r][c].isMine) {
            board[r][c].isMine = 1;
            placed++;
        }
    }
}

void calculateNumbers() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!board[i][j].isMine) {
                board[i][j].adjacentMines = countMines(i, j);
            }
        }
    }
}

void printBoard(int revealAll) {
    printf("\n   ");
    for (int j = 0; j < SIZE; j++) printf("%d ", j);
    printf("\n  ");
    for (int j = 0; j < SIZE; j++) printf("--");
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d| ", i);
        for (int j = 0; j < SIZE; j++) {
            if (revealAll || board[i][j].revealed) {
                if (board[i][j].isMine) {
                    printf("* ");
                } else {
                    printf("%d ", board[i][j].adjacentMines);
                }
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void revealCell(int row, int col) {
    if (!isValid(row, col) || board[row][col].revealed) return;

    board[row][col].revealed = 1;

    if (board[row][col].adjacentMines == 0) {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr != 0 || dc != 0) {
                    revealCell(row + dr, col + dc);
                }
            }
        }
    }
}

int isValid(int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

int countMines(int row, int col) {
    int count = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int r = row + dr, c = col + dc;
            if (isValid(r, c) && board[r][c].isMine) {
                count++;
            }
        }
    }
    return count;
}

int checkWin() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!board[i][j].isMine && !board[i][j].revealed) {
                return 0;
            }
        }
    }
    return 1;
}
