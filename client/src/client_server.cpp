#include "client_server.h"
#include <QTime>
void client_server::slotReadyRead() // read from server
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) { break; } in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) { break; } QTime time; QString str; in >> time >> str;
        m_nNextBlockSize = 0;
    }
}
void client_server::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    //out <<

    out.device()->seek(0);
   // out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);


}
