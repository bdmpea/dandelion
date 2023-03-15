#include "registration.h"
#include "./ui_registration.h"
#include <QMessageBox>
Registration::Registration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_pushButton_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    QString password_2 = ui->password_2->text();
    //check_registration(username) этой функцией мы хотим проверять
    if (username == "1" && password == "1" && password_2 == "1"){
        hide();
        welcome_window = new Welcome(this);
        welcome_window->show();
    } else {
        QMessageBox::warning(this, "registration", "this username exists, try again!");
    }
}

