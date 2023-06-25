#include "../include/registration.h"
#include "./ui_registration.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTcpSocket>
Registration::Registration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    ui->RegistrationButton->setStyleSheet("border-image: url(:card.png)");
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
  emit signal_for_registration();
}

void Registration::register_user(const QString &status){
    //    if (password != password_2){
//        QMessageBox::warning(this, "registration", "passwords are not the same:(");
//    }
    if (status == "success"){
        hide();
        welcome_window = new Welcome(this);
        welcome_window->show();
    } else {
        QMessageBox::warning(this, "registration", "something went wrong, try again!");
    }
}

