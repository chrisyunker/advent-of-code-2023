#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <map>

struct CubeSample
{
    int red = 0;
    int green = 0;
    int blue = 0;
};

CubeSample getCubeSample(const std::string& line)
{
    CubeSample cs;
    size_t pos = 0;
    size_t next;

    while (true)
    {
        next = line.find_first_of(",", pos);
        if (next == std::string::npos)
        {
            next = line.size();
        }
        std::string subline = line.substr(pos, next-pos);
        pos = next + 2;

        int mid = subline.find_first_of(" ", 0);
        int count = std::stoi(subline.substr(0, mid));
        auto color = subline.substr(mid+1, subline.size());

        if (color == "red")
        {
            cs.red= count;
        }
        else if (color == "blue")
        {
            cs.blue = count;
        }
        else if (color == "green")
        {
            cs.green = count;
        }
        else
        {
            printf("ERROR: Bad color: %s\n", color.c_str());
        }

        if (pos > line.size()) break;
    }

    return cs;
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

    std::map<int, std::vector<CubeSample>> games;

    // Load input file
    printf("Input file: %s\n", input.c_str());

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        size_t pos, next;
        pos = line.find_first_of(" ", 0);
        pos++;
        next = line.find_first_of(":", pos);

        int game = std::stoi(line.substr(pos, next - pos));        
        pos = next + 2;

        std::string section;
        std::vector<CubeSample> cubeSamples;
        while (true)
        {
            next = line.find_first_of(";", pos);
            if (next == std::string::npos)
            {
                break;
            }

            auto cs = getCubeSample(line.substr(pos, next-pos));
            cubeSamples.push_back(cs);
            pos = next + 2;
        }

        auto cs = getCubeSample(line.substr(pos, line.size()-pos));
        cubeSamples.push_back(cs);

        games.emplace(game, cubeSamples);
    }

    CubeSample bag = {
        .red = 12,
        .blue = 14,
        .green = 13
    };

    int sumGames = 0;
    for (auto& [game, cubeSamples] : games)
    {
        bool possible = true;
        for (auto& cl : cubeSamples)
        {
            //printf("[%d] cubes: %d, %d, %d\n", k, cl.red, cl.green, cl.blue);
            if (cl.red > bag.red) possible = false;
            if (cl.blue > bag.blue) possible = false;
            if (cl.green > bag.green) possible = false;
        }
        if (possible) sumGames += game;
    }
   
    printf("%d\n", sumGames);
    
    return 0;
}
