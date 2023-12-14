#include <iostream>
#include <vector>
#include <string>
#include <algorithm>



std::string alphas = "ABCDEFGHIJ";
const int size = 10;
int playerBrokenShips = 0;
int botBrokenShips = 0;
std::vector<int> colOfShips = {4, 3, 2, 1};
const int variantNo = int(rand()) % 4;
bool hit;


struct xy
{
    int x, y;
};


enum class Square
{
    EMPTY,
    SHOT,
    SHIP,
    AWAY,
    DEAD,
};


class Communication
{
public:
    void greeting()
    {
        std::cout << "Hello! If you want to know how to play - type in [ commands ]\n";
        std::cout << "If not - type in [ play ]\n";
    }
    std::string cin;
    void commands()
    {
        std::cout << "Types of commands:\n";
        std::cout << "fill - if you want to fill your field (format: [ X1Y1-X2Y2 ] (coordinates of each ship))\n";
        std::cout << "shoot - if you want to shoot any coordinate in bot field (format: [ xy ] (coordinates of each ship))\n";
        std::cout << "After every command press [ ENTER ]\n";
        std::cout << "Now, use these rules to fill your field in with ships!\n";
        std::cout << "If you understood everything - type in [ play ]\n";
    }
    void readyForFilling()
    {
        std::cout << "Now, you need to fill in your ships. To do it, type in [ fill ]\n";
    }
};



class Field
{

public:

    void printField(std::vector<std::vector<Square>> &field, std::string &name)
    {
        std::cout << name << "\t\t's field\n";
        std::cout << "   A B C D E F G H\n";
        for (int x = 0; x < size; x++)
        {
            if (x + 1 == 10)
            {
                std::cout << x + 1 << ' ';
            }
            else {
                std::cout << ' ' << x + 1 << ' ';
            }
            for (int y = 0; y < size; y++)
            {
                if (field[x - 1][y] == Square::DEAD || field[x + 1][y] == Square::DEAD ||
                    field[x - 1][y - 1] == Square::DEAD || field[x + 1][y - 1] == Square::DEAD || field[x - 1][y + 1] == Square::DEAD ||
                    field[x + 1][y + 1] == Square::DEAD || field[x][y - 1] == Square::DEAD || field[x][y + 1] == Square::DEAD ||
                    field[x][y] == Square::AWAY)
                {
                    std::cout << "* ";
                }
                if (field[x][y] == Square::EMPTY)
                {
                    std::cout << "- ";
                }
                if (field[x][y] == Square::SHOT)
                {
                    std::cout << "X ";
                }
                if (field[x][y] == Square::SHIP)
                {
                    std::cout << "# ";
                }
                if (field[x][y] == Square::DEAD)
                {
                    std::cout << "X ";
                }
            }
            std::cout << '\n';
        }
    };

    bool checkIsFieldOk(std::vector<std::vector<Square>> &field, int &x1, int &y1, int &x2, int &y2)
    {
        if (x1 == x2)
        {
            for (int x = x1 - 1; x <= x2 + 1; x++)
            {
                for (int y = y1 - 1; y <= y2 + 1; y++)
                {
                    if (field[x][y] != Square::EMPTY)
                    {
                        return false;
                    }
                }
            }
        }
        else if (y1 == y2)
        {
            for (int y = y1 - 1; y <= y2 + 1; y++)
            {
                for (int x = x1 - 1; x <= x2 + 1; x++)
                {
                    if (field[x][y] != Square::EMPTY)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    std::string fillByCoords(std::string s, std::vector<std::vector<Square>> &field)
    {
        int x1 = s[0] - 'a', y1 = s[1] - '0', x2 = s[2] - 'a', y2 = y2 = s[3] - '0';
        int lenOfShip = std::max(abs(x1 - x2), abs(y1 - y2));
        for (int i = 0; i < 4; i++)
        {
            if (lenOfShip == i + 1)
            {
                if (colOfShips[i] > 0)
                {
                    colOfShips[i]--;
                }
                else
                {
                    return "Filled unsuccesfully: the length and number of each type of ships is constant.\n";
                }
            }
        }
        if (checkIsFieldOk(field, x1, y1, x2, y2))
        {
            for (int x = x1; x <= x2; x++)
            {
                for (int y = y1; y <= y2; y++)
                {
                    field[x][y] = Square::SHIP;
                }
            }
        }
        return "Filled succesfully.\n";
    }
};

class Bot
{
public:
    bool checkForShip(std::vector<std::vector<Square>> &field, int &x, int &y, int &currentDirection, int &length)
    {

        // checking for space under potential ship
        if (currentDirection == 0)
        {
            if (y < length - 1)
            {
                return false;
            }
        }
        if (currentDirection == 1)
        {
            if (x > 10 - length)
            {
                return false;
            }
        }
        if (currentDirection == 2)
        {
            if (y > 10 - length)
            {
                return false;
            }
        }
        if (currentDirection == 3)
        {
            if (y < length - 1)
            {
                return false;
            }
        }

        // checking for space squares under potential ship
        if (currentDirection == 0)
        {
            for (int i = x; i < x + length; i++)
            {
                if (field[i][y] == Square::SHIP)
                {
                    return false;
                }
            }
        }
        if (currentDirection == 1)
        {
            for (int i = y; i < y + length; i++)
            {
                if (field[x][i] == Square::SHIP)
                {
                    return false;
                }
            }
        }
        if (currentDirection == 2)
        {
            for (int i = x - length + 1; i <= x; i++)
            {
                if (field[i][y] == Square::SHIP)
                {
                    return false;
                }
            }
        }
        if (currentDirection == 3)
        {
            for (int i = y - length + 1; i <= y; i++)
            {
                if (field[x][i] == Square::SHIP)
                {
                    return false;
                }
            }
        }


        // checking for space squares near potential ship (lying not for more than one square out of it)
        if (currentDirection == 0)
        {
            // y - 1
            for (int i = x - 1; i <= x + 1; i++)
            {
                if (i < 10 && i >= 0 && y - length < 10 && y - length >= 0)
                {
                    if (field[i][y - length] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - length; i <= y + 1; i++)
            {
                if (i < 10 && i >= 0 && x + 1 < 10 && x + 1 >= 0)
                {
                    if (field[x + 1][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x - 1; i <= x + 1; i++)
            {
                if (i < 10 && i >= 0 && y + 1 < 10 && y + 1 >= 0)
                {
                    if (field[i][y + 1] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y - length; i <= y + 1; i++)
            {
                if (i < 10 && i >= 0 && x - 1 < 10 && x - 1 >= 0)
                {
                    if (field[x - 1][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
        }
        else if (currentDirection == 1)
        {
            // y - 1
            for (int i = x - 1; i < x + length; i++)
            {
                if (i < 10 && i >= 0 && y - 1 < 10 && y - 1 >= 0)
                {
                    if (field[i][y - 1] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - 1; i < y + 1; i++)
            {
                if (i < 10 && i >= 0 && x + length < 10 && x + length >= 0)
                {
                    if (field[x + length][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x; i <= x + length; i++)
            {
                if (i < 10 && i >= 0 && y + 1 < 10 && y + 1 >= 0)
                {
                    if (field[i][y + 1] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y; i < y + 2; i++)
            {
                if (i < 10 && i >= 0 && x - 1 < 10 && x - 1 >= 0)
                {
                    if (field[x - 1][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
        }

        else if (currentDirection == 2)
        {
            // y - 1
            for (int i = x - 1; i <= x + 1; i++)
            {
                if (i < 10 && i >= 0 && y - 1 < 10 && y - 1 >= 0)
                {
                    if (field[i][y - 1] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - 1; i <= y + length; i++)
            {
                if (i < 10 && i >= 0 && x + 1 < 10 && x + 1 >= 0)
                {
                    if (field[x + 1][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x - 1; i <= x + 1; i++)
            {
                if (i < 10 && i >= 0 && y + length < 10 && y + length >= 0)
                {
                    if (field[i][y + length] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y - 1; i <= y + length; i++)
            {
                if (i < 10 && i >= 0 && x - 1 < 10 && x - 1 >= 0)
                {
                    if (field[x - 1][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
        }
        else if (currentDirection == 3)
        {
            // y - 1
            for (int i = x - length; i <= x + 1; i++)
            {
                if (i < 10 && i >= 0 && y - 1 < 10 && y - 1 >= 0)
                {
                    if (field[i][y - 1] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - 1; i <= y + 1; i++)
            {
                if (i < 10 && i >= 0 && x + 1 < 10 && x + 1 >= 0)
                {
                    if (field[x + 1][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x - length; i <= x + 1; i++)
            {
                if (i < 10 && i >= 0 && y + 1 < 10 && y + 1 >= 0)
                {
                    if (field[i][y + 1] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y - 1; i <= y + 1; i++)
            {
                if (i < 10 && i >= 0 && x - length < 10 && x - length >= 0)
                {
                    if (field[x - length][i] == Square::SHIP)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void fillShip(std::vector<std::vector<Square>> &field, int &dir, int &x, int &y, int &length)
    {
        if (dir == 0)
        {
            for (int i = y - length + 1; i <= y; i++)
            {
                field[x][i] = Square::SHIP;
            }
        }
        else if (dir == 1)
        {
            for (int i = x; i <= x + length - 1; i++)
            {
                field[i][y] = Square::SHIP;
            }
        }
        else if (dir == 2)
        {
            for (int i = y; i <= y + length - 1; i++)
            {
                field[x][i] = Square::SHIP;
            }
        }
        else if (dir == 3)
        {
            for (int i = x - length + 1; i <= x; i++)
            {
                field[i][y] = Square::SHIP;
            }
        }
    }

    void fillingShips(std::vector<std::vector<Square>> &field, int &column, int &line, int &num)
    {
        for (int i = 0; i < num; i++)
        {
            std::vector<std::vector<int>> orderOfChecking =
                    {{0, 1, 2, 3},
                    {0, 1, 3, 2},
                    {0, 2, 1, 3},
                    {0, 2, 3, 1},
                    {0, 3, 1, 2},
                    {0, 3, 2, 1},
                    {1, 0, 2, 3},
                    {1, 0, 3, 2},
                    {1, 2, 0, 3},
                    {1, 2, 3, 0}};


            int curOrderNum = 4, orderNum, dir;
            orderNum = int(rand()) % 10;
            std::vector<int> order = orderOfChecking[orderNum];
            curOrderNum = 0;
            dir = order[curOrderNum];
            int length = 5 - num;
            while (!checkForShip(field, column, line, dir, length))
            {
                curOrderNum++;
                dir = order[curOrderNum];
            }
            fillShip(field, dir, column, line, length);
            std::cout << "Done\n";
        }
    }

    void fillIn(std::vector<std::vector<Square>> &field)
    {
        int x4ships = 1, x3ships = 2, x2ships = 3, x1ships = 4;
        int line = int(rand()) % 10, column = int(rand()) % 10;
        // filling x4 ship
        fillingShips(field, column, line, x4ships);
        // filling x3 ship
        fillingShips(field, column, line, x3ships);
        // filling x2 ship
        fillingShips(field, column, line, x2ships);
        // filling x1 ship
        fillingShips(field, column, line, x1ships);
    }

    std::string makeShot(std::vector<std::vector<Square>> &field) {
        std::vector<std::vector<std::vector<int>>> variant = {{{4, 1}, {3, 2}, {2, 3}, {1, 4},
                                                {8, 1}, {7, 2}, {6, 3}, {5, 4}, {4,  5}, {3, 6}, {2,  7}, {1, 8},
                                                {10, 3}, {9, 4}, {8, 5}, {7,  6}, {6, 7}, {5, 8}, {4,  9}, {3,  10},
                                                {10, 7}, {9, 8}, {8,  9}, {7, 10}},
                                               {{3, 1}, {2, 2}, {1, 3},
                                                {7, 1}, {6, 2}, {5, 3}, {4, 4}, {3, 5}, {2,  6}, {1, 7},
                                                {10, 2}, {9, 3}, {8,  4}, {7, 5}, {6, 6}, {5,  7}, {4, 8}, {3, 9}, {2,  10},
                                                {10, 6}, {9,  7}, {8, 8}, {7,  9}, {6, 10},
                                                {10, 10}},
                                               {{2, 1}, {1, 2},
                                                {6, 1}, {5, 2}, {4, 3}, {3, 4}, {2, 5}, {1, 6},
                                                {10, 1}, {9, 2}, {8,  3}, {7, 4}, {6,  5}, {5, 6}, {4, 7}, {3,  8}, {2, 9}, {1, 10},
                                                {10, 5}, {9,  6}, {8,  7}, {7, 8}, {6,  9}, {5, 10},
                                                {10, 9}, {9, 10}},
                                               {{1, 1},
                                                {5, 1}, {4, 2}, {3, 3}, {2, 4}, {1, 5},
                                                {9, 1}, {8, 2}, {7,  3}, {6, 4}, {5,  5}, {4, 6}, {3,  7}, {2, 8}, {1, 9},
                                                {10, 4}, {9, 5}, {8, 6}, {7,  7}, {6,  8}, {5,  9}, {4, 10},
                                                {10, 8}, {9, 9}, {8,  10}}};
        bool isBoom = true;
        while (isBoom)
        {
            int variantSize = 25, j;
            for (int t = 0; t < variantSize; t++)
            {
                j = int(rand()) % 25;
                while (variant[variantNo][j][0] == 0 && variant[variantNo][j][1] == 0)
                {
                    j = int(rand()) % 25;
                }
            }
            int shootX = variant[variantNo][j][0], shootY = variant[variantNo][j][1];
            if (field[shootX][shootY] == Square::SHIP)
            {
                field[shootX][shootY] = Square::SHOT;
                playerBrokenShips++;
            }
            else if (field[shootX][shootY] == Square::EMPTY)
            {
                field[shootX][shootY] = Square::AWAY;
                isBoom = false;
            }
            if (field[shootX][shootY] == Square::DEAD || field[shootX][shootY] == Square::SHOT) {
                return "You have already shot this square. Try again.\n";
            }
        }
        return "Succesfully.\n";
    }

    void win()
    {
        std::cout << "\n\nB O T   W O N !\n";
        std::cout << "/t/tB O T   W O N !\n";
        std::cout << "/t/tB O T   W O N !\n";
    }
};


class Player
{

public:

    std::string makeShot(std::vector<std::vector<Square>> &field, int &x, int &y, bool &hit)
    {
        if (field[x][y] == Square::EMPTY)
        {
            field[x][y] = Square::AWAY;
            return "Done.\n";
        }
        if (field[x][y] == Square::SHOT || field[x][y] == Square::DEAD || field[x][y] == Square::AWAY)
        {
            return "You already shot this square. Try again.\n";
        }
        if (field[x][y] == Square::SHIP)
        {
            hit = true;
            if (field[x - 1][y] != Square::SHIP && field[x + 1][y] != Square::SHIP && field[x][y - 1] != Square::SHIP && field[x][y + 1] != Square::SHIP) {
                field[x][y] = Square::DEAD;
            }
            else {
                field[x][y] = Square::SHOT;
            }
            return "Done.\n";
        }
        return "Unknown result.\n";
    }

    void win()
    {
        std::cout << "\n\n/t/tY O U   W O N !\n";
        std::cout << "/t/tY O U   W O N !\n";
        std::cout << "/t/tY O U   W O N !\n";
    }
};


void SeaBattle()
{
    Communication communication;
    communication.greeting();
    std::cin >> communication.cin;
    if (communication.cin == "cmd")
    {
        communication.commands();
    }
    Player player;
    Bot bot;
    Field playerField;
    Field botField;
    bool gameOver = false;
    communication.readyForFilling();
    std::string cmdType;
    std::cin >> cmdType;
    if (cmdType == "fill")
    {
        std::cin >> cmdType;
        playerField.fillByCoords(cmdType, playerField);
    }
    std::cout << "You set your ships, it's time to play! If you are ready to shoot, type in [ shoot ]\n";
    while (!gameOver)
    {
        bot.fillIn(bot.field);
        std::cin >> cmdType;
        hit = false;
        if (cmdType == "shoot")
        {
            player.makeShot(bot.field, bot.field, hit);
        }
        if (hit)
        {
            std::cout << "Nice try!\n";
        } else
        {
            std::cout << "Good luck next time :(\n";
        }
        std::cout << "After your shoot:\n";
        std::cout << "\tPlayer\t\t\t  Bot\n";
        printFields(player.field, bot.field);
        while (hit)
        {
            hit = false;
            player.makeShot(bot.field, bot.field, hit);
            if (hit)
            {
                std::cout << "Nice shot!\n";
            } else
            {
                std::cout << "Better luck next time :(\n";
            }
            std::cout << "After your shoot:\n";
            std::cout << "\tPlayer\t\t\t  Bot\n";
            printFields(player.field, bot.field);
            if (playerBrokenShips == 20)
            {
                player.win();
                gameOver = true;
                return;
            }
        }
        bot.makeShot(player.field);
        if (botBrokenShips == 20)
        {
            bot.win();
            gameOver = true;
            return;
        }
    }
}


int main()
{
    SeaBattle();
}