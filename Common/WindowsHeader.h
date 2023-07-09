#pragma once
/*
- Developer: PDH
- Descriptor: Windows 필수 헤더파일을 모아둔 소스
-
*/
#define _WINSOCKAPI_

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <mswsock.h>            //AcceptEX
#include <process.h>           //BeginThreadEx, EndThreadEx
#include<ws2tcpip.h>            //Inet_ntop
#include<mstcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif
