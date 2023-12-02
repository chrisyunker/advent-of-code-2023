#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <map>

char checkDigitForward(const std::string& line, int i)
{
    if (line.size() >= i+5)
    {
        if (line.substr(i, 5) == "three")
        {
            return '3';
        }
        else if (line.substr(i, 5) == "seven")
        {
            return '7';
        }
        else if (line.substr(i, 5) == "eight")
        {
            return '8';
        }
    }

    if (line.size() >= i+4)
    {
        if (line.substr(i, 4) == "four")
        {
            return '4';
        }
        else if (line.substr(i, 4) == "five")
        {
            return '5';
        }
        else if (line.substr(i, 4) == "nine")
        {
            return '9';
        }
    }

    if (line.size() >= i+3)
    {
        if (line.substr(i, 3) == "one")
        {
            return '1';
        }
        else if (line.substr(i, 3) == "two")
        {
            return '2';
        }
        else if (line.substr(i, 3) == "six")
        {
            return '6';
        }
    }

    return 0;
}

char checkDigitBackward(const std::string& line, int i)
{
    if (i-4 >= 0)
    {
        if (line.substr(i-4, 5) == "three")
        {
            return '3';
        }
        else if (line.substr(i-4, 5) == "seven")
        {
            return '7';
        }
        else if (line.substr(i-4, 5) == "eight")
        {
            return '8';
        }
    }

    if (i-3 >= 0)
    {
        if (line.substr(i-3, 4) == "four")
        {
            return '4';
        }
        else if (line.substr(i-3, 4) == "five")
        {
            return '5';
        }
        else if (line.substr(i-3, 4) == "nine")
        {
            return '9';
        }
    }

    if (i-2 >= 0)
    {
        if (line.substr(i-2, 3) == "one")
        {
            return '1';
        }
        else if (line.substr(i-2, 3) == "two")
        {
            return '2';
        }
        else if (line.substr(i-2, 3) == "six")
        {
            return '6';
        }
    }

    return 0;
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
    int sum = 0;

    // Load input file
    printf("Input file: %s\n", input.c_str());

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        char first = 0;
        for (int i = 0; i < line.size(); i++)
        {
            if (line.at(i) >= 48 && line.at(i) <= 57)
            {
                first = line.at(i);
            }
            else
            {
                first = checkDigitForward(line, i);
            }

            if (first > 0) break;
        }

        char last = 0;
        for (int i = line.size() - 1; i >= 0; i--)
        {
            if (line.at(i) >= 48 && line.at(i) <= 57)
            {
                last = line.at(i);
            }
            else
            {
                last = checkDigitBackward(line, i);
            }

            if (last > 0) break;
        }
        std::string total = std::string{first, last};

        printf("value: %s\n", total.c_str());
        sum += stoi(total);
    }

    printf("total: %d\n", sum);

    return 0;
}
