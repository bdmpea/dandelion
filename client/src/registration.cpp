#include "registration.h"
#include "./ui_registration.h"
#include <QMessageBox>
Registration::Registration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    ui->RegistrationButton->setStyleSheet("border-image: url(:/resources/card.png)");

}


Registration::~Registration()
{
    delete ui;
}

QString Registration::get_username(){
    return ui->username->text();
}
QString Registration::get_password(){
    return ui->password->text();
}


void Registration::on_RegistrationButton_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    QString password_2 = ui->password_2->text();
    if (password != password_2){
        QMessageBox::warning(this, "registration", "passwords are not the same:(");
    }
    if (username == "1" && password == "1" && password_2 == "1"){
        hide();
        welcome_window = new Welcome(this);
        welcome_window->show();
    } else {
        QMessageBox::warning(this, "registration", "something went wrong, try again!");
    }
}

