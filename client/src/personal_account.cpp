#include "../include/personal_account.h"
#include "./ui_personal_account.h"
#include "../include/main_menu.h"
#include "../include/info_window.h"
#include "../include/vocabulary_list.h"


PersonalAccount::PersonalAccount(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PersonalAccount)
{
    ui->setupUi(this);
    ui->BackButton->setIcon(QIcon(":arrow.png"));
    ui->NewWordButton->setIcon(QIcon(":word.png"));

    info = new info_window;
    vocabulary = new vocabulary_list;

    ui->WordWidget->setGeometry(54, 14, 170, 90);
    ui->WhiteWidget->setGeometry(52, 12, 175, 95);

    ui->WordWidget->hide();
    ui->WhiteWidget->hide();

    ui->AddButton_2->setStyleSheet("border-image: url(:card.png)");
    ui->VocabularyButton->setStyleSheet("border-image: url(:card.png)");
    ui->FlashcardsButton->setStyleSheet("border-image: url(:card.png)");


    QObject::connect(info, SIGNAL(close_personal_account()), this, SLOT(close_window()));
//    QObject::connect(desk, SIGNAL(open_personal_account()), this, SLOT(open_window()));

    QObject::connect(vocabulary, SIGNAL(open_personal_account()), this, SLOT(open_window()));
}


PersonalAccount::~PersonalAccount()
{
    delete ui;
}

void PersonalAccount::close_window(){
    this->hide();
    desk = new flashcards_desk;
    QObject::connect(desk, SIGNAL(open_personal_account()), this, SLOT(open_window()));
    desk->show();
    desk->TestYourself();
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

void PersonalAccount::on_VocabularyButton_clicked(){
    emit signal_for_vocabulary();

    this->hide();
}

void PersonalAccount::on_NewWordButton_clicked(){
    ui->WordWidget->show();
    ui->WhiteWidget->show();
}

QString PersonalAccount::get_new_word(){
    return ui->Word_2->text();
}

void PersonalAccount::on_AddButton_2_clicked(){
    emit signal_for_new_word_cards();
    ui->WordWidget->hide();
    ui->WhiteWidget->hide();
}

void PersonalAccount::open_window(){
    this->show();
    delete desk;
    desk = new flashcards_desk;
}

void PersonalAccount::set_username_label(){
    QString username = m_username;
    ui->Username->setText(m_username);
    ui->Username->adjustSize();
    ui->Username->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}


