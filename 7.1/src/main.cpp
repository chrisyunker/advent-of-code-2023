#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>

enum Card
{
    U, C2, C3, C4, C5, C6, C7, C8, C9, T, J, Q, K, A
};

Card FromChar(const char c)
{
    switch (c)
    {
        case '2': return Card::C2;
        case '3': return Card::C3;
        case '4': return Card::C4;
        case '5': return Card::C5;
        case '6': return Card::C6;
        case '7': return Card::C7;
        case '8': return Card::C8;
        case '9': return Card::C9;
        case 'T': return Card::T;
        case 'J': return Card::J;
        case 'Q': return Card::Q;
        case 'K': return Card::K;
        case 'A': return Card::A;
        default: return Card::U;
    }
}

char FromCard(const Card c)
{
    switch (c)
    {
        case Card::C2: return'2';
        case Card::C3: return'3';
        case Card::C4: return'4';
        case Card::C5: return'5';
        case Card::C6: return'6';
        case Card::C7: return'7';
        case Card::C8: return'8';
        case Card::C9: return'9';
        case Card::T: return'T';
        case Card::J: return'J';
        case Card::Q: return'Q';
        case Card::K: return'K';
        case Card::A: return'A';
        default: return 'U';
    }
}

enum HandType
{
    undefined,
    high_card,
    one_pair,
    two_pair,
    three_kind,
    full_house,
    four_kind,
    five_kind
};

struct Hand
{
    std::map<Card, int> cardMap;
    std::vector<Card> cards;
    HandType type = high_card;
    int bid = 0;

    void setHandType()
    {
        for (auto& [k, v] : cardMap)
        {
            if (v == 5)
            {
                type = five_kind;
                return;
            }
            if (v == 4)
            {
                type = four_kind;
                return;
            }
            if (v == 3)
            {
                if (type == one_pair)
                {
                    type = full_house;
                }
                else
                {
                    type = three_kind;
                }
            }
            if (v == 2)
            {
                if (type == one_pair)
                {
                    type = two_pair;
                }
                else if (type == three_kind)
                {
                    type = full_house;
                }
                else
                {
                    type = one_pair;
                }
            }
        }
    }

    void print() const
    {
        printf("Hand type: %d, ", type);
        for (auto c : cards)
        {
            printf("%c", FromCard(c));
        }
        printf(" %d\n", bid);
    }
};

bool handSorter(const Hand& a, const Hand& b)
{
    if (a.type > b.type)
    {
        return false;
    }
    if (a.type < b.type)
    {
        return true;
    }
    for (int i = 0; i < 5; i++)
    {
        if (a.cards[i] > b.cards[i])
        {
            return false;
        }
        if (a.cards[i] < b.cards[i])
        {
            return true;
        }
    }
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

    std::vector<Hand> hands;

    std::ifstream fd(input);
    std::string line;
    while (std::getline(fd, line))
    {
        Hand hand;
        for (int i=0; i < 5; i++)
        {
            Card c = FromChar(line[i]);
            if (hand.cardMap.contains(c))
            {
                hand.cardMap[c]++;
            }
            else
            {
                hand.cardMap[c] = 1;
            }
            hand.cards.push_back(c);
        }
        hand.setHandType();
        
        hand.bid = stoi(line.substr(6, line.size() - 6));

        hands.push_back(hand);
    }

    printf("Hand count: %zu\n", hands.size());

    std::sort(hands.begin(), hands.end(), &handSorter);

    int total = 0;
    for (int i=0; i < hands.size(); i++)
    {
        hands[i].print();
        total += hands[i].bid * (i+1);
    }

    printf("total: %d\n", total);

    return 0;
}
