/**************************************************************************************************************
 * FileName:        Print.c
 * Processor:       Lm3s6918
 * Compiler:        Keil uvision 3
 * Company:         
 * Author(s)               Date        Comment
 ********************************************************************/

/******************************************************************************
*
* 						Archivos Cabecera
*
******************************************************************************/

#include "conf_uart_aux.h"
#include "memorias_at45db.h"
#include "conf_UART_comSoftware.h"
#include "print.h"
#include "rtc.h"
//**************************************************************************** 


//**************************************************************************************
void PrintMsg1(void)
{
	char cLinea1[20];

	Leer_cadena_memAT45(cLinea1, 20, 0x10, MEM_USUARIOS);
	g_cContByteTxPrinBar=20;
	Escribir_cadena_impresora(cLinea1, g_cContByteTxPrinBar);

} //void PrintMsg1(void)
//**************************************************************************************
void PrintMsg2(void)
{
	char cLinea1[20];

	Leer_cadena_memAT45(cLinea1, 20, 0x24, MEM_USUARIOS);
	g_cContByteTxPrinBar=20;
	Escribir_cadena_impresora(cLinea1, g_cContByteTxPrinBar);


} //void PrintMsg2(void)
//**************************************************************************************
void PrintMsg3(void)
{

	char cLinea1[20];

	Leer_cadena_memAT45(cLinea1, 20, 0x38, MEM_USUARIOS);
	g_cContByteTxPrinBar=20;
	Escribir_cadena_impresora(cLinea1, g_cContByteTxPrinBar);

} //void PrintMsg3(void)
//**************************************************************************************
//**************************************************************************************
void PrintMsgAPD(void)
{
	g_scArrTxComPrintBar[0]='A'; 				
	g_scArrTxComPrintBar[1]='P';				  
	g_scArrTxComPrintBar[2]='D';
	g_scArrTxComPrintBar[3]=' '; 				
	g_scArrTxComPrintBar[4]='D';				  
	g_scArrTxComPrintBar[5]='E';				
	g_scArrTxComPrintBar[6]=' '; 				
	g_scArrTxComPrintBar[7]='C';				  
	g_scArrTxComPrintBar[8]='O';				
	g_scArrTxComPrintBar[9]='L'; 			
	g_scArrTxComPrintBar[10]='O';
	g_scArrTxComPrintBar[11]='M';				  
	g_scArrTxComPrintBar[12]='B';				
	g_scArrTxComPrintBar[13]='I'; 			
	g_scArrTxComPrintBar[14]='A';
	g_cContByteTxPrinBar=15;							
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

} //void PrintMsg3(void)
//**************************************************************************************
void alineacion (unsigned char justificar)			   //	0=IZQUIERDA 1=CENTRAR 2=DERECHA
{
		g_scArrTxComPrintBar[0]=0x1b; 					  
		g_scArrTxComPrintBar[1]='a';
		g_scArrTxComPrintBar[2]=justificar;				
		g_cContByteTxPrinBar=3;
		Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//---------------------------------------------------------------------------------------
//				PrintSerie();
//---------------------------------------------------------------------------------------
void PrintSerie(void)
{
		unsigned char cSerie,j,p,num_ceros;

	   	num_ceros=0;
		for (j=0; j<10; j++)
		{
  			Leer_cadena_memAT45(&cSerie, 1, j, MEM_USUARIOS);
			(cSerie==0x30)?(num_ceros++):(j=10);

	 	}
		if (num_ceros<10)
		{
			p=0;
			for (j=(num_ceros); j<10; j++)
			{
				Leer_cadena_memAT45(&cSerie, 1, j, MEM_USUARIOS);
				g_scArrTxComPrintBar[p]=cSerie;
				g_scArrRxComPrintBar[p]= cSerie;
				p++;
			}
			g_cContByteRxPrinBar=p;
			g_cContByteTxPrinBar=p;
			Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
			g_cEstadoTxSoft |= COD_PRINT_TX;
		}
} //void PrintSerie(void)
//----------------------------------------------------------------------------------------
char RAM_Serie(void)
{
		unsigned char cSerie,j,p,num_ceros,BCC;
	    BCC=0;

	   	num_ceros=0;
		for (j=0; j<10; j++)
		{
  			Leer_cadena_memAT45(&cSerie, 1, j, MEM_USUARIOS);
			(cSerie==0x30)?(num_ceros++):(j=10);

	 	}
		if (num_ceros<10)
		{
			p=0;
			for (j=(num_ceros); j<10; j++)
			{
				Leer_cadena_memAT45(&cSerie, 1, j, MEM_USUARIOS);
				g_scArrTxComPrintBar[p]=cSerie;
				g_scArrRxComPrintBar[p]= cSerie;
				
				BCC = BCC^g_scArrTxComPrintBar[p]	;

				p++;
			}
			g_cContByteRxPrinBar=p;
			g_cContByteTxPrinBar=p;


				


			Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//			g_cEstadoTxSoft |= COD_PRINT_TX;
		}
		return BCC;
} //void PrintSerie(void)
//---------------------------------------------------------------------------------------
char Cantidad_Ceros(void)
{
	unsigned char cSerie,j,num_ceros;
	num_ceros=0;
	for (j=0; j<10; j++)
	{
  		Leer_cadena_memAT45(&cSerie, 1, j, MEM_USUARIOS);
		(cSerie==0x30)?(num_ceros++):(j=10);
	}
	return num_ceros;
}
//----------------------------------------------------------------------------------------
//****************************************************************************************
void PrintDateHour(void)
{
	char cshora[4];
	char csfecha[3];


	Leer_hora(&cshora[0]);
	Leer_fecha(&csfecha[0]);


	g_scArrTxComPrintBar[0]=(csfecha[2]/10)+0x30;
	g_scArrTxComPrintBar[1]=(csfecha[2]%10)+0x30;
 
 	g_scArrTxComPrintBar[2]='/';

	g_scArrTxComPrintBar[3]=(csfecha[1]/10)+0x30;
	g_scArrTxComPrintBar[4]=(csfecha[1]%10)+0x30;

	g_scArrTxComPrintBar[5]='/';

	g_scArrTxComPrintBar[6]='2';
	g_scArrTxComPrintBar[7]='0';
	g_scArrTxComPrintBar[8]=(csfecha[0]/10)+0x30;
	g_scArrTxComPrintBar[9]=(csfecha[0]%10)+0x30;

	g_scArrTxComPrintBar[10]=' ';
	g_scArrTxComPrintBar[11]=' ';
	g_scArrTxComPrintBar[12]=' ';


 	g_scArrTxComPrintBar[13]=(cshora[0]/10)+0x30;
	g_scArrTxComPrintBar[14]=(cshora[0]%10)+0x30;

	g_scArrTxComPrintBar[15]=':';

	g_scArrTxComPrintBar[16]=(cshora[1]/10)+0x30;
	g_scArrTxComPrintBar[17]=(cshora[1]%10)+0x30;

//	g_scArrTxComPrintBar[16]=':';
//	g_scArrTxComPrintBar[17]=(cshora[2]/10)+0x30;
//	g_scArrTxComPrintBar[18]=(cshora[2]%10)+0x30;


	g_cContByteTxPrinBar=18;		
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void PrintDate(void)
{
	char cshora[4];
	char csfecha[3];


	Leer_hora(&cshora[0]);
	Leer_fecha(&csfecha[0]);
 
	g_scArrTxComPrintBar[0]=(csfecha[2]/10)+0x30;
	g_scArrTxComPrintBar[1]=(csfecha[2]%10)+0x30;
 
 	g_scArrTxComPrintBar[2]='/';

	g_scArrTxComPrintBar[3]=(csfecha[1]/10)+0x30;
	g_scArrTxComPrintBar[4]=(csfecha[1]%10)+0x30;

	g_scArrTxComPrintBar[5]='/';

	g_scArrTxComPrintBar[6]='2';
	g_scArrTxComPrintBar[7]='0';
	g_scArrTxComPrintBar[8]=(csfecha[0]/10)+0x30;
	g_scArrTxComPrintBar[9]=(csfecha[0]%10)+0x30;

	g_cContByteTxPrinBar=10;		
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void PrintHora(void)
{
	char cshora[4];
	char csfecha[3];


	Leer_hora(&cshora[0]);
	Leer_fecha(&csfecha[0]);


 	g_scArrTxComPrintBar[0]=(cshora[0]/10)+0x30;
	g_scArrTxComPrintBar[1]=(cshora[0]%10)+0x30;

	g_scArrTxComPrintBar[2]=':';

	g_scArrTxComPrintBar[3]=(cshora[1]/10)+0x30;
	g_scArrTxComPrintBar[4]=(cshora[1]%10)+0x30;

	g_cContByteTxPrinBar=5;		
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void enter (void)
{
	g_scArrTxComPrintBar[0]=0x0d;						//Enter CR
	g_scArrTxComPrintBar[1]=0x0a;						//LF
	g_cContByteTxPrinBar=2;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void start_format(void)
{
	g_scArrTxComPrintBar[0]='^';						//
	g_scArrTxComPrintBar[1]='X';						//
	g_scArrTxComPrintBar[2]='A';						//
	g_cContByteTxPrinBar=3;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void map_clear(void)
{
	g_scArrTxComPrintBar[0]='^';						//
	g_scArrTxComPrintBar[1]='M';						//
	g_scArrTxComPrintBar[2]='C';						//
	g_scArrTxComPrintBar[2]='Y';						// Y=Clear Bitmap  N=Do no clear bitmap
	g_cContByteTxPrinBar=4;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void field_data(void)
{
	g_scArrTxComPrintBar[0]='^';						//
	g_scArrTxComPrintBar[1]='F';						//
	g_scArrTxComPrintBar[2]='D';						//
	g_cContByteTxPrinBar=3;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void field_separator(void)
{
	g_scArrTxComPrintBar[0]='^';						//
	g_scArrTxComPrintBar[1]='F';						//
	g_scArrTxComPrintBar[2]='S';						//
	g_cContByteTxPrinBar=3;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void tamano_letra1(void)
{
	g_scArrTxComPrintBar[0]='5';						//
	g_scArrTxComPrintBar[1]='0';						//
	g_scArrTxComPrintBar[2]=',';						//
	g_scArrTxComPrintBar[3]='5';						//
	g_scArrTxComPrintBar[4]='0';						//
	g_cContByteTxPrinBar=5;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void end_format(void)
{
	g_scArrTxComPrintBar[0]='^';						//
	g_scArrTxComPrintBar[1]='X';						//
	g_scArrTxComPrintBar[2]='Z';						//
	g_cContByteTxPrinBar=3;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void field_origen(void)
{
	g_scArrTxComPrintBar[0]='^';						//
	g_scArrTxComPrintBar[1]='F';						//
	g_scArrTxComPrintBar[2]='O';						//
	g_cContByteTxPrinBar=3;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//**************************************************************************************** P
//		INCREMENTA SERIE EN MEMORIA
//****************************************************************************************
void Incrementa_Serie(void)
{

		char j,  cSerie, dato_mem[1];
//------------------------------------------------------------------------------------
//	Verifica que sean Numeros de 0-9
//------------------------------------------------------------------------------------
		for (j=0; j<10; j++)
		{
			Leer_cadena_memAT45(&cSerie, 1, j, MEM_USUARIOS);
			if ((cSerie<0x30)|(cSerie>0x39))
			{
				dato_mem[0]=0x30;
				Escribir_cadena_pagmemAT45(&dato_mem[0], 1, j, MEM_USUARIOS);
 			}
		}
//--------------------------------------------------------------------------------------
//	Incrementa Serie
//--------------------------------------------------------------------------------------
		for (j=0; j<10; j++)
		{
			Leer_cadena_memAT45(&cSerie, 1, (9-j), MEM_USUARIOS);
			cSerie++;
			if (cSerie>0x39)
			{
				dato_mem[0]=0x30;
				Escribir_cadena_pagmemAT45(&dato_mem[0], 1, (9-j), MEM_USUARIOS);
			}
			else
			{
				dato_mem[0]= cSerie;
				Escribir_cadena_pagmemAT45(&dato_mem[0], 1, (9-j), MEM_USUARIOS);
				j=10;
			}
		}
//----------------------------------------------------------------------------------------
}
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
//		COMANDOS KYTRONICS
//****************************************************************************************

void Capture(void)
{
	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=0X03;						// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
	g_scArrTxComPrintBar[5]='C';						//
	g_scArrTxComPrintBar[6]='3';						//
	g_scArrTxComPrintBar[7]='4';						//
	g_scArrTxComPrintBar[8]=ETX;						//
	g_scArrTxComPrintBar[9]=0X46;						// BCC
	g_cContByteTxPrinBar=10;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

}
//****************************************************************************************
void Lift_Up(void)
{
	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=0X03;						// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
	g_scArrTxComPrintBar[5]='C';						//
	g_scArrTxComPrintBar[6]='3';						//
	g_scArrTxComPrintBar[7]='8';						//
	g_scArrTxComPrintBar[8]=ETX;						//
	g_scArrTxComPrintBar[9]=0X4a;						// BCC
	g_cContByteTxPrinBar=10;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
void Sel_Stacker(void)
{
	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=0X05;						// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
	g_scArrTxComPrintBar[5]='C';						//
	g_scArrTxComPrintBar[6]='3';						//
	g_scArrTxComPrintBar[7]='1';						//
	g_scArrTxComPrintBar[8]=0X00;						// data
	g_scArrTxComPrintBar[9]=0X03;						// 
	g_scArrTxComPrintBar[10]=ETX;						//
	g_scArrTxComPrintBar[11]=0X46;						// BCC
	g_cContByteTxPrinBar=12;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

}
//****************************************************************************************
void Card_Position(void)
{
	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=0X03;						// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
	g_scArrTxComPrintBar[5]='C';						//
	g_scArrTxComPrintBar[6]='1';						//
	g_scArrTxComPrintBar[7]='6';						//
	g_scArrTxComPrintBar[8]=ETX;						//
	g_scArrTxComPrintBar[9]=0X46;						// BCC
	g_cContByteTxPrinBar=10;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

}
//****************************************************************************************
void Reset_RAM(void)
{
	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=0X03;						// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
	g_scArrTxComPrintBar[5]='P';						//
	g_scArrTxComPrintBar[6]='4';						//
	g_scArrTxComPrintBar[7]='2';						//
	g_scArrTxComPrintBar[8]=ETX;						//
	g_scArrTxComPrintBar[9]=0X54;						// BCC
	g_cContByteTxPrinBar=10;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//****************************************************************************************
//	 GUARDA 2 LINEAS DE TEXTO EN RAM + CARRIL + FECHA + HORA
//****************************************************************************************
void Set_SRAM_Text(void)
{
	char BCC,j;
	char cLinea1[20];
	char cLinea2[20];

	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=0X61;						// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
//......................................................//COMIENZO DE LONGITUD
	g_scArrTxComPrintBar[5]='P';						//
	g_scArrTxComPrintBar[6]='1';						//
	g_scArrTxComPrintBar[7]='6';						//
 	g_scArrTxComPrintBar[8]=0X01;						//
 	g_scArrTxComPrintBar[9]=CR;							// ENTER
 	g_scArrTxComPrintBar[10]=CR;						// ENTER 
	g_scArrTxComPrintBar[11]=CR;						// ENTER
	g_cContByteTxPrinBar=12;							// 7 DATOS
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
 	BCC=0X38;
//---------------------------------------------------------------------------------------
	Leer_cadena_memAT45(cLinea1, 20, 0x10, MEM_USUARIOS);
	for (j=0;  j<=19; j++)
	{
		BCC= BCC^cLinea1[j]	;
	}
	g_cContByteTxPrinBar=20;
	Escribir_cadena_impresora(cLinea1, g_cContByteTxPrinBar);
//---------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]=0x0d;						//Enter CR
	g_cContByteTxPrinBar=1;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
 	Leer_cadena_memAT45(cLinea2, 20, 0x24, MEM_USUARIOS);
	for (j=0;  j<=19; j++)
	{
		BCC= BCC^cLinea2[j]	;
	}
	g_cContByteTxPrinBar=20;
	Escribir_cadena_impresora(cLinea2, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]=0x0d;						//Enter CR
	g_scArrTxComPrintBar[1]=0x0d;						//Enter CR
	g_cContByteTxPrinBar=2;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]='C';						//
	g_scArrTxComPrintBar[1]='A';						//
	g_scArrTxComPrintBar[2]='R';						// 
	g_scArrTxComPrintBar[3]='R';						// 
	g_scArrTxComPrintBar[4]='I';						//
	g_scArrTxComPrintBar[5]='L';						//
	g_scArrTxComPrintBar[6]=':';						//
	g_scArrTxComPrintBar[7]=' ';						//
 	g_scArrTxComPrintBar[8]=g_cDirBoard;				//
 	g_scArrTxComPrintBar[9]=CR;							// ENTER
	for (j=0;  j<=8; j++)								//NO INCLUYO CR
	{
		BCC = BCC^g_scArrTxComPrintBar[j]	;
	}

 	g_cContByteTxPrinBar=10;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]='F';						//
	g_scArrTxComPrintBar[1]='E';						//
	g_scArrTxComPrintBar[2]='C';						// 
	g_scArrTxComPrintBar[3]='H';						// 
	g_scArrTxComPrintBar[4]='A';						//
	g_scArrTxComPrintBar[5]=' ';						//
	g_scArrTxComPrintBar[6]=':';						//
	g_scArrTxComPrintBar[7]=' ';						//
	g_scArrTxComPrintBar[8]=' ';						//
	for (j=0;  j<=8; j++)
	{
		BCC = BCC^g_scArrTxComPrintBar[j]	;
	}
 	g_cContByteTxPrinBar=9;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

    PrintDate();
	for (j=0;  j<=9; j++)
	{
		BCC = BCC^g_scArrTxComPrintBar[j]	;
	}
//----------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]=0x0d;						//Enter CR
	g_cContByteTxPrinBar=1;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]='H';						//
	g_scArrTxComPrintBar[1]='O';						//
	g_scArrTxComPrintBar[2]='R';						// 
	g_scArrTxComPrintBar[3]='A';						// 
	g_scArrTxComPrintBar[4]=' ';						//
	g_scArrTxComPrintBar[5]=' ';						//
	g_scArrTxComPrintBar[6]=':';						//
	g_scArrTxComPrintBar[7]=' ';						//
	g_scArrTxComPrintBar[8]=' ';						//
 
	for (j=0;  j<=8; j++)
	{
		BCC = BCC^g_scArrTxComPrintBar[j]	;
	} 
 
 	g_cContByteTxPrinBar=9;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

    PrintHora();
	for (j=0;  j<=4; j++)
	{
		BCC = BCC^g_scArrTxComPrintBar[j]	;
	}
//----------------------------------------------------------------------------------------
	g_scArrTxComPrintBar[0]=0x0d;						//Enter CR
	g_scArrTxComPrintBar[1]=0x0d;						//Enter CR
	g_scArrTxComPrintBar[2]=0x0d;						//Enter CR
//----------------------------------------------------------------------------------------
   	BCC=BCC^ETX;
  	g_scArrTxComPrintBar[3]=ETX;						//
	g_scArrTxComPrintBar[4]=BCC;						// BCC
//----------------------------------------------------------------------------------------
	g_cContByteTxPrinBar=5;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

}
//****************************************************************************************
// 	GUARDA EN RAM CODIGO DE BARRAS
//****************************************************************************************
void Set_SRAM_Code(void)
{
 	char Num_Digitos, BCC, j;

	BCC=0;
	Num_Digitos=Cantidad_Ceros();
	Num_Digitos=13+(10-Num_Digitos);

	g_scArrTxComPrintBar[0]=SOH;						//
	g_scArrTxComPrintBar[1]=NUL;						//
	g_scArrTxComPrintBar[2]=0X00;						// lONGITUD
	g_scArrTxComPrintBar[3]=Num_Digitos;				// LONGITUD
	g_scArrTxComPrintBar[4]=STX;						//
	g_scArrTxComPrintBar[5]='P';						//
	g_scArrTxComPrintBar[6]='3';						//
	g_scArrTxComPrintBar[7]='7';						//
 
 	g_scArrTxComPrintBar[8]=0X00;						//POSICION X = 100
 	g_scArrTxComPrintBar[9]=0X64;						//

 	g_scArrTxComPrintBar[10]=0X02;						//POSICION Y = 600
 	g_scArrTxComPrintBar[11]=0X58;						//

	g_scArrTxComPrintBar[12]=0X01;						//TIPO CODE 128

	g_scArrTxComPrintBar[13]=0X01;						//ROTAR 90° = 00

	g_scArrTxComPrintBar[14]=0X01;						//SCALE

 	g_scArrTxComPrintBar[15]=0X00;						//ANCHO	150
 	g_scArrTxComPrintBar[16]=0X96;						//

	g_scArrTxComPrintBar[17]=0X01;						//CODIGO VISIBLE

	BCC = 0xfe^Num_Digitos	;

	g_cContByteTxPrinBar=18;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

	j=RAM_Serie();

	BCC = BCC^j;
 	BCC = BCC^ETX;

   	g_scArrTxComPrintBar[0]=ETX;						//
	g_scArrTxComPrintBar[1]=BCC;						// BCC
	g_cContByteTxPrinBar=2;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
}
//**************************************************************************************
void Inicializa_CIPx(void)
{

/*	
	g_scArrTxComPrintBar[0]='C';  		//Inquiere Sensor Status.
	g_scArrTxComPrintBar[1]='I';
	g_scArrTxComPrintBar[2]='P';
	g_scArrTxComPrintBar[3]='x';
	g_cContByteTxPrinBar=4;
	Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
*/
	Set_SRAM_Code();	
}
//****************************************************************************************

//****************************************************************************************
// 		IMPRESION DE TICKET
//****************************************************************************************
void PrintTicket(void)
{ 

		Incrementa_Serie();

		switch (g_cTipoImpresora)
		{
			case IMPRESORA_Z4:

				g_scArrTxComPrintBar[0]='~'; 			//BORRA PAUSE
				g_scArrTxComPrintBar[1]='P';
				g_scArrTxComPrintBar[2]='S';
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
				enter();
//--------------------------------------------------------------------------------------
				start_format();
			   	enter();
				map_clear();
				enter();
				end_format();
				enter();
//---------------------------------------------------------------------------------------
				start_format();
			   	enter();

				g_scArrTxComPrintBar[0]='^'; 			// FIELD ORIENTATION
				g_scArrTxComPrintBar[1]='F';
				g_scArrTxComPrintBar[2]='W';
				g_scArrTxComPrintBar[3]='N';  			// N=NORMAL R=ROTAR 90° I=INVERTIDO 180° B=BUTTON UP 270°
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// CHANGE FONT  = FONDO ALFANUMERICO
				g_scArrTxComPrintBar[1]='C';
				g_scArrTxComPrintBar[2]='F';
				g_scArrTxComPrintBar[3]='D';  			// FONT (VALORES DE A-Z 0-9)
				g_scArrTxComPrintBar[4]=',';
				g_scArrTxComPrintBar[5]='2';			// ALTO DEL CARACTER
				g_scArrTxComPrintBar[6]='4';  			
 				g_cContByteTxPrinBar=7;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// PRINT WIDTH ANCHO DE IMPRESION
				g_scArrTxComPrintBar[1]='P';
				g_scArrTxComPrintBar[2]='W';
				g_scArrTxComPrintBar[3]='5';  			// 590 ANCHO
				g_scArrTxComPrintBar[4]='9';
				g_scArrTxComPrintBar[5]='0';			//
 				g_cContByteTxPrinBar=6;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// LABEL HOME  POSICION X,Y
				g_scArrTxComPrintBar[1]='L';
				g_scArrTxComPrintBar[2]='H';
				g_scArrTxComPrintBar[3]='0';  			// Posicion x
				g_scArrTxComPrintBar[4]=',';
				g_scArrTxComPrintBar[5]='0';			// Posicion y
 				g_cContByteTxPrinBar=6;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// CHANGE INTERNACIONAL FONT
				g_scArrTxComPrintBar[1]='C';
				g_scArrTxComPrintBar[2]='I';
				g_scArrTxComPrintBar[3]='0';  			// 0=SINGLE BYTE ENCODING USA
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// PRINT RATE
				g_scArrTxComPrintBar[1]='P';
				g_scArrTxComPrintBar[2]='R';
				g_scArrTxComPrintBar[3]='2';  			// 2=2"/SEG 3=3"/SEG 4=4"/SEG
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// MEDIA TRACKING
				g_scArrTxComPrintBar[1]='M';
				g_scArrTxComPrintBar[2]='N';
				g_scArrTxComPrintBar[3]='M';  			// M=MARK N=CONTINUO
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// MEDIA TYPE
				g_scArrTxComPrintBar[1]='M';
				g_scArrTxComPrintBar[2]='T';
				g_scArrTxComPrintBar[3]='D';  			// T=TRANSFERENCIA_TERMICA  D=TERMICA DIRECTA 
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// MEDIA DARKNESS
				g_scArrTxComPrintBar[1]='M';
				g_scArrTxComPrintBar[2]='D';
				g_scArrTxComPrintBar[3]='0';  			// 0=VALOR INICIAL AL PWR UP 
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// PRINT ORIENTATION
				g_scArrTxComPrintBar[1]='P';
				g_scArrTxComPrintBar[2]='O';
				g_scArrTxComPrintBar[3]='N';  			// N=NORMAL  I=INVERTIDO 180°
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// PRINT MIRROR
				g_scArrTxComPrintBar[1]='P';
				g_scArrTxComPrintBar[2]='M';
				g_scArrTxComPrintBar[3]='N';  			// N=NO  Y=YES
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[0]='^'; 			// LABEL REVERSE
				g_scArrTxComPrintBar[1]='L';
				g_scArrTxComPrintBar[2]='R';
				g_scArrTxComPrintBar[3]='N';  			// N=NO  Y=YES  (NEGRILLA FONDO LETRA ELANCO)
 				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
				enter();

				end_format();
				enter();
//---------------------------------------------------------------------------------------
				start_format();
			   	enter();
				g_scArrTxComPrintBar[0]='^'; 			
				g_scArrTxComPrintBar[1]='B';			// Y=CODE FIEL DEFAULT  
				g_scArrTxComPrintBar[2]='Y';
				g_scArrTxComPrintBar[3]='3';  			// ANCHO DE LINEAS DE CODIGO 3=COMPRIMDA; 4=COD. MAS EXTENSO
 				g_scArrTxComPrintBar[4]=','; 			
				g_scArrTxComPrintBar[5]='2';			// ANCHO DE BARRA CON RESPECTO AL ANCHO 2.0 Y 3.0  
				g_scArrTxComPrintBar[6]='.';
				g_scArrTxComPrintBar[7]='0';  			//
 				g_cContByteTxPrinBar=8;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				field_origen();

//				g_scArrTxComPrintBar[3]='2';		   	// POSICION X
//				g_scArrTxComPrintBar[4]=',';
//				g_scArrTxComPrintBar[5]='2';  			// POSICION Y 300
//				g_scArrTxComPrintBar[6]='5';
//				g_scArrTxComPrintBar[7]='0';
//   				g_cContByteTxPrinBar=8;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				g_scArrTxComPrintBar[3]='2';		   	// POSICION X
				g_scArrTxComPrintBar[4]=',';
				g_scArrTxComPrintBar[5]='1';  			// POSICION Y 300
				g_scArrTxComPrintBar[6]='0';
				g_scArrTxComPrintBar[7]='0';
   				g_cContByteTxPrinBar=8;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);


 				g_scArrTxComPrintBar[0]='^'; 			
				g_scArrTxComPrintBar[1]='B';			// B3=CODE_39  B2=CODE_INTERLEAVED_25
				g_scArrTxComPrintBar[2]='3';
				g_scArrTxComPrintBar[3]='N'; //'R';		   	// R=ROTAR_90  N=NORMAL I=INVERTIDO_180 B=BUTTON 270
				g_scArrTxComPrintBar[4]=',';
				g_scArrTxComPrintBar[5]='N';  			// MOD 43 DIGIT CHECK  Y=YES	 N=NO
				g_scArrTxComPrintBar[6]=',';
				g_scArrTxComPrintBar[7]='1'; 			// ALTO CODIGO BARRAS	180
				g_scArrTxComPrintBar[8]='0'; //'8';
				g_scArrTxComPrintBar[9]='0';
				g_scArrTxComPrintBar[10]=',';
				g_scArrTxComPrintBar[11]='Y'; 			// IMPRIME EL NUMERO DEL COD. DE BARRAS (INTERPRETACION) YES/NO
				g_scArrTxComPrintBar[12]=',';
				g_scArrTxComPrintBar[13]='N'; //'Y';			//(N=IMPRIME COD. VISIBLE PARTE INFERIOR;  Y: PARTE SUP)
   				g_cContByteTxPrinBar=14;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
 //--------------------------------------------------------------------------------------
				field_data();
				PrintSerie();
//---------------------------------------------------------------------------------------
				field_separator();
//---------------------------------------------------------------------------------------
 				g_scArrTxComPrintBar[0]='^'; 			
				g_scArrTxComPrintBar[1]='A';			// ^A0  TAMANO DE FONDO
				g_scArrTxComPrintBar[2]='0';
				g_scArrTxComPrintBar[3]='R';		   	// 
   				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

 				tamano_letra1();						// 50,50 (TAMAÑO DE LETRA (ANCHO Y LARGO)

				field_origen();							// ^FO

				g_scArrTxComPrintBar[0]='3';		   	// POSICION X
				g_scArrTxComPrintBar[1]='1';
				g_scArrTxComPrintBar[2]='5';  			// 
				g_scArrTxComPrintBar[3]=',';
				g_scArrTxComPrintBar[4]='6';		   	// POSICION y
				g_scArrTxComPrintBar[5]='9';
				g_scArrTxComPrintBar[6]='0';  			// 
   				g_cContByteTxPrinBar=7;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//***************************************************************************************
//				IMPRIME FECHA 
//***************************************************************************************

				g_scArrTxComPrintBar[0]='^'; 			
				g_scArrTxComPrintBar[1]='F';			// ^FCa,b,c  FIELD CLOCK
				g_scArrTxComPrintBar[2]='C';
				g_scArrTxComPrintBar[3]='%';		   	// PRIMER CARACTER INDICADOR  
				g_scArrTxComPrintBar[4]=',';
				g_scArrTxComPrintBar[5]='{';			// SEGUNDO CARACTER INDICADOR  
				g_scArrTxComPrintBar[6]=',';
				g_scArrTxComPrintBar[7]='#';			// TERCER CARACTER INDICADOR
   				g_cContByteTxPrinBar=8;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
			   	
				enter();
   				field_data();							// ^FD

				g_scArrTxComPrintBar[0]=' '; 			
				g_scArrTxComPrintBar[1]='{';			// 
				g_scArrTxComPrintBar[2]='Y';
				g_scArrTxComPrintBar[3]='/';		   	// 
				g_scArrTxComPrintBar[4]='{';
				g_scArrTxComPrintBar[5]='m';			// 
				g_scArrTxComPrintBar[6]='/';
				g_scArrTxComPrintBar[7]='{';			// 
				g_scArrTxComPrintBar[8]='d';
   				g_cContByteTxPrinBar=9;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				field_separator();
				enter();

//***************************************************************************************

 				g_scArrTxComPrintBar[0]='^'; 			
				g_scArrTxComPrintBar[1]='A';			// ^A0  TAMANO DE FONDO
				g_scArrTxComPrintBar[2]='0';
				g_scArrTxComPrintBar[3]='R';
				g_cContByteTxPrinBar=4;		   	// 
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				tamano_letra1();						// 50,50 (TAMAÑO DE LETRA (ANCHO Y LARGO)
  				field_origen();							// ^FO

				g_scArrTxComPrintBar[0]='2';		   	// POSICION X
				g_scArrTxComPrintBar[1]='7';
				g_scArrTxComPrintBar[2]='0';  			// 
				g_scArrTxComPrintBar[3]=',';
				g_scArrTxComPrintBar[4]='7';		   	// POSICION y
				g_scArrTxComPrintBar[5]='4';
				g_scArrTxComPrintBar[6]='0';  			// 
   				g_cContByteTxPrinBar=7;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//***************************************************************************************
//				IMPRIME HORA 
//***************************************************************************************

				g_scArrTxComPrintBar[0]='^'; 			
				g_scArrTxComPrintBar[1]='F';			// ^FCa,b,c  FIELD CLOCK
				g_scArrTxComPrintBar[2]='C';
				g_scArrTxComPrintBar[3]='%';		   	// PRIMER CARACTER INDICADOR  
				g_scArrTxComPrintBar[4]=',';
				g_scArrTxComPrintBar[5]='{';			// SEGUNDO CARACTER INDICADOR  
				g_scArrTxComPrintBar[6]=',';
				g_scArrTxComPrintBar[7]='#';			// TERCER CARACTER INDICADOR
   				g_cContByteTxPrinBar=8;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				enter();
				field_data();

				g_scArrTxComPrintBar[0]=' '; 			
				g_scArrTxComPrintBar[1]='{';			// Hora:Minutos
				g_scArrTxComPrintBar[2]='H';
				g_scArrTxComPrintBar[3]=':';		   	// 
				g_scArrTxComPrintBar[4]='{';
				g_scArrTxComPrintBar[5]='M';			// 
   				g_cContByteTxPrinBar=6;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
				
				end_format();
				enter();

//---------------------------------------------------------------------------------------
			break;
//----------------------------------------------------------------------------------------------------
			case IMPRESORA_EPSON:
				
 				g_scArrTxComPrintBar[0]=0x1b; 			//INICIALIZA IMPRESORA
				g_scArrTxComPrintBar[1]='@';
				g_cContByteTxPrinBar=2;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//---------------------------------------------------------------------------------------------------- 
 				alineacion('1');						   //ALINEACION  0=IZQUIERDA 1=CENTRAR 2=DERECHA
//----------------------------------------------------------------------------------------------------
  				g_scArrTxComPrintBar[0]=0x1d; 			//LABEL CODE BAR
				g_scArrTxComPrintBar[1]='H';
				g_scArrTxComPrintBar[2]='1';			//0=NO IMPRIME COD  1=CODIGO ARRIBA  2=ABAJO  3=AMBOS
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

  
  				g_scArrTxComPrintBar[0]=0x1d; 			//ANCHO CODE BAR
				g_scArrTxComPrintBar[1]='h';
				g_scArrTxComPrintBar[2]=0x6d;			//Ancho
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
 
  				g_scArrTxComPrintBar[0]=0x1d; 			//TIPO CODE BAR
				g_scArrTxComPrintBar[1]='k';
				g_scArrTxComPrintBar[2]=0x04;			//4=CODE 39
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
	
//---------------------------------------------------------------------------------------
				PrintSerie();
//---------------------------------------------------------------------------------------
  				g_scArrTxComPrintBar[0]=0x00; 			//
				g_scArrTxComPrintBar[1]=0x0d;
				g_scArrTxComPrintBar[2]=0x0a;			//Enter
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

  				g_scArrTxComPrintBar[0]=0x1b; 			//
				g_scArrTxComPrintBar[1]=0x21;
				g_scArrTxComPrintBar[2]=0x25;			//ANCHO BOLD
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
	
//------------------------------------------------------------------------------------
				enter();
 				PrintMsg1();
				enter();
//------------------------------------------------------------------------------------
   				g_scArrTxComPrintBar[0]=0x1b; 			//
				g_scArrTxComPrintBar[1]=0x21;
				g_scArrTxComPrintBar[2]=0x25;			//ANCHO BOLD
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				PrintMsg2();
				enter();
				enter();
//------------------------------------------------------------------------------------
//   			g_scArrTxComPrintBar[0]=0x1b; 			//
//				g_scArrTxComPrintBar[1]=0x21;
//				g_scArrTxComPrintBar[2]=0x01;			//NORMAL BOLD
//				g_cContByteTxPrinBar=3;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//
//   			g_scArrTxComPrintBar[0]=0x1b; 			//BOLD
//				g_scArrTxComPrintBar[1]='E';
//				g_scArrTxComPrintBar[2]='1';			// 
//				g_cContByteTxPrinBar=3;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//
//------------------------------------------------------------------------------------
   				g_scArrTxComPrintBar[0]=0x1b; 					//ANCHO NORMAL
				g_scArrTxComPrintBar[1]=0x21;
				g_scArrTxComPrintBar[2]=0x16;					// 
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

   				g_scArrTxComPrintBar[0]=0x1b; 					//BOLD
				g_scArrTxComPrintBar[1]='E';
				g_scArrTxComPrintBar[2]='1';					// 
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//------------------------------------------------------------------------------------
				PrintDateHour();
				enter();
				enter();
//------------------------------------------------------------------------------------
   				g_scArrTxComPrintBar[0]=0x1b; 						//
				g_scArrTxComPrintBar[1]=0x21;
				g_scArrTxComPrintBar[2]=0x01;						//NORMAL BOLD
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

   				g_scArrTxComPrintBar[0]=0x1b; 						//BOLD
				g_scArrTxComPrintBar[1]='E';
				g_scArrTxComPrintBar[2]='1';						// 
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

   				g_scArrTxComPrintBar[0]=0x1d; 						//FONT A
				g_scArrTxComPrintBar[1]='f';
				g_scArrTxComPrintBar[2]='0';						// 
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
				alineacion('1');							//0=IZQUIERDA 1=CENTRAR 2=DERECHA
//----------------------------------------------------------------------------------------
//  			g_scArrTxComPrintBar[0]=0x1b; 				//UNDERLINE
//				g_scArrTxComPrintBar[1]='-';
//				g_scArrTxComPrintBar[2]='2';				//0=OFF 1=ON 2=LINEA GRUESA
//				g_cContByteTxPrinBar=3;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//----------------------------------------------------------------------------------------
				PrintMsg3();
				enter();
				enter();
				enter();
//----------------------------------------------------------------------------------------	
//   			g_scArrTxComPrintBar[0]=0x1b; 				//UNDERLINE
//				g_scArrTxComPrintBar[1]='-';				  
//				g_scArrTxComPrintBar[2]='0';				//0=OFF 1=ON 2=LINEA GRUESA
//				g_cContByteTxPrinBar=3;							
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//-----------------------------------------------------------------------------------------
 				alineacion('1');							// 0=IZQUIERDA 1=CENTRAR 2=DERECHA
//----------------------------------------------------------------------------------------- 
  				g_scArrTxComPrintBar[0]=0x1d; 				//LABEL CODE BAR
				g_scArrTxComPrintBar[1]='H';
				g_scArrTxComPrintBar[2]='1';				//0=NO IMPRIME COD  1=CODIGO ARRIBA  2=ABAJO  3=AMBOS
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

  
  				g_scArrTxComPrintBar[0]=0x1d; 				//ANCHO CODE BAR
				g_scArrTxComPrintBar[1]='h';
				g_scArrTxComPrintBar[2]=0x6d;				//Ancho
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
 
  				g_scArrTxComPrintBar[0]=0x1d; 				//TIPO CODE BAR
				g_scArrTxComPrintBar[1]='k';
				g_scArrTxComPrintBar[2]=0x04;				//4=CODE 39
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
		
//---------------------------------------------------------------------------------------
				PrintSerie();
//---------------------------------------------------------------------------------------
  				g_scArrTxComPrintBar[0]=0x00; 				//Final de impresion de serie
				g_scArrTxComPrintBar[1]=0x0d;
				g_scArrTxComPrintBar[2]=0x0a;				//Enter
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//---------------------------------------------------------------------------------------
   				g_scArrTxComPrintBar[0]=0x1b; 				//
				g_scArrTxComPrintBar[1]=0x21;
				g_scArrTxComPrintBar[2]=0x01;				//NORMAL BOLD
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				alineacion('2');							// 0=IZQUIERDA 1=CENTRAR 2=DERECHA
 				PrintMsgAPD();
				enter();
				enter();
				enter();
				enter();
				enter();
				enter();

    			g_scArrTxComPrintBar[0]=0x1d; 				//Total_Cut
				g_scArrTxComPrintBar[1]='V';
				g_scArrTxComPrintBar[2]='0';				//
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

 				break;
//****************************************************************************************
			case IMPRESORA_EPIC880:
	
 				g_scArrTxComPrintBar[0]=0x18;
				g_scArrTxComPrintBar[1]=0x1b; 			//INICIALIZA IMPRESORA
				g_scArrTxComPrintBar[2]='@';
 				g_scArrTxComPrintBar[3]=0x18;
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
	
				enter();
				enter();	
//----------------------------------------------------------------------------------------
//				label_codebarEPIC();
	
				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]=0x19;						//
				g_scArrTxComPrintBar[2]='J';						//0=NO IMPRIME COD 1=CODIGO ARRIBA 2=ABAJO 3=AMBOS
				g_scArrTxComPrintBar[3]=0x11;						//Centrado y arriba el Label del codigo. (Pagina 142)
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
	
//------------------------------------------------------------------------------------------
//				ancho_codebarEPIC();
	
				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]=0x19;						//
				g_scArrTxComPrintBar[2]='W';						//
				g_scArrTxComPrintBar[3]=0x03;						//
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-------------------------------------------------------------------------------------------
//				alto_codebarEPIC();
	
				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]=0x19;						//
				g_scArrTxComPrintBar[2]='B';						//
				g_scArrTxComPrintBar[3]=0x05;						//
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-------------------------------------------------------------------------------------------
//				tipo_codebarEPIC();
	
				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]='b';						//
				g_scArrTxComPrintBar[2]=0x01;						// 1=CODE 39
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
	
//-------------------------------------------------------------------------------------------
				PrintSerie();
//-------------------------------------------------------------------------------------------
//				tx_aux(0x03);							//Parte del codigo de barra 		
//				enter(0x02);
				g_scArrTxComPrintBar[0]=0x03;
				g_cContByteTxPrinBar=1;						
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
				enter();
				enter();
//-----------------------------------------------------------------------------------------
				alineacion('1');						// 0=IZQUIERDA 1=CENTRAR 2=DERECHA
//------------------------------------------------------------------------------------------
   				g_scArrTxComPrintBar[0]=0x0e; 			//Doble Ancho
				g_scArrTxComPrintBar[1]=0x1b;		   	//Enfasis
				g_scArrTxComPrintBar[2]=0x45;
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

  				PrintMsg1();
//------------------------------------------------------------------------------------------
				enter();
  				PrintMsg2();
//------------------------------------------------------------------------------------------
				enter();
				enter();
				enter();
//-----------------------------------------------------------------------------------------
   				g_scArrTxComPrintBar[0]=0x0e; 						//Doble Ancho
				g_cContByteTxPrinBar=1;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				PrintDate();

				enter();

   				g_scArrTxComPrintBar[0]=0x0e; 						//Doble Ancho
				g_cContByteTxPrinBar=1;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

 				PrintHora();
				enter();
				enter();			//new
//----------------------------------------------------------------------------
//	   			g_scArrTxComPrintBar[0]=0x1b; 					//UNDERLINE
//				g_scArrTxComPrintBar[1]='_';
//				g_scArrTxComPrintBar[2]='1';					//0=OFF 1=ON 2=LINEA GRUESA
//				g_cContByteTxPrinBar=3;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				PrintMsg3();
				enter();
				enter();
//----------------------------------------------------------------------------------------------
//	   			g_scArrTxComPrintBar[0]=0x1b; 					//UNDERLINE
//				g_scArrTxComPrintBar[1]='_';
//				g_scArrTxComPrintBar[2]='0';					//0=OFF 1=ON 2=LINEA GRUESA
//				g_cContByteTxPrinBar=3;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//------------------------------------------------------------------------------
				enter();										// 				
//-----------------------------------------------------------------------------------
//				label_codebarEPIC();

				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]=0x19;						//
				g_scArrTxComPrintBar[2]='J';						//0=NO IMPRIME COD 1=CODIGO ARRIBA 2=ABAJO 3=AMBOS
				g_scArrTxComPrintBar[3]=0x11;						//Centrado y arriba el Label del codigo. (Pagina 142)
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
 
//-----------------------------------------------------------------------------------
//				ancho_codebarEPIC();

				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]=0x19;						//
				g_scArrTxComPrintBar[2]='W';						//
				g_scArrTxComPrintBar[3]=0x03;						//
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-----------------------------------------------------------------------------------
//				alto_codebarEPIC();

				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]=0x19;						//
				g_scArrTxComPrintBar[2]='B';						//
				g_scArrTxComPrintBar[3]=0x05;						//
				g_cContByteTxPrinBar=4;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-----------------------------------------------------------------------------------
//   			tipo_codebarEPIC();

				g_scArrTxComPrintBar[0]=0x1b;						//
				g_scArrTxComPrintBar[1]='b';						//
				g_scArrTxComPrintBar[2]=0x01;						// 1=CODE 39
				g_cContByteTxPrinBar=3;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-----------------------------------------------------------------------------------
				PrintSerie();

				g_scArrTxComPrintBar[0]=0x03;						//Parte del codigo de barra 
				g_cContByteTxPrinBar=1;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

				enter(); 	//new
//------------------------------------------------------------------------------------
//				g_scArrTxComPrintBar[0]=0x1b;						//Espera 10 s para retract
//				g_scArrTxComPrintBar[1]='i';						//
//				g_scArrTxComPrintBar[2]=0x04;						//
//				g_scArrTxComPrintBar[3]=0x0a;						// 10s
//				g_cContByteTxPrinBar=4;
//				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);
//-------------------------------------------------------------------------------
				g_scArrTxComPrintBar[0]=0x1b;						//Deliver
				g_scArrTxComPrintBar[1]=0x6b;						//
				g_cContByteTxPrinBar=2;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-----------------------------------------------------------------------------------
//				cut_EPIC();											//AUTOCUTTER

				g_scArrTxComPrintBar[0]=0x1b;						//	
				g_scArrTxComPrintBar[1]='v';						//
				g_cContByteTxPrinBar=2;
				Escribir_cadena_impresora(g_scArrTxComPrintBar, g_cContByteTxPrinBar);

//-----------------------------------------------------------------------------------	 
				break;

//-----------------------------------------------------------------------------------
			case IMPRESORA_KYTRONICS:


			break;


			} 	//switch
			
}



