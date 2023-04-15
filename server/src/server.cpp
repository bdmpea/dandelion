#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>

void server::slotReadClient()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());

    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    while(true) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QString   login;
        QString password_1;
        QString password_2;
        in >> login >> password_1 >> password_2;

        m_nNextBlockSize = 0;

//        sendToClient(pClientSocket,
//                     "Server Response: Received"
//                    );
    }
}



/*virtual*/ void server::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    sockets.push_back(pClientSocket);  // push new socket;
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );

   // sendToClient(pClientSocket, "Server Response: Connected!");
}


