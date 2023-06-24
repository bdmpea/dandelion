#include "vocabulary_list.h"
#include "ui_vocabulary_list.h"
//#include <QEditeLine>


vocabulary_list::vocabulary_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vocabulary_list)
{
    ui->setupUi(this);

    scroll_area = new QGridLayout();
    display();
}

vocabulary_list::~vocabulary_list()
{
    delete ui;
}
/*
void vocabulary_list::start(){
    QWidget *new_word = new QWidget;
    scroll_area->addWidget(new_word, 165, 165);
    ui->scrollAreaWidgetContents->setLayout(scroll_area);
    ui->scrollArea->show();
    //ui->scrollArea->setWidget(new_word);
}*/

void vocabulary_list::display(){
    QWidget *new_word = new QWidget;
    new_word->resize(100, 100);
    scroll_area->addWidget(new_word, 10, 10);
    ui->scrollAreaWidgetContents->setLayout(scroll_area);
    ui->scrollArea->show();
}
