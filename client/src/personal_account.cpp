#include "personal_account.h"
#include "./ui_personal_account.h"
#include "main_menu.h"
#include "info_window.h"
#include "vocabulary_list.h"
#include <QLabel>


PersonalAccount::PersonalAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalAccount)
{
    ui->setupUi(this);
    ui->BackButton->setIcon(QIcon(":/arrow.png"));
    ui->NewWordButton->setIcon(QIcon(":/word.png"));

    info = new info_window;
    desk = new flashcards_desk;
    vocabulary = new vocabulary_list;

    ui->WordWidget->setGeometry(54, 14, 170, 90);
    ui->WhiteWidget->setGeometry(52, 12, 175, 95);

    ui->WordWidget->hide();
    ui->WhiteWidget->hide();

    ui->AddButton_2->setStyleSheet("border-image: url(:/card.png)");
    ui->VocabularyButton->setStyleSheet("border-image: url(:/card.png)");
    ui->FlashcardsButton->setStyleSheet("border-image: url(:/card.png)");


    QObject::connect(info, SIGNAL(close_personal_account()), this, SLOT(close_window()));
    QObject::connect(desk, SIGNAL(open_personal_account()), this, SLOT(open_window()));
    QObject::connect(vocabulary, SIGNAL(open_personal_account()), this, SLOT(open_window()));

    set_username_label();
}


PersonalAccount::~PersonalAccount()
{
    delete ui;
}

void PersonalAccount::close_window(){
    this->hide();
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
    vocabulary->show();
    vocabulary->display();
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
    ui->WordWidget->hide();
    ui->WhiteWidget->hide();
}

void PersonalAccount::open_window(){
    this->show();
    delete desk;
    desk = new flashcards_desk;
}

void PersonalAccount::set_username_label(){
    QString username = "aboba";             //здесь нужно взять имя юзера
    ui->Username->setScaledContents( false );
    ui->Username->setText(username);
    ui->Username->setAlignment(Qt::AlignCenter);
}
