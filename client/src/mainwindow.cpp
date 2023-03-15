#include "mainwindow.h"
#include "./ui_mainwindow.h"
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




void MainWindow::on_pushButton_clicked()
{
   QString username = ui->username->text();
   QString password = ui->password->text();
   //check_signing
   if (username == "1" && password == "1"){
       hide();
       window = new Main_Menu(this);
       window->show();
   } else {
       QMessageBox::warning(this, "signing in", "ooops, try again!");
   }
}


void MainWindow::on_pushButton_2_pressed()
{
    hide();
    registration = new Registration(this);
    registration->show();
}

