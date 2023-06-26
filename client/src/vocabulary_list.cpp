#include "../include/vocabulary_list.h"
#include "ui_vocabulary_list.h"
#include <QLabel>

QList<QList<QString>> TEST_2 = {{"cat", "a small mammal with soft fur"}, {"dog", "a mammal that has a long snout"}, {"commit", "perform an act with a negative connotation"},{"issue","some situation or event that is thought about"},
                             {"cat", "a small mammal with soft fur"}, {"dog", "a mammal that has a long snout"}, {"commit", "perform an act with a negative connotation"},{"issue","some situation or event that is thought about"},
                             {"cat", "a small mammal with soft fur"}, {"dog", "a mammal that has a long snout"}, {"commit", "perform an act with a negative connotation"},{"issue","some situation or event that is thought about"}};


vocabulary_list::vocabulary_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vocabulary_list)
{
    ui->setupUi(this);

    scroll_area = new QGridLayout();
   // display();

    ui->BackButton->setStyleSheet("border-image: url(:/arrow.png)");
}

vocabulary_list::~vocabulary_list()
{
    delete ui;
}

void vocabulary_list::display(QVector<QVector<QString>> &vocabulary){
    int columns = qMax( (this->size().width()) / (313 + 25), 1 );
    int total = 0;

    for (auto data: vocabulary) {
        QWidget *new_word= new QWidget;
        new_word->setStyleSheet("background-color:white;");
        new_word->setFixedSize(QSize(400, 50));

        QLabel *word = new QLabel(new_word);
        word->setText(data[0]);
        word->setGeometry(10,10,160,30);

        QLabel *meaning = new QLabel(new_word);
        meaning->setText(data[1]);
        meaning->setGeometry(180,10,210,30);

        int row = total / columns;
        int column = total % columns;
        scroll_area->addWidget(new_word, row, column);
        total++;
    }

    ui->scrollAreaWidgetContents->setLayout(scroll_area);
    ui->scrollArea->show();
}

void vocabulary_list::on_BackButton_clicked(){
    this->hide();
    emit open_personal_account();
}
