м#ifndef SERVER_H
#define SERVER_H
#include <QMessageBox>
#include <QTcpServer>
#include <QWidget>
class QTcpServer;
class QTextEdit;
class QTcpSocket;

class server : public QWidget
{
Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
   // QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;
    QVector<QTcpSocket *> sockets;
    qint16 port;

public:
    server(int nPort, QWidget* pwgt =0) : QWidget(pwgt),m_nNextBlockSize(0)
    {
        m_ptcpServer = new QTcpServer(this);
        port = nPort;
        if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
            QMessageBox::critical(0,
                                  "Server Error",
                                  "Unable to start the server:"
                                  + m_ptcpServer->errorString()
                                 );
            m_ptcpServer->close();
            return;
        }
        connect(m_ptcpServer, SIGNAL(newConnection()),
                this,         SLOT(slotNewConnection())
               );
    }
    static server &getInstance(){
                static server server(2323);
                return server;
            }
    friend bool register_user(const QString &login, const QString &password);
    friend bool check_signing_in(const QString &login,const QString &password);
    void sendToClient(QTcpSocket* pSocket, const QString& str);

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
};

bool register_user(const QString &login, const QString &password){
        try {
          //  is_used_login(login);    use function hear to connect with BD
        }catch(std::exception &exp){
            return false;
        }
        //create_user(login, password);
        return true;
    }
bool check_signing_in(const QString &login,const QString &password){
        try {
          //  is_correct_account_login(login, password);   use function hear to connect with BD
        }catch(std::exception &exp){
            return false;
        }
        return true;
    }


#endif // SERVER_H
