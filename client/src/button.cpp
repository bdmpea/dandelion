#include "../include/button.h"
#include "ui_button.h"
#include "../include/flashcard.h"

button::button(QPushButton *parent) :
    QPushButton(parent),
    ui(new Ui::button)
{
    ui->setupUi(this);
}


button::~button()
{
    delete ui;
}


void button::set_lable(QString word){
    ui->Word->setText(word);
}


void button::set_flashcard(flashcard *new_card){
    card = new_card;
}


void button::on_EditButton_clicked(){
    card->create_form_to_edit_card();
}


void button::on_DeleteButton_clicked(){
    this->card->create_form_to_delete_card();
}

