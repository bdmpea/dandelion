#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QString>
#include "button.h"

namespace Ui {
    class flashcard;
}

// holds flashcards's information

class flashcard : public QWidget
{
Q_OBJECT

public:
    Ui::flashcard *card_ui;
    button *flashcard_button;
    QGridLayout *layout;

    explicit flashcard(QWidget *parent = nullptr);
    ~flashcard();


    QString get_word() const;
    void set_word(const QString new_word);

    QString get_meaning() const;
    void set_meaning(const QString new_meaning);

    void create_form_to_delete_card();
    bool create_form_to_edit_card();


signals:
    void context_changed(bool changed);
    void delete_card(flashcard *card);

private slots:
    void show_card();
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();

private:
    QString word;
    QString meaning;
    QString border_image = "border-image: url(:../resources/card.png)";
};

#endif // FLASHCARD_H