#include <bits/stdc++.h>

using namespace std;

string alphas = "ABCDEFGHIJ";
string digits = "1234567890";
int playerBrokenShips = 0;
int botBrokenShips = 0;
const int fieldSize = 10;
const int variantNo = int(rand()) % 4;
bool hit;


struct xy {
    int x, y;
};



void printFields(vector<vector<char>> a, vector<vector<char>> b) {
    cout << "   ";
    for (int i = 0; i < fieldSize; i++) {
        cout << alphas[i] << ' ';
    }
    cout << "\t";
    cout << "   ";
    for (int i = 0; i < fieldSize; i++) {
        cout << alphas[i] << ' ';
    }
    cout << '\n';
    for (int i = 0; i < fieldSize; i++) {
        if (i != 9) {
            cout << ' ';
        }
        cout << i + 1 << ' ';
        for (int j = 0; j < fieldSize; j++) {
            cout << a[i][j] << ' ';
        }
        cout << '\t';
        if (i != 9) {
            cout << ' ';
        }
        cout << i + 1 << ' ';
        for (int j = 0; j < fieldSize; j++) {
            cout << b[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}


string makeMonotonous(string a, string type) {
    for (int i = 0; i < type.size(); i++) {
        type[i] = toupper(type[i]);
    }
    if (type == "UP") {
        for (int i = 0; i < a.size(); i++) {
            a[i] = toupper(a[i]);
        }
        return a;
    }
    else if (type == "DOWN") {
        for (int i = 0; i < a.size(); i++) {
            a[i] = tolower(a[i]);
        }
        return a;
    }
}


class Communication
{
public:

    void greeting() {
        cout << "Hello! If you want to know how to play - type in [ commands ]\n";
        cout << "If not - type in [ play ]\n";
    }
    string cin;
    void commands() {
        cout << "Types of commands:\n";
        cout << "fill - if you want to fill your field (format: [ x1y1 x2y2 ] (coordinates of each ship))\n";
        cout << "shoot - if you want to shoot any coordinate in bot field (format: [ xy ] (coordinates of each ship))\n";
        cout << "After every command press [ ENTER ]\n";
        cout << "Now, use these rules to fill your field in with ships!\n";
        cout << "If you understood everything - type in [ play ]\n";
    }
};


class Bot
{

public:

    vector<vector<char>> fieldIn = {
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    };
    vector<vector<char>> fieldOut = {
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
            {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
    };

    void coutField() {
        cout << "   ";
        for (int i = 0; i < fieldSize; i++) {
            cout << alphas[i] << ' ';
        }
        cout << '\n';
        for (int i = 0; i < fieldSize; i++) {
            if (i != 9) {
                cout << ' ';
            }
            cout << i + 1 << ' ';
            for (int j = 0; j < fieldSize; j++) {
                cout << fieldOut[i][j] << ' ';
            }
            cout << '\n';
        }
    }


    bool checkForShip(vector<vector<char>> &fieldIn, int x, int y, int currentDirection, int length) {

        // checking for space under potential ship
        if (currentDirection == 0) {
            if (y < length - 1) {
                return false;
            }
        }
        if (currentDirection == 1) {
            if (x > 10 - length) {
                return false;
            }
        }
        if (currentDirection == 2) {
            if (y > 10 - length) {
                return false;
            }
        }
        if (currentDirection == 3) {
            if (y < length - 1) {
                return false;
            }
        }

        // checking for space squares under potential ship
        if (currentDirection == 0) {
            for (int i = x; i < x + length; i++) {
                if (fieldIn[i][y] == '#') {
                    return false;
                }
            }
        }
        if (currentDirection == 1) {
            for (int i = y; i < y + length; i++) {
                if (fieldIn[x][i] == '#') {
                    return false;
                }
            }
        }
        if (currentDirection == 2) {
            for (int i = x - length + 1; i <= x; i++) {
                if (fieldIn[i][y] == '#') {
                    return false;
                }
            }
        }
        if (currentDirection == 3) {
            for (int i = y - length + 1; i <= y; i++) {
                if (fieldIn[x][i] == '#') {
                    return false;
                }
            }
        }


        // checking for space squares near potential ship (lying not for more than one square out of it)
        if (currentDirection == 0) {
            // y - 1
            for (int i = x - 1; i <= x + 1; i++) {
                if (i < 10 && i >= 0 && y - length < 10 && y - length >= 0) {
                    if (fieldIn[i][y - length] == '#') {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - length; i <= y + 1; i++) {
                if (i < 10 && i >= 0 && x + 1 < 10 && x + 1 >= 0) {
                    if (fieldIn[x + 1][i] == '#') {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x - 1; i <= x + 1; i++) {
                if (i < 10 && i >= 0 && y + 1 < 10 && y + 1 >= 0) {
                    if (fieldIn[i][y + 1] == '#') {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y - length; i <= y + 1; i++) {
                if (i < 10 && i >= 0 && x - 1 < 10 && x - 1 >= 0) {
                    if (fieldIn[x - 1][i] == '#') {
                        return false;
                    }
                }
            }
        }
        else if (currentDirection == 1) {
            // y - 1
            for (int i = x - 1; i < x + length; i++) {
                if (i < 10 && i >= 0 && y - 1 < 10 && y - 1 >= 0) {
                    if (fieldIn[i][y - 1] == '#') {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - 1; i < y + 1; i++) {
                if (i < 10 && i >= 0 && x + length < 10 && x + length >= 0) {
                    if (fieldIn[x + length][i] == '#') {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x; i <= x + length; i++) {
                if (i < 10 && i >= 0 && y + 1 < 10 && y + 1 >= 0) {
                    if (fieldIn[i][y + 1] == '#') {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y; i < y + 2; i++) {
                if (i < 10 && i >= 0 && x - 1 < 10 && x - 1 >= 0) {
                    if (fieldIn[x - 1][i] == '#') {
                        return false;
                    }
                }
            }
        }

        else if (currentDirection == 2) {
            // y - 1
            for (int i = x - 1; i <= x + 1; i++) {
                if (i < 10 && i >= 0 && y - 1 < 10 && y - 1 >= 0) {
                    if (fieldIn[i][y - 1] == '#') {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - 1; i <= y + length; i++) {
                if (i < 10 && i >= 0 && x + 1 < 10 && x + 1 >= 0) {
                    if (fieldIn[x + 1][i] == '#') {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x - 1; i <= x + 1; i++) {
                if (i < 10 && i >= 0 && y + length < 10 && y + length >= 0) {
                    if (fieldIn[i][y + length] == '#') {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y - 1; i <= y + length; i++) {
                if (i < 10 && i >= 0 && x - 1 < 10 && x - 1 >= 0) {
                    if (fieldIn[x - 1][i] == '#') {
                        return false;
                    }
                }
            }
        }
        else if (currentDirection == 3) {
            // y - 1
            for (int i = x - length; i <= x + 1; i++) {
                if (i < 10 && i >= 0 && y - 1 < 10 && y - 1 >= 0) {
                    if (fieldIn[i][y - 1] == '#') {
                        return false;
                    }
                }
            }
            // x + 1
            for (int i = y - 1; i <= y + 1; i++) {
                if (i < 10 && i >= 0 && x + 1 < 10 && x + 1 >= 0) {
                    if (fieldIn[x + 1][i] == '#') {
                        return false;
                    }
                }
            }
            // y + 1
            for (int i = x - length; i <= x + 1; i++) {
                if (i < 10 && i >= 0 && y + 1 < 10 && y + 1 >= 0) {
                    if (fieldIn[i][y + 1] == '#') {
                        return false;
                    }
                }
            }
            // x - 1
            for (int i = y - 1; i <= y + 1; i++) {
                if (i < 10 && i >= 0 && x - length < 10 && x - length >= 0) {
                    if (fieldIn[x - length][i] == '#') {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    void fillShip(vector<vector<char>> &fieldIn, int dir, int x, int y, int length) {
        if (dir == 0) {
            for (int i = y - length + 1; i <= y; i++) {
                fieldIn[x][i] = '#';
            }
        }
        else if (dir == 1) {
            for (int i = x; i <= x + length - 1; i++) {
                fieldIn[i][y] = '#';
            }
        }
        else if (dir == 2) {
            for (int i = y; i <= y + length - 1; i++) {
                fieldIn[x][i] = '#';
            }
        }
        else if (dir == 3) {
            for (int i = x - length + 1; i <= x; i++) {
                fieldIn[i][y] = '#';
            }
        }
    }
    void fillingShips(vector<vector<char>> &fieldIn, int column, int line, int num) {
        for (int i = 0; i < num; i++) {
            vector<vector<int>> orderOfChecking = {{0, 1, 2, 3},
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
            vector<int> order = orderOfChecking[orderNum];
            curOrderNum = 0;
            dir = order[curOrderNum];
            while (!checkForShip(fieldIn, column, line, dir, 5 - num)) {
                curOrderNum++;
                dir = order[curOrderNum];
            }
            fillShip(fieldIn, dir, column, line, 5 - num);
            cout << "Done\n";
        }
    }

    void fillIn(vector<vector<char>> &fieldIn) {
        int x4ships = 1, x3ships = 2, x2ships = 3, x1ships = 4;
        int line = int(rand()) % 10, column = int(rand()) % 10;
        // filling x4 ship
        fillingShips(fieldIn, column, line, x4ships);
        // filling x3 ship
        fillingShips(fieldIn, column, line, x3ships);
        // filling x2 ship
        fillingShips(fieldIn, column, line, x2ships);
        // filling x1 ship
        fillingShips(fieldIn, column, line, x1ships);
    }

    void makeShot(vector<vector<char>> &field) {
        vector<vector<vector<int>>> variant = {{{4, 1}, {3, 2}, {2, 3}, {1, 4},
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
        while (isBoom) {
            int variantSize = 25, j;
            for (int t = 0; t < variantSize; t++) {
                j = int(rand()) % 25;
                while (variant[variantNo][j][0] == 0 && variant[variantNo][j][1] == 0) {
                    j = int(rand()) % 25;
                }
            }
            int shootX = variant[variantNo][j][0], shootY = variant[variantNo][j][1];
            if (field[shootX][shootY] == '#') {
                field[shootX][shootY] = 'X';
                playerBrokenShips++;
            }
            else if (field[shootX][shootY] == '.') {
                field[shootX][shootY] = '-';
                isBoom = false;
            }

        }
    }

    void win() {
        cout << "\n\nB O T   W O N !\n";
        cout << "/t/tB O T   W O N !\n";
        cout << "/t/tB O T   W O N !\n";
    }
};


class Player
{

public:


    vector<vector<char>> field = {
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    };

    void coutField() {
        cout << "   ";
        for (int i = 0; i < fieldSize; i++) {
            cout << alphas[i] << ' ';
        }
        cout << '\n';
        for (int i = 0; i < fieldSize; i++) {
            if (i != 9) {
                cout << ' ';
            }
            cout << i + 1 << ' ';
            for (int j = 0; j < fieldSize; j++) {
                cout << field[i][j] << ' ';
            }
            cout << '\n';
        }
    }

    void fillIn() {
        string x1y1;
        cin >> x1y1;
        x1y1[0] = toupper(x1y1[0]);
        int x1 = int(x1y1[0]) - 65;
        int y1 = int(x1y1[1]) - '0' - 1;
        string x2y2;
        cin >> x2y2;
        x2y2[0] = toupper(x2y2[0]);
        int x2 = int(x2y2[0]) - 65;
        int y2 = int(x2y2[1]) - '0' - 1;
        if (x1 == x2) {
            for (int i = y1; i < y2 + 1; i++) {
                field[x1][i] = '#';
            }
        }
        else if (y1 == y2) {
            for (int i = x1; i < x2 + 1; i++) {
                field[i][y1] = '#';
            }
        }
        cout << "Done!\n";
    }

    void makeShot(vector<vector<char>> botFieldIn, vector<vector<char>> botFieldOut, bool hit) {
        string xy;
        cin >> xy;
        xy[0] = toupper(xy[0]);
        int x = int(xy[0]) - 65;
        int y = int(xy[1]) - '0' - 1;
        if (botFieldOut[x][y] == '~') {
            if (botFieldIn[x][y] == '#') {
                botFieldOut[x][y] = 'X';
                botFieldIn[x][y] = 'X';
                botBrokenShips++;
                hit = true;
            }
            else if (botFieldIn[x][y] == '.') {
                botFieldOut[x][y] = '-';
                botFieldIn[x][y] = '-';
                hit = false;
            }
        }
        else {
            cout << "You already shoot this square!\n";
        }
    }

    void win() {
        cout << "\n\n/t/tY O U   W O N !\n";
        cout << "/t/tY O U   W O N !\n";
        cout << "/t/tY O U   W O N !\n";
    }
};


void SeaBattle(string version) {
    if (version == "alphaVersion") {
        Communication communication;
        communication.greeting();
        cin >> communication.cin;
        if (communication.cin != "play") {
            communication.commands();
            cin >> communication.cin;
        }
        Player player;
        Bot bot;
        bool gameOver = false;
        string cmdType;
        cin >> cmdType;
        if (cmdType == "fill") {
            for (int i = 0; i < 1; i++) {
                player.fillIn();
            }
        }
        cout << "You set your ships, it's time to play! If you are ready to shoot, type in [ shoot ]\n";
        while (!gameOver) {
            bot.fillIn(bot.fieldIn);
            cin >> cmdType;
            cmdType = makeMonotonous(cmdType, "DOWN");
            hit = false;
            if (cmdType == "shoot") {
                player.makeShot(bot.fieldIn, bot.fieldOut, hit);
            }
            if (hit) {
                cout << "Nice try!\n";
            } else {
                cout << "Good luck next time :(\n";
            }
            cout << "After your shoot:\n";
            cout << "\tPlayer\t\t\t  Bot\n";
            printFields(player.field, bot.fieldOut);
            while (hit) {
                hit = false;
                player.makeShot(bot.fieldIn, bot.fieldOut, hit);
                if (hit) {
                    cout << "Nice shot!\n";
                } else {
                    cout << "Better luck next time :(\n";
                }
                cout << "After your shoot:\n";
                cout << "\tPlayer\t\t\t  Bot\n";
                printFields(player.field, bot.fieldOut);
                if (playerBrokenShips == 20) {
                    player.win();
                    gameOver = true;
                    return;
                }
            }
            bot.makeShot(player.field);
            if (botBrokenShips == 20) {
                bot.win();
                gameOver = true;
                return;
            }
        }
    }
}


int main() {
    SeaBattle("alphaVersion");
}