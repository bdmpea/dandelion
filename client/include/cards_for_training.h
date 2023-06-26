#ifndef CARDS_FOR_TRAINING_H
#define CARDS_FOR_TRAINING_H

#include <QObject>
#include <QTextStream>
#include <QList>
#include <QFile>
#include <QString>
#include "flashcard.h"

// class cards_for_training holds all flashcards for current training

class cards_for_training : public QObject
{
    Q_OBJECT

public:
    explicit cards_for_training(QObject *parent = nullptr);
    QList <flashcard *> cards;

    bool is_cards_modified();
    void clear();
    void add_new_card();
    void show_all_cards();
    void create(QList<QList<QString>> users_data);

signals:
    void update_display_after_deleting();

public slots:
    void set_cards_modified(bool modified);
    void delete_one_card(flashcard* card);

private:
    bool modified_flag;
};

#endif // CARDS_FOR_TRAINING_H
