#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QMainWindow>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QThread>
#include <QSystemTrayIcon>
#include <QShortcut>
#include <QTranslator>
#include "modelpasswordentry.h"
#include "treemodel.h"
#include "passwordcontainerthread.h"
#include "wizardnewcontainer.h"
#include "dialogopencontainer.h"
#include "configuration.h"
#include "qglobalshortcut/qglobalshortcut.h"
#include "../plugin/pluginwebsocket.h"

namespace Ui {
class WindowMain;
}


///
/// \class      WindowMain
/// \author     Joachim Danmayr
/// \date       2016-09-23
///
/// \brief      Main window
///
class WindowMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit WindowMain(Configuration *settings, QWidget *parent = 0);
    ~WindowMain();
    void showWindow();

private slots:
    void on_tablePasswords_doubleClicked(const QModelIndex &index);

    void on_actionNew_triggered();

    void on_actionClose_triggered();

    void on_textSearch_textChanged(const QString &text);

    void tableHeader_sortIndicatorChanged(int idx, Qt::SortOrder indicator);

    void on_actionSettings_triggered();

    void emptyClipBoardTimer_timeout();

    void onCycleTimer_timeout();

    void onInactiveTimer_timeout();

    void customTableContext_MenuRequested(QPoint pos);

    void on_actionPrint_triggered();

    void on_buttonAddGroup_triggered();

    void on_tableGroups_doubleClicked(const QModelIndex &index);

    void on_buttonRemoveGroup_triggered();

    void on_tableGroups_clicked(const QModelIndex &index);

    void on_buttonAddPassword_triggered();

    void on_buttonEditPassword_triggered();

    void on_buttonDeletePassword_triggered();

    void on_buttonCopyPassword_triggered();

    void on_buttonCopyUser_triggered();

    void on_actionAbout_triggered();

    void on_buttonEditGroup_triggered();

    void on_systemTryAactivated(QSystemTrayIcon::ActivationReason);

    void on_shortCutBringToFront();

    void on_buttonOpenUrl_triggered();

    void on_toolButtonOpen_clicked();

    void on_toolButtonSave_clicked();

    void on_tablePasswords_pressed(const QModelIndex &index);

    void on_toolButtonLock_clicked();

    void on_toolButtonClose_clicked();

    void on_toolButtonComment_clicked();

    void on_containerEvent(int event, int error);

    void on_actionAutotype_triggered();

    void on_actionHelp_triggered();

    void on_actionImport_triggered();

    void on_toolButtonOpenURL_clicked();

private:
    enum ButtonEnableState_t{eButtonContainerClosed, eButtonContainerOpen, eButtonContainerLocked};
    static const uint32_t EMPTY_CLIPBOARD_TIMER_BASE = 1;   /**< Time base for empty clipboard time*/
    static const uint32_t EMPTY_CLIPBOARD_TIME = 10000;
    QIcon ICON_SYSTEM_TRAY_LOCKED = QIcon(":logo_sw.png");
    QIcon ICON_SYSTEM_TRAY_OPEN = QIcon(":logo.png");


    //--------------------------------------------------------
    // Methods
    Ui::WindowMain *ui;
    void RegisterShortcuts();
    void OpenContainer(QWidget* parentWindow);
    void CloseContainer();
    void OpenNewPasswordDialog();
    void OpenEditPasswordDialog();
    void DeleteSelectedEntries();
    void CopySelectedPasswordToClipboard();
    void CopySelectedUsernameToClipboard();
    void AutoTypeSelectedEntry();
    void OpenSelectedUrl();
    void EmptyClipboard();
    void UpdateGroupTable();
    void SetChanged();
    void ResetChanged();
    bool HasUnsavedChanges(){return mHasUnsavedChanges;}
    void SetButtonsEnable(ButtonEnableState_t state);
    void LockContainerFinish();
    void UnlockContainer();
    void LockUnlock();
    void HideFromTaskbar();
    void MinimizeWindow();
    void RestoreWindow();
    void RestartInactiveTimer();
    void CreateMenu();
    void Save();
    QString GetSelectedGroup();
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void changeEvent(QEvent*e) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    //--------------------------------------------------------
    // Members
    ModelPasswordEntry* mPasswdTableModel;
    TreeModel*          mGroupModel;
    PasswordContainerThread     mContainerThread;
    WizardNewContainer* mNewContainerWizard;
    DialogOpenContainer* mDialogOpen;
    QTimer *mTimerToEmptyClipboard;
    QTimer *mWindowTimer;
    QTimer *mInactiveTimer;
    uint32_t mEmptyClipboardTimeCounter;
    uint32_t mInactiveTimerCounter;
    QString mActualOpenPasswordContainerFileName;
    bool mHasUnsavedChanges;
    QSystemTrayIcon mSystemTray;
    WId mLastActiveWindow;
    Configuration* mSettings;
    PluginWebSocket mPluginWebSocket;

    /// Key sequences ///////////////////////////////////////
    QGlobalShortcut mShortCutOpenUrl;
    QGlobalShortcut mShortCutCopyUser;
    QGlobalShortcut mShortCopyPassword;
    QGlobalShortcut mShortCutPerformAutoType;
    QGlobalShortcut mShortCutBringToFront;
    QShortcut       mShortCutSave;

};

#endif // WINDOWMAIN_H
