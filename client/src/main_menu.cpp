#include "main_menu.h"
#include "./ui_main_menu.h"

Main_Menu::Main_Menu(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Menu)
{
    ui->setupUi(this);
    QIcon icon("://resources/home.png");
    ui->HomeButton->setIcon(icon);
}


Main_Menu::~Main_Menu()
{
    delete ui;
}


void Main_Menu::on_HomeButton_clicked()
{
    personal_account = new PersonalAccount();
    personal_account->show();
    this->hide();
}

