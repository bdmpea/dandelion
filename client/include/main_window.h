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
public slots:
    void authorize(const QString &status);
    void on_SignInButton_clicked();
    void on_NoAccountButton_clicked();
    void register_();
public:
    Ui::MainWindow *ui;
    Main_Menu *window;
    Registration *registration;
};
#endif // MAINWINDOW_H
