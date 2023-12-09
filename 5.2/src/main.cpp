#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <optional>
#include <utility>


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
        almanac[nm.value().dest] = nm.value();
        //printNumberMap(nm.value());
    }

    uint64_t lowest = INT_MAX;

    uint64_t idx = 0;
    std::vector<std::pair<uint64_t, uint64_t>> seedRanges;

    while (idx < seeds.size() - 1)
    {
        uint64_t start = seeds[idx++];
        uint64_t count = seeds[idx++];
        seedRanges.push_back({start, start+count});
    }

    uint64_t x = 0;

    do
    {
        uint64_t n = x;

        std::string key = "location";
        do
        {
            if (key == "seed")
            {
                break;
            }
            numberMap nm = almanac[key];
            key = nm.src;

            int64_t next = -1;
            for (auto &r : nm.ranges)
            {
                if (r.dest <= n && n <= r.dest + r.len)
                {
                    next = r.src + (n - r.dest);
                    break;
                }
            }
            if (next != -1)
            {
                n = next;
            }
        } while (true);

        for (auto &sr : seedRanges)
        {
            if (n >= sr.first && n <= sr.second)
            {
                lowest = x;
                printf("lowest: %llu\n", lowest);
                return 0;
            }
        }

        x += 1;

        if (x % 10000000 == 0)
        {
            printf("n: %llu\n", x);
        }

    } while (true);

    return 0;
}
