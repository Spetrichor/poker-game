#include "game.h"
#include "ui_game.h"
#include "dialog2.h"
#include "Card.h"
#include "Player.h"
#include <QSqlQuery>
#include <QMessageBox>
extern QString thisusername;
Game::Game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    //获取用户信息~~~~~~~~~~~~~~~~~~~~~~~
    setlabelinformation();
    //实现“开始游戏”“规则说明”“退出”按钮功能
    connect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(on_pushButton_begin_clicked()),Qt::UniqueConnection);
    connect(ui->pushButton_rule,SIGNAL(clicked()),this,SLOT(on_pushButton_rule_clicked()),Qt::UniqueConnection);
    connect(ui->text,SIGNAL(clicked()),this,SLOT(on_text1_clicked()),Qt::UniqueConnection);
}

Game::~Game()
{
    delete ui;
}
QString Game::getusername()
{
    return thisusername;
}

void Game::setlabelinformation()
{
    ui->label_username->setProperty("text",getusername());

    ui->label_credits->setProperty("text",Player::getcredits(thisusername));

    update();
}

void Game::on_pushButton_rule_clicked()
{
    Rule *r=new Rule();
    r->exec();
}


void Game::on_pushButton_begin_clicked()
{
    //切换窗口
    Play *p=new Play();
    connect(p,SIGNAL(switchwindow()),this,SLOT(reshow()));
    this->close();
    p->show();
}
void Game::reshow()
{
    this->show();
    setlabelinformation();
}
void Game::on_text1_clicked()
{
    QMessageBox::information(this,"关于","Lzl创建于2022/05/31",QMessageBox::Ok);
}
