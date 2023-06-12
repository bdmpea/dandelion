#include "main_menu.h"
#include "./ui_main_menu.h"

Main_Menu::Main_Menu(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Menu)
{
    ui->setupUi(this);
    ui->HomeButton->setIcon(QIcon(":/resources/home.png"));

    personal_account = new PersonalAccount();


}


Main_Menu::~Main_Menu()
{
    delete ui;
}


void Main_Menu::on_HomeButton_clicked()
{

    personal_account->show();
    this->hide();
}

void Main_Menu::on_BaldaButton_clicked()
{
    balda_game = new balda;

    balda_game->show();
    this->hide();
    //balda_game->start();
}


