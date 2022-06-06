//
// Created by lzl on 2022/5/13.
//

#include "Player.h"
#include <iostream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
bool Player::is_lose() {
    if (mypoint > 21) {
        lose = true;
        return true;
    } else {
        lose = false;
        return false;
    }
}

void Player::caculate_point(int size) {
    switch (mycard[size - 1].getpoint()) {
        case PA1:
            if (mypoint <= 10) {
                mypoint += 11;

            } else {
                mypoint += 1;
            }
            break;
        case P2:

            mypoint += 2;
            break;
        case P3:
            mypoint += 3;
            break;
        case P4:
            mypoint += 4;
            break;
        case P5:
            mypoint += 5;
            break;
        case P6:
            mypoint += 6;
            break;
        case P7:
            mypoint += 7;
            break;
        case P8:
            mypoint += 8;
            break;
        case P9:
            mypoint += 9;
            break;
        case P10:
            mypoint += 10;
            break;
        case PJ:
            mypoint += 10;
            break;
        case PK:
            mypoint += 10;
            break;
        case PQ:
            mypoint += 10;
            break;
    }
}
QString Player::getcredits(QString thisuser)
{

    QSqlQuery query1;
    query1.exec("SELECT * FROM user WHERE username");
    while(query1.next())
    {
        if(query1.value(0).toString()==thisuser)
        {
            return query1.value(2).toString();
        }
    }
    return 0;
}
void Player::increasecredits(QString thisuser,int a)
{
    QSqlQuery query1;

    query1.prepare("update user set credits=:credits WHERE username=:username");
    query1.bindValue(":username",thisuser);
    query1.bindValue(":credits",Player::getcredits(thisuser).toInt()+a);
    query1.exec();
}
void Player::decreasecredits(QString thisuser,int a)
{
    QSqlQuery query1;

    query1.prepare("update user set credits=:credits WHERE username=:username");
    query1.bindValue(":username",thisuser);
    query1.bindValue(":credits",Player::getcredits(thisuser).toInt()-a);
    query1.exec();
}
void Banker::is_blackjack() {
    if (((mycard[0].getpoint() == P10) || (mycard[0].getpoint() == PJ) || (mycard[0].getpoint() == PK) ||
         (mycard[0].getpoint() == PQ)) && (darkcard.getpoint() == PA1)) {
        blackjack = true;
    } else if (((darkcard.getpoint() == P10) || (darkcard.getpoint() == PJ) || (darkcard.getpoint() == PK) ||
                (darkcard.getpoint() == PQ)) && (mycard[0].getpoint() == PA1)) {
        blackjack = true;
    }else blackjack=false;
}


Card& Banker::get_card(Card *card) {
    int size = mycard.getSize();
    mycard[size - 1] = deal_card(card);
    mycard.resize(size + 1);
    caculate_point(size);
    return mycard[size-1];
}

Card& Banker::get_darkcard(Card *card) {
    darkcard = deal_card(card);
    switch (darkcard.getpoint()) {
        case PA1:
            darkpoint = 11;
            break;
        case P2:
            darkpoint = 2;
            break;
        case P3:
            darkpoint = 3;
            break;
        case P4:
            darkpoint = 4;
            break;
        case P5:
            darkpoint = 5;
            break;
        case P6:
            darkpoint = 6;
            break;
        case P7:
            darkpoint = 7;
            break;
        case P8:
            darkpoint = 8;
            break;
        case P9:
            darkpoint = 9;
            break;
        case P10:
            darkpoint = 10;
            break;
        case PJ:
            darkpoint = 10;
            break;
        case PK:
            darkpoint = 10;
            break;
        case PQ:
            darkpoint = 10;
            break;
    }
    return darkcard;
}

void Banker::get_initialcard(Card *card) {
    get_card(card);
    get_darkcard(card);

}

void Banker::pluspoint() {
    mypoint += darkpoint;
}
void Banker::reduction()
{
    mycard.resize(1);
    mycard[0]=Card();
    mypoint=0;
    blackjack=false;
    lose=false;

    darkcard=Card();
    darkpoint=0;
}

void Buyer::is_blackjack() {
    if (((mycard[0].getpoint() == P10) || (mycard[0].getpoint() == PJ) || (mycard[0].getpoint() == PK) ||
         (mycard[0].getpoint() == PQ)) && (mycard[1].getpoint() == PA1)) {
        blackjack = true;
    } else if (((mycard[1].getpoint() == P10) || (mycard[1].getpoint() == PJ) || (mycard[1].getpoint() == PK) ||
                (mycard[1].getpoint() == PQ)) && (mycard[0].getpoint() == PA1)) {
        blackjack = true;
    } else blackjack=false;
}



void Buyer::bet(int allmoney,int myybet) {
    if (myybet > allmoney) {
        exit(1);
    }
    mybet = myybet;
}

void Buyer::redouble(Card *card) {
    mybet = mybet * 2;
    std::cout << "下家选择加倍\n现在的赌注为: " << mybet << std::endl;
    get_card(card);
}

void Buyer::surrender() {
    std::cout << "闲家选择投降\n";
    mybet = mybet / 2;
}

Card& Buyer::get_card(Card *card) {
    int size = mycard.getSize();
    mycard[size - 1] = deal_card(card);
    mycard.resize(size + 1);
    caculate_point(size);
    return mycard[size-1];
}

void Buyer::get_initialcard(Card *card) {
    get_card(card);
    get_card(card);
}

void Buyer::reduction()
{
    mycard.resize(1);
    mycard[0]=Card();
    mypoint=0;
    blackjack=false;
    lose=false;
    mybet=0;
}

