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

public slots:
     void on_NewLetterButton_clicked();
     void on_NewWordButton_clicked();
     void on_AddButton_clicked();
     QString get_new_word();


private:
    Ui::balda *ui;
    QGraphicsScene *scene;
    QList<letter *> letter_list;
    int x_last_letter_position;
    int y_last_letter_position;
};

#endif // BALDA_H
