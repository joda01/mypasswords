/**
* @file	Secure.h
*
* Secure functions
*
* @author	Joachim Danmayr
* @date	206-07-16
*
*/

#include "Secure.h"
#include <string>
#include <iostream>




namespace crypto
{
    void* SecureClearArray(char* data, uint32_t size)
    {
        volatile  char  *p = data;
        while (size--)  *p++ = 0;
        return data;
    }


    void* SecureClearString(std::string* data)
    {
        std::fill(data->begin(), data->end(), 0);
        return 0;
    }

#ifndef LINUX

    #include <windows.h>

    bool LockMemory(char* data, uint32_t size)
    {
        return VirtualLock(data,size);
    }

    bool UnLockMemory(char* data, uint32_t size)
    {
        return VirtualUnlock(data,size);
    }

    HDESK hDesk;
    HWINSTA winSta;
    bool StartSecureDesktop()
    {
        return true;
        winSta = GetProcessWindowStation();
        std::cout << winSta << std::endl;

        hDesk=CreateDesktop(TEXT("DesktopName"),NULL,NULL,0,GENERIC_ALL,NULL);
        SwitchDesktop(hDesk);
        SetThreadDesktop(hDesk);
        SetProcessWindowStation(winSta);


        if(hDesk==NULL) {
            return false;
        }

        return true;
    }

    bool CloseSecureDesktop()
    {
        return CloseDesktop(hDesk);
    }

#else
    bool LockMemory(char* data, uint32_t size)
    {
        return false;
    }

    bool UnLockMemory(char* data, uint32_t size)
    {
        return false;
    }
#endif


}
