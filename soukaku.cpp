#include<iostream>
#include<array>
#include<fstream>
#include<bitset>
#include<vector>
#include<string>
#include<chrono>

#include<stdio.h>

#define clearScreen() std::cout << "\033[2J\033[H" 
#define displayUsage() std::cout << "soukaku <PUZZLE FILE>" << std::endl;

struct solveInfo {
    bool solved;
    int cycles = 0;
    std::chrono::microseconds ms;
};

typedef std::array<std::array<int, 9>,9> boardArray;

std::array<std::bitset<10>,9> rowConstraints;
std::array<std::bitset<10>, 9> colConstraints;
std::array<std::bitset<10>, 9> boxConstraints;

std::vector<std::pair<int, int>> editableCells;

bool DEBUG = false;

bool checkValid(int row, int col, int num) {
    int boxIndex = (row/3) * 3 + (col/3);
    return !rowConstraints[row][num] && !colConstraints[col][num] && !boxConstraints[boxIndex][num];
}

void initalizeConstraints(const boardArray& board) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                editableCells.push_back({row,col});
                continue;
            }
            rowConstraints[row][board[row][col]] = true; 
            colConstraints[col][board[row][col]] = true;
            boxConstraints[(row/3) * 3 + (col/3)][board[row][col]] = true;
        }
    }
}

void placeNumber(boardArray& board, int row, int col, int num) {
    int boxIndex = (row/3) * 3 + (col/3);

    board[row][col] = num;

    rowConstraints[row][num] = true;
    colConstraints[col][num] = true;
    boxConstraints[boxIndex][num] = true;
}

void removeNumber(boardArray& board, int row, int col, int num) {
    int boxIndex = (row/3) * 3 + (col/3);

    board[row][col] = 0;

    rowConstraints[row][num] = false;
    colConstraints[col][num] = false;
    boxConstraints[boxIndex][num] = false;
}

bool fillBoard(boardArray& board, const char* puzzleFile) {
    std::ifstream file(puzzleFile);

    if (!file.is_open()) {
        return false;
    }

    int row = 0;
    int col = 0;
    char c;
    while (file.get(c)) {
        if (c == '|' || c == '-' || c == '+' || c == '\n') {
            continue;
        }

        int num = c - '0';
        board[row][col] = num;
        col += 1;

        if (col == 9) {
            col = 0;
            row += 1;
        }
    }
    return true;
}

void displayBoard(const boardArray& board) {
    int pColorCell = 0;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {

            if (std::pair<int, int>(row, col) == editableCells[pColorCell]) {
                pColorCell += 1;
                printf("\033[92m%d\033[0m", board[row][col]);
            }
            else {
                printf("%d", board[row][col]);
            }

            if ((col+1) % 3 == 0 && col != 8) {
                printf(" | ");
            }
            else {
                printf(" ");
            }
        }

        printf("\n");

        if ((row+1) % 3 == 0 && row != 8) {
            printf("------+-------+------\n");
        }
    }
}

void backTraceSolve(solveInfo& si, boardArray& board) {
    int cycles = 0;
    int pIndex = 0;
    int cNum = 1;
    int lenEditable = editableCells.size();

    while (true) {
        auto t1 = std::chrono::high_resolution_clock::now();
        if (pIndex >= lenEditable) {
            auto t2 = std::chrono::high_resolution_clock::now();
            si.ms = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
            if (DEBUG) {
                clearScreen();
            }
            si.solved = true;
            return;
        }
        int cRow = editableCells[pIndex].first;
        int cCol = editableCells[pIndex].second;

        if (checkValid(cRow, cCol, cNum)) {
            placeNumber(board, cRow, cCol, cNum);
            pIndex += 1;
            cNum = 1;
        }
        else if (cNum >= 9) {
            if (pIndex == 0) {
                si.solved = false;
                return;
            }
            int prevCell;

            do {
                if (pIndex == 0) {
                    si.solved = false;
                    return;
                }
                pIndex -= 1;
                prevCell = board[editableCells[pIndex].first][editableCells[pIndex].second];
                removeNumber(board, editableCells[pIndex].first, editableCells[pIndex].second, prevCell);
                cNum = prevCell + 1;

                if (DEBUG) {
                    clearScreen();
                    displayBoard(board);
                }

            } while (prevCell == 9);
        }
        else {
            cNum += 1;
        }
        si.cycles += 1;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        displayUsage();
        return 1;
    }

    std::string arg;

    for (int i = 1; i < argc; i++) {
        arg = argv[i];

        if (arg == "-d") {
            DEBUG = true;
        }
    }

    boardArray board;
    solveInfo si;
    if(!fillBoard(board, argv[1])) {
        std::cout << "Failed to read puzzle file" << std::endl;
    }

    initalizeConstraints(board);

    backTraceSolve(si, board);
    if(!si.solved) {
        std::cout << "Solution not found" << std::endl;
        return 0;
    }

    std::cout << std::endl;
    displayBoard(board);
    std::cout << "\nSolution Found\nSolve Time: " << si.ms.count() << "us" << "\nCycles: "  << si.cycles << "\n" << std::endl;
}