#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>

using namespace std;

string alphas = "ABCDEFGHIJ";
string digits = "1234567890";

int playerBrokenShipCnt = 0, botBrokenShipCnt = 0;
bool isFree = false;
const int fieldSize = fieldSize;
// Создание пустого поля для игрока
vector<vector<char>> playerField(fieldSize, vector<char> (fieldSize, '~'));
vector<vector<char>> playerFieldLooking(fieldSize, vector<char> (fieldSize, '~'));
// Создание пустого поля для бота для просмотра
vector<vector<char>> botFieldForLooking(fieldSize, vector<char> (fieldSize, '~'));
// Создание фактического поля для бота
vector<vector<char>> botField = {
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

void printField(vector<vector<char>> a) {
    cout << "   ";
    for (int i = 0; i < fieldSize; i++) {
        cout << alphas[i] << ' ';
    }
    cout << '\n';
    for (int i = 0; i < a.size(); i++) {
        if (i != 9) {
            cout << ' ';
        }
        cout << i + 1 << ' ';
        for (int j = 0; j < a[i].size(); j++) {
            cout << a[i][j] << ' ';
        }
        cout << '\n';
    }
}


void fillingPlayerField(vector<vector<char>> a) {

}


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
            _sleep(7);
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
}


void greeting() {
    cout << "Hello! If you want to know how to play - type in [ commands ]\n";
    cout << "If not - type in [ play ]\n";
}


void commands() {
    cout << "Types of commands:\n";
    _sleep(150);
    cout << "fill - if you want to fill your field (format: [ x1y1 x2y2 ] (coordinates of each ship))\n";
    _sleep(150);
    cout << "shoot - if you want to shoot any coordinate in bot field (format: [ xy ] (coordinates of each ship))\n";
    _sleep(150);
    cout << "After every command press [ ENTER ]\n";
    _sleep(150);
    cout << "Now, use these rules to fill your field in with ships!\n";
}


string toSimpleUp(string a) {
    for (int i = 0; i < a.size(); i++) {
        a[i] = toupper(a[i]);
    }
    return a;
}


string toSimpleDown(string a) {
    for (int i = 0; i < a.size(); i++) {
        a[i] = tolower(a[i]);
    }
    return a;
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
            playerField[x1][i] = '#';
        }
    }
    else if (y1 == y2) {
        for (int i = x1; i < x2 + 1; i++) {
            playerField[i][y1] = '#';
        }
    }
}


void playerMakeShot() {
    string xy;
    cin >> xy;
    xy[0] = toupper(xy[0]);
    int x = int(xy[0]) - 65;
    int y = int(xy[1]) - '0' - 1;
    if (botFieldForLooking[x][y] == '~') {
        if (botField[x][y] == '#') {
            botFieldForLooking[x][y] = 'X';
            botField[x][y] = 'X';
        }
    }
}


void botMakeShot() {
    int column = rand() % fieldSize;
    int line = rand() % fieldSize;
    if (playerFieldLooking[column][line] == '~') {
        if (playerField[column][line] == '#') {
            playerFieldLooking[column][line] = 'X';
            playerField[column][line] = 'X';
            playerBrokenShipCnt++;
        }
        else {
            playerFieldLooking[column][line] = '.';
        }
        isFree = true;
    }
}


void botWin() {
    cout << "\n\nB O T   W O N !\n";
    cout << "/t/tB O T   W O N !\n";
    cout << "/t/tB O T   W O N !\n";
    _sleep(3000);
}


void playerWin() {
    cout << "\n\n/t/tY O U   W O N !\n";
    cout << "/t/tY O U   W O N !\n";
    cout << "/t/tY O U   W O N !\n";
    _sleep(3000);
}


int main() {
    greeting();
    string command;
    cin >> command;
    command = toSimpleDown(command);
    if (command == "commands") {
        commands();
    }
    bool gameOver = false;
    while(!gameOver) {
        string cmdType;
        cin >> cmdType;
        cmdType = toSimpleDown(cmdType);
        if (cmdType == "fill") {
            fillIn();
        }
        if (cmdType == "shoot") {
            playerMakeShot();
        }
        cout << "After your shoot:\n";
        cout << "\tPlayer\t\t\t  Bot\n";
        printFields(playerField, botField);
        while (!isFree) {
            botMakeShot();
        }
        if (playerBrokenShipCnt == 20) {
            botWin();
            return 0;
        }
        if (botBrokenShipCnt == 20) {
            playerWin();
            return 0;
        }
    }
}

