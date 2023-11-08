#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>

using namespace std;

string alphas = "ABCDEFGHIJ";
string digits = "1234567890";
const int fieldSize = 10;
bool hit;



class Bot
{

public:

    int brokenShips = 0;
    vector<vector<char>> fieldIn = {
        {'#', '#', '#', '.', '.', '#', '#', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'#', '.', '.', '.', '#', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '#', '#', '#', '#'},
        {'.', '#', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '#', '.', '.', '.', '#', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '#', '.', '.', '.', '.'},
        {'#', '.', '.', '.', '.', '#', '.', '.', '.', '.'},
        {'.', '.', '#', '#', '.', '.', '.', '#', '.', '.'},
        };
    vector<vector<char>> fieldOut(fieldSize, vector<char> (fieldSize, '~'));

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

    void makeShot() {
        //to be continued...
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

    int brokenShips = 0;
    vector<vector<char>> field(fieldSize, vector<char> (fieldSize, '.'));

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


bool isFree = false;

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


int main() {
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
    while(!gameOver) {
        string cmdType;
        cin >> cmdType;
        cmdType = toSimpleDown(cmdType);
        if (cmdType == "fill") {
            player.fillIn();
            continue;
        }
        hit = false;
        if (cmdType == "shoot") {
            player.makeShot(bot.fieldIn, bot.fieldOut, hit);
        }
        if (hit) {
            cout << "Nice try!\n";
        }
        else {
            cout << "Good luck next time :(\n";
        }
        cout << "After your shoot:\n";
        cout << "\tPlayer\t\t\t  Bot\n";
        printFields(player.field, bot.fieldOut);
        while (hit) {
            hit = false;
            player.makeShot(bot.fieldIn, bot.fieldOut, hit);
            if (hit) {
                cout << "Nice try!\n";
            }
            else {
                cout << "Good luck next time :(\n";
            }
            cout << "After your shoot:\n";
            cout << "\tPlayer\t\t\t  Bot\n";
            printFields(player.field, bot.fieldOut);
            if (player.brokenShips == 20) {
                player.win();
                return 0;
            }
        }
        bot.makeShot();

        // to be continued...

        if (bot.brokenShips == 20) {
            bot.win();
            return 0;
        }
    }
}

