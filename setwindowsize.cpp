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

// Bảng màu mới (2 cái đầu là 2 màu của cái board)
string color[18] = {"d3d3d3", "5a5a5a", "39777c", "edbb99", "2ecc71", "f7dc6f", "e6b0aa", "6600ff", "e5e8e8", "e74c3c", "80af21", "808080", "af5f00", "005fff", "ff5fd7", "e4e4e4", "ffd700", "00af00"};

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