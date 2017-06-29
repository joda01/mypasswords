///
/// \file       pluginwebsocket.cpp
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
/// \brief      Web socket plugin to communicate via Socket
///

#include "pluginwebsocket.h"
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include "../core/NativeFunctions/NativeFunctions.h"
#include <QMessageBox>

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
PluginWebSocket::PluginWebSocket(PasswordContainerThread* passContainer,Configuration* config):
    mConfig(config),
    mContainer(passContainer),
    mAuthentTokenCounter(0)
{
    //----------------------------------------------------
    // Connect server
    mServer = new WebSocketServer(mConfig->GetPluginPort());
    connect(mServer,SIGNAL(MessageReceived(QString,QWebSocket*)),this,SLOT(on_serverMsgReceived(QString,QWebSocket*)));
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
PluginWebSocket::~PluginWebSocket()
{
    delete mServer;
}

///
/// \brief      Message from the server received
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
void PluginWebSocket::on_serverMsgReceived(QString msg, QWebSocket *client)
{
    if(true==mConfig->GetBrowserPluginEnable()){
        // Parse document
        QJsonDocument doc(QJsonDocument::fromJson(msg.toUtf8()));

        // Get JSON object
        QJsonObject json = doc.object();
        QString minVersion = json["MinVersion"].toString();     // Check if the plugin is compatible to the application

        if(minVersion=="1.1")
        {
            QString command = json["Message"].toObject()["Command"].toString();
            QString url = json["Message"].toObject()["URL"].toString();
            QString token = json["Token"].toString();

            // Access properties
            //std::cout <<"Author: " << json["Author"].toString().toStdString() << std::endl;
            //std::cout <<"Token: " << json["Token"].toString().toStdString() << std::endl;

            if(mConfig->GetPluginToken().size()>0){
                if(mConfig->GetPluginToken() == token){
                    mAuthentTokenCounter = 0;
                    // Perform an auto type
                    if(command == mCmdAutoType){
                        url = url.replace("http://","");
                        url = url.replace("www.","");
                        url = url.replace("https://","");
                        int lastPoint = url.lastIndexOf(".");
                        if(lastPoint<0) lastPoint = url.size();
                        int lastFrage = url.lastIndexOf("?");
                        if(lastFrage<0) lastFrage = url.size();
                        int firstSlash = url.indexOf('/');
                        if(firstSlash<0) firstSlash = url.size();
                        if(lastFrage < lastPoint) lastPoint = lastFrage;
                        if(firstSlash < lastPoint) lastPoint = firstSlash;
                        url = url.mid(0,lastPoint);
                        std::cout << "URL: " << url.toStdString() << std::endl;

                        std::vector<Container::PasswordEntry*> result = mContainer->SelectFromURL(url.toStdString());
                        if(result.size()>0){
                            Container::PasswordEntry* entry = result.at(0);
                            uint32_t usrSize;char* username = entry->GetPasswordElement(0)->GetUsername(usrSize);
                            uint32_t passSize;char* password = entry->GetPasswordElement(0)->GetPassword(passSize);

                            QJsonObject returnMsg;
                            returnMsg.insert("status","found");
                            returnMsg.insert("returnCode","0");
                            returnMsg.insert("username",QString::fromUtf8(username,usrSize));
                            returnMsg.insert("password",QString::fromUtf8(password,passSize));
                            QJsonDocument returnDoc(returnMsg);
                            entry->GetPasswordElement(0)->ClosePassword();
                            client->sendTextMessage(QString(returnDoc.toJson()));
                        }else{
                            QJsonObject returnMsg;
                            returnMsg.insert("status","not_found");
                            returnMsg.insert("returnCode","-1");
                            QJsonDocument returnDoc(returnMsg);
                            client->sendTextMessage(QString(returnDoc.toJson()));
                        }
                    }
                }else{
                    QJsonObject returnMsg;
                    returnMsg.insert("status","Wrong token");
                    returnMsg.insert("returnCode","-2");
                    QJsonDocument returnDoc(returnMsg);
                    client->sendTextMessage(QString(returnDoc.toJson()));

                    mAuthentTokenCounter++;
                    if(mAuthentTokenCounter > MAX_TOKEN_TRY){
                        mContainer->Lock();
                        mAuthentTokenCounter = 0;
                    }
                    std::cout << "PluginWebSocket::on_serverMsgReceived: Wrong token!" << std::endl;
                }
            }
            else{
                std::cout << "PluginWebSocket::on_serverMsgReceived: No token!" << std::endl;
            }
        }
    }else{
        std::cout << "PluginWebSocket::on_serverMsgReceived: Version of plugin does not match!" << std::endl;
        QMessageBox::information(0,tr("Plugin..."),tr("Please update browser plugin!"));
    }
    client->close();
}
