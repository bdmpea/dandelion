#include "main_menu.h"
#include "./ui_main_menu.h"

Main_Menu::Main_Menu(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Menu)
{
    ui->setupUi(this);
}

Main_Menu::~Main_Menu()
{
    delete ui;
}
