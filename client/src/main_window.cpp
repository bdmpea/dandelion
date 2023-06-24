#include "../include/main_window.h"
#include "./ui_main_window.h"
#include "QMessageBox"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    emit signal_info_for_authorization();
}

void MainWindow::register_(){
    emit signal_info_for_registration();
}

void MainWindow::authorize(const QString &status){
    if (status == "success"){
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
    connect(registration, SIGNAL(signal_for_registration()),  this , SLOT(register_()));
    registration->show();
}

