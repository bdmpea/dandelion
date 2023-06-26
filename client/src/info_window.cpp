 #include "../include/info_window.h"
#include "ui_info_window.h"

info_window::info_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::info_window)
{
    ui->setupUi(this);
    ui->OkButton->setStyleSheet("border-image: url(:card.png)");

}

info_window::~info_window()
{
    delete ui;
}


int info_window::get_flashcards_number() {
    return (ui->CardsNumber->text()).toInt();
}


void info_window::on_OkButton_clicked(){
    this->hide();
    emit close_personal_account();
}



