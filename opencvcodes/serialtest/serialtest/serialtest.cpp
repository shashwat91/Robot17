// serialtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<dos.h>
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>

int _tmain(int argc, _TCHAR* argv[])
{
	system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
	FILE* port;
//getchar();
// SEND LOOP

  port = fopen( "COM1:", "w" ) ;
	//port=fopen("C:\\1.txt","w");
	fprintf( port, "f" ) ;
fclose( port ) ;
//waitkey(0);
fcloseall();
	return 0;
}

