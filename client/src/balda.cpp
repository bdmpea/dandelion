#include "balda.h"
#include "qgraphicsitem.h"
#include "qlabel.h"
#include "ui_balda.h"
#include <QTableWidgetItem>
#include <QPushButton>
#include <QMessageBox>

balda::balda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::balda)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    ui->graphicsView->resize(836,512);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    QPixmap pixmap(":/field.png");
    ui->graphicsView->setBackgroundBrush(pixmap);

    for (int i = 0; i < 10; i++){
        letter *item = new letter;
        letter_list.append(item);
        QObject::connect(item, SLOT(itemMoved(letter*)), this, SIGNAL(a(letter*)));
        if (i < 5){
            item->setPos(600 + i*40, 35);

        } else {
            item->setPos(600 + (10 - i - 1)*40, 75);
        }
        scene->addItem(item);
    }
    scene->setSceneRect(0,0,820,500);

    x_last_letter_position = 600 + 9*40;
    y_last_letter_position = 75;

    ui->NewWordButton->setIcon(QIcon(":/word.png"));
    ui->WordWidget->setGeometry(658, 360, 170, 90);
    ui->WhiteWidget->setGeometry(655, 358, 176, 95);
    ui->WordWidget->hide();
    ui->WhiteWidget->hide();

    ui->NewLetterButton->setStyleSheet("border-image: url(:/card.png)");
    ui->AddButton->setStyleSheet("border-image: url(:/card.png)");
    ui->GiveUpButton->setStyleSheet("border-image: url(:/card.png)");


    //this->setStyleSheet("QMessageBox {color: white;}");
    this->setStyleSheet("QMessageBox QPushButton{border-image: url(:/card.png); min-width:  80px; max-width:  80px; min-height: 20px; max-height: 20px;}");
    this->setStyleSheet("QLabel {background-color:rgb(194,228,255); border-style: solid; border-width: 1px; border-color: black; }");

    set_start_word();
}

balda::~balda()
{
    delete ui;
}

void balda::on_NewLetterButton_clicked(){
    letter *item = new letter;
    letter_list.append(item);
    if (x_last_letter_position > 600 + 3*40){
        item->setPos(600, y_last_letter_position + 40);
        x_last_letter_position = 600;
        y_last_letter_position += 40;
    } else {
        item->setPos(x_last_letter_position + 40, y_last_letter_position);
        x_last_letter_position += 40;
    }
    scene->addItem(item);
}

void balda::on_NewWordButton_clicked(){
    ui->WordWidget->show();
    ui->WhiteWidget->show();
}

void balda::on_AddButton_clicked(){
    ui->WordWidget->hide();
    ui->WhiteWidget->hide();
}

QString balda::get_new_word(){
    return ui->Word->text();
}

void balda::on_GiveUpButton_clicked()
{
    QMessageBox::question(this, "dandelion", "Are you sure you want to finish the game?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (QMessageBox::Yes){
        this->hide();
        emit open_main_menu();
    }
}

void balda::set_start_word(){
    QString start_word = "IMPRESSION";
    for (int i = 0; i < 10; i++){
        QLabel *letter = new QLabel(this);
        letter->setText(start_word[i]);
        letter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        letter->setGeometry(42+i*51,225,30,30);
        QFont font = letter->font();
        font.setPointSize(15);
        letter->setFont(font);
    }
}
