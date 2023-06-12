#ifndef FLASHCARDS_DESK_H
#define FLASHCARDS_DESK_H
#include <QWidget>
#include <QMainWindow>
#include <QString>
#include "cards_for_training.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include "training.h"

namespace Ui {
class flashcards_desk;
}

// shows all users flashcards

class flashcards_desk : public QMainWindow
{
    Q_OBJECT

public:
    explicit flashcards_desk(QMainWindow *parent = nullptr);
    ~flashcards_desk();


public slots:
    void display();

private slots:
    void on_AddNewCardButton_clicked();
    void on_TestYourselfButton_clicked();
    void on_TestYourselfFinished();
    void on_TestProgressUpdate();
    void on_StopTestYourselfButton_clicked();

private:
    Ui::flashcards_desk *ui;
    cards_for_training cards;
    QGridLayout *scroll_area;
    QHBoxLayout *training_display_layout;
    QWidget *training_display_widget;
    training *training_display;

    int window_border = 32;
    int space_between_cards = 24;
    int flashcard_height = 432;

    void update_display_window();
    void update_window_title();
    void clear_scroll_area();
};

#endif // FLASHCARDS_DESK_H
