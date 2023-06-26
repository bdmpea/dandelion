#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "main_menu.h"
#include "registration.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString get_username();
    QString get_password();

signals:
    void signal_info_for_registration();
    void signal_info_for_authorization();
    void signal_info_for_new_word();
    void signal_info_for_new_word_cards();
    void signal_get_words_from_vocabulary_();
    void signal_for_new_words_from_vocabulary();
public slots:
    void authorize(const QString &status, const QString &username_);
    void on_SignInButton_clicked();
    void on_NoAccountButton_clicked();
    void register_();
    void add_new_word_();
    void add_new_word_cards();
    void get_new_words_();
public:
    Ui::MainWindow *ui;
    Main_Menu *window;
    Registration *registration;
};
#endif // MAINWINDOW_H
