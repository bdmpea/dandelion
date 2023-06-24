#include "welcome.h"
#include "ui_welcome.h"

Welcome::Welcome(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    ui->ContinueButton->setStyleSheet("border-image: url(:/resources/card.png)");

}


Welcome::~Welcome()
{
    delete ui;
}


void Welcome::on_ContinueButton_clicked()
{
    hide();
    main_window = new Main_Menu(this);
    main_window->show();
}

