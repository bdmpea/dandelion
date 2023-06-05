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
    void signal_for_registration();
    void signal_info_for_authorization();
public slots:
     void on_RegistrationButton_clicked();
     void register_user(const QString &status);
private:
    Ui::Registration *ui;
    Welcome *welcome_window;
};
#endif // REGISTRATION_H
