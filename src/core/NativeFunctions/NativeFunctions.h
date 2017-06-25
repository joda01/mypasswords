/**
  * @file	NativeFunctions.h
  *
  * Native functions like type converting
  *
  * @author	Joachim Danmayr
  * @date	206-07-16
  *
  */
#ifndef NATIVE_FUNCTIONS_H_
#define NATIVE_FUNCTIONS_H_

#include <cstdint>
#include <QWindow>
#include <QString>

uint32_t ToUint32(const char * const data);
void UInt32ToChar(char* const data, uint32_t value);
void CopyToClipboard(char* string, uint32_t size);
void ClearClipboard();
//void avoidFocus(WId winHandle);

void setForgroundWindow(WId winId);
WId getForgroundWindow();
QString GetWindowTitle(WId windowId);

void sendKeyChars(char* data, uint32_t size);
void sendKeyEventCtrlV();
void sendKeyEventTab();


namespace jolib
{
    int memicmp (const void *s1, const void *s2, uint32_t n1, uint32_t n2);
    char* stristr( char* str1, const char* str2, uint32_t n1, uint32_t n2 );
}

#endif // NATIVE_FUNCTIONS_H_
