#ifndef VOCABULARY_LIST_H
#define VOCABULARY_LIST_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include <QString>

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
    void display(QVector<QVector<QString>> &vocabulary);

signals:
    void open_personal_account();

private slots:
    void on_BackButton_clicked();

private:
    Ui::vocabulary_list *ui;
    QGridLayout *scroll_area;
};

#endif // VOCABULARY_LIST_H
