///
/// \file       configuration.h
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
/// \brief      Class which stores th application configuration
///

#include <QStandardPaths>
#include <QUuid>

#include "configuration.h"


///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
Configuration::Configuration()
{
    QString settingsFile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/").append(".passwords/").append("config.ini");
    mSettings = new QSettings(settingsFile,QSettings::IniFormat);
}


///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
Configuration::~Configuration()
{
    delete mSettings;
}

void Configuration::SetFileHistory(QStringList& list){mSettings->setValue("fileHistory/openedFiles",list);}
QStringList Configuration::GetFileHistory(){return mSettings->value("fileHistory/openedFiles").toStringList();}
void Configuration::SetFileHistoryLastOpenIdx(int idx) {mSettings->setValue("fileHistory/lastFileIdx",idx);}
int Configuration::GetFileHistoryLastOpenIdx(){return mSettings->value("fileHistory/lastFileIdx",0).toUInt();}
void Configuration::SetKeySequenceOpenUrl(QKeySequence sequence){mSettings->setValue("shortcut/openUrl",sequence.toString());}
void Configuration::SetKeySequenceCopyUsername(QKeySequence sequence)   {mSettings->setValue("shortcut/copyUsername"   ,sequence.toString());}
void Configuration::SetKeySequenceCopyPassword(QKeySequence sequence)   {mSettings->setValue("shortcut/copyPassword"   ,sequence.toString());}
void Configuration::SetKeySequencePerformAutoType(QKeySequence sequence){mSettings->setValue("shortcut/performAutoType",sequence.toString());}
void Configuration::SetKeySequenceSave(QKeySequence sequence){mSettings->setValue("shortcut/save",sequence.toString());}
void Configuration::SetKeySequenceBringToFront(QKeySequence sequence){mSettings->setValue("shortcut/bringToFront",sequence.toString());}


QKeySequence Configuration::GetKeySequenceOpenUrl(){return QKeySequence::fromString(mSettings->value("shortcut/openUrl","Ctrl+t").toString());}
QKeySequence Configuration::GetKeySequenceCopyUsername(){return QKeySequence::fromString(mSettings->value("shortcut/copyUsername","Ctrl+u").toString());}
QKeySequence Configuration::GetKeySequenceCopyPassword(){return QKeySequence::fromString(mSettings->value("shortcut/copyPassword","Ctrl+p").toString());}
QKeySequence Configuration::GetKeySequencePerformAutoType(){return QKeySequence::fromString(mSettings->value("shortcut/performAutoType","F11").toString());}
QKeySequence Configuration::GetKeySequenceSave(){return QKeySequence::fromString(mSettings->value("shortcut/save","Ctrl+s").toString());}
QKeySequence Configuration::GetKeySequenceBringToFront(){return QKeySequence::fromString(mSettings->value("shortcut/bringToFront","F9").toString());}

void Configuration::SetMinimizeWhenLocking(bool enab){mSettings->setValue("program/minimizeWhenLocked",enab);}
void Configuration::SetLockWhenMinimized(bool enab){mSettings->setValue("program/lockWhenMinimized",enab);}
void Configuration::SetLockAfterTime(bool enab){mSettings->setValue("program/lockAfterTime",enab);}
void Configuration::SetTimeBeforeLocking(int time){mSettings->setValue("program/lockTime",time);}
void Configuration::SetOpenLastDbOnStartUp(bool enab){mSettings->setValue("program/openLastFileAtStartUp",enab);}
void Configuration::SetAutoSave(bool enab){mSettings->setValue("program/autoSave",enab);}
void Configuration::SetSecureDesktop(bool enab){mSettings->setValue("program/useSecureDesctop",enab);}
void Configuration::SetShowSubGroupElements(bool enab){mSettings->setValue("program/showSubGroups",enab);}
void Configuration::SetHideFromTaskbarWhenMinimized(bool enab){mSettings->setValue("program/hideFromTaskbarWhenMinimized",enab);}
bool Configuration::GetMinimizeWhenLocking() {return mSettings->value("program/minimizeWhenLocked").toBool();   }
bool Configuration::GetLockWhenMinimized()   {return mSettings->value("program/lockWhenMinimized").toBool();    }
bool Configuration::GetLockAfterTime()       {return mSettings->value("program/lockAfterTime").toBool();        }
int  Configuration::GetTimeBeforeLocking()   {return mSettings->value("program/lockTime",20).toInt();}
bool Configuration::GetOpenLastDbOnStartUp() {return mSettings->value("program/openLastFileAtStartUp").toBool();}
bool Configuration::GetAutoSave()            {return mSettings->value("program/autoSave").toBool();             }
bool Configuration::GetSecureDesktop()       {return mSettings->value("program/useSecureDesctop").toBool();     }
bool Configuration::GetShowSubGroupElements(){return mSettings->value("program/showSubGroups").toBool();        }
bool Configuration::GetHideFromTaskbarWhenMinimized(){return mSettings->value("program/hideFromTaskbarWhenMinimized").toBool();        }


QString Configuration::GetPluginToken(){
    QString token =  mSettings->value("plugin/token","").toString();
    if(token.isEmpty()){
        token = QUuid::createUuid().toString();
    }
    return token;
}
void Configuration::SetPluginToken(QString token){mSettings->setValue("plugin/token",token);}
int Configuration::GetPluginPort(){return mSettings->value("plugin/port","1234").toInt();}
void Configuration::SetPluginPort(int port){mSettings->setValue("plugin/port",port);}
bool Configuration::GetBrowserPluginEnable(){return mSettings->value("plugin/enableBrowserPlugin","1234").toBool();}
void Configuration::SetBrowserPluginEnable(bool enabled){mSettings->setValue("plugin/enableBrowserPlugin",enabled);}


QString Configuration::GetLanguage()
{
    return mSettings->value("program/language","en-US").toString();
}

void Configuration::SetLanguage(QString local)
{
    mSettings->setValue("program/language",local);
}




