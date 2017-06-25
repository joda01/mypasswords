/**
  * @file	NativeFunctions.h
  *
  * Native functions like type converting
  *
  * @author	Joachim Danmayr
  * @date	206-07-16
  *
  */

#include "NativeFunctions.h"
#include <string>
#include <iostream>

/**
 * Converts an 4 byte data array to a little endian 32 bit integer
 * @param[in] data  Data to be converted
 * @return converted data
 */
uint32_t ToUint32(const char* const data)
{
    uint32_t i=0;
    i = (uint32_t)  ((unsigned char)data[0]) << (uint32_t)24;
    i |= (uint32_t) ((unsigned char)data[2]) << (uint32_t)8;
    i |= (uint32_t) ((unsigned char)data[1]) << (uint32_t)16;
    i |= ((uint32_t)((unsigned char)data[3])& 0x000000ff);
    return i;
}

/**
 * Converts a little endian integer to a char array
 * @param[out] data  place where to store data
 * @param[in] value data shich should be converted
 */
void UInt32ToChar(char* const data, uint32_t value)
{
    data[0] = (value & 0xff000000) >> 24;
    data[1] = (value & 0x00ff0000) >> 16;
    data[2] = (value & 0x0000ff00) >> 8;
    data[3] = (value & 0x000000ff) >> 0;
}


#ifdef LINUX

/**
 * Copy a string to the clipboard
 * @param[in] string String to copy
 * @param[in] size   Size of the string
 */
void CopyToClipboard(char* string, uint32_t size)
{

}

/**
 * Clear Clipboars contens
 */
void ClearClipboard()
{

}
#else

#include <windows.h>
#include <qt_windows.h>



///
/// \brief      Gets the window ID of the actuel forgrund window
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     Window ID
///
WId  getForgroundWindow()
{
    //std::wstring titleString;
    HWND hwnd=GetForegroundWindow(); // get handle of currently active window
    return (WId)hwnd;

}

///
/// \brief      Returns the title of the window with the fiveb ID
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  windowId    ID of the window
/// \return     Window      Title
///
QString GetWindowTitle(WId windowId)
{
    WCHAR wnd_title[256];
    GetWindowText((HWND)windowId,wnd_title,256);
    std::wstring wString(wnd_title);
    QString title = QString::fromStdWString(wString);
    return title;
}

///
/// \brief      Sets a window to the forground window
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  winId   ID of the window to bring to foreground
///
void setForgroundWindow(WId winId)
{
    SetActiveWindow((HWND)winId);
    SetForegroundWindow((HWND)winId);
}



/**
 * Do not focus the QT window when clicking a button
 */
void avoidFocus(WId winHandle){
    ShowWindow((HWND)winHandle, SW_HIDE);
    SetWindowLong((HWND)winHandle, GWL_EXSTYLE, GetWindowLong((HWND)winHandle, GWL_EXSTYLE)| WS_EX_NOACTIVATE | WS_EX_APPWINDOW);
    ShowWindow((HWND)winHandle, SW_SHOW);
}

void sendKeyChars(char* data, uint32_t size)
{
    while (GetKeyState(VK_LCONTROL) < 0){}

    for(uint32_t n=0;n<size;n++){
        Sleep(10);
        keybd_event(VkKeyScan(data[n]),0,0 , 0);
        Sleep(10);
        keybd_event(VkKeyScan(data[n]),0, KEYEVENTF_KEYUP,0);
    }
    Sleep(10);
}

void sendKeyEventCtrlV()
{
    while (GetKeyState(VK_LCONTROL) < 0){}
    Sleep(100);
    // Simulating a Ctrl+v keystroke
    keybd_event(VK_CONTROL,0x9d,0 , 0); // Ctrl Press
    keybd_event(VkKeyScan('V'),0xAF,0 , 0); // ‘A’ Press
    keybd_event(VkKeyScan('V'),0xAF, KEYEVENTF_KEYUP,0); // ‘A’ Release
    keybd_event(VK_CONTROL,0x9d,KEYEVENTF_KEYUP,0); // Ctrl Release
    Sleep(100);
}


void sendKeyEventTab()
{
    Sleep(100);
    keybd_event(VK_TAB,0x8f,0 , 0); // Tab Press
    keybd_event(VK_TAB,0x8f, KEYEVENTF_KEYUP,0); // Tab Release
    Sleep(100);
}

/**
 * Copy a string to the clipboard
 * @param[in] string String to copy
 * @param[in] size   Size of the string
 */
void CopyToClipboard(char* string, uint32_t size)
{
    // I don't know why you hav to alloc size +1 bytes !?
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, (size+1));
    memcpy(GlobalLock(hMem), string, size);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

/**
 * Clear Clipboars contens
 */
void ClearClipboard()
{
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, 256);
    char stringRemove[256];
    memset (stringRemove,'-',256);
    memcpy(GlobalLock(hMem), stringRemove, 256);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    EmptyClipboard();
    CloseClipboard();
}


#endif

namespace jolib
{
/**
 * Case insensitve string compare
 * @param[in] s1 String1
 * @param[in] s2 String2
 * @param[in] n Size of string
 */
#define TOUPPER(CH) (((CH) >= 'a' && (CH) <= 'z') ? ((CH) - 'a' + 'A') : (CH))
int memicmp (const void *s1, const void *s2, uint32_t n1,uint32_t n2)
{
    if(n1==0 || n2==0){
        return n1-n2;
    }
    int length = n1;
    if(n2 < n1){
        length=n2;
    }


    const char *sc1 = (const char *) s1;
    const char *sc2 = (const char *) s2;
    const char *sc2end = sc2 + length;

    while (sc2 < sc2end && TOUPPER (*sc1) == TOUPPER (*sc2)){
        sc1++, sc2++;
    }
    if ((sc2 == sc2end) && (n1==n2)){
        return 0;
    }else if (sc2 == sc2end){
        return n1-n2;
    }
    return (int) (TOUPPER (*sc1) - TOUPPER (*sc2));

}


/**
 * Case insensitve string find
 * @param[in] s1 String1
 * @param[in] s2 String2
 * @param[in] n1 Size of string1
 * @param[in] n2 Size of string2
 */
char* stristr( char* str1, const char* str2, uint32_t n1, uint32_t n2 )
{
    char* p1 = str1 ;
    const char* p2 = str2 ;
    char* r = *p2 == 0 ? str1 : 0 ;

    if((n1==0 && n2>0) || (n1>0 && n2==0)){
        return 0;
    }

    while(n1>0 && *p1 != 0 && n2>0  && *p2 != 0)
    {
        if( tolower( *p1 ) == tolower( *p2 ) )
        {
            if( r == 0 )
            {
                r = p1 ;
            }

            p2++ ;
            n2--;
        }
        else
        {
            p2 = str2 ;
            if( tolower( *p1 ) == tolower( *p2 ) )
            {
                r = p1 ;
                p2++ ;
                n2--;
            }
            else
            {
                r = 0 ;
            }
        }

        p1++ ;
        n1--;
    }

    return *p2 == 0 ? r : 0 ;
}


}
