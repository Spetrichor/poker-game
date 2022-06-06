#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "rule.h"
#include"play.h"
namespace Ui {
class Game;
}

class Game : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    QString getusername();
//    QString getcredits(QString);
    void setlabelinformation();
public slots:
    void reshow();
private slots:
    void on_pushButton_rule_clicked();

    void on_pushButton_begin_clicked();
    void on_text1_clicked();

private:
    Ui::Game *ui;
};

#endif // GAME_H
