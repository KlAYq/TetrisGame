//Bỏ lệnh này
SetConsoleOutputCP(65001);

//Bảng màu mới (2 cái đầu là 2 màu của cái board)
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

//Hàm display mới của bảng cho nó xen kẽ
void display() const
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (grid[i][j] == '0')
                drawBlock(5, i, j);
            else
                drawEmptyCell((i + j) % 2, i, j);
        }
    }
}