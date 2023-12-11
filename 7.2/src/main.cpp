#include <stdio.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>

enum Card
{
    U, J, C2, C3, C4, C5, C6, C7, C8, C9, T, Q, K, A
};

Card FromChar(const char c)
{
    switch (c)
    {
        case 'J': return Card::J;
        case '2': return Card::C2;
        case '3': return Card::C3;
        case '4': return Card::C4;
        case '5': return Card::C5;
        case '6': return Card::C6;
        case '7': return Card::C7;
        case '8': return Card::C8;
        case '9': return Card::C9;
        case 'T': return Card::T;
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
        case Card::J: return'J';
        case Card::C2: return'2';
        case Card::C3: return'3';
        case Card::C4: return'4';
        case Card::C5: return'5';
        case Card::C6: return'6';
        case Card::C7: return'7';
        case Card::C8: return'8';
        case Card::C9: return'9';
        case Card::T: return'T';
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

std::string FromType(const HandType t)
{
    switch (t)
    {
        case five_kind:  return " five kind";
        case four_kind:  return " four kind";
        case full_house: return "full house";
        case three_kind: return "three kind";
        case two_pair:   return "  two pair";
        case one_pair:   return "  one pair";
        case high_card:  return " high card";
        default:         return " undefined";
    }
}

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
            }
            if (v == 4)
            {
                type = four_kind;
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

        int numJokers = cardMap.contains(J) ? cardMap[J] : 0;

        if (numJokers > 0)
        {            
            if (type == four_kind)
            {
                type = five_kind;
            }
            else if (type == full_house)
            {
                type = five_kind;
            }
            else if (type == three_kind)
            {
                type = four_kind;
            }
            else if (type == two_pair && numJokers == 1)
            {
                type = full_house;
            }
            else if (type == two_pair && numJokers == 2)
            {
                type = four_kind;
            }
            else if (type == one_pair)
            {
                type = three_kind;
            }
            else if (type == high_card && numJokers == 1)
            {
                type = one_pair;
            }
            else
            {
                printf("ERROR: numJokers: %d\n", numJokers);
            }
        }
    }

    void print() const
    {
        printf("Hand type: %s, ", FromType(type).c_str());
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

    uint64_t total = 0;
    for (int i=0; i < hands.size(); i++)
    {
        hands[i].print();
        total += hands[i].bid * (i+1);
    }

    printf("total: %llu\n", total);

    return 0;
}
