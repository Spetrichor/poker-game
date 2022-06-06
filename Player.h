//
// Created by lzl on 2022/5/13.
//

#ifndef MY_PROJECT_PLAYER_H
#define MY_PROJECT_PLAYER_H

#include"Card.h"
#include"Array.h"

#include<QString>
#include<qsqlquery.h>

class Player {
protected:
    Array<Card> mycard;//card(1) 手牌
    int mypoint = 0;  //点数
    bool blackjack; //是否是黑杰克
    bool lose = false;  //是否爆掉
public:
    virtual void is_blackjack() = 0;  //是否是黑杰克
    virtual Card& get_card(Card *) = 0; //拿牌
    virtual void get_initialcard(Card *) = 0;
    virtual void reduction() = 0;
    void caculate_point(int);

    bool is_lose(); //是否爆掉

    const Array<Card> &qmycard() const {
        return mycard;
    }

    int qmypoint() const {
        return mypoint;
    }

    bool qblackjack() const {
        return blackjack;
    }

    bool qlose() const {
        return lose;
    }
    //获取余额
    static QString getcredits(QString);
    //修改余额
    static void increasecredits(QString,int);
    static void decreasecredits(QString,int);
};

class Banker : public Player {
private:
    Card darkcard;//暗牌
    int darkpoint;
public:
    Card& get_darkcard(Card *);

    void pluspoint();

    void is_blackjack() override;   //是否是黑杰克
    Card& get_card(Card *) override;   //拿牌
    void get_initialcard(Card *) override;

     Card &qdarkcard()  {
        return darkcard;
    }

    int qdarkpoint() const {
        return darkpoint;
    }

    void reduction()override;
};

class Buyer : public Player {
private:
    int mybet;  //赌注

public:
    bool if_insurance;//保险
    void bet(int ,int);  //输入赌注

    void is_blackjack() override;   //是否是黑杰克

    Card& get_card(Card *) override; //拿牌
    void get_initialcard(Card *) override;

//    void get_insurance();

    void redouble(Card *);

    void surrender();

    int qmybet() const {
        return mybet;
    }
    void reduction()override;


};


#endif //MY_PROJECT_PLAYER_H
