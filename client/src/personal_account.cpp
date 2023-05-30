#include "personal_account.h"
#include "./ui_personal_account.h"
#include "main_menu.h"
#include "flashcards_desk.h"


PersonalAccount::PersonalAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalAccount)
{
    ui->setupUi(this);
    QIcon icon("C:/Users/Honor/Desktop/dandelion/resources/arrow.png");
    ui->BackButton->setIcon(icon);
}


PersonalAccount::~PersonalAccount()
{
    delete ui;
}


void PersonalAccount::on_BackButton_clicked()
{
    hide();
    Main_Menu *main_window = new Main_Menu();
    main_window->show();
}



void PersonalAccount::on_FlashcardsButton_clicked()
{
    this->hide();
    flashcards_desk *desk = new flashcards_desk;
    desk->show();
}

