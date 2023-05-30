#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QMainWindow>
#include "welcome.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Registration;
}
QT_END_NAMESPACE

class Registration : public QMainWindow
{
    Q_OBJECT

public:
     Registration(QWidget *parent = nullptr);
    ~Registration();

     QString get_username();
     QString get_password();

signals:
private slots:
     void on_RegistrationButton_clicked();

private:
    Ui::Registration *ui;
    Welcome *welcome_window;
};
#endif // REGISTRATION_H
