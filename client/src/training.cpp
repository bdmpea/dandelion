#include "training.h"
#include "ui_training.h"
#include <algorithm>
#include <QDebug>
#include <QMessageBox>
#include <QEasingCurve>

training::training(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::training)
{
    ui->setupUi(this);
}


training::~training()
{
    delete ui;
}


void training::set_desk(cards_for_training* new_desk) {
    training_desk = new_desk;
    shuffled_sequence.clear();
    return;
}


void training::start_training() {
    for (int i = 0; i < training_desk->cards.length(); i++) {
        shuffled_sequence.append(i);
    }
    std::random_shuffle(shuffled_sequence.begin(), shuffled_sequence.end());

    current_sequence_index = 0;
    right_cards = 0;
    wrong_cards = 0;
    ui->ButtonWidget->setVisible(false);
    current_card = training_desk->cards[ shuffled_sequence[current_sequence_index] ];
    state = question;

    flip_card.setParent(ui->CardWidget);
    flip_card.setTargetObject(ui->CardWidget);
    flip_card.setPropertyName("maximumSize");
    flip_card.setStartValue(QSize(800, 600));
    flip_card.setEndValue(QSize(0, 600));
    flip_card.setDuration(100);
    connect(&flip_card, SIGNAL(finished()), this, SLOT(animation_finished()) );
    update_card_appearance();
}


void training::advance_training() {
    if (state == question) {
        state = answer;
        play_flip_card();
    }
    else if (state == answer && current_sequence_index < shuffled_sequence.length()-1) {
        current_sequence_index++;
        state = question;
        current_card = training_desk->cards[shuffled_sequence[current_sequence_index]];
        play_flip_card();
    }
    else {
        emit test_finished();
    }
    emit progress_update();
}


void training::update_card_appearance() {
    if (state == question) {
        ui->CardWidget->setStyleSheet("#training {background-color: white; border-image: url(card.png)}");
        ui->PromptLabel->setText(current_card->get_word());
    }
    else {
        ui->CardWidget->setStyleSheet("#training {background-color: white; border-image: none}");
        ui->PromptLabel->setText(current_card->get_meaning());
    }
}



void training::play_flip_card() {
    flip_card.setEasingCurve(QEasingCurve::OutCubic);
    flip_card.setDirection(QPropertyAnimation::Forward);
    ui->PromptLabel->setVisible(false);
    ui->ButtonWidget->setVisible(false);
    flip_card.start();
}


void training::mousePressEvent(QMouseEvent* event) {
    if (state == question) {
        advance_training();
    }
    else {
        return;
    }
}


void training::animation_finished() {
    if (flip_card.direction() == QPropertyAnimation::Forward) {
        flip_card.setEasingCurve(QEasingCurve::InCubic);
        flip_card.setDirection(QPropertyAnimation::Backward);
        update_card_appearance();
        flip_card.start();
    }
    else if (flip_card.direction() == QPropertyAnimation::Backward) {
        ui->PromptLabel->setVisible(true);
        if (state == question) {
            ui->ButtonWidget->setVisible(false);
        }
        else {
            ui->ButtonWidget->setVisible(true);
        }
    }
}



void training::on_KnowButton_clicked() {
    right_cards++;
    advance_training();
}


void training::on_NotKnowButton_clicked() {
    wrong_cards++;
    advance_training();
}
