#include "../include/main_menu.h"
#include "./ui_main_menu.h"
#include <QDataStream>
#include <QMessageBox>

Main_Menu::Main_Menu(QMainWindow *parent) :
        QMainWindow(parent),
        ui(new Ui::Main_Menu)
{
    ui->setupUi(this);
    ui->HomeButton->setIcon(QIcon(":home.png"));
    ui->NewWordButton->setIcon(QIcon(":word.png"));
    personal_account = new PersonalAccount();
    connect(personal_account, SIGNAL(signal_for_vocabulary()), this, SLOT(get_words_from_vocabulary()));
    connect(personal_account, SIGNAL(signal_for_new_word_cards()), this, SLOT(add_new_word_cards()));
    balda_game = new balda;

    ui->WordWidget->hide();
    ui->WhiteWidget->hide();

    ui->WordWidget->setGeometry(54, 14, 170, 90);
    ui->WhiteWidget->setGeometry(52, 12, 175, 95);


    ui->BaldaButton->setStyleSheet("border-image: url(card.png)");
    ui->WhoAmIButton->setStyleSheet("border-image: url(:card.png)");
    ui->WordChainButton->setStyleSheet("border-image: url(:card.png)");
    ui->FillwordsButton->setStyleSheet("border-image: url(:card.png)");
    ui->AddButton->setStyleSheet("border-image: url(:card.png)");

    QObject::connect(balda_game, SIGNAL(open_main_menu()), this, SLOT(open_window()));
}


Main_Menu::~Main_Menu()
{
    delete ui;
}


void Main_Menu::on_HomeButton_clicked()
{
    personal_account->m_username =  username;
    personal_account->set_username_label();
    personal_account->show();
    this->hide();
}

void Main_Menu::on_BaldaButton_clicked()
{
    balda_game->show();
    this->hide();
}


void Main_Menu::on_NewWordButton_clicked(){
    ui->WordWidget->show();
    ui->WhiteWidget->show();
}

QString Main_Menu::get_new_word(){
    return ui->Word->text();
}

void Main_Menu::add_new_word_cards() {
    emit signal_for_new_word_cards();
}

void Main_Menu::get_words_from_vocabulary() {
     emit signal_get_words_from_vocabulary_();
}

void Main_Menu::on_AddButton_clicked(){
    emit signal_for_new_word();
    ui->WordWidget->hide();
    ui->WhiteWidget->hide();
}

void Main_Menu::open_window(){
    this->show();
}
