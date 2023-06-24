#include "main_window.h"
#include "./ui_main_window.h"
#include "QMessageBox"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SignInButton->setStyleSheet("border-image: url(:/resources/card.png)");

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::get_username(){
    return ui->username->text();
}

QString MainWindow::get_password(){
    return ui->password->text();
}

void MainWindow::on_SignInButton_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    //check_signing
    if (username == "" && password == ""){
        hide();
        window = new Main_Menu(this);
        window->show();
    } else {
        QMessageBox::warning(this, "signing in", "ooops, try again!");
    }
}


void MainWindow::on_NoAccountButton_clicked()
{
    hide();
    registration = new Registration(this);
    registration->show();
}

