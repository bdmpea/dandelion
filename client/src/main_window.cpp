#include "../include/main_window.h"
#include "./ui_main_window.h"
#include "QMessageBox"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->SignInButton->setStyleSheet("border-image: url(:card.png)");
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::get_username() {
    return ui->username->text();
}

QString MainWindow::get_password() {
    return ui->password->text();
}

void MainWindow::on_SignInButton_clicked() {
    emit signal_info_for_authorization();
}

void MainWindow::register_() {
    emit signal_info_for_registration();
}

void MainWindow::add_new_word_() {
    emit signal_info_for_new_word();
}

void MainWindow::add_new_word_cards() {
    emit signal_info_for_new_word_cards();
}

void MainWindow::get_new_words_() {
    emit signal_for_new_words_from_vocabulary();
}

void MainWindow::authorize(const QString &status, const QString &username_) {
    if (status == "success") {
        hide();
        window = new Main_Menu(this);
        window->username = username_;
        connect(window, SIGNAL(signal_for_new_word()), this, SLOT(add_new_word_()));
        connect(window, SIGNAL(signal_for_new_word_cards()), this, SLOT(add_new_word_cards()));
        connect(window, SIGNAL(signal_get_words_from_vocabulary_()), this, SLOT(get_new_words_()));
        window->show();
    } else {
        QMessageBox::warning(this, "signing in", "ooops, try again!");
    }
}

void MainWindow::on_NoAccountButton_clicked() {
    hide();
    registration = new Registration(this);
    connect(registration, SIGNAL(signal_for_registration()), this, SLOT(register_()));
    registration->show();
}

