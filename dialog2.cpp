#include "dialog2.h"
#include "ui_dialog2.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QSqlQuery>

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    connect(ui->PBregister,SIGNAL(clicked),this,SLOT(on_PBregister_clicked()));
    connect(ui->pushButton_return,SIGNAL(clicked),this,SLOT(on_pushButton_return_clicked()),Qt::UniqueConnection);
}
Dialog2::~Dialog2()
{
    delete ui;
}
//注册验证服务器~~~~~~~~~~~~~~~~~~~~~~~~~~
void Dialog2::on_PBregister_clicked()
{
//    MainWindow *m=new MainWindow();
//    this->close();
//    m->show();
    QString myusername=ui->lineEdit11->text().simplified();
    QString mypassword=ui->lineEdit12->text();
    QSqlQuery query;
    query.prepare("select username from user where username=:username");
    query.bindValue(":username", myusername);
    query.exec();
    if(myusername==""||mypassword=="")
    {
        QMessageBox::information(this,"警告","用户名或密码不能为空",QMessageBox::Ok);

    }
    else if(query.next())
    {
        QMessageBox::information(this,"警告","用户名已存在！",QMessageBox::Ok);

    }
    else
    {
        query.prepare("insert into user(username,password,credits)"
                        "values(:username,:password,:credits)");
        query.bindValue(":username",myusername);
        query.bindValue(":password",mypassword);
        query.bindValue(":credits",10000);
        query.exec();
        QMessageBox::information(this,"成功","注册成功,请重新登录",QMessageBox::Ok);
        MainWindow *m=new MainWindow();
        this->close();
        m->show();
    }

}


void Dialog2::on_pushButton_return_clicked()
{
    MainWindow *m=new MainWindow();

//    emit switchdialog();
    this->close();
    m->show();
}

