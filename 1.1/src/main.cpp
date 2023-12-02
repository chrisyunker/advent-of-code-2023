#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>

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
                break;
            }
        }

        char last = 0;
        for (int i = line.size() - 1; i >= 0; i--)
        {
            if (line.at(i) >= 48 && line.at(i) <= 57)
            {
                last = line.at(i);
                break;
            }
        }
        std::string total = std::string{first, last};

        printf("value: %s\n", total.c_str());
        sum += stoi(total);
    }

    printf("total: %d\n", sum);

    return 0;
}
