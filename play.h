#ifndef PLAY_H
#define PLAY_H

#include <QMainWindow>
#include <qlabel.h>
#include "Player.h"

namespace Ui {
class Play;
}

class Play : public QMainWindow
{
    Q_OBJECT

public:
    QLabel *bankla[5];
    QLabel *buyla[5];
    explicit Play(QWidget *parent = nullptr);
    ~Play();
    void initui();
    void bankputcard(Card&);
    void buyputcard(Card&);
    void putdarkcard();
    void putdarktocard(Card &);
    void getinsurance();
public slots:
    void put_initialcard(Card *);
    void is_blackjack();
    void totaljudge();
//    void pbnapai();
//    void pbtingpai();
    void bankerround();
    void handleresult();
    void qreturnclicked();
    void qexitclicked();
//    void pbdouble();
    void buyerround();
    void getbet();
private:
    Ui::Play *ui;
    bool bank[5]={false,false,false,false,false};
    bool buy[5]={false,false,false,false,false};
    Card thiscard[52];
    bool isover=false;

signals:
    void operate(Card *);
    void operate_2();
    void operate_3();
    void over();
    void operatebuyerround();
    void operatebankerround();
    void switchwindow();
private slots:
    void on_pushButtonnapai_clicked();
    void on_pushButton_2tingpai_clicked();
    void on_pushButton_3touxiang_clicked();
    void on_pushButton_4jiabei_clicked();
};

#endif // PLAY_H
