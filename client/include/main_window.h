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

private slots:

    void on_SignInButton_clicked();
    void on_NoAccountButton_clicked();

private:
    Ui::MainWindow *ui;
    Main_Menu *window;
    Registration *registration;
};
#endif // MAINWINDOW_H
