#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <optional>


struct range
{
    uint64_t src = 0;
    uint64_t dest = 0;
    uint64_t len = 0;
};

struct numberMap
{
    std::string src;
    std::string dest;
    std::vector<range> ranges;
};

void printNumberMap(numberMap& nm)
{
    printf("%s -> %s\n", nm.src.c_str(), nm.dest.c_str());
    for (auto r : nm.ranges)
    {
        printf("%llu, %llu, %llu\n", r.src, r.dest, r.len);
    }
}

std::optional<numberMap> extractNumberMap(std::ifstream& fd)
{
    numberMap nm;

    std::string line;
    if (!std::getline(fd, line))
    {
        return std::nullopt;
    }

    size_t pos, next;
    pos = line.find_first_of("-", 0);
    nm.src = line.substr(0, pos);
    pos += 4;
    next = line.find_first_of(" ", pos);
    nm.dest = line.substr(pos, next-pos);

    while (std::getline(fd, line))
    {
        if (line.length() == 0)
        {
            break;
        }
        range r;

        pos = line.find_first_of(" ", 0);
        r.dest = stoll(line.substr(0, pos));
        pos++;
        next = line.find_first_of(" ", pos);
        r.src = stoll(line.substr(pos, next-pos));
        pos = next+1;
        r.len = stoll(line.substr(pos, line.length()));

        nm.ranges.push_back(r);
    }

    return nm;
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

    std::ifstream fd(input);
    std::string line;
    std::getline(fd, line);

    size_t pos, next;
    pos = line.find_first_of(" ", 0);
    pos++;

    std::vector<uint64_t> seeds;
    while (pos < line.size())
    {
        next = line.find_first_of(" ", pos);
        if (next == std::string::npos)
        {
            next = line.size();
        }
        
        uint64_t n = std::stoll(line.substr(pos, next - pos));
        pos = next + 1;
        seeds.push_back(n);
    }
    std::getline(fd, line);

    std::map<std::string, numberMap> almanac;
    while (auto nm = extractNumberMap(fd))
    {
        if (!nm.has_value())
        {
            break;
        }
        almanac[nm.value().src] = nm.value();

        //printNumberMap(nm.value());
    }

    uint64_t lowest = INT_MAX;

    for (uint64_t n : seeds)
    {
        std::string key = "seed";
        do
        {
            if (key == "location")
            {
                break;
            }
            numberMap nm = almanac[key];
            key = nm.dest;

            uint64_t next = 0;
            for (auto& r : nm.ranges)
            {
                if (r.src <= n && n <= r.src + r.len)
                {
                    next = r.dest + (n - r.src);
                    break;
                }
            }
            if (next != 0)
            {
                n = next;
            }
        } while (true);

        printf("%s: %llu\n", key.c_str(), n);

        if (n < lowest)
        {
            lowest = n;
        }
    }

    printf("lowest: %llu\n", lowest);

    return 0;
}
