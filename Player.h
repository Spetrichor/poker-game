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
    Array<Card> mycard;//card(1) ����
    int mypoint = 0;  //����
    bool blackjack; //�Ƿ��Ǻڽܿ�
    bool lose = false;  //�Ƿ񱬵�
public:
    virtual void is_blackjack() = 0;  //�Ƿ��Ǻڽܿ�
    virtual Card& get_card(Card *) = 0; //����
    virtual void get_initialcard(Card *) = 0;
    virtual void reduction() = 0;
    void caculate_point(int);

    bool is_lose(); //�Ƿ񱬵�

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
    //��ȡ���
    static QString getcredits(QString);
    //�޸����
    static void increasecredits(QString,int);
    static void decreasecredits(QString,int);
};

class Banker : public Player {
private:
    Card darkcard;//����
    int darkpoint;
public:
    Card& get_darkcard(Card *);

    void pluspoint();

    void is_blackjack() override;   //�Ƿ��Ǻڽܿ�
    Card& get_card(Card *) override;   //����
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
    int mybet;  //��ע

public:
    bool if_insurance;//����
    void bet(int ,int);  //�����ע

    void is_blackjack() override;   //�Ƿ��Ǻڽܿ�

    Card& get_card(Card *) override; //����
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
