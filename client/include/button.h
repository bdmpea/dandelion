#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPushButton>

class flashcard;

namespace Ui {
class button;
}

// class button displayes on the screen as a flashcard

class button : public QPushButton
{
    Q_OBJECT

public:
    explicit button(QPushButton *parent = nullptr);
    ~button();

    void set_lable(QString word);
    void set_flashcard(flashcard *new_card);


signals:
    bool clicked();

private slots:
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();


private:
    Ui::button *ui;
    flashcard *card;
};

#endif // BUTTON_H
