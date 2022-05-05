#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

void generateComputer(char field[10][10]); //creates a field to play the game on
void printField(char field[10][10]); //prints the field to play on
void printFields(char leftField[10][10], char rightField[10][10], int playerMessage); //prints two fields for the players, message for the player if needed
int attack(char field[10][10], char hitField[10][10], int x, int y, int isComputer); //attacks the chosen position with (x,y) position; returns 0 if the target has already been hit,returns -1 if the hit is miss, returns #checkDirections() if it hits a ship
void generateEmptyField(char field[10][10]);//generates empty field
int placeShip(char field[10][10], int x, int y, int len, int direction); //places the ship using x and y, the length of the ship and the direction in which we want it to  be
void printFormatted(char str[], char c); //prints symbols used in the game in specific color- water-blue, ship-green, hit-red
void generatePlayer(char field[10][10]); // starts generating the player's field
void startGame(char playerField[10][10], char computerField[10][10],char playerHitField[10][10], char computerHitField[10][10]); //runs the game
int checkDirections(char field[10][10], char hitField[10][10], int x, int y, int isComputer); //checks if all parts of the ship have been hit and sets all fields around the ship to missed hits; returns 1 if not, returns 2 if all of them have been hit
void printLegend(int row); //prints information about the game on the right side of the field

const char WATER_SYMBOL = '~';
const char SHIP_SYMBOL = '#';
const char HIT_SYMBOL = 'x';
const char MISS_SYMBOL = 'o';

int lifeComputer = 0;
int lifePlayer = 0;
int playerRound = 1;
int ships[] = {1, 2, 1, 1};

int main()
{
    char playerField[10][10];
    char playerHitField[10][10];
    char computerField[10][10];
    char computerHitField[10][10];

    srand((time(NULL)));

    generateEmptyField(playerField);
    generateEmptyField(playerHitField);
    generateEmptyField(computerField);
    generateEmptyField(computerHitField);
    generateComputer(computerField);
    generatePlayer(playerField);
    printFields(computerHitField, playerField, 0);
    startGame(playerField, computerField, playerHitField, computerHitField);

    return 0;
}

void startGame(char playerField[10][10], char computerField[10][10],
               char playerHitField[10][10], char computerHitField[10][10]) {
    int playerMessage = 0;
    int round = 1;

    int botHasHit = 0;
    int hitX;
    int hitY;
    int backupHitX = -1;
    int backupHitY = -1;

    int tries = 0;
    while (1)
    {
        if (round % 2 == 1)
        {
            printf("\nSelect a position to attack (A-J 0-9): \n");
            char x;
            int y;
            scanf("%c %d", &x, &y);
            int clear;
            while ((clear = getchar()) != '\n' && clear != EOF);
            if (x >= 'a' && x <= 'j')
            {
                x -= 'a' - 'A';
            }
            if (y < 0 || y > 9 || x < 'A' || x > 'J')
            {
                printFields(computerHitField, playerField, 0);
                printf("Invalid location!\n");
                continue;
            }
            x -= 'A';
            int successful = attack(computerField, computerHitField, x, y, 1);
            if (successful)
            {
                if (successful == -1)
                {
                    playerMessage = 1;
                } else if (successful == 1)
                {
                    lifeComputer--;
                    playerMessage = 2;
                } else
                {
                    lifeComputer--;
                    playerMessage = 3;
                }
                round++;
                playerRound++;
            } else
            {
                printFields(computerHitField, playerField, 0);
                printf("You have already hit this position!\n");
                continue;
            }
            if (lifeComputer == 0)
            {
                printFields(computerHitField, playerField, 0);
                printf("!!! YOU WON :) !!!");
                return;
            }

        } else {
            int x;
            int y;
            if (tries == 50) {
                if (backupHitX == -1) {
                    botHasHit = 0;
                }
                hitX = backupHitX;
                hitY = backupHitY;
                backupHitX = -1;
                backupHitY = -1;
                tries = 0;
            }
            if (botHasHit) {
                tries++;
                int ch = rand() % 2;
                if (ch) {
                    ch = rand() % 2;
                    if (ch) {
                        x = hitX + 1;
                        y = hitY;
                    } else {
                        x = hitX - 1;
                        y = hitY;
                    }
                } else {
                    ch = rand() % 2;
                    if (ch) {
                        x = hitX;
                        y = hitY + 1;
                    } else {
                        x = hitX;
                        y = hitY - 1;
                    }
                }

            } else
            {
                x = rand() % 10;
                y = rand() % 10;
            }
            int successful = attack(playerField, playerHitField, x, y, 0);
            if (successful)
                {
                if (successful == -1)
                {
                    printFields(computerHitField, playerField, playerMessage);
                    printf("PHEW, THE BOT MISSED! (%c %d)\n", 'A' + x, y);
                } else if (successful == 1)
                {
                    lifePlayer--;
                    printFields(computerHitField, playerField, playerMessage);
                    printf("YOU GOT HIT! (%c %d)\n", 'A' + x, y);
                    botHasHit = 1;
                    hitX = x;
                    hitY = y;
                    if (backupHitX == -1)
                    {
                        backupHitX = x;
                        backupHitY = y;
                    }
                } else
                {
                    lifePlayer--;
                    if (lifePlayer == 0)
                    {
                        printFields(computerField, playerField, playerMessage);
                    } else
                    {
                        printFields(computerHitField, playerField, playerMessage);
                    }
                    printf("ONE OF YOUR SHIPS HAS SUNKEN! (%c %d)\n", 'A' + x, y);
                    backupHitX = -1;
                    backupHitY = -1;
                    botHasHit = 0;
                }
                round++;
            }
            if (lifePlayer == 0)
            {
                printf("!!! YOU LOST :( !!!");
                return;
            }
        }
    }
}


void printField(char field[10][10]) //prints field
{
    printf("\n\n\n\n\n\n\n\n  0 1 2 3 4 5 6 7 8 9\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("%c ", 'A' + i);
        for (int j = 0; j < 10; ++j)
        {
            printFormatted("%c ", field[i][j]);
        }
        printf("\n");
    }
}

void printFields(char leftField[10][10], char rightField[10][10], int playerMessage)
{
    printf("\n\n\n\n\n\n\n\n");
    printf(" ___________________________________________________\n");
    printf("|       BOT'S FIELD       |       YOUR FIELD        |\n");
    printf("|_________________________|_________________________|_____________\n");
    printf("|   0 1 2 3 4 5 6 7 8 9   |   0 1 2 3 4 5 6 7 8 9   | LEGEND:     |\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("| %c ", 'A' + i);
        for (int j = 0; j < 10; ++j)
        {
            printFormatted("%c ", leftField[i][j]);
        }
        printf("%c | %c ", 'A' + i, 'A' + i);
        for (int j = 0; j < 10; ++j)
        {
            printFormatted("%c ", rightField[i][j]);
        }
        printf("%c |", 'A' + i);
        printLegend(i);
        printf("\n");
    }
    printf("|   0 1 2 3 4 5 6 7 8 9   |   0 1 2 3 4 5 6 7 8 9   |");
    printLegend(10);
    printf("|_________________________|_________________________|");
    printLegend(11);
    switch (playerMessage)
    {
        case 1:
            printf("YOU MISSED!\n");
            break;
        case 2:
            printf("BAM, YOU HIT SOMETHING!\n");
            break;
        case 3:
            printf("BOOM, YOU SUNK A SHIP!\n");
            break;
        case 0:
        default:
            break;
    }
}

void printLegend(int row)
{
    switch (row)
    {
        case 0:
            printFormatted("  %c", WATER_SYMBOL);
            printf(" = WATER  |");
            break;
        case 1:
            printFormatted("  %c", HIT_SYMBOL);
            printf(" = HIT    |");
            break;
        case 2:
            printFormatted("  %c", MISS_SYMBOL);
            printf(" = MISS   |");
            break;
        case 3:
            printFormatted("  %c", SHIP_SYMBOL);
            printf(" = SHIP   |");
            break;
        case 4:
            printf(" SHIPS LEFT: |");
            break;
        case 5:
            printf("  %dx ", ships[3]);
            for (int i = 0; i < 4; ++i) {
                printFormatted("%c", SHIP_SYMBOL);
            }
            printf("    |");
            break;
        case 6:
            printf("  %dx ", ships[2]);
            for (int i = 0; i < 3; ++i) {
                printFormatted("%c", SHIP_SYMBOL);
            }
            printf("     |");
            break;
        case 7:
            printf("  %dx ", ships[1]);
            for (int i = 0; i < 2; ++i) {
                printFormatted("%c", SHIP_SYMBOL);
            }
            printf("      |");
            break;
        case 8:
            printf("  %dx ", ships[0]);
            for (int i = 0; i < 1; ++i) {
                printFormatted("%c", SHIP_SYMBOL);
            }
            printf("       |");
            break;
        case 9:
            printf(" ROUND:   ");
            printf("   |");
            break;
        case 10:
            printf("  %d", playerRound);
            if (playerRound < 10) {
                printf("          |\n");
            } else {
                printf("         |\n");
            }
            break;
        case 11:
            printf("_____________|\n");
            break;
    }
}

void generateEmptyField(char field[10][10]) {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            field[i][j] = WATER_SYMBOL;
        }
    }
}

void generateComputer(char field[10][10]) {
    int sizes[] = {1, 2, 2, 3, 4};
    int i = 0;
    while (i != 5) {
        int cont = placeShip(field, rand() % 10, rand() % 10, sizes[i], rand() % 10);
        if (cont) {
            lifeComputer += sizes[i];
            i++;
        }
    }
}

void generatePlayer(char field[10][10]) {
    int sizes[] = {4, 3, 2, 2, 1};
    int i = 0;
    printField(field);
    while (i != 5) {
        printf("\nSelect the position for your ship with length %d (A-J 0-9): \n", sizes[i]);
        char x;
        int y;
        scanf("%c %d", &x, &y);
        int clear;
        while ((clear = getchar()) != '\n' && clear != EOF);
        if (x >= 'a' && x <= 'j') {
            x -= 'a' - 'A';
        }
        if (y < 0 || y > 9 || x < 'A' || x > 'J') {
            printf("Invalid location!\n");
            continue;
        }
        char direction = 0;
        if (sizes[i] != 1) {
            printf("W - UP | A - LEFT | S - DOWN | D - RIGHT\n"); //0 - RIGHT | 1 - LEFT | 2 - DOWN | 3 - UP
            printf("Select the direction of your ship at (%c %d): \n", x, y);
            scanf("%c", &direction);
            while ((clear = getchar()) != '\n' && clear != EOF);
            switch (direction) {
                case 'W':
                case 'w':
                    direction = 3;
                    break;
                case 'A':
                case 'a':
                    direction = 1;
                    break;
                case 'S':
                case 's':
                    direction = 2;
                    break;
                case 'D':
                case 'd':
                    direction = 0;
                    break;
                default:
                    printf("Invalid direction!\n");
                    continue;
            }
        }
        x -= 'A';
        int cont = placeShip(field, x, y, sizes[i], direction);
        if (cont) {
            printField(field);
            lifePlayer += sizes[i];
            i++;
        } else {
            printField(field);
            printf("You can't place your ship here!\n");
        }
    }
}

int attack(char field[10][10], char hitField[10][10], int x, int y, int isComputer) {
    //if the hit is not valid, return it
    if (x < 0 || x > 9 || y < 0 || y > 9) {
        return 0;
    }
    //if the hit is on already hitted symbol, return it
    if (field[x][y] == HIT_SYMBOL || field[x][y] == MISS_SYMBOL) {
        return 0;
    }

    //if the hit is on water - make it miss
    if (field[x][y] == WATER_SYMBOL) {
        field[x][y] = MISS_SYMBOL;
        hitField[x][y] = MISS_SYMBOL;
        return -1;
    }

    //if the hit is on ship - hit it and check if the whole ship has been hit
    if (field[x][y] == SHIP_SYMBOL) {
        field[x][y] = HIT_SYMBOL;
        hitField[x][y] = HIT_SYMBOL;
        return checkDirections(field, hitField, x, y, isComputer);
    }

    return 0;
}

int checkDirections(char field[10][10], char hitField[10][10], int x, int y, int isComputer) {
    int sinkCounter = 0;
    int sinkAround[2][10] = {
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    };

    int shipSize = 0;

    //checks if the whole ship has been hit, this is the down check
    if (x - 1 >= 0 && (field[x - 1][y] == SHIP_SYMBOL || field[x - 1][y] == HIT_SYMBOL)) {
        int i = 1;
        while (1) {
            if (x - i >= 0) {
                if (field[x - i][y] == WATER_SYMBOL || field[x - i][y] == MISS_SYMBOL) {
                    if (field[x - i][y] == WATER_SYMBOL) {
                        sinkAround[0][sinkCounter] = x - i;
                        sinkAround[1][sinkCounter] = y;
                        sinkCounter++;
                    }
                    break;
                } else {
                    if (field[x - i][y] == HIT_SYMBOL) {
                        sinkAround[0][sinkCounter] = x - i;
                        sinkAround[1][sinkCounter] = y + 1;
                        sinkCounter++;
                        sinkAround[0][sinkCounter] = x - i;
                        sinkAround[1][sinkCounter] = y - 1;
                        sinkCounter++;
                        shipSize++;
                        i++;
                    } else {
                        return 1;
                    }
                }
            } else {
                break;
            }
        }
    } else if (x - 1 >= 0 && field[x - 1][y] == WATER_SYMBOL) {
        sinkAround[0][sinkCounter] = x - 1;
        sinkAround[1][sinkCounter] = y;
        sinkCounter++;
    }

    //checks if the whole ship has been hit, this is the up check
    if (x + 1 < 10 && (field[x + 1][y] == SHIP_SYMBOL || field[x + 1][y] == HIT_SYMBOL)) {
        int i = 1;
        while (1) {
            if (x + i < 10) {
                if (field[x + i][y] == WATER_SYMBOL || field[x + i][y] == MISS_SYMBOL) {
                    if (field[x + i][y] == WATER_SYMBOL) {
                        sinkAround[0][sinkCounter] = x + i;
                        sinkAround[1][sinkCounter] = y;
                        sinkCounter++;
                    }
                    break;
                } else {
                    if (field[x + i][y] == HIT_SYMBOL) {
                        sinkAround[0][sinkCounter] = x + i;
                        sinkAround[1][sinkCounter] = y + 1;
                        sinkCounter++;
                        sinkAround[0][sinkCounter] = x + i;
                        sinkAround[1][sinkCounter] = y - 1;
                        sinkCounter++;
                        shipSize++;
                        i++;
                    } else {
                        return 1;
                    }
                }
            } else {
                break;
            }
        }
    } else if (x + 1 < 10 && field[x + 1][y] == WATER_SYMBOL) {
        sinkAround[0][sinkCounter] = x + 1;
        sinkAround[1][sinkCounter] = y;
        sinkCounter++;
    }

    //checks if the whole ship has been hit, this is the right check
    if (y + 1 < 10 && (field[x][y + 1] == SHIP_SYMBOL || field[x][y + 1] == HIT_SYMBOL)) {
        int i = 1;
        while (1) {
            if (y + i < 10) {
                if (field[x][y + i] == WATER_SYMBOL || field[x][y + i] == MISS_SYMBOL) {
                    if (field[x][y + i] == WATER_SYMBOL) {
                        sinkAround[0][sinkCounter] = x;
                        sinkAround[1][sinkCounter] = y + i;
                        sinkCounter++;
                    }
                    break;
                } else {
                    if (field[x][y + i] == HIT_SYMBOL) {
                        sinkAround[0][sinkCounter] = x + 1;
                        sinkAround[1][sinkCounter] = y + i;
                        sinkCounter++;
                        sinkAround[0][sinkCounter] = x - 1;
                        sinkAround[1][sinkCounter] = y + i;
                        sinkCounter++;
                        shipSize++;
                        i++;
                    } else {
                        return 1;
                    }
                }
            } else {
                break;
            }
        }
    } else if (y + 1 < 10 && field[x][y + 1] == WATER_SYMBOL) {
        sinkAround[0][sinkCounter] = x;
        sinkAround[1][sinkCounter] = y + 1;
        sinkCounter++;
    }

    //checks if the whole ship has been hit, this is the left check
    if (y - 1 >= 0 && (field[x][y - 1] == SHIP_SYMBOL || field[x][y - 1] == HIT_SYMBOL)) {
        int i = 1;
        while (1) {
            if (y - i >= 0) {
                if (field[x][y - i] == WATER_SYMBOL || field[x][y - i] == MISS_SYMBOL) {
                    if (field[x][y - i] == WATER_SYMBOL) {
                        sinkAround[0][sinkCounter] = x;
                        sinkAround[1][sinkCounter] = y - i;
                    }
                    break;
                } else {
                    if (field[x][y - i] == HIT_SYMBOL) {
                        sinkAround[0][sinkCounter] = x + 1;
                        sinkAround[1][sinkCounter] = y - i;
                        sinkCounter++;
                        sinkAround[0][sinkCounter] = x - 1;
                        sinkAround[1][sinkCounter] = y - i;
                        sinkCounter++;
                        shipSize++;
                        i++;
                    } else {
                        return 1;
                    }
                }
            } else {
                break;
            }
        }
    } else if (y - 1 >= 0 && field[x][y - 1] == WATER_SYMBOL) {
        sinkAround[0][sinkCounter] = x;
        sinkAround[1][sinkCounter] = y - 1;
    }

    //sink the fields around if the whole ship has been hit
    for (int j = 0; j < 10; ++j) {
        if (sinkAround[0][j] == -1 && sinkAround[1][j] == -1) {
            break;
        }
        if (sinkAround[0][j] > 9 || sinkAround[0][j] < 0 || sinkAround[1][j] < 0 || sinkAround[1][j] > 9) {
            continue;
        }
        field[sinkAround[0][j]][sinkAround[1][j]] = MISS_SYMBOL;
        hitField[sinkAround[0][j]][sinkAround[1][j]] = MISS_SYMBOL;
    }

    if (isComputer) ships[shipSize]--;

    return 2;
}

int placeShip(char field[10][10], int x, int y, int len, int direction) {
    switch (direction) {
        //GOING RIGHT
        case 0:
        case 4:
        case 8:
            //is the space enough
            if (y + len - 1 > 9) {
                return 0;
            }
            //are the field, the field above and the field below available
            for (int i = 0; i < len; ++i) {
                if (field[x][y + i] != WATER_SYMBOL) {
                    return 0;
                }
                if (x - 1 >= 0 && field[x - 1][y + i] != WATER_SYMBOL) {
                    return 0;
                }
                if (x + 1 <= 9 && field[x + 1][y + i] != WATER_SYMBOL) {
                    return 0;
                }
            }
            //is the field before available
            if (y - 1 >= 0 && field[x][y - 1] != WATER_SYMBOL) {
                return 0;
            }
            //is the field after available
            if (y + len <= 9 && field[x][y + len] != WATER_SYMBOL) {
                return 0;
            }
            //place the ship
            for (int i = 0; i < len; ++i) {
                field[x][y + i] = SHIP_SYMBOL;
            }
            break;

            //GOING LEFT
        case 1:
        case 5:
        case 9:
            //if the space is enough
            if (y - len + 1 < 0) {
                return 0;
            }
            //are the field, the field above and the field below available
            for (int i = 0; i < len; ++i) {
                if (field[x][y - i] != WATER_SYMBOL) {
                    return 0;
                }
                if (x - 1 >= 0 && field[x - 1][y - i] != WATER_SYMBOL) {
                    return 0;
                }
                if (x + 1 <= 9 && field[x + 1][y - i] != WATER_SYMBOL) {
                    return 0;
                }
            }
            //is the field before available
            if (y - len >= 0 && field[x][y - len] != WATER_SYMBOL) {
                return 0;
            }
            //is the field after available
            if (y + 1 <= 9 && field[x][y + 1] != WATER_SYMBOL) {
                return 0;
            }
            //place the ship
            for (int i = 0; i < len; ++i) {
                field[x][y - i] = SHIP_SYMBOL;
            }
            break;

            //GOING DOWN
        case 2:
        case 6:
            //is the space enough
            if (x + len - 1 > 9) {
                return 0;
            }
            //are the field, the field above and the field below available
            for (int i = 0; i < len; ++i) {
                if (field[x + i][y] != WATER_SYMBOL) {
                    return 0;
                }
                if (y - 1 >= 0 && field[x + i][y - 1] != WATER_SYMBOL) {
                    return 0;
                }
                if (y + 1 <= 9 && field[x + i][y + 1] != WATER_SYMBOL) {
                    return 0;
                }
            }
            //is the field before available
            if (x - 1 >= 0 && field[x - 1][y] != WATER_SYMBOL) {
                return 0;
            }
            //is the field after available
            if (x + len <= 9 && field[x + len][y] != WATER_SYMBOL) {
                return 0;
            }
            //place the ship
            for (int i = 0; i < len; ++i) {
                field[x + i][y] = SHIP_SYMBOL;
            }
            break;

            //GOING UP
        case 3:
        case 7:
        default:
            //if the space is enough
            if (x - len + 1 < 0) {
                return 0;
            }
            //are the field, the field above and the field below available
            for (int i = 0; i < len; ++i) {
                if (field[x - i][y] != WATER_SYMBOL) {
                    return 0;
                }
                if (y - 1 >= 0 && field[x - i][y - 1] != WATER_SYMBOL) {
                    return 0;
                }
                if (y + 1 <= 9 && field[x - i][y + 1] != WATER_SYMBOL) {
                    return 0;
                }
            }
            //is the field before available
            if (x - len >= 0 && field[x - len][y] != WATER_SYMBOL) {
                return 0;
            }
            //is the field after available
            if (x + 1 <= 9 && field[x + 1][y] != WATER_SYMBOL) {
                return 0;
            }
            //place the ship
            for (int i = 0; i < len; ++i) {
                field[x - i][y] = SHIP_SYMBOL;
            }
            break;
    }
    return 1;
}

void printFormatted(char str[], char c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    if (c == WATER_SYMBOL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    }
    if (c == SHIP_SYMBOL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    }
    if (c == HIT_SYMBOL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    }
    printf(str, c);
    SetConsoleTextAttribute(hConsole, saved_attributes);
}
