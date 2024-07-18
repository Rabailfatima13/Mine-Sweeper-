#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

// Constants for difficulty levels
const int EASY_MINES = 10;
const int INTERMEDIATE_MINES = 40;
const int ADVANCED_MINES = 99;
//Representing each cell of  minesweeper 
struct Cell {
    bool hasMine;
    bool isExposed;
    bool isFlagged;
    int adjacentMinesCount;

    Cell() : hasMine(false), isExposed(false), isFlagged(false), adjacentMinesCount(0) {}
};
#include <limits>
class Board {
private:
    Cell** cells;
    int rows;
    int cols;
    int totalMines;

public:
    Board(int numRows, int numCols, int numMines) : rows(numRows), cols(numCols), totalMines(numMines) {
        cells = new Cell*[rows];
        for (int i = 0; i < rows; i++) {
            cells[i] = new Cell[cols];
        }

        placeMines();
        calculateAdjacentMines();
    }

    ~Board() {
        for (int i = 0; i < rows; i++) {
            delete[] cells[i];
        }
        delete[] cells;
    }

    void placeMines() {
        int minesPlaced = 0;
        srand(time(nullptr));

        while (minesPlaced < totalMines) {
            //using % that it does not exceed the limit 
            int r = rand() % rows;
            int c = rand() % cols;

            if (!cells[r][c].hasMine) {
                cells[r][c].hasMine = true;
                minesPlaced++;
            }
        }
    }

    void calculateAdjacentMines() {
        //providing 8 directions :
        //north 
        //south
        //east 
        //west......
        int directions[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},         {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (cells[r][c].hasMine) {
                    continue;
                    //if their is a mine then move on ;
                }
                 //if no mine then count the number of of adjacent ines to that cell; 
                int adjacentCount = 0;
                for (int i = 0; i < 8; i++) {
                    int nr = r + directions[i][0];
                    int nc = c + directions[i][1];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && cells[nr][nc].hasMine) {
                        adjacentCount++;
                    }
                }
                cells[r][c].adjacentMinesCount = adjacentCount;
            }
        }
    }

    void exposeCell(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols || cells[r][c].isExposed || cells[r][c].isFlagged) {
        return;
    }

    cells[r][c].isExposed = true;

    if (cells[r][c].hasMine) {
        // Game over, reveal all mine cells
        cout << "You Lost! You hit a mine." << endl;
        revealAllMines();
        displayBoard(); // Show the final board with all cells revealed
        return; // Exit the function after revealing all mines
    }

    if (cells[r][c].adjacentMinesCount == 0) {
        // Recursive expose neighboring cells if current cell has no adjacent mines
        exposeNeighbors(r, c);
    }
    }
    void revealAllMines() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (cells[r][c].hasMine) {
                cells[r][c].isExposed = true;
            }
        }
    }
        
    }

    void exposeNeighbors(int r, int c) {
        int directions[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},         {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        for (int i = 0; i < 8; i++) {
            int nr = r + directions[i][0];
            int nc = c + directions[i][1];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !cells[nr][nc].isExposed) {
                exposeCell(nr, nc);
            }
        }
    }

    void toggleFlag(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || cells[r][c].isExposed) {
            return;
        }

        cells[r][c].isFlagged = !cells[r][c].isFlagged;
    }

    void displayBoard() {
        cout << "     ";
        for (int c = 0; c < cols; c++) {
            if(c<10)
            cout << c << "  ";
            else
            cout << c << " ";
        }
        cout << endl;

        for (int r = 0; r < rows; r++) {
            if(r<10)
            cout << r << "   ";
            else
            cout<< r << "  ";
            for (int c = 0; c < cols; c++) {
                if (cells[r][c].isExposed) {
                    if (cells[r][c].hasMine) {
                        cout << " * ";
                    } else if (cells[r][c].adjacentMinesCount > 0) {
                        cout << " " << cells[r][c].adjacentMinesCount << " ";
                    } else {
                        cout << "   ";
                    }
                } else {
                    if (cells[r][c].isFlagged) {
                        cout << " F ";
                    } else {
                        cout << " - ";
                    }
                }
            }
            cout << endl;
        }
    }

    bool isGameWon() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (!cells[r][c].isExposed && !cells[r][c].hasMine) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isGameOver() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (cells[r][c].isExposed && cells[r][c].hasMine) {
                    return true; // Game over if any exposed cell is a mine
                }
            }
        }
        return false;
    }
};

// Define Game class
class Game {
private:
//Agregation
    Board* board;

public:
    Game(int numRows, int numCols, int numMines) {
        board = new Board(numRows, numCols, numMines);
    }

    ~Game() {
        delete board;
    }

    void startGame() {
        displayGame();
        playGame();
    }

    void displayGame() {
        board->displayBoard();
    }

    void playGame() {
    while (true) {
        int r, c;
        char action;

//take inputs untill valid input is taken 
while (true) {
            cout << "Enter row, column, and action (L/R): ";
            if (!(cin >> r >> c >> action) || (action != 'L' && action != 'R')) {
                // If input extraction fails or invalid action is entered
                cout << "Invalid input or action. Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break; // Break out of the loop if valid input is provided
            }
        }

        if (action == 'L') {
            board->exposeCell(r, c);
            if (board->isGameOver()) {
                cout << "Game Over! You hit a mine." << endl;
               
                break; // Exit the loop when game over
            }
            if (board->isGameWon()) {
                cout << "Congratulations! You won!" << endl;
                break; // Exit the loop when game won
            }
        } else if (action == 'R') {
            board->toggleFlag(r, c);
        }

        // Display the updated board after each valid move
        board->displayBoard();
    }
        
    }

};

int main() {
    int numRows, numCols, numMines;
    char difficulty;

    cout << "Welcome to Minesweeper!" << endl;
    cout << "Select difficulty (E: Easy, I: Intermediate, A: Advanced): ";
    cin >> difficulty;

    switch (difficulty) {
        case 'E':
            numRows = 8;
            numCols = 8;
            numMines = EASY_MINES;
            break;
        case 'I':
            numRows = 16;
            numCols = 16;
            numMines = INTERMEDIATE_MINES;
            break;
        case 'A':
            numRows = 16;
            numCols = 30;
            numMines = ADVANCED_MINES;
            break;
        default:
            cout << "Invalid difficulty selection. Exiting." << endl;
            return 0;
    }

    Game game(numRows, numCols, numMines);
    game.startGame();

    return 0;
}
