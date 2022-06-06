#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog2.h"
#include "game.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QSqlQuery>

QString thisusername;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
    setWindowFlags(Qt::WindowCloseButtonHint);//显示关闭按钮
    createconnection();//连接数据库
    connect(ui->PBlogin,SIGNAL(clicked()),this,SLOT(on_PBlogin_clicked()),Qt::UniqueConnection);
    //注册按钮动作~~~~~~~~~~~~~~~~~~~~~
    connect(ui->PBregister,SIGNAL(clicked()),this,SLOT(on_PBregister_clicked()),Qt::UniqueConnection);
    connect(ui->PBconcel,SIGNAL(clicked()),this,SLOT(on_PBconcel_clicked()),Qt::UniqueConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//连接服务器~~~~~~~~~~~~~~~~~~~~~~
bool MainWindow::createconnection()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3306);//你的地址
    db.setHostName("localhost");
    db.setUserName("root");//你的my sql的账号
    db.setPassword("6369");//你进去时候输入的密码
    db.setDatabaseName("login_1"); //数据库名

    if(!db.open()){
           QMessageBox::critical(this,"数据库打开失败",db.lastError().text());
           return false;
       }
       else {
           return true;
       }
}
void MainWindow::on_PBregister_clicked()
{
    Dialog2 *d=new Dialog2();
//    connect(d,SIGNAL(switchdialog()),this,SLOT(reshow()));
    this->close();
    d->show();

}


void MainWindow::on_PBconcel_clicked()
{
    this->close();
}
void MainWindow::reshow()
{
    this->show();
}

void MainWindow::on_PBlogin_clicked()
{
    QString usert;
    usert=ui->lineEdit->text();
    QString passwdt;
    passwdt=ui->lineEdit_2->text();

    if(usert==""||passwdt=="")
    {
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    }
    else
    {
        QSqlQuery query;
                query.prepare("select username,password from user where username=:username and password = :password ");

                query.bindValue(":username", usert);
                query.bindValue(":password", passwdt);
                query.exec();
                if(!query.next())
                {
                    QMessageBox::information(this,"警告","用户名或密码错误！",QMessageBox::Ok);
                }
                else
                {
                    QMessageBox::information(this,"提醒","登录成功！",QMessageBox::Ok);
                    thisusername=usert;
                    Game *g = new Game();
                    g->show();
                    this->close();
                }
    }
}

