#ifndef INFO_WINDOW_H
#define INFO_WINDOW_H

#include <QWidget>

namespace Ui {
class info_window;
}

class info_window : public QWidget
{
    Q_OBJECT

public:
    explicit info_window(QWidget *parent = nullptr);
    ~info_window();

    int get_flashcards_number();


signals:
    void close_personal_account();

private slots:
    void on_OkButton_clicked();

private:
    Ui::info_window *ui;
};

#endif // INFO_WINDOW_H
