#ifndef RULE_H
#define RULE_H

#include <QDialog>

namespace Ui {
class Rule;
}

class Rule : public QDialog
{
    Q_OBJECT

public:
    explicit Rule(QWidget *parent = nullptr);
    ~Rule();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Rule *ui;
};

#endif // RULE_H
