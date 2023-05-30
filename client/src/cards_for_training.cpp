#include <QObject>
#include <ui_flashcard.h>
#include "cards_for_training.h"
#include "flashcard.h"

cards_for_training::cards_for_training(QObject *parent) :
    QObject(parent),
    modified_flag(false){}

bool cards_for_training::is_cards_modified(){
    return modified_flag;
}


void cards_for_training::clear(){
    for (auto *card: cards){
        delete card;
    }
    cards.clear();
}


void cards_for_training::add_new_card(){
    flashcard* new_flashcard = new flashcard;
    new_flashcard->set_word("");
    if ( new_flashcard->create_form_to_edit_card() == true ){
        cards.append(new_flashcard);
        QObject::connect(new_flashcard, SIGNAL(delete_card(flashcard*)), this, SLOT(delete_one_card(flashcard*)));
        QObject::connect(new_flashcard, SIGNAL(context_changed(bool)), this, SLOT(set_cards_modified(bool)));
        set_cards_modified(true);
    }

}


void cards_for_training::set_cards_modified(bool modified){
    modified_flag = modified;
}


void cards_for_training::delete_one_card(flashcard *card){
    cards.removeOne(card);
    delete card;
    emit update_display_after_deleting();
    set_cards_modified(true);
}
