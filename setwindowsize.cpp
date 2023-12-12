// Bỏ lệnh này
SetConsoleOutputCP(65001);

//Hàm highlight
string Highlight(string color, string text)
{
    int r = stoi(color.substr(0, 2), nullptr, 16);
    int g = stoi(color.substr(2, 2), nullptr, 16);
    int b = stoi(color.substr(4, 2), nullptr, 16);
    
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + text + "\033[0m";
}

// Bảng màu mới (2 cái cuối là 2 màu của cái board)
string color[9] = {"fff100", "f6921e", "8bc53f", "ec008b", "ffc000", "00adee", "ec1b24", "d3d3d3", "5a5a5a"};
                  {O, I, T, S, Z, J, L}
// Hàm vẽ mới
void drawBlock(int index, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << Highlight(color[index], "   ");
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color[index], "   ");
}

void drawEmptyCell(int index, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << Highlight(color[index], "   ");
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color[index], "   ");
}

// Hàm display mới của bảng cho nó xen kẽ
void display() const
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (grid[i][j] == '0')
                drawBlock(6, i, j);
            else
                drawEmptyCell((i + j) % 2, i, j);
        }
    }
}

//Display cho từng ô của tetromino (cái số trong drawBlock khác 0 với 1 là dc)
void display()
{
    for (int i = 0; i < blocks[state].size(); i++)
    {
        int u = pos.first + blocks[state][i].first;
        int v = pos.second + blocks[state][i].second;
        drawBlock(6, u, v);
    }
};