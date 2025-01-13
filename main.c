#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct TokenPlacement {
    int rowIndex;
    int columnIndex;
    int amount;
};

int rowSize = 7;
int columnSize = 6;
int currentPlayer = 0;
int scorePlayerOne = 0;
int scorePlayerTwo = 0;

void printBoardHeader() {
    // Uitprinten van de eerste
    printf("\n");
    printf("===================\n");
    printf("SCORES:\n");
    printf("Player '%d' points: %d\n", 0, scorePlayerOne);
    printf("Player '%d' points: %d\n", 1, scorePlayerTwo);
    printf("===================\n");
    printf("\n");
    printf("===================\n");
    printf("\n");
    printf("    1 2 3 4 5 6\n");
    printf("    -----------\n");
}
void printBoard(int ***board) {
    printBoardHeader();
    int columnIndex;
    int rowIndex;
    for (rowIndex = 0; rowIndex < rowSize; rowIndex++) {
        printf("    ");

        for (columnIndex = 0; columnIndex < columnSize; columnIndex++) {
            if (board[rowIndex][columnIndex] == NULL) {
                printf("0 ");
            } else {
                if(*board[rowIndex][columnIndex]){
                    printf("* ");

                } else {
                    printf("^ ");
                }
            }
        }

        printf("\n");
    }
    printf("\n");
    printf("===================\n");
    printf("\n");
}

int ***createBoard() {
    // Declaratie van de arrays met pointers die naar pointers wijzen die naar int's wijzen
    int ***board = malloc(rowSize * sizeof(int**));

    for (int rowIndex = 0; rowIndex < rowSize; rowIndex++) {
        // Op plek board[rowIndex] word er ruimte gemaakt voor columnSize aan int pointers
        board[rowIndex] =  malloc(columnSize * sizeof(int**));
        for (int columnIndex = 0; columnIndex < columnSize; columnIndex++) {
            // Zet de pointers van de "cellen" op NULL
            board[rowIndex][columnIndex] = NULL;

        }
    }
    return board;
}

void destroyBoard(int ***board) {
    for (int rowIndex = 0; rowIndex < rowSize; rowIndex++) {
        for (int columnIndex = 0; columnIndex < columnSize; columnIndex++) {
            //Frees the memory of the specific "cell" (column in a row)
            free(board[rowIndex][columnIndex]);
            board[rowIndex][columnIndex] = NULL;
        }
        // Frees the memory of the row
        free(board[rowIndex]);
        board[rowIndex] = NULL;
    }
    // Frees the memory of the whole board
    free(board);
    board = NULL;
}

struct TokenPlacement placeToken(int*** board, int userSelectedColumn) {
    // search the rows from bottom to top
    for (int rowIndex = rowSize - 1; rowIndex >= 0; rowIndex--) {
        // place the piece on the top most free space
        if (board[rowIndex][userSelectedColumn - 1] == NULL) {
            int *one = malloc(sizeof(int));
            *one = currentPlayer;
            board[rowIndex][userSelectedColumn - 1] = one;
            struct TokenPlacement tokenPlacement;
            tokenPlacement.rowIndex = rowIndex;
            tokenPlacement.columnIndex = userSelectedColumn - 1;
            return tokenPlacement;
        } else {
            // if the all rows of a column are full end the game in a tie
            if (rowIndex == 0) {
                printf("IT'S A TIE!");
                struct TokenPlacement tokenPlacement;
                tokenPlacement.rowIndex = -1;
                tokenPlacement.columnIndex = -1;
                return tokenPlacement;
            }
        }
    }
//    printf("Something went wrong...");
    struct TokenPlacement tokenPlacement;
    tokenPlacement.rowIndex = -1;
    tokenPlacement.columnIndex = -1;
    return tokenPlacement;
}

struct TokenPlacement checkVerticalWinCondition(int***board, struct TokenPlacement placement) {
// if the column has at least 4 of the same piece adjacent vertically
    // Free the memory of all these pieces (also the current piece)
    struct TokenPlacement startingVerticalClearingIndex = { -1,-1};
    int amountInAColumn = 0;
    for (int rowIndex = rowSize - 1; rowIndex >= 0; rowIndex--) {
        int rowOnTheTop = rowIndex - 1;
        if (rowOnTheTop == -1 ) {
//            printf("Vert: No win \n");
            startingVerticalClearingIndex.columnIndex = -1;
            startingVerticalClearingIndex.rowIndex = -1;
            return startingVerticalClearingIndex;
        } else if (board[rowIndex][placement.columnIndex] == NULL) {
//            printf("Vert: Reset & continue \n");
            startingVerticalClearingIndex.columnIndex = -1;
            startingVerticalClearingIndex.rowIndex = -1;
            amountInAColumn = 0;
            if (rowOnTheTop - 1 == -1) return startingVerticalClearingIndex;
            continue;
        }

        if (*board[rowIndex][placement.columnIndex] == currentPlayer &&
            (board[rowOnTheTop][placement.columnIndex] == NULL || *board[rowOnTheTop][placement.columnIndex] == currentPlayer)) {
            if (amountInAColumn == 0) {
                startingVerticalClearingIndex.columnIndex = placement.columnIndex;
                startingVerticalClearingIndex.rowIndex = rowIndex;
            }

            amountInAColumn++;
            if (amountInAColumn >= 4) {
                if (amountInAColumn + 1 <= rowSize && (board[rowOnTheTop][placement.columnIndex] != NULL)) {
//                    printf("Vert: Can get more than 4 \n");
                    continue;
                } else {
//                    printf("Vert: Win \n");
                    startingVerticalClearingIndex.amount = amountInAColumn;
                    return startingVerticalClearingIndex;
                }
            }
        } else {
            amountInAColumn = 0;
            startingVerticalClearingIndex.columnIndex = -1;
            startingVerticalClearingIndex.rowIndex = -1;
        }
    }
//    printf("Vert: No win yet... \n");
    startingVerticalClearingIndex.columnIndex = -1;
    startingVerticalClearingIndex.rowIndex = -1;
    return startingVerticalClearingIndex;
}

struct TokenPlacement checkHorizontalWinCondition(int*** board, struct TokenPlacement placement) {
    // if the row has at least 4 of the same piece adjacent horizontally
        // free the memory of all these pieces except the current piece
    struct TokenPlacement startingHorizontalClearingIndex = { -1,-1};
        
        int amountInARow = 0;
        for (int columnIndex = 0; columnIndex < columnSize; columnIndex++) {
            int columnToTheRight = columnIndex + 1;

            if (columnToTheRight > columnSize) {
//                printf("Hor: No win \n");
                startingHorizontalClearingIndex.columnIndex = -1;
                startingHorizontalClearingIndex.rowIndex = -1;
                startingHorizontalClearingIndex.amount = -1;
                return startingHorizontalClearingIndex;
            }

            if (board[placement.rowIndex][columnIndex] == NULL || *board[placement.rowIndex][columnIndex] != currentPlayer) {
                startingHorizontalClearingIndex.columnIndex = -1;
                startingHorizontalClearingIndex.rowIndex = -1;
                startingHorizontalClearingIndex.amount = -1;
                amountInARow = 0;
                if (columnToTheRight + 1 > columnSize) return startingHorizontalClearingIndex;
//                printf("Hor: Reset & continue \n");
                continue;
            }

            if (*board[placement.rowIndex][columnIndex] == currentPlayer) {
                if (amountInARow == 0) {
                    startingHorizontalClearingIndex.columnIndex = columnIndex;
                    startingHorizontalClearingIndex.rowIndex = placement.rowIndex;
                }

                amountInARow++;
                if (amountInARow >= 4) {
                    if (columnToTheRight + 1 <= columnSize && (board[placement.rowIndex][columnToTheRight] != NULL && *board[placement.rowIndex][columnToTheRight] == currentPlayer)) {
//                        printf("Hor: Can get more than 4 \n");
                        continue;
                    } else {
                        if (amountInARow == columnSize) {
//                            printf("Hor: WIN WHOLE ROW CLEAR !!! \n");
                        }
//                        printf("Hor: Win \n");
                        startingHorizontalClearingIndex.amount = amountInARow;
                        return startingHorizontalClearingIndex;
                    }
                }
            } else {
                amountInARow = 0;
                startingHorizontalClearingIndex.columnIndex = -1;
                startingHorizontalClearingIndex.rowIndex = -1;
                startingHorizontalClearingIndex.amount = -1;
            }
        }
//    printf("Something went wrong...");
    startingHorizontalClearingIndex.columnIndex = -1;
    startingHorizontalClearingIndex.rowIndex = -1;
    startingHorizontalClearingIndex.amount = -1;
    return startingHorizontalClearingIndex;
}

int ***removeHorizontalPieces(int*** board, struct TokenPlacement horizontalStart) {
    for (int columnIndex = horizontalStart.columnIndex; columnIndex < horizontalStart.amount + 1; columnIndex++) {
        free(board[horizontalStart.rowIndex][columnIndex]);
        board[horizontalStart.rowIndex][columnIndex] = NULL;
    }
    return board;
}

int ***removeVerticalPieces(int*** board, struct TokenPlacement verticalStart) {
    for (int rowIndex = verticalStart.rowIndex; rowIndex >= (verticalStart.rowIndex - verticalStart.amount + 1); rowIndex--) {
        free(board[rowIndex][verticalStart.columnIndex]);
        board[rowIndex][verticalStart.columnIndex] = NULL;
    }
    return board;
}

int ***collapseBoard(int ***board) {
    for (int columnIndex = 0; columnIndex < columnSize; columnIndex++) {
        int startingCollapseIndex = rowSize - 1;
        for (int rowIndex = rowSize - 1; rowIndex >= 0; rowIndex--) {

            int rowOnTheTop = rowIndex - 1;
            if (rowOnTheTop == -1) {
                break;
            }

            if (board[rowIndex][columnIndex] != NULL) {
                if (board[rowOnTheTop][columnIndex] == NULL) {
                    startingCollapseIndex = rowOnTheTop;
                    continue;
                }
            } else {
                if (board[rowOnTheTop][columnIndex] != NULL) {
                    if (board[startingCollapseIndex][columnIndex] == NULL) {
                        board[startingCollapseIndex][columnIndex] = malloc(columnSize * sizeof(int*));
                    }

                    memcpy(board[startingCollapseIndex][columnIndex], board[startingCollapseIndex - 1][columnIndex], sizeof(int*));
                    free(board[startingCollapseIndex - 1][columnIndex]);
                    board[startingCollapseIndex - 1][columnIndex] = NULL;
                    startingCollapseIndex = rowOnTheTop;
                } else {
                    startingCollapseIndex = rowOnTheTop;
                    continue;
                }
            }
        }
    }
}

int ***startGame(int*** board) {
    // print the board
    printBoard(board);
    while (1) {
        // decide the correct player
        currentPlayer = !currentPlayer;

        int userSelectedColumn;
        // ask for the column to place your piece
        printf("CURRENT PLAYER: ");
        if(currentPlayer){
            printf("* \n");
        } else {
            printf("^ \n");
        }
        printf("Choose a column: \n");
        scanf("%d", &userSelectedColumn);

        struct TokenPlacement placedToken = placeToken(board, userSelectedColumn    );
        if (placedToken.columnIndex == -1 && placedToken.rowIndex == -1) {
            break;
        }
        int checkWinContitions = 1;

        while (checkWinContitions) {
            checkWinContitions = 0;
            struct TokenPlacement horizontalStartIndexes = checkHorizontalWinCondition(board, placedToken);
            struct TokenPlacement verticalStartIndexes = checkVerticalWinCondition(board, placedToken);

            if (horizontalStartIndexes.columnIndex != -1 && horizontalStartIndexes.rowIndex != -1) {
                checkWinContitions = 1;
                printf("Horizontal POINT FOR: %d \n", currentPlayer);
                removeHorizontalPieces(board, horizontalStartIndexes);
                if (currentPlayer) {
                    scorePlayerTwo++;
                    if (horizontalStartIndexes.amount > 4) {
                        printf("Horizontal BONUS POINT FOR: %d \n", currentPlayer);
                        scorePlayerTwo++;
                    }
                } else {
                    scorePlayerOne++;
                    if (horizontalStartIndexes.amount > 4) {
                        printf("Horizontal BONUS POINT FOR: %d \n", currentPlayer);
                        scorePlayerOne++;
                    }
                }
            }

            if (verticalStartIndexes.columnIndex != -1 && verticalStartIndexes.rowIndex != -1) {
                checkWinContitions = 1;
                printf("Vertical POINT FOR: %d \n", currentPlayer);
                removeVerticalPieces(board, verticalStartIndexes);
                if (currentPlayer) {
                    scorePlayerTwo++;
                    if (verticalStartIndexes.amount > 4) {
                        printf("Vertical BONUS POINT FOR: %d \n", currentPlayer);
                        scorePlayerTwo++;
                    }
                } else {
                    scorePlayerOne++;
                    if (verticalStartIndexes.amount > 4) {
                        printf("Vertical BONUS POINT FOR: %d \n", currentPlayer);
                        scorePlayerOne++;
                    }
                }
            }

//            makePiecesFallDown(board);
        }
//        printBoard(board);

        collapseBoard(board);

        printBoard(board);
    }


    // =============================================

    // set rowsMoved to false
    // -----------
    // loop through the columns left to right (j = 0)
      // create a counter for the empty spaces
        // loop through the rows from bottom to top (i = rowSize)
            // Stop the count when i - 1 == -1 or row[i][j] is NOT NULL and row[i - 1][j] is NULL
                // save the end index
            // when the row[i][j] is NULL and row[i - 1][j] is NOT NULL or row[i][j] is NULL and row[i - 1][j] is NULL
                // add to the counter and continue
                // if the counter is 0 save i as starting index

        // if counter > 0
            // rowsMoved to true
            // loop through the rows from startingIndex to endIndex
                // rows[startingIndex + i][j] = rows[startingIndex + count + i][j]
    // -----------
    // if rowsMoved == true
        // repeat the comparisson method
    // return the board
    return board;
}


int main(void) {
    int ***board = createBoard();
    board = startGame(board);

    destroyBoard(board);

    return 0;
}
