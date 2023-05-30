#ifndef TRAINING_H
#define TRAINING_H

#include <QWidget>
#include "flashcard.h"
#include "cards_for_training.h"
#include <QList>
#include <QMouseEvent>
#include <QPropertyAnimation>

enum State{
    question,
    answer
};

// shows field for training with flashcards

namespace Ui {
class training;
}

class training : public QWidget
{
    Q_OBJECT

public:
    cards_for_training *training_desk;
    flashcard *current_card;
    QList<int> shuffled_sequence;
    State state;
    QPropertyAnimation flip_card;
    int right_cards;
    int wrong_cards;
    int current_sequence_index;

    explicit training(QWidget *parent = nullptr);
    ~training();

    void set_desk(cards_for_training* desk);
    void start_training();
    void advance_training();
    void update_card_appearance();
    void play_flip_card();

    void mousePressEvent(QMouseEvent* event);

signals:
    void progress_update();
    void test_finished();

private slots:
    void animation_finished();
    void on_KnowButton_clicked();
    void on_NotKnowButton_clicked();

private:
    Ui::training *ui;
};

#endif // TRAINING_H
