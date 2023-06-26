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
    QString get_new_word();
    vocabulary_list *vocabulary;
    QString m_username;

    explicit PersonalAccount(QWidget *parent = nullptr);
    ~PersonalAccount();


private slots:
    void on_BackButton_clicked();
    void on_VocabularyButton_clicked();
    void on_FlashcardsButton_clicked();
    void close_window();
    void open_window();
    void on_NewWordButton_clicked();

    void on_AddButton_2_clicked();
public slots:
    void set_username_label();
signals:
    void signal_for_vocabulary();
    void signal_for_new_word_cards();

private:
    Ui::PersonalAccount *ui;
    info_window *info;
    flashcards_desk *desk;
};

#endif // PERSONAL_ACCOUNT_H