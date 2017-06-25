#include "websocketserver.h"

#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <iostream>

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
/// \param[in]  port   Server port
/// \param[in]  parent Parent widget
///
WebSocketServer::WebSocketServer(uint32_t port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("vidulus"),QWebSocketServer::NonSecureMode, this)),
    mClients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        std::cout << "WebSocketServer::WebSocketServer: listening on port: " << port << std::endl;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
WebSocketServer::~WebSocketServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(mClients.begin(), mClients.end());
}

///
/// \brief      New connection has been established
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);
    mClients << pSocket;
}

///
/// \brief      Client message has been received
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
/// \param[in]  message     Message of the client
///
void WebSocketServer::processTextMessage(QString message)
{
    std::cout << "WebSocketServer::processTextMessage: " << message.toStdString() << std::endl;
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    MessageReceived(message,pClient);
}

///
/// \brief      Client message has been received
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
/// \param[in]  message     Message of the client
///
void WebSocketServer::processBinaryMessage(QByteArray message)
{
    std::cout << "Binary Message received:" << message.toStdString() << std::endl;

    //QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    //    if (pClient) {
    //    pClient->sendBinaryMessage(message);
    //}
}

///
/// \brief      Client disconnected
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    std::cout <<  "socketDisconnected:" << pClient->localAddress().toString().toStdString() << std::endl;
    if (pClient) {
        mClients.removeAll(pClient);
        pClient->deleteLater();
    }
}
