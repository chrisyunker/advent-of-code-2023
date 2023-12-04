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

int extractNumber(const std::vector<std::string>& board, int bx, int by, int x, int y)
{
    std::string numStr = { board[y][x] };

    for (int i = x-1; i >=0; i--)
    {
        if (isNum(board[y][i]))
        {
            numStr = board[y][i] + numStr;
        }
        else
        {
            break;
        }
    }
    for (int i = x+1; i < bx; i++)
    {
        if (isNum(board[y][i]))
        {
            numStr = numStr + board[y][i];
        }
        else
        {
            break;
        }
    }

    return stoi(numStr);
}

long getGearRatios(const std::vector<std::string>& board, int bx, int by, int x, int y)
{
    std::vector<int> nums;

    if (x > 0 && isNum(board[y][x-1]))
    {
        nums.push_back(extractNumber(board, bx, by, x-1, y));
    }
    if (x < bx-2 && isNum(board[y][x+1]))
    {
        nums.push_back(extractNumber(board, bx, by, x+1, y));
    }
    if (y > 0)
    {
        if (isNum(board[y-1][x]))
        {
            nums.push_back(extractNumber(board, bx, by, x, y-1));
        }
        else
        {
            if (x > 0 && isNum(board[y-1][x-1]))
            {
                nums.push_back(extractNumber(board, bx, by, x-1, y-1));
            }
            if (x < bx-1 && isNum(board[y-1][x+1]))
            {
                nums.push_back(extractNumber(board, bx, by, x+1, y-1));
            }
        }
    }
    if (y < by-1)
    {
        if (isNum(board[y+1][x]))
        {
            nums.push_back(extractNumber(board, bx, by, x, y+1));
        }
        else
        {
            if (x > 0 && isNum(board[y+1][x-1]))
            {
                nums.push_back(extractNumber(board, bx, by, x-1, y+1));
            }
            if (x < bx-1 && isNum(board[y+1][x+1]))
            {
                nums.push_back(extractNumber(board, bx, by, x+1, y+1));
            }
        }
    }

    if (nums.size() == 2)
    {
        printf("Found 2 nums: %d, %d\n", nums[0], nums[1]);
        return nums[0] * nums[1];
    }
    else
    {
        return 0;
    }
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

    long sum = 0;
    for (int y=0; y < by; y++)
    {
        std::string num = "";
        for (int x=0; x < bx; x++)
        {
            if (board[y][x] == '*')
            {
                sum += getGearRatios(board, bx, by, x, y);
            }
        }
    }

    printf("Sum: %ld\n", sum);

    return 0;
}
