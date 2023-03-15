#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QMainWindow>
#include "main_menu.h"
namespace Ui {
class Welcome;
}

class Welcome : public QMainWindow
{
    Q_OBJECT

public:
    Welcome(QMainWindow *parent = nullptr);
    ~Welcome();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Welcome *ui;
    Main_Menu *main_window;
};

#endif // WELCOME_H
