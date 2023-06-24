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
    void on_VocabularyButton_clicked();
    void on_FlashcardsButton_clicked();
    void close_window();

    void on_NewWordButton_clicked();
    QString get_new_word();
    void on_AddButton_2_clicked();

private:
    Ui::PersonalAccount *ui;
    info_window *info;
    flashcards_desk *desk;
};

#endif // PERSONAL_ACCOUNT_H
