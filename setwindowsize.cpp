//Thêm cái này vô main
SetConsoleOutputCP(65001);

//Hàm vẽ mới
void drawBlock(int index, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << SetColor(color[index], "\u250c\u2500\u2510");
    GoTo(Y * 2 + 1, X * 3);
    cout << SetColor(color[index], "\u2514\u2500\u2518");
}

void drawEmptyCell(int index, int Y, int X)
{
    GoTo(Y * 2 + 1, X * 3 + 1);
    cout << SetColor(color[index], ".");
}