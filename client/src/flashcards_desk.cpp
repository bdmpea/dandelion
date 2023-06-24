#include "flashcards_desk.h"
#include "./ui_flashcards_desk.h"
#include <QMessageBox>
#include <QFileDialog>

flashcards_desk::flashcards_desk(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::flashcards_desk)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Flashcards"));

    scroll_area = new QGridLayout();

    training_display = new training();
    training_display_widget = new QWidget();
    training_display_layout = new QHBoxLayout();
    training_display_layout->addWidget(this->training_display);
    training_display_widget->setLayout(this->training_display_layout);
    ui->DisplayStackedWidget->addWidget(this->training_display_widget);

    //ui->DisplayStackedWidget->setCurrentWidget(ui->DisplayFlashcards);
    //ui->BottomStackedWidget->setCurrentWidget(ui->BottomStackedWidgetPage1);
    //ui->TopStackedWidget->setCurrentWidget(ui->TopStackedWidgetPage1);

    //training_display->set_desk(&cards); //TODO:
    ui->DisplayStackedWidget->setCurrentWidget(training_display_widget);
    ui->BottomStackedWidget->setCurrentWidget(ui->BottomStackedWidgetPage2);
    ui->TopStackedWidget->setCurrentWidget(ui->TopStackedWidgetPage2);
    ui->progressBar->setMaximum(cards.cards.length());
    ui->progressBar->setValue(1);
    //training_display->start_training();

    ui->StopTestYourselfButton->setStyleSheet("border-image: url(:/resources/card.png)");


    QObject::connect(&cards, SIGNAL(update_display_after_deleting()), this, SLOT(display()));
    QObject::connect(training_display, SIGNAL(test_finished()), this, SLOT(on_TestYourselfFinished()));
    QObject::connect(training_display, SIGNAL(progress_update()), this, SLOT(on_TestProgressUpdate()));
}

flashcards_desk::~flashcards_desk()
{
    delete ui;
}




void flashcards_desk::display() {
    int columns = qMax( (this->size().width() - window_border) / (flashcard_height + space_between_cards), 1 );
    int total = 0;

    for (auto *card: cards.cards){
        int row = total / columns;
        int column = total % columns;
        scroll_area->addWidget(card->flashcard_button, row, column);
        total++;
    }

    ui->ScrollAreaWidgetContents->setLayout(scroll_area);
    ui->TestYourselfButton->show();
}


void flashcards_desk::on_AddNewCardButton_clicked(){
    cards.add_new_card();
    display();
}


void flashcards_desk::on_TestYourselfButton_clicked() {
    if ( cards.cards.length() > 0 ){
        training_display->set_desk(&cards);
        ui->DisplayStackedWidget->setCurrentWidget(training_display_widget);
        ui->BottomStackedWidget->setCurrentWidget(ui->BottomStackedWidgetPage2);
        ui->TopStackedWidget->setCurrentWidget(ui->TopStackedWidgetPage2);
        ui->progressBar->setMaximum(cards.cards.length());
        ui->progressBar->setValue(1);
        training_display->start_training();
    }
    else {
        QMessageBox::information(this, tr("Test Yourself"),
                                 tr("Deck is empty. Please open a deck"));
    }
}


void flashcards_desk::on_TestYourselfFinished(){
    QMessageBox::information(this, "Result",
                 "You know " + QString::number(training_display->right_cards) +
                 " /" + QString::number(training_display->training_desk->cards.length()) + " cards");
    ui->DisplayStackedWidget->setCurrentWidget(ui->DisplayFlashcards);
    ui->BottomStackedWidget->setCurrentWidget(ui->BottomStackedWidgetPage1);
    ui->TopStackedWidget->setCurrentWidget(ui->TopStackedWidgetPage1);
}


void flashcards_desk::on_TestProgressUpdate() {
    ui->progressBar->setValue(training_display->current_sequence_index+1);
    //ui->progressBar->update();
}


void flashcards_desk::on_StopTestYourselfButton_clicked()
{
    on_TestYourselfFinished();
}


void flashcards_desk::update_display_window(){
    clear_scroll_area();
    display();
}


void flashcards_desk::clear_scroll_area(){
    if (scroll_area->layout() != nullptr ){
        QLayoutItem* item;
        while ( ( item = scroll_area->layout()->takeAt( 0 ) ) != nullptr ){
            delete item->widget();
            delete item;
        }
    }
}









