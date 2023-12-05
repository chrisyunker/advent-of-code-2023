#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>


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


    std::map<int, int> copies;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        size_t pos, next;
        pos = line.find_first_of(":", 5);
        int id = std::stoi(line.substr(5, pos - 5));
        int matching = 0;
        pos += 2;

        std::map<int, int> winningNums;

        while (line[pos] != '|')
        {
            if (line[pos] == ' ')
            {
                pos += 1;
                continue;
            }
            next = line.find_first_of(" ", pos);
            int n = std::stoi(line.substr(pos, next - pos));
            pos = next + 1;
            winningNums[n] = 1;
        }
        pos += 2;
        while (pos < line.size())
        {
            if (line[pos] == ' ')
            {
                pos += 1;
                continue;
            }
            next = line.find_first_of(" ", pos);
            if (next == std::string::npos)
            {
                next = line.size();
            }
            int n = std::stoi(line.substr(pos, next - pos));
            pos = next + 1;
            if (winningNums.contains(n))
            {
                matching += 1;
            }
        }
        for (int i = id+1; i < id+1+matching; i++)
        {
            copies[i] += (copies[id] + 1);
        }

        printf("card: %d score: %d\n", id, copies[id]);
    }

    int total = 0;
    for (auto [_k, v] : copies)
    {
        total += (v+1);
    }
    
    printf("total: %d\n", total);

    return 0;
}
