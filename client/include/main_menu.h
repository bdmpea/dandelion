#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <QMainWindow>
#include "personal_account.h"

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

private slots:
    void on_HomeButton_clicked();

private:
    Ui::Main_Menu *ui;
    PersonalAccount *personal_account;
};

#endif // MAIN_MENU_H
