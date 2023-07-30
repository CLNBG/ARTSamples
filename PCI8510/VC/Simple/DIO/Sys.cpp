// 本程序演示了开关量输入输出过程

#include "stdafx.h"
#include "windows.h"
#include <conio.h>
#include <ctype.h>

#include "PCI8510.h" // 驱动接口头文件(必须)

int main(int argc, char* argv[])
{
	BYTE bDOSts[8];
	BYTE bDISts[8];
	bDOSts[0] = 0;      // 设置开关量输出的状态
	bDOSts[1] = 1;
	bDOSts[2] = 0;
	bDOSts[3] = 1;
	bDOSts[4] = 0;
	bDOSts[5] = 1;
	bDOSts[6] = 0;
	bDOSts[7] = 1;

	HANDLE hDevice;
	int DeviceID = 0; 

	hDevice = PCI8510_CreateDevice( DeviceID );
	if(hDevice == INVALID_HANDLE_VALUE) return 0;
	
	PCI8510_SetDeviceDO(hDevice, bDOSts); // 开关量输出

	// 反应时间比较长，所以需要一定的延时
	if ( bDOSts[0] )  printf( "DO0 = On\n" ); else printf( "DO0 = Off\n" );
	if ( bDOSts[1] )  printf( "DO1 = On\n" ); else printf( "DO1 = Off\n" );
	if ( bDOSts[2] )  printf( "DO2 = On\n" ); else printf( "DO2 = Off\n" );
	if ( bDOSts[3] )  printf( "DO3 = On\n" ); else printf( "DO3 = Off\n" );
	if ( bDOSts[4] )  printf( "DO4 = On\n" ); else printf( "DO4 = Off\n" );
	if ( bDOSts[5] )  printf( "DO5 = On\n" ); else printf( "DO5 = Off\n" );
	if ( bDOSts[6] )  printf( "DO6 = On\n" ); else printf( "DO6 = Off\n" );
	if ( bDOSts[7] )  printf( "DO7 = On\n" ); else printf( "DO7 = Off\n" );
	
	printf("Press any key to continue get DI...");
	_getch();
	printf( "\n\n" );

	PCI8510_GetDeviceDI( hDevice, bDISts);  // 开关量输入

	if ( bDISts[0] )  printf( "DI0 = On\n" ); else printf( "DI0 = Off\n" );
	if ( bDISts[1] )  printf( "DI1 = On\n" ); else printf( "DI1 = Off\n" );
	if ( bDISts[2] )  printf( "DI2 = On\n" ); else printf( "DI2 = Off\n" );
	if ( bDISts[3] )  printf( "DI3 = On\n" ); else printf( "DI3 = Off\n" );
	if ( bDISts[4] )  printf( "DI4 = On\n" ); else printf( "DI4 = Off\n" );
	if ( bDISts[5] )  printf( "DI5 = On\n" ); else printf( "DI5 = Off\n" );
	if ( bDISts[6] )  printf( "DI6 = On\n" ); else printf( "DI6 = Off\n" );
	if ( bDISts[7] )  printf( "DI7 = On\n" ); else printf( "DI7 = Off\n" );

	_getch();
	PCI8510_ReleaseDevice( hDevice );

	return 0;
}

