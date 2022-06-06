#ifndef MY_PROJECT_CARD_H
#define MY_PROJECT_CARD_H

#include<random>

namespace CARDS_POINT {
    enum Point {
        PA1 = 1,
        P2 = 2,
        P3 = 3,
        P4 = 4,
        P5 = 5,
        P6 = 6,
        P7 = 7,
        P8 = 8,
        P9 = 9,
        P10 = 10,
        PJ = 11,
        PK = 13,
        PQ = 12
    };
}
namespace CARDS_COLOR {
    enum Color {
        clubs, diamonds, hearts, spades
    };
}
using namespace CARDS_COLOR;
using namespace CARDS_POINT;

class Card {
private:
    Point point;
    Color color;
public:
    Card() {
        point = PA1;
        color = clubs;
    }
    int cur;

    bool is_choose = false;

    Point getpoint() const;

    Color getcolor() const;

    Card &operator=(Point);

    Card &operator=(Color);
};

//≥ı ºªØ≈∆ø‚
void init_card(Card *card);

//∑¢≈∆
Card deal_card(Card *);

#endif //MY_PROJECT_CARD_H
