#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <QMainWindow>
#include "personal_account.h"
#include "balda.h"
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Main_Menu;
}
QT_END_NAMESPACE

class Main_Menu : public QMainWindow
{
Q_OBJECT

public:
    Main_Menu(QMainWindow *parent = nullptr);
    ~Main_Menu();

signals:
    void signal_for_vocabulary();

    void signal_for_new_word();

    void signal_for_new_word_cards();

    void signal_get_words_from_vocabulary_();
public slots:
    void on_HomeButton_clicked();
    void on_BaldaButton_clicked();
    void on_NewWordButton_clicked();
    QString get_new_word();
    void on_AddButton_clicked();
    void add_new_word_cards();
    void get_words_from_vocabulary();
    void open_window();
public:
    QString username;
    Ui::Main_Menu *ui;
    PersonalAccount *personal_account;
    balda *balda_game;
};

#endif // MAIN_MENU_H