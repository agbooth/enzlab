/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)


              MODULE: DISASTER

			 PURPOSE: exception handling - math errors, signals etc

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/

#define STRICT
#include "stdafx.h"
#pragma hdrstop
/*
#include <float.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
*/
#include "limits.h"

#include <signal.h>
#pragma argsused
void FPECatcher( int arg1 )
{
/*
	Commented this bit out, because the exception handling in Borland C++ just doesn't work like
	they say it should.   -Doug, 4/2/93.

    char 	*szErrType;
	char	buffer[BUFLEN];

	switch (arg1) {
		case FPE_INTOVFLOW:
			szErrType = "FPE_INTOVFLOW: INTO executed with OF flag set";
			break;
		case FPE_INTDIV0:
			szErrType = "FPE_INTDIV0: Integer divide by zero";
			break;
		case FPE_ZERODIVIDE:
			szErrType = "FPE_ZERODIVIDE: Division by zero";
			break;
		case FPE_OVERFLOW:
			szErrType = "FPE_OVERFLOW: Numeric overflow";
			break;  
		case FPE_UNDERFLOW:
			szErrType = "FPE_UNDERFLOW:	Numeric underflow";
			break;  
		case FPE_INEXACT:
			szErrType = "FPE_INEXACT: Precision";
			break;
		case FPE_EXPLICITGEN:  
			szErrType = "FPE_EXPLICITGEN: Program executed raise(SIGFPE)";
			break;
		default:
			szErrType = "UNKNOWN";
			break;
    };
*/
	signal( SIGFPE, (void(*)(int))SIG_IGN );
    MessageBox( NULL, L"A serious floating-point error has occured in the program.\nThe program will terminate when you press Ok.", L"EnzLab: FATAL INTERNAL ERROR", MB_OK );
	signal( SIGFPE, (void(*)(int))FPECatcher );

	exit(EXIT_FAILURE);
}

void SetupDisasterCatchers( void )
{
	signal( SIGFPE, (void(*)(int))FPECatcher );
	signal( SIGSEGV, (void(*)(int))FPECatcher );
	signal( SIGILL, (void(*)(int))FPECatcher );
    signal( SIGINT, (void(*)(int))SIG_IGN );
	return;
}


int matherr( struct exception *e )
{
	WCHAR	buffer[BUFLEN];
    WCHAR	*szType;

	switch(e->type) {
		case DOMAIN:
			szType = L"DOMAIN";
            break;
		case SING:
			szType = L"SING";
            break;
		case OVERFLOW:
			szType = L"OVERFLOW";
            break;
		case UNDERFLOW:
			szType = L"UNDERFLOW";
            break;
		case TLOSS:
			szType = L"TLOSS";
			break;
		default:
			szType = L"UNKNOWN";
	};

	swprintf( buffer,  L"A serious error has occurred in the program.\n\terror type: %s\n\tcaused by: %s(%g)\nThe program will terminate when you click on OK.", szType, e->name, e->arg1 );
	MessageBox( NULL, buffer, L"EnzLab: FATAL INTERNAL ERROR", MB_OK );
	exit(EXIT_FAILURE);

    // just to keep the compiler happy
    return FALSE;
}