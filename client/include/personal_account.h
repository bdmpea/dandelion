#ifndef PERSONAL_ACCOUNT_H
#define PERSONAL_ACCOUNT_H

#include <QWidget>
#include "info_window.h"
#include "flashcards_desk.h"
#include "vocabulary_list.h"



namespace Ui {
class PersonalAccount;
}

class PersonalAccount : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalAccount(QWidget *parent = nullptr);
    ~PersonalAccount();

    QString get_new_word();


private slots:
    void on_BackButton_clicked();
    void on_VocabularyButton_clicked();
    void on_FlashcardsButton_clicked();
    void close_window();
    void open_window();

    void on_NewWordButton_clicked();
    void on_AddButton_2_clicked();

private:
    Ui::PersonalAccount *ui;
    info_window *info;
    flashcards_desk *desk;
    vocabulary_list *vocabulary;
};

#endif // PERSONAL_ACCOUNT_H
