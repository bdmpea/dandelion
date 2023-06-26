#ifndef BALDA_H
#define BALDA_H

#include <QWidget>
#include <QGraphicsScene>
#include "letter.h"


namespace Ui {
class balda;
}

class balda : public QWidget

{
    Q_OBJECT

public:
    explicit balda(QWidget *parent = nullptr);
    ~balda();

    QString get_new_word();

signals:
    void open_main_menu();

public slots:
     void on_NewLetterButton_clicked();
     void on_NewWordButton_clicked();
     void on_AddButton_clicked();

private slots:
     void on_GiveUpButton_clicked();

private:
    Ui::balda *ui;
    QGraphicsScene *scene;
    QList<letter *> letter_list;
    int x_last_letter_position;
    int y_last_letter_position;
};

#endif // BALDA_H
