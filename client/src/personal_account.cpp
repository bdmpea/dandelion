#include "personal_account.h"
#include "./ui_personal_account.h"
#include "main_menu.h"
#include "info_window.h"


PersonalAccount::PersonalAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalAccount)
{
    ui->setupUi(this);
    ui->BackButton->setIcon(QIcon(":/resources/arrow.png"));

    info = new info_window;
    desk = new flashcards_desk;

    QObject::connect(info, SIGNAL(close_personal_account()), this, SLOT(close_window()));
}


PersonalAccount::~PersonalAccount()
{
    delete ui;
}

void PersonalAccount::close_window(){
    this->hide();
    desk->show();
}


void PersonalAccount::on_BackButton_clicked()
{
    hide();
    Main_Menu *main_window = new Main_Menu();
    main_window->show();
}



void PersonalAccount::on_FlashcardsButton_clicked()
{
    info->show();
}



