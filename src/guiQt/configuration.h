///
/// \file       configuration.h
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
/// \brief      Class which stores th application configuration
///


#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <QSettings>
#include <QKeySequence>
#include <QLocale>


///
/// \class      Configuration
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
/// \brief      Class which stores th application configuration
///
class Configuration
{
public:
    /////////////////////////////////////////////////////////
    /// Methods
    Configuration();
    ~Configuration();

    /// File history ////////////////////////////////////////
    void SetFileHistory(QStringList& list);
    QStringList GetFileHistory();
    void SetFileHistoryLastOpenIdx(int idx);
    int GetFileHistoryLastOpenIdx();

    /// ShortCuts ////////////////////////////////////////
    void SetKeySequenceOpenUrl(QKeySequence sequence);
    void SetKeySequenceCopyUsername(QKeySequence sequence);
    void SetKeySequenceCopyPassword(QKeySequence sequence);
    void SetKeySequencePerformAutoType(QKeySequence sequence);
    void SetKeySequenceSave(QKeySequence sequence);
    void SetKeySequenceBringToFront(QKeySequence sequence);
    QKeySequence GetKeySequenceOpenUrl();
    QKeySequence GetKeySequenceCopyUsername();
    QKeySequence GetKeySequenceCopyPassword();
    QKeySequence GetKeySequencePerformAutoType();
    QKeySequence GetKeySequenceSave();
    QKeySequence GetKeySequenceBringToFront();

    /// Program settings ////////////////////////////////////////
    void SetMinimizeWhenLocking(bool enab);
    void SetLockWhenMinimized(bool enab);
    void SetLockAfterTime(bool enab);
    void SetTimeBeforeLocking(int time);
    void SetOpenLastDbOnStartUp(bool enab);
    void SetAutoSave(bool enab);
    void SetSecureDesktop(bool enab);
    void SetShowSubGroupElements(bool enab);
    void SetHideFromTaskbarWhenMinimized(bool enab);
    bool GetMinimizeWhenLocking() ;
    bool GetLockWhenMinimized()   ;
    bool GetLockAfterTime()       ;
    int  GetTimeBeforeLocking();
    bool GetOpenLastDbOnStartUp() ;
    bool GetAutoSave()            ;
    bool GetSecureDesktop()       ;
    bool GetShowSubGroupElements();
    bool GetHideFromTaskbarWhenMinimized();

    /// Language ////////////////////////////////////////
    QString GetLanguage();
    void SetLanguage(QString local);

    /// Plugin ////////////////////////////////////////
    QString GetPluginToken();
    void SetPluginToken(QString token);
    int GetPluginPort();
    void SetPluginPort(int port);
    bool GetBrowserPluginEnable();
    void SetBrowserPluginEnable(bool enabled);

private:

    /////////////////////////////////////////////////////////
    /// Memebers
    QSettings* mSettings;

};

#endif // CONFIGURATION_H
