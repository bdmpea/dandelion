#ifndef PERSONAL_ACCOUNT_H
#define PERSONAL_ACCOUNT_H

#include <QWidget>



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

private:
    Ui::PersonalAccount *ui;
};

#endif // PERSONAL_ACCOUNT_H
