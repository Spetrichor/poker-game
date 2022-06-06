#include "play.h"
#include "ui_play.h"
#include "Player.h"
#include<iostream>
#include <QList>
#include <qlineedit.h>
#include <charconv>
#include<thread>
#include<windows.h>
#include<QRadioButton>
using namespace std;
extern QString thisusername;
//链表
QList<QLineEdit*> listone;//链表
QList<QPushButton*> listsecond;
Buyer buyer;Banker banker;
Play::Play(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Play)
{
    ui->setupUi(this);

    initui();
    //获取赌注后相关操作
    connect(listsecond[0],SIGNAL(clicked()),this,SLOT(getbet()));
    connect(this,SIGNAL(operate(Card*)),this,SLOT(put_initialcard(Card*)));
    connect(this,SIGNAL(operate_2()),this,SLOT(is_blackjack()));
    connect(this,SIGNAL(operatebuyerround()),this,SLOT(buyerround()));
    connect(ui->pushButtonnapai,SIGNAL(clicked()),this,SLOT(on_pushButtonnapai_clicked()),Qt::UniqueConnection);
    connect(ui->pushButton_2tingpai,SIGNAL(clicked()),this,SLOT(on_pushButton_2tingpai_clicked()));
    connect(ui->pushButton_3touxiang,SIGNAL(clicked()),this,SLOT(on_pushButton_3touxiang_clicked()));
    connect(ui->pushButton_4jiabei,SIGNAL(clicked()),this,SLOT(on_pushButton_4jiabei_clicked()));
    connect(this,SIGNAL(operatebankerround()),this,SLOT(bankerround()));
    connect(this,SIGNAL(over()),this,SLOT(handleresult()));

}

Play::~Play()
{
    delete ui;
}
//初始化界面
void Play::initui()
{
    //初始化牌库
    init_card(thiscard);
    bankla[0]=ui->label_banker_1;
    bankla[1]=ui->label_banker_2;
    bankla[2]=ui->label_banker_3;
    bankla[3]=ui->label_banker_4;
    bankla[4]=ui->label_banker_5;
    buyla[0]=ui->label_buyer_1;
    buyla[1]=ui->label_buyer_2;
    buyla[2]=ui->label_buyer_3;
    buyla[3]=ui->label_buyer_4;
    buyla[4]=ui->label_buyer_5;
    //初始化界面
    ui->label_yonghu->setProperty("text",thisusername);
    ui->label_yue->setProperty("text",Player::getcredits(thisusername));
    ui->label_duzhu->setProperty("text",0);
    ui->label_baoxian->setProperty("text","无");
    ui->label_currentpoint->setProperty("text",0);
    ui->pushButtonnapai->setCursor(Qt::ArrowCursor);
    ui->pushButtonnapai->setEnabled(false);
    ui->pushButton_2tingpai->setCursor(Qt::ArrowCursor);
    ui->pushButton_2tingpai->setEnabled(false);
    ui->pushButton_3touxiang->setCursor(Qt::ArrowCursor);
    ui->pushButton_3touxiang->setEnabled(false);
    ui->pushButton_4jiabei->setCursor(Qt::ArrowCursor);
    ui->pushButton_4jiabei->setEnabled(false);
    //设置赌注
    ui->label_jiaohu->setProperty("text","请输入赌注...");
    QLineEdit *l=new QLineEdit();
    l->setPlaceholderText("请输入赌注...");
    l->setStyleSheet("color:rgb(225,225,225);");
    ui->horizontalLayout_8->addWidget(l);
    listone.append(l);
    QPushButton *p=new QPushButton();
    p->setText("确认");
    p->setStyleSheet("color:rgb(225, 225, 225);");
    p->setCursor(Qt::PointingHandCursor);
    ui->horizontalLayout_8->addWidget(p);
    listsecond.append(p);

}
void Play::getbet()
{
    buyer.bet(Player::getcredits(thisusername).toInt(),listone[0]->text().toInt());
    Player::decreasecredits(thisusername,buyer.qmybet());
    ui->label_duzhu->setText(listone[0]->text());
    delete listsecond[0];listsecond.removeAt(0);
    delete listone[0];listone.removeAt(0);
    ui->label_jiaohu->setText("游戏开始...");
    repaint();
    Sleep(500);
    ui->label_yue->setText(Player::getcredits(thisusername));
    emit operate(thiscard);
}
//初始化拿牌
void Play::put_initialcard(Card* card)
{
    ui->label_jiaohu->setText("庄家摸牌...");
    repaint();
    Sleep(500);
    bankputcard(banker.get_card(card));
    repaint();
    Sleep(500);
    banker.get_darkcard(card);
    putdarkcard();
    repaint();
    Sleep(500);
    ui->label_jiaohu->setText("闲家摸牌...");
    repaint();
    Sleep(500);
    buyputcard(buyer.get_card(card));
    ui->label_currentpoint->setProperty("text",buyer.qmypoint());
    repaint();
    Sleep(500);
    buyputcard(buyer.get_card(card));
    ui->label_currentpoint->setProperty("text",buyer.qmypoint());
    emit operate_2();
}
void Play::is_blackjack()
{
    Sleep(500);
    buyer.is_blackjack();
    banker.is_blackjack();
    if(buyer.qblackjack())
    {
        if(banker.qblackjack())
        {
            Player::increasecredits(thisusername,buyer.qmybet());
            ui->label_yue->setText(Player::getcredits(thisusername));
            ui->label_jiaohu->setText("双方均为黑杰克,闲家赢得一倍赌注,游戏结束！");
            repaint();
            isover=true;
            emit over();
        }
        else
        {
            Player::increasecredits(thisusername,buyer.qmybet()*2);
            ui->label_yue->setText(Player::getcredits(thisusername));
            ui->label_jiaohu->setText("闲家为黑杰克,赢得两倍赌注,游戏结束！");
            repaint();
            Player::increasecredits(thisusername,buyer.qmybet()*2);
            isover=true;
            emit over();
        }
        return;
    }
    if (banker.qmypoint() == 10) {
            if (banker.qdarkpoint() == 11) {
                if (/*下家是否是黑杰克*/buyer.qblackjack()) {
                    Player::increasecredits(thisusername,buyer.qmybet());
                    ui->label_yue->setText(Player::getcredits(thisusername));
                    ui->label_jiaohu->setText("双方均为黑杰克,闲家赢得一倍赌注,游戏结束！");
                    repaint();
                    isover=true;
                    emit over();
                } else {
                    ui->label_jiaohu->setText("庄家为黑杰克,庄家赢得一倍赌注,游戏结束！");
                    repaint();
                    isover=true;
                    emit over();
                }
            }
            else {
                ui->label_jiaohu->setText("双方都不是黑杰克,游戏继续...");
                repaint();

                emit operatebuyerround();
            }
    }
    else if(banker.qmypoint()==11)
    {
        getinsurance();
        connect(this,SIGNAL(operate_3()),this,SLOT(totaljudge()));
//        if(banker.qdarkpoint()==10)
//        {
//            if(buyer.if_insurance==true)
//            {
//                ui->label_jiaohu->setText("庄家为黑杰克。下家拥有保险,赢得2倍赌注,游戏结束！");
//                repaint();
//                Player::increasecredits(thisusername,buyer.qmybet()*2);
//                isover=true;
//            }else if(buyer.if_insurance==false){
//                ui->label_jiaohu->setText("庄家为黑杰克,庄家赢得一倍赌注,游戏结束！");
//                repaint();
//                isover=true;
//            }
//        }
//        else
//        {
//            ui->label_jiaohu->setText("庄家不是黑杰克,游戏继续...");
//            repaint();
//        }
    }
    else {
        ui->label_jiaohu->setText("双方都不是黑杰克,游戏继续...");
        repaint();
        emit operatebuyerround();
    }
}
//判断黑杰克
void Play::getinsurance()
{
    ui->label_jiaohu->setText("庄家明牌为A,下家是否购买保险(保险花费赌注一半且不退)?");
    repaint();
    QPushButton *q1=new QPushButton();
    QPushButton *q2=new QPushButton();
    q1->setText("是");
    q1->setStyleSheet("color:rgb(225,225,225);");
    q1->setCursor(Qt::PointingHandCursor);
    q2->setText("否");
    q2->setStyleSheet("color:rgb(225,225,225);");
    q2->setCursor(Qt::PointingHandCursor);
    ui->horizontalLayout_8->addWidget(q1);
    ui->horizontalLayout_8->addWidget(q2);
    connect(q1,&QPushButton::clicked,this,[=]{
        Player::decreasecredits(thisusername,buyer.qmybet()/2);
        ui->label_jiaohu->setText("保险购买成功");
        ui->label_baoxian->setText("有");
        ui->label_currentpoint->setText(Player::getcredits(thisusername));
        repaint();
        buyer.if_insurance=true;
        delete q1;
        delete q2;
        emit operate_3();
    });
    connect(q2,&QPushButton::clicked,this,[=]{
        ui->label_jiaohu->setText("放弃购买保险");
        ui->label_baoxian->setText("无");
        repaint();
        buyer.if_insurance=false;
        delete q1;
        delete q2;
        emit operate_3();
    });
}
void Play::totaljudge()
{
    Sleep(500);
    if(banker.qdarkpoint()==10)
    {
        if(buyer.if_insurance==true)
        {
            ui->label_jiaohu->setText("庄家为黑杰克。下家拥有保险,赢得2倍赌注,游戏结束！");
            repaint();
            Player::increasecredits(thisusername,buyer.qmybet()*2);
            isover=true;
            emit over();
        }else if(buyer.if_insurance==false){
            ui->label_jiaohu->setText("庄家为黑杰克,庄家赢得一倍赌注,游戏结束！");
            repaint();
            isover=true;
            emit over();
        }
    }
    else
    {
        ui->label_jiaohu->setText("庄家不是黑杰克,游戏继续...");
        repaint();
        emit operatebuyerround();
    }
}
//绘图函数
void Play::putdarkcard()
{

    bankla[1]->setStyleSheet("border-image: url(:/img/background/ba_ck.jpg);");bank[1]=true;

}
void Play::putdarktocard(Card &darkcard)
{
    if(darkcard.getcolor()==CARDS_COLOR::clubs)
    {
        switch(darkcard.getpoint()){
            case CARDS_POINT::PA1:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/1_c.jpg);");break;
            case CARDS_POINT::P2:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/2_c.jpg);");break;
            case CARDS_POINT::P3:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/3_c.jpg);");break;
            case CARDS_POINT::P4:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/4_c.jpg);");break;
            case CARDS_POINT::P5:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/5_c.jpg);");break;
            case CARDS_POINT::P6:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/6_c.jpg);");break;
            case CARDS_POINT::P7:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/7_c.jpg);");break;
            case CARDS_POINT::P8:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/8_c.jpg);");break;
            case CARDS_POINT::P9:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/9_c.jpg);");break;
            case CARDS_POINT::P10:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/10_c.jpg);");break;
            case CARDS_POINT::PJ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/11_c.jpg);");break;
            case CARDS_POINT::PQ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/12_c.jpg);");break;
            case CARDS_POINT::PK:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/13_c.jpg);");break;
        default: break;
        }
    }
    else if(darkcard.getcolor()==CARDS_COLOR::diamonds)
    {
        switch(darkcard.getpoint()){
            case CARDS_POINT::PA1:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/1_d.jpg);");break;
            case CARDS_POINT::P2:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/2_d.jpg);");break;
            case CARDS_POINT::P3:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/3_d.jpg);");break;
            case CARDS_POINT::P4:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/4_d.jpg);");break;
            case CARDS_POINT::P5:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/5_d.jpg);");break;
            case CARDS_POINT::P6:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/6_d.jpg);");break;
            case CARDS_POINT::P7:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/7_d.jpg);");break;
            case CARDS_POINT::P8:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/8_d.jpg);");break;
            case CARDS_POINT::P9:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/9_d.jpg);");break;
            case CARDS_POINT::P10:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/10_d.jpg);");break;
            case CARDS_POINT::PJ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/11_d.jpg);");break;
            case CARDS_POINT::PQ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/12_d.jpg);");break;
            case CARDS_POINT::PK:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/13_d.jpg);");break;
        default: break;
        }
    }
    else if(darkcard.getcolor()==CARDS_COLOR::hearts)
    {
        switch(darkcard.getpoint()){
            case CARDS_POINT::PA1:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/1_h.jpg);");break;
            case CARDS_POINT::P2:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/2_h.jpg);");break;
            case CARDS_POINT::P3:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/3_h.jpg);");break;
            case CARDS_POINT::P4:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/4_h.jpg);");break;
            case CARDS_POINT::P5:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/5_h.jpg);");break;
            case CARDS_POINT::P6:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/6_h.jpg);");break;
            case CARDS_POINT::P7:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/7_h.jpg);");break;
            case CARDS_POINT::P8:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/8_h.jpg);");break;
            case CARDS_POINT::P9:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/9_h.jpg);");break;
            case CARDS_POINT::P10:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/10_h.jpg);");break;
            case CARDS_POINT::PJ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/11_h.jpg);");break;
            case CARDS_POINT::PQ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/12_h.jpg);");break;
            case CARDS_POINT::PK:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/13_h.jpg);");break;
        default: break;
        }
    }
    else if(darkcard.getcolor()==CARDS_COLOR::spades)
    {
        switch(darkcard.getpoint()){
            case CARDS_POINT::PA1:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/1_s.jpg);");break;
            case CARDS_POINT::P2:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/2_s.jpg);");break;
            case CARDS_POINT::P3:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/3_s.jpg);");break;
            case CARDS_POINT::P4:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/4_s.jpg);");break;
            case CARDS_POINT::P5:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/5_s.jpg);");break;
            case CARDS_POINT::P6:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/6_s.jpg);");break;
            case CARDS_POINT::P7:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/7_s.jpg);");break;
            case CARDS_POINT::P8:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/8_s.jpg);");break;
            case CARDS_POINT::P9:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/9_s.jpg);");break;
            case CARDS_POINT::P10:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/10_s.jpg);");break;
            case CARDS_POINT::PJ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/11_s.jpg);");break;
            case CARDS_POINT::PQ:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/12_s.jpg);");break;
            case CARDS_POINT::PK:
                bankla[1]->setStyleSheet("border-image: url(:/img/background/13_s.jpg);");break;
        default: break;
        }
    }
}
void Play::bankputcard(Card &cd)
{
    int i=0;
    if(cd.getcolor()==CARDS_COLOR::clubs)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!bank[i])
                    {
                        bankla[i]->setStyleSheet("border-image: url(:/img/background/1_c.jpg);");bank[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/2_c.jpg);");bank[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/3_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/4_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/5_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/6_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/7_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/8_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/9_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/10_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/11_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/12_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/13_c.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
    else if(cd.getcolor()==CARDS_COLOR::hearts)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!bank[i])
                    {
                        bankla[i]->setStyleSheet("border-image:url(:/img/background/1_h.jpg);");bank[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/2_h.jpg);");bank[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/3_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/4_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/5_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/6_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/7_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/8_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/9_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/10_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/11_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/12_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/13_h.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
    else if(cd.getcolor()==CARDS_COLOR::diamonds)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!bank[i])
                    {
                        bankla[i]->setStyleSheet("border-image:url(:/img/background/1_d.jpg);");bank[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/2_d.jpg);");bank[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/3_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/4_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/5_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/6_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/7_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/8_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/9_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/10_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/11_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/12_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/13_d.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
    else if(cd.getcolor()==CARDS_COLOR::spades)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!bank[i])
                    {
                        bankla[i]->setStyleSheet("border-image:url(:/img/background/1_s.jpg);");bank[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/2_s.jpg);");bank[i]=true;break;
                }
                else i++;
                break;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/3_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/4_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/5_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/6_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/7_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/8_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/9_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/10_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/11_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/12_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!bank[i])
                {
                    bankla[i]->setStyleSheet("border-image:url(:/img/background/13_s.jpg);");bank[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
}

void Play::buyputcard(Card& cd)
{
    int i=0;
    if(cd.getcolor()==CARDS_COLOR::clubs)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!buy[i])
                    {
                        buyla[i]->setStyleSheet("border-image:url(:/img/background/1_c.jpg);");buy[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/2_c.jpg);");buy[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/3_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/4_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/5_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/6_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/7_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/8_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/9_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/10_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/11_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/12_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/13_c.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
    else if(cd.getcolor()==CARDS_COLOR::hearts)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!buy[i])
                    {
                        buyla[i]->setStyleSheet("border-image:url(:/img/background/1_h.jpg);");buy[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/2_h.jpg);");buy[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/3_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/4_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/5_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/6_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/7_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/8_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/9_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/10_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/11_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/12_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/13_h.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
    else if(cd.getcolor()==CARDS_COLOR::diamonds)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!buy[i])
                    {
                        buyla[i]->setStyleSheet("border-image:url(:/img/background/1_d.jpg);");buy[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/2_d.jpg);");buy[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/3_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/4_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/5_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/6_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/7_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/8_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/9_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/10_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/11_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/12_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/13_d.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
    else if(cd.getcolor()==CARDS_COLOR::spades)
    {
        switch(cd.getpoint()){
            case CARDS_POINT::PA1:
                while(1)
                {
                    if(!buy[i])
                    {
                        buyla[i]->setStyleSheet("border-image:url(:/img/background/1_s.jpg);");buy[i]=true;
                        break;
                    }
                    else i++;
                }
            break;

            case CARDS_POINT::P2:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/2_s.jpg);");buy[i]=true;break;
                }
                else i++;
            }
            break;

            case CARDS_POINT::P3:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/3_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P4:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/4_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P5:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/5_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P6:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/6_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P7:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/7_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P8:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/8_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P9:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/9_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::P10:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/10_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PJ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/11_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PQ:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/12_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

            case CARDS_POINT::PK:
            while(1)
            {
                if(!buy[i])
                {
                    buyla[i]->setStyleSheet("border-image:url(:/img/background/13_s.jpg);");buy[i]=true;
                    break;
                }
                else i++;
            }
        break;

        default:break;
        }
    }
}
//下家回合
void Play::buyerround()
{
    isover=false;
        ui->label_jiaohu->setText("请选择...");
        ui->pushButtonnapai->setCursor(Qt::PointingHandCursor);
        ui->pushButtonnapai->setEnabled(true);
        ui->pushButton_2tingpai->setCursor(Qt::PointingHandCursor);
        ui->pushButton_2tingpai->setEnabled(true);
        ui->pushButton_3touxiang->setCursor(Qt::PointingHandCursor);
        ui->pushButton_3touxiang->setEnabled(true);
        if(buyer.qmybet()*2>Player::getcredits(thisusername).toInt())
        {
            ui->pushButton_4jiabei->setCursor(Qt::ArrowCursor);
            ui->pushButton_4jiabei->setEnabled(false);
        }
        else if(buyer.qmybet()*2<=Player::getcredits(thisusername).toInt())
        {
            ui->pushButton_4jiabei->setCursor(Qt::PointingHandCursor);
            ui->pushButton_4jiabei->setEnabled(true);
        }
        repaint();
}

//庄家回合
void Play::bankerround()
{
    banker.pluspoint();
    ui->label_jiaohu->setText("庄家回合...");
    putdarktocard(banker.qdarkcard());
    repaint();
    Sleep(500);
    do{
        ui->label_jiaohu->setText("庄家摸牌...");
        repaint();
        Sleep(500);
        bankputcard(banker.get_card(thiscard));
        repaint();
    }while((banker.is_lose()==false)||banker.qmypoint()<17);
    if(banker.is_lose()==true)
    {
        Player::increasecredits(thisusername,buyer.qmybet()*2);
        ui->label_yue->setText(Player::getcredits(thisusername));
        ui->label_jiaohu->setText("庄家点数超过21点,下家赢得2倍赌注,游戏结束！");
        ui->label_duzhu->setProperty("text",0);
        ui->label_baoxian->setText("无");
        repaint();
        isover=true;
        emit over();
    }
    else
    {
        if(banker.qmypoint()>buyer.qmypoint())
        {
            ui->label_jiaohu->setText("庄家点数大于下家,游戏结束！");
            ui->label_duzhu->setProperty("text",0);
            ui->label_baoxian->setText("无");
            repaint();
            isover=true;
            emit over();
        }
        else if(banker.qmypoint()==buyer.qmypoint())
        {
            Player::increasecredits(thisusername,buyer.qmybet());
            ui->label_yue->setText(Player::getcredits(thisusername));
            ui->label_jiaohu->setText("下家点数大于庄家,赢得1倍赌注,游戏结束！");
            ui->label_duzhu->setProperty("text",0);
            ui->label_baoxian->setText("无");
            repaint();
            isover=true;
            emit over();
        }
        else
        {
            Player::increasecredits(thisusername,buyer.qmybet()*2);
            ui->label_yue->setText(Player::getcredits(thisusername));
            ui->label_jiaohu->setText("下家点数大于庄家,赢得2倍赌注,游戏结束！");
            ui->label_duzhu->setProperty("text",0);
            ui->label_baoxian->setText("无");
            repaint();
            isover=true;
            emit over();
        }
    }
}
//结果处理函数
void Play::handleresult()
{
    delete ui->pushButtonnapai;
    delete ui->pushButton_2tingpai;
    delete ui->pushButton_3touxiang;
    delete ui->pushButton_4jiabei;
//    QPushButton *qbegin=new QPushButton;
    QPushButton *qreturn=new QPushButton;
    QPushButton *qexit=new QPushButton;
//    qbegin->setText("再来一次");
    qreturn->setText("返回");
    qexit->setText("离开赌场");
//    qbegin->setCursor(Qt::PointingHandCursor);
    qreturn->setCursor(Qt::PointingHandCursor);
    qexit->setCursor(Qt::PointingHandCursor);
//    qbegin->setStyleSheet("color:rgb(225,225,225);");
    qreturn->setStyleSheet("color:rgb(225,225,225);");
    qexit->setStyleSheet("color:rgb(225,225,225);");
//    ui->verticalLayout_2->addWidget(qbegin);
    ui->verticalLayout_2->addWidget(qreturn);
    ui->verticalLayout_2->addWidget(qexit);
    listsecond.append(qreturn);
    listsecond.append(qexit);
//    connect(qbegin,SIGNAL(clicked()),this,SLOT(qbeginclicked()));
    connect(qreturn,SIGNAL(clicked()),this,SLOT(qreturnclicked()));
    connect(qexit,SIGNAL(clicked()),this,SLOT(qexitclicked()));

}
void Play::qreturnclicked()
{
    emit switchwindow();

    this->close();
    listsecond.removeAt(0);listsecond.removeAt(0);
    buyer.reduction();
    banker.reduction();
//    Buyer newbuyer;Banker newbanker;
//    buyer=newbuyer;banker=newbanker;
}
void Play::qexitclicked()
{
    this->close();
}

void Play::on_pushButtonnapai_clicked()
{
    buyputcard(buyer.get_card(thiscard));
    ui->label_currentpoint->setProperty("text",buyer.qmypoint());
    repaint();
    Sleep(500);
    if(buyer.is_lose()==true)
    {
        ui->label_jiaohu->setText("下家点数超过21点,游戏结束！");
        ui->label_duzhu->setProperty("text",0);
        ui->label_baoxian->setText("无");
        repaint();
        Sleep(500);
        isover=true;
        emit over();
    }
}


void Play::on_pushButton_2tingpai_clicked()
{
    ui->label_jiaohu->setText("下家选择停牌...");
    repaint();
    Sleep(500);

    emit operatebankerround();
}


void Play::on_pushButton_3touxiang_clicked()
{
    Player::increasecredits(thisusername,buyer.qmybet()/2);
    ui->label_yue->setText(Player::getcredits(thisusername));
    ui->label_jiaohu->setText("下家选择投降,获得一半赌注,游戏结束！");
    ui->label_duzhu->setProperty("text",0);
    ui->label_baoxian->setText("无");
    repaint();
    Sleep(500);
    isover=true;
    emit over();
}


void Play::on_pushButton_4jiabei_clicked()
{
    ui->label_jiaohu->setText("下家选择加倍...");
    repaint();
    Sleep(500);
    ui->label_duzhu->setProperty("text",buyer.qmybet()*2);
    Player::decreasecredits(thisusername,buyer.qmybet());
    ui->label_yue->setText(Player::getcredits(thisusername));
    ui->label_currentpoint->setProperty("text",buyer.qmypoint());
    buyputcard(buyer.get_card(thiscard));
    repaint();
    Sleep(500);
    if(buyer.is_lose()==true)
    {
        ui->label_jiaohu->setText("下家点数超过21点,游戏结束！");
        ui->label_duzhu->setProperty("text",0);
        ui->label_baoxian->setText("无");
        repaint();
        Sleep(500);
        isover=true;
        emit over();
    }
    else
    {
        emit operatebankerround();
    }
}

