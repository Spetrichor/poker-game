#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog2.h"
#include "game.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool createconnection();
private slots:
    void on_PBregister_clicked();

    void on_PBconcel_clicked();
    void on_PBlogin_clicked();

public slots:
    void reshow();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
