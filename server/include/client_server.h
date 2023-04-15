#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H


#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class client_server : public QWidget {
Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;

public:
    client_server(const QString& ip, int nPort, QWidget* pwgt = 0) : QWidget(pwgt), m_nNextBlockSize(0){
        m_pTcpSocket = new QTcpSocket(this);
        m_pTcpSocket->connectToHost(ip, nPort);
        connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
        connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    }

private slots:
   void slotReadyRead();
   void slotSendToServer();
};

#endif // CLIENT_SERVER_H
