#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

class QWebSocketServer;
class QWebSocket;


///
/// \class      WebSocketServer
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Web server socket for communication
///             with browser plugin
///
class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(uint32_t port, QObject *parent = Q_NULLPTR);
    ~WebSocketServer();

signals:
    void closed();
    void MessageReceived(QString message,QWebSocket *pClient);

private slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> mClients;
};

#endif // WEBSOCKETSERVER_H
