#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <cctype>
#include <windows.h>
#include <fstream>
#include <iomanip>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SIZE 4
#define BORDER 219
#define ESC 27

using namespace std;

vector<vector<int>> Matrix(SIZE, vector<int>(SIZE, 0));
int score = 2048;
bool gameOver = false;

void displayInstructions();
void displayMatrix();
void performAction(int);
void spawnRandomTile();
void initializeGame();
int checkGameState();
void updateRecord();
void displayScore();
void handleArrowKey(int);
bool moveDown();
bool moveUp();
bool moveRight();
bool moveLeft();
void displayPreviousRecords();

int main()
{
    int arrowKey;
    displayInstructions();
    initializeGame();
    displayMatrix(); // Display initial matrix

    while (!gameOver)
    {
        if (_kbhit())
        {
            arrowKey = _getch();
            if (arrowKey == 0 || arrowKey == 224)
            { // Handle extended key codes
                arrowKey = _getch();
            }

            if (arrowKey == ESC)
            {
                break;
            }

            performAction(arrowKey);

            int gameState = checkGameState();
            if (gameState == 1)
            {
                cout << "\n\t\t\tYOU WON!\n";
                displayScore();
                gameOver = true;
            }
            else if (gameState == 0)
            {
                cout << "\n\t\t\tGAME OVER!\n";
                displayScore();
                gameOver = true;
            }
        }
        Sleep(10); // Add a small delay to reduce CPU usage
    }

    cout << "\n\n\n\tTHANKS FOR PLAYING, GOOD LUCK FOR NEXT TIME!\n";
    return 0;
}

void displayInstructions()
{
    cout << "===================== 2048 GAME ==========================\n\n";
    cout << "INSTRUCTIONS:\n";
    cout << " - Use arrow keys to move the tiles.\n";
    cout << " - Reach 2048 in any tile to win!\n\n";
    cout << "Press any key to start...\n";
    while (!_kbhit())
        ;
    _getch(); // Consume the key press
}

void displayMatrix()
{
    system("cls");
    displayScore();
    cout << "\t\t\t ";
    for (int i = 0; i < SIZE; ++i)
    {
        cout << (char)BORDER << (char)BORDER << (char)BORDER << (char)BORDER << (char)BORDER << (char)BORDER;
    }
    cout << (char)BORDER << endl;

    for (int i = 0; i < SIZE; ++i)
    {
        cout << "\t\t\t " << (char)BORDER;
        for (int j = 0; j < SIZE; ++j)
        {
            if (Matrix[i][j] == 0)
            {
                cout << "      ";
            }
            else
            {
                cout << " " << setw(4) << Matrix[i][j] << " ";
            }
        }
        cout << (char)BORDER << endl;
    }

    cout << "\t\t\t ";
    for (int i = 0; i < SIZE; ++i)
    {
        cout << (char)BORDER << (char)BORDER << (char)BORDER << (char)BORDER << (char)BORDER << (char)BORDER;
    }
    cout << (char)BORDER << endl;
}

void performAction(int arrowKey)
{
    bool moved = false;
    switch (arrowKey)
    {
    case UP:
        moved = moveUp();
        break;
    case DOWN:
        moved = moveDown();
        break;
    case LEFT:
        moved = moveLeft();
        break;
    case RIGHT:
        moved = moveRight();
        break;
    default:
        return;
    }

    if (moved)
    {
        score--;
        spawnRandomTile();
        displayMatrix();
    }
}

void spawnRandomTile()
{
    vector<pair<int, int>> emptyCells;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (Matrix[i][j] == 0)
            {
                emptyCells.push_back({i, j});
            }
        }
    }

    if (!emptyCells.empty())
    {
        srand(time(NULL));
        int randomIndex = rand() % emptyCells.size();
        int row = emptyCells[randomIndex].first;
        int col = emptyCells[randomIndex].second;
        int value = (rand() % 2 == 0) ? 2 : 4;
        Matrix[row][col] = value;
    }
}

void initializeGame()
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            Matrix[i][j] = 0;
        }
    }
    // Initialize with a couple of starting tiles
    spawnRandomTile();
    spawnRandomTile();
}

int checkGameState()
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (Matrix[i][j] == 2048)
            {
                return 1; // Won
            }
            if (Matrix[i][j] == 0)
            {
                return -1; // Game still ongoing
            }
        }
    }

    // Check for possible merges
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE - 1; ++j)
        {
            if (Matrix[i][j] == Matrix[i][j + 1])
                return -1;
        }
    }
    for (int j = 0; j < SIZE; ++j)
    {
        for (int i = 0; i < SIZE - 1; ++i)
        {
            if (Matrix[i][j] == Matrix[i + 1][j])
                return -1;
        }
    }

    return 0; // Game over (no empty cells and no possible merges)
}

void displayScore()
{
    cout << "\n\t\t\t2048\n";
    cout << "\t\t\tSCORE: " << score << "\n\n\n";
}

bool moveDown()
{
    bool moved = false;
    for (int col = 0; col < SIZE; ++col)
    {
        int nextRow = SIZE - 1;
        for (int row = SIZE - 1; row >= 0; --row)
        {
            if (Matrix[row][col] != 0)
            {
                if (row < nextRow && Matrix[nextRow][col] == Matrix[row][col])
                {
                    Matrix[nextRow][col] *= 2;
                    Matrix[row][col] = 0;
                    score += Matrix[nextRow][col];
                    nextRow--;
                    moved = true;
                }
                else if (Matrix[nextRow][col] == 0)
                {
                    Matrix[nextRow][col] = Matrix[row][col];
                    Matrix[row][col] = 0;
                    moved = true;
                }
                else if (nextRow != row)
                {
                    nextRow--;
                    if (Matrix[nextRow][col] == 0)
                    {
                        Matrix[nextRow][col] = Matrix[row][col];
                        Matrix[row][col] = 0;
                        moved = true;
                    }
                }
            }
        }
    }
    return moved;
}

bool moveUp()
{
    bool moved = false;
    for (int col = 0; col < SIZE; ++col)
    {
        int nextRow = 0;
        for (int row = 0; row < SIZE; ++row)
        {
            if (Matrix[row][col] != 0)
            {
                if (row > nextRow && Matrix[nextRow][col] == Matrix[row][col])
                {
                    Matrix[nextRow][col] *= 2;
                    Matrix[row][col] = 0;
                    score += Matrix[nextRow][col];
                    nextRow++;
                    moved = true;
                }
                else if (Matrix[nextRow][col] == 0)
                {
                    Matrix[nextRow][col] = Matrix[row][col];
                    Matrix[row][col] = 0;
                    moved = true;
                }
                else if (nextRow != row)
                {
                    nextRow++;
                    if (Matrix[nextRow][col] == 0)
                    {
                        Matrix[nextRow][col] = Matrix[row][col];
                        Matrix[row][col] = 0;
                        moved = true;
                    }
                }
            }
        }
    }
    return moved;
}

bool moveRight()
{
    bool moved = false;
    for (int row = 0; row < SIZE; ++row)
    {
        int nextCol = SIZE - 1;
        for (int col = SIZE - 1; col >= 0; --col)
        {
            if (Matrix[row][col] != 0)
            {
                if (col < nextCol && Matrix[row][nextCol] == Matrix[row][col])
                {
                    Matrix[row][nextCol] *= 2;
                    Matrix[row][col] = 0;
                    score += Matrix[row][nextCol];
                    nextCol--;
                    moved = true;
                }
                else if (Matrix[row][nextCol] == 0)
                {
                    Matrix[row][nextCol] = Matrix[row][col];
                    Matrix[row][col] = 0;
                    moved = true;
                }
                else if (nextCol != col)
                {
                    nextCol--;
                    if (Matrix[row][nextCol] == 0)
                    {
                        Matrix[row][nextCol] = Matrix[row][col];
                        Matrix[row][col] = 0;
                        moved = true;
                    }
                }
            }
        }
    }
    return moved;
}

bool moveLeft()
{
    bool moved = false;
    for (int row = 0; row < SIZE; ++row)
    {
        int nextCol = 0;
        for (int col = 0; col < SIZE; ++col)
        {
            if (Matrix[row][col] != 0)
            {
                if (col > nextCol && Matrix[row][nextCol] == Matrix[row][col])
                {
                    Matrix[row][nextCol] *= 2;
                    Matrix[row][col] = 0;
                    score += Matrix[row][nextCol];
                    nextCol++;
                    moved = true;
                }
                else if (Matrix[row][nextCol] == 0)
                {
                    Matrix[row][nextCol] = Matrix[row][col];
                    Matrix[row][col] = 0;
                    moved = true;
                }
                else if (nextCol != col)
                {
                    nextCol++;
                    if (Matrix[row][nextCol] == 0)
                    {
                        Matrix[row][nextCol] = Matrix[row][col];
                        Matrix[row][col] = 0;
                        moved = true;
                    }
                }
            }
        }
    }
    return moved;
}

void updateRecord()
{
    char playerName[20];
    cout << "Enter your name: ";
    cin >> setw(20) >> playerName;

    ofstream file("record.txt", ios::app);
    if (file.is_open())
    {
        file << "Player Name: " << playerName << endl;
        file << "Score: " << score << endl;
        time_t now = time(0);
        char *dt = ctime(&now);
        file << "Played Date: " << dt;
        file << "---------------------------------\n";
        file.close();
    }
    else
    {
        cout << "Unable to open record file.\n";
    }

    char choice;
    cout << "View previous records? (y/n): ";
    cin >> choice;
    if (tolower(choice) == 'y')
    {
        displayPreviousRecords();
    }
}

void displayPreviousRecords()
{
    ifstream file("record.txt");
    if (file.is_open())
    {
        char ch;
        while (file.get(ch))
        {
            cout << ch;
        }
        file.close();
    }
    else
    {
        cout << "No previous records found.\n";
    }
}