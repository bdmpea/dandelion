#ifndef VOCABULARY_LIST_H
#define VOCABULARY_LIST_H

#include <QWidget>
#include <QGridLayout>

namespace Ui {
class vocabulary_list;
}

class vocabulary_list : public QWidget
{
    Q_OBJECT

public:
    explicit vocabulary_list(QWidget *parent = nullptr);
    ~vocabulary_list();

    void start();
    void display();

private:
    Ui::vocabulary_list *ui;
    QGridLayout *scroll_area;

};

#endif // VOCABULARY_LIST_H
