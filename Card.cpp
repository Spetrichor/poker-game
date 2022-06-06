//
// Created by lzl on 2022/5/16.
//

#include "Card.h"

using namespace std;

//重载运算符
Card &Card::operator=(Point points) {
    this->point = points;
    return *this;
}

Card &Card::operator=(Color colors) {
    this->color = colors;
    return *this;

}

Point Card::getpoint() const {
    return point;
}

Color Card::getcolor() const {
    return color;
}

//牌库初始化
void init_card(Card *card) {
    card[0].cur=53;
    card[53].cur=1;
    int position = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {
            card[position] = (Color) i;
            card[position] = (Point) j;
            card[position].is_choose = false;
            card[position].cur=position+1;
            position++;
        }
    }
    card[52].cur=0;
}

//发牌
Card deal_card(Card *card) {
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<unsigned> distr(1, 51);
    unsigned serial = 0;
    do {
        serial = distr(eng);
    } while (card[serial].is_choose==true);
    card[serial].is_choose = true;
    return card[serial];
}
