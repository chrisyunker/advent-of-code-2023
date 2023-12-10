#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>

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

    std::ifstream fd(input);
    std::string line;
    std::getline(fd, line);

    std::vector<uint64_t> times;
    std::vector<uint64_t> dists;

    size_t start;
    size_t end = 5;
    while ((start = line.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = line.find(' ', start);
        times.push_back(stoll(line.substr(start, end - start)));  
    }
    printf("times: ");
    for (auto t : times)
    {
        printf("%llu ", t);
    }
    printf("\n");

    std::getline(fd, line);
    end = 10;
    while ((start = line.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = line.find(' ', start);
        dists.push_back(stoll(line.substr(start, end - start)));  
    }
    printf("dists: ");
    for (auto d : dists)
    {
        printf("%llu ", d);
    }
    printf("\n");

    std::vector<int> numberOfWins;
    for (int i=0; i < times.size(); i++)
    {
        std::vector<uint64_t> wins;

        uint64_t max = dists[i];

        for (uint64_t v=0; v < times[i]; v++)
        {
            uint64_t mt = times[i] - v;

            uint64_t d = mt * v;

            if (d > max) {
                wins.push_back(v);
            }
        }

        numberOfWins.push_back(wins.size());
    }

    int total = 1;
    for (auto n : numberOfWins)
    {
        printf("number of wins: %d\n", n);
        total *= n;
    }

    printf("total: %d\n", total);

    return 0;
}
