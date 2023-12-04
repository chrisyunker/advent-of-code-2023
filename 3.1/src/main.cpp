#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>

bool isNum(const char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}

bool isSymbol(const char c)
{
    if (c >= 48 && c <= 57)
    {
        return false;
    }
    else if (c == '.')
    {
        return false;
    }
    return true;
}

bool isValidNum(const std::vector<std::string>& board, int bx, int by, int x, int y)
{
    if (x > 0 && y > 0 && isSymbol(board[y-1][x-1])) return true;
    if (x < bx-2 && y > 0 && isSymbol(board[y-1][x+1])) return true;
    if (x < bx-2 && y < by-1 && isSymbol(board[y+1][x+1])) return true;
    if (x > 0 && y < by-1 && isSymbol(board[y+1][x-1])) return true;
    if (x > 0 && isSymbol(board[y][x-1])) return true;
    if (x < bx-2 && isSymbol(board[y][x+1])) return true;
    if (y > 0 && isSymbol(board[y-1][x])) return true;
    if (y < by-2 && isSymbol(board[y+1][x])) return true;
    return false;
}

int main(int argc, char *argv[])
{
    const int numArgs = 1;

    if ((argc - optind) < numArgs)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    std::string input(argv[optind]);

    // Load input file
    printf("Input file: %s\n", input.c_str());
    std::vector<std::string> board;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        board.push_back(line);
    }
    int by = board.size();
    int bx = board[0].size();
    printf("Board size: %d x %d\n", bx, by);


    // Print board
    printf("\n");
    for (int y=0; y < by; y++)
    {
        for (int x=0; x < bx; x++)
        {
            printf("%c", board[y][x]);
        }
        printf("\n");
    }
    printf("\n");


    std::vector<int> validNums;

    for (int y=0; y < by; y++)
    {
        std::string num = "";
        bool validNum = false;
        for (int x=0; x < bx; x++)
        {
            if (isNum(board[y][x]))
            {
                if (!validNum)
                {
                    validNum = isValidNum(board, bx, by, x, y);
                }
                num += board[y][x];
            }
            else
            {
                if (num.size() > 0)
                {
                    if (validNum)
                    {
                        validNums.push_back(stoi(num));
                    }
                    num = "";
                    validNum = false;
                }
            }
        }
        if (num.size() > 0)
        {
            if (validNum)
            {
                validNums.push_back(stoi(num));
            }
            num = "";
            validNum = false;
        }
    }

 
    int sum = 0;
    for (auto num : validNums)
    {
        sum += num;
    }

    printf("Sum: %d\n", sum);

    return 0;
}
