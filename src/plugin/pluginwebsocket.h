///
/// \file       pluginwebsocket.h
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
/// \brief      Web socket plugin to communicate via Socket
///

#ifndef PLUGINWEBSOCKET_H
#define PLUGINWEBSOCKET_H

#include <QString>
#include <QWebSocket>
#include "../web/websocketserver.h"
#include "../guiQt/configuration.h"
#include "../guiQt/passwordcontainerthread.h"

///
/// \class      PluginWebSocket
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
/// \brief      Web socket plugin
///
class PluginWebSocket : public QObject
{
    Q_OBJECT

public:
    PluginWebSocket(PasswordContainerThread* passContainer,Configuration* config);
    ~PluginWebSocket();

public slots:
    void on_serverMsgReceived(QString msg,QWebSocket* client);

private:
    //////////////////////////////////////////////
    /// Constants
    QString mCmdAutoType = QString("AutoType");
    static const int MAX_TOKEN_TRY = 3;     ///< After 3 wrong received tokens the container will be locked

    //////////////////////////////////////////////
    /// Members
    Configuration*   mConfig;
    PasswordContainerThread* mContainer;
    WebSocketServer* mServer;
    int mAuthentTokenCounter;
};

#endif // PLUGINWEBSOCKET_H
