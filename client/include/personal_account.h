#ifndef PERSONAL_ACCOUNT_H
#define PERSONAL_ACCOUNT_H

#include <QWidget>
#include "info_window.h"
#include "flashcards_desk.h"



namespace Ui {
class PersonalAccount;
}

class PersonalAccount : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalAccount(QWidget *parent = nullptr);
    ~PersonalAccount();


private slots:
    void on_BackButton_clicked();
    void on_FlashcardsButton_clicked();
    void close_window();


private:
    Ui::PersonalAccount *ui;
    info_window *info;
    flashcards_desk *desk;
};

#endif // PERSONAL_ACCOUNT_H
