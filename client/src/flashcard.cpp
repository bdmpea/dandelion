#include "./ui_flashcard.h"
#include "flashcard.h"
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QDialogButtonBox>


flashcard::flashcard(QWidget *parent) :
    QWidget(parent),
    card_ui(new Ui::flashcard)
{
    card_ui->setupUi(this);

    flashcard_button = new button;
    flashcard_button->setStyleSheet("#widget {background-color: #ffffff; border-image: url(C:/Users/Honor/Desktop/dandelion/resources/card.png); background-position: center;}");
    flashcard_button->set_flashcard(this);
}


flashcard::~flashcard()
{
    delete card_ui;
    delete flashcard_button;
}


QString flashcard::get_word() const{
    return word;
}


void flashcard::set_word(const QString new_word) {
    this->word = new_word;
    card_ui->Word->setText(new_word);
    flashcard_button->set_lable(new_word);
    QObject::connect(flashcard_button, SIGNAL(clicked(bool)), this, SLOT(show_card()));
}


QString flashcard::get_meaning() const{
    return meaning;
}


void flashcard::set_meaning(const QString new_meaning) {
    this->meaning = new_meaning;
    card_ui->Meaning->setText(new_meaning);
}


void flashcard::create_form_to_delete_card(){
    int result = QMessageBox::warning(0, tr("Warning!"),
                                 tr("Are you sure you wish to delete this flashcard?"),
                                 QMessageBox::Ok | QMessageBox::Cancel);

    if (result == QMessageBox::Ok){
        emit delete_card(this);            // triggers function delete_card in cards_for_training.cpp to delete the flashcard

    }
}


bool flashcard::create_form_to_edit_card(){
    QDialog dialog(0);

    QFormLayout form(&dialog);

    form.addRow(new QLabel("Word"));
    QPlainTextEdit *word_field = new QPlainTextEdit(word);
    form.addRow(word_field);

    form.addRow(new QLabel("Meaning"));
    QPlainTextEdit* meaning_field = new QPlainTextEdit(meaning);
    form.addRow(meaning_field);

    QDialogButtonBox button_box(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&button_box);
    QObject::connect(&button_box, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&button_box, SIGNAL(rejected()), &dialog, SLOT(reject()));

    bool is_context_changed = false;
    if (dialog.exec() == QDialog::Accepted){
        if (this->get_word() != word_field->toPlainText()){
            this->set_word(word_field->toPlainText());
            is_context_changed = true;
        }
        if (this->get_meaning() != meaning_field->toPlainText()){
            this->set_meaning(meaning_field->toPlainText());
            is_context_changed = true;
        }
    }
    return is_context_changed;
}


void flashcard::show_card(){
    this->show();
}


void flashcard::on_EditButton_clicked(){
    this->hide();
    if (create_form_to_edit_card() == true){
        emit context_changed(true);           // triggers function set_cards_modified in cards_for_training.cpp to modify flashcards
    }
    this->show();
}


void flashcard::on_DeleteButton_clicked(){
    create_form_to_delete_card();
}




