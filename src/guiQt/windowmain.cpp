///
/// \file       windowmain.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Main window
///

#include "windowmain.h"
#include "ui_windowmain.h"
#include "dialogpasswordenty.h"
#include "dialogabout.h"
#include "dialoggroup.h"
#include "DialogSettings.h"
#include "dialogcomment.h"
#include "dialogimport.h"

#include <QMenu>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QShortcut>
#include <QKeyEvent>
#include "../core/NativeFunctions/NativeFunctions.h"
#include "../core/Crypto/Secure.h"
#include "../version.h"

///
/// \brief Constructor
///
/// \param[in] parent   Pointer to the parent widget
///
WindowMain::WindowMain(Configuration* settings, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowMain),
    mGroupModel(0),
    mContainerThread(this),
    mDialogOpen(0),
    mInactiveTimer(0),
    mEmptyClipboardTimeCounter(0),
    mInactiveTimerCounter(0),
    mHasUnsavedChanges(false),
    mSystemTray(ICON_SYSTEM_TRAY_OPEN,this),
    mSettings(settings),
    mPluginWebSocket(&mContainerThread,settings),
    mShortCutSave(this)
{
    ui->setupUi(this);
    QString title(VER_PRODUCTNAME_STR);
    title.append(" ").append(VER_PRODUCTVERSION_STR);
    setWindowTitle(title);

    //----------------------------------------------------
    // Setup progress bar
    ui->progressEmptyClipboard->setMaximum(EMPTY_CLIPBOARD_TIME);
    ui->progressEmptyClipboard->setVisible(false);

    //----------------------------------------------------
    // Setup table
    mPasswdTableModel = new ModelPasswordEntry(&mContainerThread);
    ui->tablePasswords->setModel(mPasswdTableModel);

    //----------------------------------------------------
    // Load settings
    QObject::connect(ui->tablePasswords->horizontalHeader(),SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),this,SLOT(tableHeader_sortIndicatorChanged(int,Qt::SortOrder)));
    mPasswdTableModel->UpdateView();

    //----------------------------------------------------
    // Set context menu for table
    ui->tablePasswords->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tablePasswords, SIGNAL(customContextMenuRequested(QPoint)),SLOT(customTableContext_MenuRequested(QPoint)));

    //----------------------------------------------------
    // Setup group List
    mGroupModel = new TreeModel(&mContainerThread);
    ui->tableGroups->setModel(mGroupModel);
    mNewContainerWizard = new WizardNewContainer(mSettings,&mContainerThread,this);
    CloseContainer();


    //----------------------------------------------------
    // Setup splitter (west = groups | right = password table)
    QList<int> sizes;
    sizes << 150 << 500;
    ui->splitter->setSizes(sizes);

    //----------------------------------------------------
    // System tray
    mSystemTray.show();
    connect(&mSystemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_systemTryAactivated(QSystemTrayIcon::ActivationReason)));

    //----------------------------------------------------
    // Set short cuts
    RegisterShortcuts();
    connect(&mShortCutOpenUrl,SIGNAL(activated()),this,SLOT(on_buttonOpenUrl_triggered()));
    connect(&mShortCutCopyUser,SIGNAL(activated()),this,SLOT(on_buttonCopyUser_triggered()));
    connect(&mShortCopyPassword,SIGNAL(activated()),this,SLOT(on_buttonCopyPassword_triggered()));
    connect(&mShortCutPerformAutoType,SIGNAL(activated()),this,SLOT(on_actionAutotype_triggered()));
    connect(&mShortCutBringToFront,SIGNAL(activated()),this,SLOT(on_shortCutBringToFront()));
    connect(&mShortCutSave,SIGNAL(activated()),this,SLOT(on_toolButtonSave_clicked()));

    //----------------------------------------------------
    // Setup timer
    mTimerToEmptyClipboard = new QTimer(this);
    connect(mTimerToEmptyClipboard, SIGNAL(timeout()), this, SLOT(emptyClipBoardTimer_timeout()));

    mWindowTimer = new QTimer(this);
    connect(mWindowTimer, SIGNAL(timeout()), this, SLOT(onCycleTimer_timeout()));
    mWindowTimer->start(500);

    mInactiveTimer = new QTimer(this);
    connect(mInactiveTimer,SIGNAL(timeout()),this,SLOT(onInactiveTimer_timeout()));
    mInactiveTimer->start(1000);

    //----------------------------------------------------
    // Creates menu
    CreateMenu();

    //----------------------------------------------------
    // Connect container
    connect(&mContainerThread,SIGNAL(ContainerEvent(int,int)),this,SLOT(on_containerEvent(int,int)));
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  parent   Pointer to the parent widget
///
WindowMain::~WindowMain()
{
    mSystemTray.hide();
    CloseContainer();
    EmptyClipboard();
    delete mNewContainerWizard;
    delete mPasswdTableModel;
    delete ui;
}

///
/// \brief      Creates the menu
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::CreateMenu()
{
    //Tool Button Open
    ui->toolButtonOpen->setPopupMode(QToolButton::MenuButtonPopup);
    ui->toolButtonOpen->addAction(ui->actionNew);

    //Tool Button Save
    ui->toolButtonSave->setPopupMode(QToolButton::MenuButtonPopup);
    ui->toolButtonSave->addAction(ui->actionImport);
    ui->toolButtonSave->addAction(ui->actionExport);
    ui->toolButtonSave->addAction(ui->actionClose);

    //Tool Button Group
    ui->toolButtonGroup->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonGroup->addAction(ui->buttonAddGroup);
    ui->toolButtonGroup->addAction(ui->buttonEditGroup);
    ui->toolButtonGroup->addAction(ui->buttonRemoveGroup);

    //Tool Button Password
    ui->toolButtonEntry->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonEntry->addAction(ui->buttonAddPassword);
    ui->toolButtonEntry->addAction(ui->buttonEditPassword);
    ui->toolButtonEntry->addAction(ui->buttonDeletePassword);

    //Tool Button autoType
    ui->toolButtonAutoType->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonAutoType->addAction(ui->buttonOpenUrl);
    ui->toolButtonAutoType->addAction(ui->actionAutotype);
    ui->toolButtonAutoType->addAction(ui->buttonCopyUser);
    ui->toolButtonAutoType->addAction(ui->buttonCopyPassword);

    // Menu
    ui->buttonMenu->setPopupMode(QToolButton::InstantPopup);
    ui->buttonMenu->addAction(ui->actionSettings);
    ui->buttonMenu->addAction(ui->actionPrint);
    ui->buttonMenu->addAction(ui->actionHelp);
    ui->buttonMenu->addAction(ui->actionAbout);
}

///
/// \brief      Show the window
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::showWindow()
{
    if(true==mSettings->GetOpenLastDbOnStartUp()){
        OpenContainer(NULL);
    }
    QMainWindow::show();
}

///
/// \brief      Window close event
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  event   Event
///
void WindowMain::closeEvent(QCloseEvent* event)
{
    CloseContainer();
    mSystemTray.hide();
    QMainWindow::closeEvent(event);
}

///
/// \brief      Event filter
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
/// \param[im]  watched Who has triggerd the event
/// \param[in]  event   Event
///
bool WindowMain::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress || event->type() == QEvent::MouseMove){
        RestartInactiveTimer();
    }

    return QObject::eventFilter(watched, event);
}

///
/// \brief      Window state changed
/// \author     Joachim Danmayr
/// \date       2016-11-13
///
/// \param[in]  event   Event
///
void  WindowMain::changeEvent(QEvent* e)
{

    if( e->type() == QEvent::WindowStateChange )
    {
        QWindowStateChangeEvent* event = static_cast< QWindowStateChangeEvent* >( e );

        if(true==isMinimized()){
            event->ignore();
            MinimizeWindow();
        }
        else if( event->oldState() & Qt::WindowMinimized )
        {
            if(false == isMinimized()){
                // event->ignore();
                // RestoreWindow();
            }
        }
        else if( event->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized )
        {
            //qDebug() << "Window Maximized!";
        }
    }

    RestartInactiveTimer();

    // std::cout<< "WindowMain::changeEvent::HasFocus" << isActiveWindow() << std::endl;


}

///
/// \brief      Container event
/// \author     Joachim Danmayr
/// \date       2016-12-01
///
void WindowMain::on_containerEvent(int event, int error)
{
    if(event == PasswordContainerThread::eCmdLock){
        LockContainerFinish();
    }
}

///
/// \brief      Register short cuts for the application
/// \author     Joachim Danmayr
/// \date       2016-11-12
///
void WindowMain::RegisterShortcuts()
{
    mShortCutOpenUrl.setKey(mSettings->GetKeySequenceOpenUrl());
    mShortCutCopyUser.setKey(mSettings->GetKeySequenceCopyUsername());
    mShortCopyPassword.setKey(mSettings->GetKeySequenceCopyPassword());
    mShortCutPerformAutoType.setKey(mSettings->GetKeySequencePerformAutoType());
    mShortCutBringToFront.setKey(mSettings->GetKeySequenceBringToFront());
    mShortCutSave.setKey(mSettings->GetKeySequenceSave());
}

///
/// \brief      System try icon has been clicked
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  event   Event (Unknown, Context, DoubleClick, Trigger, MiddleClick)
///
void WindowMain::on_systemTryAactivated(QSystemTrayIcon::ActivationReason event)
{
    if(QSystemTrayIcon::Trigger == event){
        if(isMinimized() || isHidden()){
            RestoreWindow();
        }else{
            MinimizeWindow();
        }
    }
}


///
/// \brief      Bring window to top
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_shortCutBringToFront()
{
    on_systemTryAactivated(QSystemTrayIcon::Trigger);
}


///
/// \brief      Minimaze program to tray
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::HideFromTaskbar()
{
    hide();
}

///
/// \brief      Minimaze window
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::MinimizeWindow()
{
    if(true==mSettings->GetHideFromTaskbarWhenMinimized()){
        HideFromTaskbar();
    }else{
        setWindowState(Qt::WindowMinimized);
    }

    if(mSettings->GetLockWhenMinimized()){
        mContainerThread.Lock();
    }

    // Close an open dialog
    if(0!=mDialogOpen){
        mDialogOpen->close();
        delete mDialogOpen;
        mDialogOpen = 0;
    }

}

///
/// \brief      Restore program from tray
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::RestoreWindow()
{
    show();
    setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
    UnlockContainer();
}


///
/// \brief      Show context menu for table
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  pos  Position where the menu should be shown
///
void WindowMain::customTableContext_MenuRequested(QPoint pos)
{

}

///
/// \brief      Open a dialog to create a new password entry
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::OpenNewPasswordDialog(void)
{
    DialogPasswordEnty* dialog =  new DialogPasswordEnty(this);
    Container::PasswordEntry* entry = new Container::PasswordEntry();
    bool ok = dialog->ShowDialog(&mContainerThread,entry,GetSelectedGroup());

    delete dialog;
    if(true==ok){
        // Update view
        mContainerThread.AddPasswordEntry(entry);
        mPasswdTableModel->UpdateView();
        uint32_t size;char* gId = entry->GetGroupId(size);
        Container::GroupEntry* grp= mContainerThread.GetGroup(gId,size);
        mContainerThread.FilterGroup(grp);
    }else{
        delete entry;
    }
}

///
/// \brief      Open an entry dialog to edit a password entry
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::OpenEditPasswordDialog(void)
{
    QModelIndexList selecetRows= ui->tablePasswords->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        DialogPasswordEnty* dialog =  new DialogPasswordEnty(this);
        Container::PasswordEntry* entry;
        entry = mContainerThread.GetPasswordEntry((int32_t)selecetRows.at(0).row());
        bool ok = dialog->ShowDialog(&mContainerThread,entry);
        delete dialog;
        if(true == ok){
            SetChanged();
        }
    }
}

///
/// \brief      Delete all selectes entries
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::DeleteSelectedEntries(void)
{
    QModelIndexList selecetRows= ui->tablePasswords->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        mContainerThread.RemovePasswordEntry(selecetRows.at(0).row());
        mPasswdTableModel->UpdateView();
        mContainerThread.UpdatePasswordFilter();
        SetChanged();

    }
}

///
/// \brief      Disables or enables the action butttons
/// \author     Joachim Danmayr
/// \date       2016-11-05
///
/// \param[in]  enable  Disable or enable buttons
///
void WindowMain::SetButtonsEnable(ButtonEnableState_t state)
{
    bool enabled = false;
    switch(state){
    case eButtonContainerOpen:
        enabled = true;
        ui->toolButtonOpen->setVisible(false);
        ui->toolButtonSave->setVisible(true);
        ui->toolButtonClose->setVisible(false);
        ui->toolButtonLock->setVisible(true);
        ui->toolButtonOpenURL->setVisible(true);
        ui->toolButtonLock->setIcon(QIcon(":icons_black/icons_black/lock_black.png"));
        break;

    case eButtonContainerClosed:
        enabled = false;
        ui->toolButtonOpen->setVisible(true);
        ui->toolButtonSave->setVisible(false);
        ui->toolButtonClose->setVisible(false);
        ui->toolButtonLock->setVisible(false);
        ui->toolButtonOpenURL->setVisible(false);
        break;

    case eButtonContainerLocked:
        enabled = false;
        ui->toolButtonLock->setIcon(QIcon(":icons_black/icons_black/unlock_black.png"));
        ui->toolButtonOpen->setVisible(false);
        ui->toolButtonSave->setVisible(false);
        ui->toolButtonOpenURL->setVisible(false);
        ui->toolButtonClose->setVisible(true);
        ui->toolButtonLock->setVisible(true);
        break;

    }

    ui->toolButtonGroup->setVisible(enabled);
    ui->toolButtonAutoType->setVisible(enabled);
    ui->toolButtonEntry->setVisible(enabled);

}


///
/// \brief      Shows an open dialog to open an existing container
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
///             If the container has been opened succesful the member
///             mActualOpenPasswordContainerFileName contains
///             the file name of the opened container
///
void WindowMain::OpenContainer(QWidget* parentWindow)
{
    if(0==mDialogOpen){
        mDialogOpen = new DialogOpenContainer(mSettings,&mContainerThread,parentWindow);
    }
    //crypto::StartSecureDesktop();
    mActualOpenPasswordContainerFileName = mDialogOpen->ShowDialog();
    delete mDialogOpen;
    mDialogOpen = 0;

    if(mActualOpenPasswordContainerFileName.size()>0){
        //-------------------------------------
        // Loading ok
        SetButtonsEnable(eButtonContainerOpen);
        mHasUnsavedChanges = false;
        mPasswdTableModel->UpdateView();
        UpdateGroupTable();
        RestartInactiveTimer();
    }
}

///
/// \brief      Closes an opened container file
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::CloseContainer()
{
    if(true==HasUnsavedChanges()){
        int ret = QMessageBox::question(this,tr("Save?"),tr("Save changes?"),QMessageBox::Yes | QMessageBox::Default,QMessageBox::No | QMessageBox::Escape);
        if(QMessageBox::Yes == ret){
            mContainerThread.Save(this);
            while(false==mContainerThread.isFinished()){}
            ResetChanged();
        }
    }

    //-------------------------------------
    // Disable buttons
    SetButtonsEnable(eButtonContainerClosed);
    ResetChanged();
    mContainerThread.Close(this);
    ui->textSearch->clear();
    mPasswdTableModel->UpdateView();
    if(0!=mGroupModel){
        mGroupModel->UpdateView();
    }
}


///
/// \brief      Locks and unlocks the databse
/// \author     Joachim Danmayr
/// \date       2016-11-05
///
void WindowMain::LockUnlock()
{
    if(true==mContainerThread.IsLocked()){
        UnlockContainer();
    }else{
        mContainerThread.Lock();
    }
}


///
/// \brief      Locks the container
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::LockContainerFinish()
{
    //-------------------------------------
    // Disable buttons
    SetButtonsEnable(eButtonContainerLocked);
    mSystemTray.setIcon(ICON_SYSTEM_TRAY_LOCKED);
    ui->textSearch->clear();
    mPasswdTableModel->UpdateView();

    if(true==mSettings->GetMinimizeWhenLocking()){
        MinimizeWindow();
    }
}

///
/// \brief      Unlock the container
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::UnlockContainer()
{
    if(true==mContainerThread.IsLocked()){
        if(0==mDialogOpen){
            mDialogOpen = new DialogOpenContainer(mSettings,&mContainerThread,this);
        }
        bool openOk = mDialogOpen->ShowUnlockDialog();
        delete mDialogOpen;
        mDialogOpen = 0;

        if(true==openOk){
            // Unlock ok
            mSystemTray.setIcon(ICON_SYSTEM_TRAY_OPEN);
            SetButtonsEnable(eButtonContainerOpen);
            mHasUnsavedChanges = false;
            mPasswdTableModel->UpdateView();
            UpdateGroupTable();
        }
    }
}

///
/// \brief      Returns the selected group
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     Selected group
///
QString WindowMain::GetSelectedGroup()
{
    QModelIndexList index = ui->tableGroups->selectionModel()->selectedIndexes();
    if(index.length()>0){
        Container::GroupEntry* entry = ((TreeModel*)index[0].model())->itemFromIndex(index[0])->GetData();
        uint32_t size;char* grpId = entry->GetGroupId(size);
        QString grpIdString = QString::fromUtf8(grpId,size);
        return grpIdString;
    }
    return "";
}

/// \brief      Saves the Window ID of the last active
///             window if it was not the own one
/// \author     Joachim Danmayr
/// \date       2016-10-25
///
void WindowMain::onCycleTimer_timeout()
{
    #ifndef LINUX
    WId actwinId = getForgroundWindow();
    if(actwinId != winId()){
        GetWindowTitle(actwinId);
        mLastActiveWindow = actwinId;
    }
    #endif
}

/// \brief      Restarts the inactive timer
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::RestartInactiveTimer()
{
    mInactiveTimerCounter=0;
}


/// \brief      Inactive timer
///             Container will be locked after this time
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::onInactiveTimer_timeout()
{
    if(true==mSettings->GetLockAfterTime()){
        if(false==mContainerThread.IsLocked() && true==mContainerThread.IsOpen()){
            mInactiveTimerCounter++;
            uint32_t lockTimer =mSettings->GetTimeBeforeLocking();
            //std::cout << "WindowMain::onInactiveTimer_timeout: " << mInactiveTimerCounter << "/" << lockTimer <<std::endl;
            if(mInactiveTimerCounter>lockTimer){
                mInactiveTimerCounter = 0;
                mContainerThread.Lock();
            }
        }
    }
}

///
/// \brief      Copy the password of the selected entry to the clipboard
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
///             A timer will be started after the copy process.
///             When the time is over the clipboard will be erased
///
void WindowMain::CopySelectedPasswordToClipboard()
{
    QModelIndexList selecetRows= ui->tablePasswords->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        Container::PasswordEntry* entry;
        entry = mContainerThread.GetPasswordEntry(selecetRows.at(0).row());
        if(entry->GetNumberOfPasswordElement()>0){
            uint32_t size;
            char* data = entry->GetPasswordElement(0)->GetPassword(size);
            CopyToClipboard(data,size);
            entry->GetPasswordElement(0)->ClosePassword();
            mEmptyClipboardTimeCounter = 0;
            ui->progressEmptyClipboard->setValue(0);
            ui->progressEmptyClipboard->setVisible(true);
            mTimerToEmptyClipboard->start(EMPTY_CLIPBOARD_TIMER_BASE);
        }
    }
}


///
/// \brief      Copy the username of the selected entry to the clipboard
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
///             A timer will be started after the copy process.
///             When the time is over the clipboard will be erased
///
void WindowMain::CopySelectedUsernameToClipboard()
{
    QModelIndexList selecetRows= ui->tablePasswords->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        Container::PasswordEntry* entry;
        entry = mContainerThread.GetPasswordEntry(selecetRows.at(0).row());
        if(entry->GetNumberOfPasswordElement()>0){
            uint32_t size;
            char* data = entry->GetPasswordElement(0)->GetUsername(size);
            CopyToClipboard(data,size);
            mEmptyClipboardTimeCounter = 0;
            ui->progressEmptyClipboard->setValue(0);
            ui->progressEmptyClipboard->setVisible(true);
            mTimerToEmptyClipboard->start(EMPTY_CLIPBOARD_TIMER_BASE);
        }
    }
}

///
/// \brief      Performs an auto type
///             - Gets username and password
///             - Brings last focused window to front
///             - Copy and past username and password
/// \author     Joachim Danmayr
/// \date       2016-10-25
///
void WindowMain::AutoTypeSelectedEntry()
{
    #ifndef LINUX
   // QString windowTitle = GetWindowTitle(mLastActiveWindow);
   // ui->textSearch->setText(windowTitle);
   //mContainerThread.FilterTitle(windowTitle.toStdString());
    //mContainerThread
   // ui->tablePasswords->selectAll();

    QModelIndexList selecetRows= ui->tablePasswords->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        Container::PasswordEntry* entry;
        entry = mContainerThread.GetPasswordEntry(selecetRows.at(0).row());
        if(entry->GetNumberOfPasswordElement()>0){
            uint32_t usrSize;char* username = entry->GetPasswordElement(0)->GetUsername(usrSize);
            uint32_t passSize;char* password = entry->GetPasswordElement(0)->GetPassword(passSize);
            setForgroundWindow(mLastActiveWindow);
            CopyToClipboard(username,usrSize);
            sendKeyEventCtrlV();
            sendKeyEventTab();
            CopyToClipboard(password,passSize);
            sendKeyEventCtrlV();
            entry->GetPasswordElement(0)->ClosePassword();
            ClearClipboard();
        }
    }
#endif
}

///
/// \brief      Open the selected URL
/// \author     Joachim Danmayr
/// \date       2016-11-12
///
void WindowMain::OpenSelectedUrl()
{
    QModelIndexList selecetRows= ui->tablePasswords->selectionModel()->selectedRows();
    if(selecetRows.size()>0){
        Container::PasswordEntry* entry;
        entry = mContainerThread.GetPasswordEntry(selecetRows.at(0).row());
        if(entry->GetNumberOfPasswordElement()>0){
            uint32_t size;
            char* data = entry->GetPasswordElement(0)->GetURL(size);
            QUrl url(QString::fromUtf8(data,size));
            QDesktopServices::openUrl(url);
        }
    }
}

///
/// \brief      Erase the content of the clipboard
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::EmptyClipboard()
{
    mTimerToEmptyClipboard->stop();
    mEmptyClipboardTimeCounter = 0;
    ClearClipboard();
    ui->progressEmptyClipboard->setVisible(false);
}


///
/// \brief      This is the timer routine which is called every
///             1 ms if a password has been copied to the clipboard
///             After a timeout the clipboard will be erased
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::emptyClipBoardTimer_timeout()
{
    int32_t newVal = ui->progressEmptyClipboard->value()+1;
    ui->progressEmptyClipboard->setValue(newVal);
    if(++mEmptyClipboardTimeCounter > EMPTY_CLIPBOARD_TIME){
        EmptyClipboard();
    }
}

///
/// \brief      Doubleclick on a table entry
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_tablePasswords_doubleClicked(const QModelIndex&)
{
    OpenEditPasswordDialog();
}

///
/// \brief      Save container button has been pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::Save()
{
    mContainerThread.Save(this);
    ResetChanged();
}

///
/// \brief      New container button has been pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_actionNew_triggered()
{
    mNewContainerWizard->ShowDialog(WizardNewContainer::eNewContainer);
}

///
/// \brief      Close container button has been pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_actionClose_triggered()
{
    CloseContainer();
}

///
/// \brief      Text in search tect box has been changed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  text    Text of the search texfield
///
void WindowMain::on_textSearch_textChanged(const QString &text)
{
    mContainerThread.FilterTitle(text.toLower().toStdString());
    mPasswdTableModel->UpdateView();
}

///
/// \brief      Table header has been clicked
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  idx         Index of the column which has been clicked
/// \param[in]  indicator   Indicator element which contains order information
///
void WindowMain::tableHeader_sortIndicatorChanged(int idx, Qt::SortOrder indicator)
{
    Q_UNUSED(idx);
    Q_UNUSED(indicator);
}

///
/// \brief      Show settings panel button has been clicked
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_actionSettings_triggered()
{
    DialogSettings settings(mSettings,&mContainerThread,this);
    settings.exec();
    RegisterShortcuts();
}

///
/// \brief      Table group item clicked
///             Groups are stored in the format Root->Child1->Child2
///             From the selected tree view item a string with this
///             struct is generated to filter entries for this group
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_tableGroups_clicked(const QModelIndex &index)
{
    Container::GroupEntry* entry = ((TreeModel*)index.model())->itemFromIndex(index)->GetData();
    mContainerThread.FilterGroup(entry);
    mPasswdTableModel->UpdateView();
}

///
/// \brief      Print button has been pressed
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_actionPrint_triggered()
{
    QMessageBox::warning(this,tr("information"),tr("Printing not supported yet!"));
}

///
/// \brief      Add group has been clicked
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void WindowMain::on_buttonAddGroup_triggered()
{
    DialogGroup* dialog = new DialogGroup(this);
    Container::GroupEntry* entry = new Container::GroupEntry();
    bool ok = dialog->ShowDialog(&mContainerThread,entry,GetSelectedGroup());

    delete dialog;
    if(true==ok){
        mContainerThread.AddGroup(entry);
        UpdateGroupTable();
        SetChanged();
    }else{
        delete entry;
    }
}

///
/// \brief      Double click on group entry
/// \author     Joachim Danmayr
/// \date       2016-09-25
///
void WindowMain::on_tableGroups_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    /*  DialogGroup* dialog = new DialogGroup(this);
    Container::GroupEntry* entry = ((TreeModel*)index.model())->itemFromIndex(index)->GetData();
    dialog->ShowDialog(&mPasswordContainer,entry);
    delete dialog;
    mGroupModel->UpdateView();*/
}

///
/// \brief      Delete group clicked
/// \author     Joachim Danmayr
/// \date       2016-09-28
///
void WindowMain::on_buttonRemoveGroup_triggered()
{
    QModelIndexList index = ui->tableGroups->selectionModel()->selectedIndexes();
    if(index.length()>0){
        Container::GroupEntry* entry = ((TreeModel*)index[0].model())->itemFromIndex(index[0])->GetData();
        mContainerThread.RemoveGroupEntry(entry);
        UpdateGroupTable();
        SetChanged();
    }
}

///
/// \brief      Add New passord clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_buttonAddPassword_triggered()
{
    OpenNewPasswordDialog();
}

///
/// \brief      Edit passord clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_buttonEditPassword_triggered()
{
    OpenEditPasswordDialog();
}

///
/// \brief      Delete passord clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_buttonDeletePassword_triggered()
{
    DeleteSelectedEntries();
}

///
/// \brief      Copy password clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_buttonCopyPassword_triggered()
{
    CopySelectedPasswordToClipboard();
}

///
/// \brief      Copy username clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_buttonCopyUser_triggered()
{
    CopySelectedUsernameToClipboard();
}

///
/// \brief      About button clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_actionAbout_triggered()
{
    DialogAbout about(this);
    about.exec();
}

///
/// \brief      Edit group button clicked
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void WindowMain::on_buttonEditGroup_triggered()
{
    QModelIndexList index = ui->tableGroups->selectionModel()->selectedIndexes();
    if(index.length()>0){
        DialogGroup* dialog = new DialogGroup(this);
        Container::GroupEntry* entry = ((TreeModel*)index[0].model())->itemFromIndex(index[0])->GetData();
        dialog->ShowDialog(&mContainerThread,entry);
        delete dialog;
        UpdateGroupTable();
        SetChanged();
    }
}

///
/// \brief      Updates the group tree at the left side
/// \author     Joachim Danmayr
/// \date       2016-10-31
///
void WindowMain::UpdateGroupTable()
{
    mGroupModel->UpdateView();
    ui->tableGroups->expandAll();
}

///
/// \brief      Should be called when something has been changed in the database
/// \author     Joachim Danmayr
/// \date       2016-10-31
///
void WindowMain::SetChanged()
{
    mHasUnsavedChanges = true;
    QString title = windowTitle();
    if(title.endsWith("*")==false){
        title.append("*");
        setWindowTitle(title);
    }

    // Save if auto save is active
    if(true==mSettings->GetAutoSave()){
        Save();
    }
}

///
/// \brief      Should be called when changes have been saved
/// \author     Joachim Danmayr
/// \date       2016-10-31
///
void WindowMain::ResetChanged()
{
    QString title = windowTitle();
    title.replace("*","");
    setWindowTitle(title);
    mHasUnsavedChanges = false;
}

///
/// \brief      Open URL has been clicked
/// \author     Joachim Danmayr
/// \date       2016-10-31
///
void WindowMain::on_buttonOpenUrl_triggered()
{
    OpenSelectedUrl();
}


///
/// \brief      Toolbutton open clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_toolButtonOpen_clicked()
{
    OpenContainer(this);
}

///
/// \brief      Toolbutton save clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_toolButtonSave_clicked()
{
    Save();
}

///
/// \brief      Key in password table pressed
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_tablePasswords_pressed(const QModelIndex &index)
{
    ui->textSearch->setFocus();
}

///
/// \brief      Toolbutton lock clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_toolButtonLock_clicked()
{
    LockUnlock();
}

///
/// \brief      Toolbutton close clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_toolButtonClose_clicked()
{
    CloseContainer();
}

///
/// \brief      Toolbutton comment clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_toolButtonComment_clicked()
{
    DialogComment dialog(this);
    dialog.exec();
}

///
/// \brief      Toolbutton autotype clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_actionAutotype_triggered()
{
    AutoTypeSelectedEntry();
}

///
/// \brief      Help button clicked
/// \author     Joachim Danmayr
/// \date       2016-12-04
///
void WindowMain::on_actionHelp_triggered()
{
    QMessageBox::information(this,tr("Help..."),tr("Still in progress...\nPlease use the feedback button in the right top corner for support!"));
}

///
/// \brief      Import triggered
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_actionImport_triggered()
{
    //DialogImport import(this);
    //import.exec();
}

///
/// \brief      Open Url clicked
/// \author     Joachim Danmayr
/// \date       2016-11-20
///
void WindowMain::on_toolButtonOpenURL_clicked()
{
    OpenSelectedUrl();
}
