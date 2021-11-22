//*******************************************************************************************
//	usa LPR, envia wiegand al secundario
//	Solo funciona con Hardware v4
//
//*******************************************************************************************
#include <P89V51Rx2.h>             	// define 8051 registers 								*
#include <stdio.h>					//														*
#include <INTRINS.H>				//														*
#include <math.h>                   //														*          
#include "display.h"				//														*
//#include "wiegand.h"
//*******************************************************************************************
//	DEFINICION DE IO DEL MICROCONTROLADOR													*
//*******************************************************************************************
sbit msg1  = P0^0;					//Relevo de Salida (AUDIO)								*
sbit msg2  = P0^1;					//Relevo de Salida (AUDIO)								*
sbit msg3  = P0^2;					//Relevo de Salida (AUDIO)								*
sbit msg4  = P0^3;					//Relevo de Salida (AUDIO)								*

sbit sw_1 = P1^2;					//Direccion												*
sbit sw_2 = P1^3;					//Direccion												*
sbit ledv = P1^4;					//Led del boton expedidor								*
sbit lock1  = P1^6;					//Relevo de Entrada										*
sbit lock2  = P1^5;					//Relevo de Salida (Inhabilitado Proc. Aux usa ERR IMP)	*
sbit Electroiman  = P1^7;			//Para Cajon Monedero									*
//sbit automovil  = P1^7;			//Entrada sensor automovil								*
sbit SignalAcceso = P3^7;	 		//Sigue la señal de Sensor								*
sbit busy=P3^5;		  				//														*
sbit ready=P3^4;					//														*
sbit bus_clk=P3^6;					//														*
//*******************************************************************************************
// unsigned char expedidor   	[]= " AccesScan v0.2 " ;

 unsigned char offlinea     []= "EN MANTENIMIENTO" ;
 unsigned char err_mifare   []= "TARJETA INVALIDA" ;
 unsigned char err_cod      []= "ERROR COD. PARQ." ;
 unsigned char err_in       []= "  SIN INGRESO   " ;
// unsigned char err_tipo     []= "STATUS NO VALIDO" ;
 unsigned char err_alarma   []= "ALARMA    ACTIVA" ;

 unsigned char linea        []= "CAJERO OPERATIVO" ;
// unsigned char err_uso      []= "LO SENTIMOS....." ;

 unsigned char CMD_Out	    []= "CMD EXPULSION..." ;
 unsigned char OutWR        []= "TARJETA GRABADA." ;
 unsigned char OutSEL       []= "ERROR SEL. CARD " ;
 unsigned char ingreso      []= "In:             " ;
 unsigned char FueraServ    []= "  SIN SERVICIO  " ;
 unsigned char err_wr	    []= "ERROR GRABACION " ;
 unsigned char err_lect	    []= "ERROR RTA LECTOR" ;
// unsigned char err_Tarjeta  []= " ERROR TARJETA  " ;
// unsigned char err_TIME   []= " ERROR TIME OUT SW" ;
 unsigned char NIVEL;
 unsigned char Time_Devolucion=0;
//*******************************************************************************************		
//		DEFINICIÓN DE TIPOS
//*******************************************************************************************
// typedef enum {false=0, true=1} boolean;
//*******************************************************************************************
 unsigned int contador;					//contador de proposito general						*

// unsigned char cte_seg;	  				//Constante para segundo
 unsigned char byte;
// unsigned char apunta=0;

 unsigned char num_bytes;
// unsigned char control;
// unsigned char dir_h;
// unsigned char dir_l;
// unsigned char Print_Model;

 unsigned char Estado_Luz;
 unsigned char byte_out;

// unsigned char fc;
// unsigned char cod_h;
// unsigned char cod_l;

 unsigned char chr;
 unsigned char num_data;
 unsigned char len_buffer;

 unsigned char seg2;
 unsigned char seg;

 unsigned char xdata buffer_rx[60];   	//Buffer de recepcion de comunicacion
 unsigned char xdata buffer_ticket[60];
// unsigned char xdata buffer_wie[5];
 unsigned char xdata buffer_bus[60];


 unsigned char num_datos;

 unsigned int TimeOut_Codigo;


// unsigned char baud_rate;
 unsigned char val_time_bit;
 unsigned char temp; 
 unsigned char TimeOutLinea;
 unsigned char g_cRelevos;




// char nbitsW;
// unsigned long int byte_wie=0x00000000;

 bit time_out;
 bit time_out_com;
 bit error_com;
 bit flag_segundo; 						//Bandera de segundo
 bit txACK;
 bit txEOT;
// bit inicio_impresion;
// bit release;
 
// bit cmd_esc;
// bit rx_serie;
// bit err_print;
// bit CoverPrint;
// bit PaperOut;
// bit Transport;
// bit JamPaper;
// bit bandera_rx_soft;
// bit flanco_wiegand;

 bit toogle;
 bit FueraLinea=0;
// bit inicio_wiegand;
 bit retry;
// bit Flag_Dcto=0;
 bit txRTA=0;
 FueraServicio=0;
// unsigned char msg_error;
 unsigned char temp;
 bit sendactive=0;

 bit audio1=0;
 bit audio2=0;							  
 bit audio3=0;
 bit audio4=0;

 bit Grabacion=0;
 bit Rta_Send=0;
//--------------------------------------------------------------------------------------------------*
#define TAMANO_RX_COM_SOFT	100	
#define TAMANO_TX_COM_SOFT	50
#define CICLO_UTIL_BUZZER_SEQ	100
//--------------------------------------------------------------------------------------------------*
//#define VERIFICADOR		0X00
//#define EXPEDIDOR		0X01
//#define CAJA			0X02

#define cte_seg			0X1c
#define TIMEW			0x1e   //Tiempo para indicar TimeOut


//ESTADOR RECEPCION SOFTWARE
#define ESPERA_RX				20
#define VER_DIR					21
#define VER_COMANDO				22
#define LONG					23
#define SAVE_STR_SOF			24

#define SIN_CMD					25
#define POLL_COM_SOF			26
#define EJECT					27
#define APERTURA				28
#define WR_CLK					29
#define WR_SECTORMF				30
#define WIEGAND					31
#define WR_DISPLAY				32
#define STANDBY					33
#define EJECT_WR				34
#define EXIT					35
#define USUARIOT				36
#define CANCELO					37
#define FUERASERV				38
#define EJECT_WR2				39
//ESTADOR TRANSMICION SOFTWARE

#define SIN_LECTURA_TX		0x00
#define LECTURA_COD_TX		0x01
#define LECTURA_WIEG_TX		0x02
#define APERTURA_PUERTA		0x04
#define RETIRO_COFRE_COIN	0x08
#define RETIRO_COFRE_B2B	0x10  
#define ERROR_INFO 			0x20
#define CONSULTA_LECTOR		0x40
#define ERROR_WR_CARD		0x80
#define ERROR_TIME_OUT		0x41


#define ENQ	5
#define EOT	4
#define ACK	6
#define STX	2
#define ETX	3

#define NACK	0X15

#define CR	0x0d
#define LF	0x0a
//--------------------------------------------------------------------------------------------------*
unsigned char g_cEstadoComSoft=ESPERA_RX;
unsigned char g_cCMD_Soft=SIN_CMD;
unsigned char g_cEstadoTxSoft=SIN_LECTURA_TX;
unsigned char g_cContByteTx=0;
unsigned char g_scArrRxComSoft[TAMANO_RX_COM_SOFT];
unsigned char g_scArrTxComSoft[TAMANO_TX_COM_SOFT];
unsigned char g_cContByteRx=0;
unsigned char g_cDirBoard=0x30;
//unsigned char g_cFlagTipoControl=EXPEDIDOR;   ///CAJA;
unsigned char g_cTimerRxPrinBar=0;

//int  g_iContBuzzer;

//--------------------------------------------------------------------------------------------------*
//************************************************************************************************************
//
//************************************************************************************************************


//************************************************************************************************************
//********************************************************************************************
bit  tx_bus (unsigned char num_chr)
{
  	unsigned char j, timeOut;
	long int cont;
	bit Envio=0;

	bus_clk=1;
	cont=30000;
	timeOut=0;
	busy=0;
//----------------------------------------
 	while ((ready==1)&&(timeOut==0))
	{
		cont--;
		if (cont==0)
		{
			timeOut=1;
 		}
	}
//---------------------------------------
   	if (timeOut==0)
	{
		for (j=0; j<num_chr; j++)
		{
   			P2=buffer_bus[j];
			bus_clk=0;
			wait_long();
			bus_clk=1;
			wait_long();
		}
		Envio=1;
	}

	
	bus_clk=1;
	ready=1;
 	busy=1;

	return Envio;

}

//********************************************************************************************
//*******************************************************************************************
//
// Serial Interrupt Service Routine
//
//*******************************************************************************************
static void com_isr (void) interrupt 4 using 1 
{
	char cDatoRx;
//------------------------------------------------------------------------------------------*
// Received data interrupt. 																*
//------------------------------------------------------------------------------------------*
	if (RI) 
	{
	  	cDatoRx = SBUF; 			          		// read character
	  	RI = 0; 									// clear interrupt request flag

		switch (g_cEstadoComSoft)
		{
//---------------------------------------------------------------------------------------
			case ESPERA_RX:
			
			g_cContByteRx=0;
			if(cDatoRx==STX)
			{
				g_scArrRxComSoft[g_cContByteRx++]=cDatoRx;
				g_cEstadoComSoft=VER_DIR;
			}
			else  if(cDatoRx==ACK)
			{
					//Notifica al secundario que hubo Lectura o SW
				   	
					buffer_bus[0]=ACK;
					tx_bus(0x01);

			}

			break;
//--------------------------------------------------------------------------------------
			case VER_DIR:

			if(cDatoRx==g_cDirBoard)
			{
				g_scArrRxComSoft[g_cContByteRx++]=cDatoRx;
				g_cEstadoComSoft=LONG;
			}
			else
			{
				g_cEstadoComSoft=ESPERA_RX;
			}
			break;
//--------------------------------------------------------------------------------------
			case LONG:

			g_scArrRxComSoft[g_cContByteRx++]=cDatoRx;
			g_cEstadoComSoft=SAVE_STR_SOF;

			break;
//-----------------------------------------------------------------------------------------
			case SAVE_STR_SOF:
		
			g_scArrRxComSoft[g_cContByteRx++]=cDatoRx;
			if(g_cContByteRx>TAMANO_RX_COM_SOFT)
			{
				g_cEstadoComSoft=ESPERA_RX;
			}
			else if(cDatoRx==ETX)
			{
				TimeOutLinea=TIMEW;
				FueraLinea=0;

				switch (g_scArrRxComSoft[3])
				{
					case 'p':
						g_cCMD_Soft=POLL_COM_SOF;					// Poll
					break;
			
		
					case 'A':							  			// Apertura
						g_cCMD_Soft=APERTURA;
					break;

					case 'O':							  			// Out Card: Eject
					
						if (g_cContByteRx==16)
						{
						 	Grabacion=1;
							g_cCMD_Soft=EJECT_WR;
							Estado_Luz=0x03;
							Time_Devolucion=10;
 						}
						else  
						{
						 	g_cCMD_Soft=EJECT;
							if 	(Grabacion==0)
							{
								Estado_Luz=0x01;
							}
							
						}
						

					break;


					case 's':							  			// Out Card: Eject
					
						if (g_cContByteRx==21)
						{
						 	Grabacion=1;
							g_cCMD_Soft=EJECT_WR2;
							Estado_Luz=0x03;
							Time_Devolucion=10;
 						}
						else  
						{
						 	g_cCMD_Soft=EJECT;
							if 	(Grabacion==0)
							{
								Estado_Luz=0x01;
							}
							
						}						

					break;



					case 'M':							  			// Out Card: Eject
						
						 	g_cCMD_Soft=EJECT_WR;
 	
					break;

					case 'o':							  			// Cancelo Tarjeta T

						g_cCMD_Soft=CANCELO;
					   	Estado_Luz=0x01;

					break;

					case 'F':							  			// Cancelo Tarjeta T

						g_cEstadoComSoft=ESPERA_RX;
						g_cCMD_Soft=FUERASERV;
					   	FueraServicio=1;

					break;

 					case 'S':							  			// Wiegand
						
						g_cEstadoComSoft= ESPERA_RX;
						if (g_scArrRxComSoft[4]=='1')
						{
							g_cEstadoTxSoft &=~APERTURA_PUERTA;
						}
						else if (g_scArrRxComSoft[4]=='2')
						{
							g_cEstadoTxSoft &=~RETIRO_COFRE_B2B;
						}
						else if (g_scArrRxComSoft[4]=='3')
						{
							g_cEstadoTxSoft &=~RETIRO_COFRE_COIN;
						}

					break;

  					case 'W':							  			// Tarjeta MF WR
						g_cCMD_Soft=WR_SECTORMF;
					break;

 					case 'D':							  			// Mensaje Display
						g_cCMD_Soft=WR_DISPLAY;
			   		break;

 					case 'w':							  			// Wiegand
						g_cCMD_Soft=WIEGAND;
					break;

 					case 'e':							  			// Wiegand
						g_cEstadoTxSoft &= ~ERROR_INFO;
						
					break;

 					case 'U':							  			// Wiegand
						g_cEstadoTxSoft &= ~ERROR_INFO;
						g_cCMD_Soft=USUARIOT;
						NIVEL=g_scArrRxComSoft[4];
					break;
					

 					case 'E':							  			// Wiegand

						Electroiman=1;
						FueraServicio=0;
						g_cCMD_Soft=EXIT;


					break;	
					
					case '?':							  			// Wiegand
						
						buffer_bus[0]=STX;
						buffer_bus[1]=0x30;
						buffer_bus[2]=0x05;
						buffer_bus[3]='?';
						buffer_bus[4]=ETX;
						tx_bus(0x05);
						buffer_bus[3]=0x00;
						g_cEstadoComSoft=ESPERA_RX;
						g_cCMD_Soft=SIN_CMD;

					break;



					default:
						buffer_bus[3]=0x00;
						g_cEstadoComSoft=ESPERA_RX;
						g_cCMD_Soft=SIN_CMD;					
					break;														
				} 
				g_cEstadoComSoft=ESPERA_RX;

			}
			break;
//-----------------------------------------------------------------------------------------*
			default:
				g_cEstadoComSoft=ESPERA_RX;
			break;
		} 	  
	}
//------------------------------------------------------------------------------------------*
// Transmitted data interrupt. 																*
//------------------------------------------------------------------------------------------*
  	if (TI != 0) 
	{
    	TI = 0; 								// clear interrupt request flag
		sendactive=0;
  	}
}
//*******************************************************************************************
//*******************************************************************************************
// 	Transmision de un caracter																*
//*******************************************************************************************
 void tx_chr (unsigned char data_com)
 {
	while (sendactive==1) 
	{
	}
   	SBUF=data_com;
	sendactive=1;
 }
//*******************************************************************************************
void EscribirCadenaSoft(unsigned char tamano_cadena)
{
unsigned char i;
 
	for(i=0;i<tamano_cadena;i++)
   	{
        tx_chr(g_scArrTxComSoft[i]);
    }
	
}
//*******************************************************************************************
// 	Transmision de un caracter en 2 nibbles + 30h calcula BCC								*
//*******************************************************************************************
void tx_chrx2 (unsigned char chr)
{	
	unsigned char temp;
		 
		temp=chr&(0xf0);
		temp>>=4;
		temp=temp|(0x30);
		tx_chr(temp);

		temp=chr&(0x0f);
		temp=temp|(0x30);
		tx_chr(temp);
}

//*******************************************************************************************
//*******************************************************************************************
void relevos_aux(void)
{
	(audio1==1)?(msg1=1):(msg1=0);
	(audio2==1)?(msg2=1):(msg2=0);
	(audio3==1)?(msg3=1):(msg3=0);
	(audio4==1)?(msg4=1):(msg4=0);
}
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
// 	Transmision de 2bytes alamacenados en un entero INT										*
//*******************************************************************************************
//	Une 2 bytes a entero
//******************************************************************************************
unsigned int concatena (unsigned char byteh, unsigned char byte_l)
{
	unsigned int valor, valorl;

		valor=byteh;
		valor<<=8;
		valorl=byte_l;
   		valorl=valorl&0x00ff;
		valor=valor|valorl;	
		return valor;
}
//*******************************************************************************************
//*******************************************************************************************
// 	Transmision de 2bytes alamacenados en un entero INT										*
//*******************************************************************************************
 void tx_chr2bytes (unsigned int lchr)
 {
   		unsigned int bytelong;
		unsigned char caracter;

		bytelong=lchr;
		bytelong>>=8;
		caracter=bytelong;
//		strobe=1;
		SBUF=caracter;
 		TI=0;
 		while (TI==0) 
		{
 
 		}
 		caracter=lchr&0x00ff;
		SBUF=caracter;
 		TI=0;
 		while (TI==0) 
		{
 
 		}
//		strobe=0;
 }
//********************************************************************************************

//********************************************************************************************
void rx_bus (void)
{
	long int count;
	unsigned char timeOut,j=0, MaxData;

	busy=1;
	num_data=0;
	MaxData=60;

	if (ready==0)
	{
		timeOut=0;
		busy=0;
		for (j=0; j<MaxData; j++)
		{									 //50000
			count=30000;
			while ((bus_clk==1)&&(ready==0)&&(timeOut==0))
			{
				count--;
				if (count==0)
				{
					timeOut=1;
					j=MaxData+1;
				}		
			}	 
			if ((bus_clk==0)&&(timeOut==0)&&(ready==0))
			{
				buffer_bus[j]=P2;
				num_data++; 
				count=10000;				//65000
				while ((bus_clk==0)&&(timeOut==0))
				{
					buffer_bus[j]=P2;
					count--;
					if (count==0)
					{
						timeOut=1;
						j=MaxData+1;
					}				
				}
			}
			if (ready==1)
			{
				j=MaxData+1;
			}
		}
  	}
	busy=1;
	wait_long();
}

//********************************************************************************************
//********************************************************************************************
unsigned char two_one (unsigned char byte_h,unsigned char byte_l)
{
	byte_h=byte_h&0x0f;
	byte_h<<=4;
	byte_l=byte_l&0x0f;
	byte_out=byte_h|byte_l;
	return byte_out;
}
//*******************************************************************************************
//*******************************************************************************************
//	COVIERTE DE HEXADECIMAL A DECIMAL 2BYTES												*
//*******************************************************************************************
void ve_id(unsigned char xTIPO, unsigned char fc, unsigned char id_h, unsigned char id_l, unsigned char id_l2)
{
	unsigned long int valor,numero, j;
 	unsigned char buffer_name[16];

 	unsigned char Mmillon;
	unsigned char cmillon;
	unsigned char dmillon;
	unsigned char millon;
	unsigned char;
	unsigned char cmil;
	unsigned char dmil;
	unsigned char mil;
	unsigned char centena;
	unsigned char decena;
	unsigned char unidad;


//--------------------------------------------------------- 
// 	tx_chr(xTIPO);
//	tx_chr(fc);
//	tx_chr(id_h);
//	tx_chr(id_l);
//	tx_chr(id_l2);
//--------------------------------------------------------- 	 
 
 	valor=0x00000000;
	numero=0x00000000;

	Mmillon=0x00;
	cmillon=0x00;
	dmillon=0x00;
	millon=0x00;
	cmil=0x00;
	dmil=0x00;
	mil=0x00;
	centena=0x00;
	decena=0x00;
	unidad=0x00;


//  tx_chr(Mmillon);
//	tx_chr(cmillon);
//	tx_chr(dmillon);
//	tx_chr(millon);
//	tx_chr(cmil);
//  tx_chr(dmil);
//	tx_chr(mil);
//	tx_chr(centena);
//	tx_chr(millon);
//	tx_chr(cmil);

	valor=valor|fc;
	valor<<=8;
	valor=valor|id_h;
	valor<<=8;
	valor=valor|id_l;
	valor<<=8;
	valor=valor|id_l2;
	numero=valor;


	while (numero>=0x3B9ACA00)			// resto 1.000.000.000 
	{
		numero=numero-0x3B9ACA00;
		Mmillon++;
	}
	Mmillon=Mmillon|0x30;
//	tx_chr(Mmillon);

	while (numero>=0x05F5E100)			// resto 100.000.000 
	{
		numero=numero-0x05F5E100;
		cmillon++;
	}
	cmillon=cmillon|0x30;
//	tx_chr(cmillon);

	while (numero>=0x00989680)			// resto 10.000.000 
	{
		numero=numero-0x00989680;
		dmillon++;
	}
	dmillon=dmillon|0x30;
//	tx_chr(dmillon);

	while (numero>=0x000f4240)			// resto 1.000.000 
	{
		numero=numero-0x000f4240;
		millon=millon+1;
	}
	millon=millon|0x30;
// 	tx_chr(millon);

	while (numero>=0x000186a0)			// resto 100.000 
	{
		numero=numero-0x000186a0;
		cmil=cmil+1;
	}
	cmil=cmil|0x30;
//	tx_chr(cmil);

	while (numero>=0x2710)				// resto 10.000 
	{
		numero=numero-0x2710;
		dmil=dmil+1;
	}
	dmil=dmil|0x30;
//	tx_chr(dmil);

	while (numero>=0x03e8) 				// resto 1.000
	{
		numero=numero-0x03e8;
		mil=mil+1;
	}
	mil=mil|0x30;
//	tx_chr(mil);

	while (numero>=0x064) 				// resto 100
	{
		numero=numero-0x64;
		centena=centena+1;
	}
	centena=centena|0x30;
//	tx_chr(centena);

	while (numero>=0x0a)				// resto 10
	{
		numero=numero-0x0a;
		decena=decena+1;
	}
 	decena=decena|0x30;
//	tx_chr(decena);

 	unidad=numero;	 					// 1
	unidad=unidad|0x30;
//	tx_chr(unidad);

//	tx_chr(0x0d);
//	tx_chr(0x0a);
//------------------------------------------------------------------
	vdato(Mmillon);

	vdato(cmillon);
	vdato(dmillon);
	vdato(millon);

	vdato(cmil);
	vdato(dmil);
	vdato(mil);
	
	vdato(centena);
	vdato(decena);
	vdato(unidad);
//------------------------------------------------------------------
	if (xTIPO=='U')
	{
		num_data=15;
	    buffer_bus[0]=STX;
	    buffer_bus[1]='0';
	    buffer_bus[2]=15;
		buffer_bus[3]='U';
		buffer_bus[4]=Mmillon;
		buffer_bus[5]=cmillon;
		buffer_bus[6]=dmillon;
		buffer_bus[7]=millon;
		buffer_bus[8]=cmil;
		buffer_bus[9]=dmil;
		buffer_bus[10]=mil;
		buffer_bus[11]=centena;
		buffer_bus[12]=decena;
		buffer_bus[13]=unidad;
		buffer_bus[14]=ETX;	 
	}
	if (xTIPO=='D')
	{

		for (j=0; j<16; j++)
		{
			buffer_name[j]=buffer_bus[11+j];
		}

   		num_data=34;
	    buffer_bus[0]=STX;
	    buffer_bus[1]='0';
	    buffer_bus[2]=num_data;
		buffer_bus[3]='v';
		buffer_bus[4]=':';
		buffer_bus[5]=Mmillon;
		buffer_bus[6]=cmillon;
		buffer_bus[7]=dmillon;
		buffer_bus[8]=millon;
		buffer_bus[9]=cmil;
		buffer_bus[10]=dmil;
		buffer_bus[11]=mil;
		buffer_bus[12]=centena;
		buffer_bus[13]=decena;
		buffer_bus[14]=unidad;
		buffer_bus[15]=':';
		for (j=0; j<16; j++)
		{
			buffer_bus[j+16]=buffer_name[j];
		}
		buffer_bus[32]=':';
		buffer_bus[33]=ETX;	 
	}
	else 									// if (TIPO=='T')
	{
		num_data=18;
	    buffer_bus[0]=STX;
	    buffer_bus[1]='0';
	    buffer_bus[2]=18;
		buffer_bus[3]='T';
		buffer_bus[4]=':';
		buffer_bus[5]=xTIPO;
		buffer_bus[6]=':';
 		buffer_bus[7]=Mmillon;
		buffer_bus[8]=cmillon;
		buffer_bus[9]=dmillon;
		buffer_bus[10]=millon;
		buffer_bus[11]=cmil;
		buffer_bus[12]=dmil;
		buffer_bus[13]=mil;
		buffer_bus[14]=centena;
		buffer_bus[15]=decena;
		buffer_bus[16]=unidad;
		buffer_bus[17]=ETX;	 

	}
//-------------------------------------------------------------------
}	  
//******************************************************************************************
//*******************************************************************************************
 unsigned char bcd_dec (unsigned char data_clk)
 {
 	unsigned char temp,j;
	temp=data_clk;
	temp>>=4;
	temp=temp & 0x0f;
	if (temp!=0x00)
	{
		data_clk=data_clk & 0x0f;
	 	for (j=0;j<temp;j++)
	 	{
	  	 data_clk=data_clk+0x0a;
	 	}	
	}
	byte=(data_clk+0x30);
	return byte;
 }
//*******************************************************************************************
 unsigned char bcd_hex (unsigned char l_data)
 {
 	unsigned char temp,j;
	temp=l_data;
	temp>>=4;
	temp=temp & 0x0f;
	if (temp!=0x00)
	{
		l_data=l_data & 0x0f;
	 	for (j=0;j<temp;j++)
	 	{
	  	 	l_data=l_data+0x0a;
	 	}	
	}
	return l_data;
 }
//*******************************************************************************************
//*******************************************************************************************
//		Transforma HEX to BCD																*
//*******************************************************************************************
unsigned char transf (unsigned char byte)
{
    unsigned char nibble_h; 
	unsigned char nibble_l;
	unsigned char k;

	nibble_h=0x00;
	nibble_l=0x00;
	 
	if (byte > 0x30)
	{
		byte=byte-0x30;
		 for (k=0;k<byte;k++)
		 {
			nibble_l=nibble_l+0x01;
			if (nibble_l==0x0a)
			{
				nibble_l=0x00;
				nibble_h=nibble_h+0x01;
			}
		 }
		 nibble_h<<=4;
		 nibble_h=nibble_h & 0xf0;
		 nibble_l=nibble_l & 0x0f;
		 byte_out=(nibble_h | nibble_l);
 		 return byte_out;
	}
	return byte_out;
}
//*******************************************************************************************
//*******************************************************************************************
//		Transforma HEX Nativo to BCD														*
//*******************************************************************************************
unsigned char hex_bcd (unsigned char byte)
{
    unsigned char nibble_h; 
	unsigned char nibble_l;
	unsigned char k;

	nibble_h=0x00;
	nibble_l=0x00;

	for (k=0;k<byte;k++)
	{
		nibble_l=nibble_l+0x01;
		if (nibble_l==0x0a)
		{
			nibble_l=0x00;
			nibble_h=nibble_h+0x01;
		}
	 }
	 nibble_h<<=4;
	 nibble_h=nibble_h & 0xf0;
	 nibble_l=nibble_l & 0x0f;
	 byte_out=(nibble_h | nibble_l);
 	 return byte_out;
	
}
//*******************************************************************************************
//	CONVIERTE DE 1BYTE HEXADECIMAL A DECIMAL Y LOS VE EN DISPLAY							*
//*******************************************************************************************
void ve_num(unsigned char valorhex)
{
	unsigned char numero, centena, decena, unidad;

	decena=0;
	unidad=0;
 	numero=valorhex;

	while (numero>=0x064) 				// resto 100
	{
		numero=numero-0x64;
		centena=centena+1;
	}
	while (numero>=0x0a)				// resto 10
	{
		numero=numero-0x0a;
		decena=decena+1;
	}
	unidad=numero;
	vdato(decena|0x30);
	vdato(unidad|0x30);
}
//*******************************************************************************************
//*******************************************************************************************

//*******************************************************************************************
//		FUNCIONES PARA VISUALIZAR FECHA HORA												*
//*******************************************************************************************
 void vdata_clk (unsigned char data_clk) 
 {
 
 unsigned int temp;

 temp=data_clk;
 temp=temp & 0xf0;
 temp>>=4;
 temp=temp|0x30;
 vdato(temp);

 temp=data_clk;
 temp=temp & 0x0f;
 temp=temp|0x30;
 vdato(temp);

}

//*******************************************************************************************

//*******************************************************************************************

//*******************************************************************************************
//	CONDICIONES INICIALES																	*
//*******************************************************************************************
void cond_ini(void)
{
//------------------------------------------------------------------------------------------*
//Condiciones iniciales de hardware
//------------------------------------------------------------------------------------------*

	ledv=0;
  	lock1=0; 
//	if (lock2==1)  			//Lo configura de entrada. Es usado por Procesador 2, para ERR
//	{
//	}  
 	lock2=0;
//	cajon=1;
//------------------------------------------------------------------------------------------*
 	E=1;					//Display
 	RS=1;
//------------------------------------------------------------------------------------------*
// 	sck=0;					//Reloj
//	rst=0;
//	io=1;
//------------------------------------------------------------------------------------------*
// 	sda=1;					//Memoria
//	scl=1;		
//------------------------------------------------------------------------------------------*
//Condiciones Iniciales de variables de Acceso
//------------------------------------------------------------------------------------------*
//	buffer_wie[0]=0xff;
//	buffer_wie[1]=0xff;
//	buffer_wie[2]=0xff;
//	buffer_wie[3]=0xff;
//	buffer_wie[4]=0xff;

//------------------------------------------------------------------------------------------*
//Condiciones Iniciales de variables de Controlador
//------------------------------------------------------------------------------------------*

//------------------------------------------------------------------------------------------*
//Condiciones Iniciales de variables de Comunicaciones
//------------------------------------------------------------------------------------------*

//------------------------------------------------------------------------------------------*
//Condiciones Iniciales de variables de Tiempo
//------------------------------------------------------------------------------------------*
 	error_com=0;


}
//*******************************************************************************************
//*******************************************************************************************
void ve_dir(void)
{
	sw_1=0;
	sw_2=0;

	wait_long();
    g_cDirBoard=0x30;

	if (sw_1==1)
	{
		g_cDirBoard++;
	}
	if (sw_2==1)
	{
		g_cDirBoard=g_cDirBoard+2;
	}
	cont(0xc9);
	vdato(g_cDirBoard);
}
//**************************************************************************************************************
void AtencComSoft(void)
{

unsigned char j, longTx, num_chrL1, num_chrL2;

	switch (g_cCMD_Soft)
	{
//--------------------------------------------------------------------------------------------------------------
 		case SIN_CMD:


		break;
//--------------------------------------------------------------------------------------------------------------
		case POLL_COM_SOF:
			if (g_cEstadoTxSoft==SIN_LECTURA_TX)
			{				
				txACK=1;														//No hay Novedad
			}
//--------------------------------------------------------------------------------------------------------------
			else if ((g_cEstadoTxSoft&LECTURA_COD_TX)==LECTURA_COD_TX)			// Lectura Cod Barras
			{																	// Expedidor
					longTx=len_buffer;
					for (j=0; j<len_buffer; j++)
			  		{
						g_scArrTxComSoft[j]=buffer_ticket[j];
			  		}
//				   	buffer_ticket[0]=STX;
//					buffer_ticket[len_buffer-1]=ETX;

					EscribirCadenaSoft(longTx);
					g_cEstadoComSoft=ESPERA_RX;
					g_cCMD_Soft=SIN_CMD;


 			}
//--------------------------------------------------------------------------------------------------------------
			else if ((g_cEstadoTxSoft&ERROR_INFO)==ERROR_INFO)			
			{																
  
					longTx=len_buffer;
					for (j=0; j<len_buffer; j++)
			  		{
						g_scArrTxComSoft[j]=buffer_ticket[j];
			  		}
 				   	EscribirCadenaSoft(longTx);
					g_cEstadoComSoft=ESPERA_RX;
					g_cCMD_Soft=SIN_CMD;

 			}
//-----------------------------------------------------------------------------------------------------------------
		   	else if	((g_cEstadoTxSoft&ERROR_WR_CARD)==ERROR_WR_CARD)						// 
			{
				g_scArrTxComSoft[0]=STX;	 									
				g_scArrTxComSoft[1]=g_cDirBoard;
				g_scArrTxComSoft[2]=0X06;
				g_scArrTxComSoft[3]='G';
				g_scArrTxComSoft[4]='1';
				g_scArrTxComSoft[5]=ETX;
				EscribirCadenaSoft(6);
				g_cEstadoComSoft=ESPERA_RX;
				g_cCMD_Soft=SIN_CMD;
//				g_cEstadoTxSoft &=~ERROR_WR_CARD;										  
			}
//----------------------------------------------------------------------------------------------------------------			
		   	else if	((g_cEstadoTxSoft&APERTURA_PUERTA)==APERTURA_PUERTA)						// 
			{
  				g_scArrTxComSoft[0]=STX;
				g_scArrTxComSoft[1]='0';
  				g_scArrTxComSoft[2]=0X06;
				g_scArrTxComSoft[3]='S';
				g_scArrTxComSoft[4]='1';
  				g_scArrTxComSoft[5]=ETX;
				EscribirCadenaSoft(6);
				g_cEstadoComSoft=ESPERA_RX;
				g_cCMD_Soft=SIN_CMD;
			}
//----------------------------------------------------------------------------------------------------------------			
		   	else if	((g_cEstadoTxSoft&RETIRO_COFRE_B2B)==RETIRO_COFRE_B2B)						// 
			{
  				g_scArrTxComSoft[0]=STX;
				g_scArrTxComSoft[1]='0';
  				g_scArrTxComSoft[2]=0X06;
				g_scArrTxComSoft[3]='S';
				g_scArrTxComSoft[4]='2';
  				g_scArrTxComSoft[5]=ETX;
				EscribirCadenaSoft(6);
				g_cEstadoComSoft=ESPERA_RX;
				g_cCMD_Soft=SIN_CMD;
			}	
//---------------------------------------------------------------------------------------------------------------
		   	else if	((g_cEstadoTxSoft&RETIRO_COFRE_COIN)==RETIRO_COFRE_COIN)					//
			{
  				g_scArrTxComSoft[0]=STX;
				g_scArrTxComSoft[1]='0';
  				g_scArrTxComSoft[2]=0X06;
				g_scArrTxComSoft[3]='S';
				g_scArrTxComSoft[4]='3';
  				g_scArrTxComSoft[5]=ETX;
				EscribirCadenaSoft(6);
				g_cEstadoComSoft=ESPERA_RX;
				g_cCMD_Soft=SIN_CMD;
			}															
		break;
//**************************************************************************************************************
		case APERTURA:															   				//

			flag_segundo=0;
			seg=cte_seg;
			TH0=0X00;								
			TL0=0X00;
			TF0=0;
			txACK=1;
			buffer_bus[3]=0x00;		
																
		break;
//**************************************************************************************************************
		case WR_SECTORMF:

		    for (j=0; j<g_cContByteRx; j++)
			{
				buffer_bus[j]= g_scArrRxComSoft[j];
			}
			tx_bus(g_cContByteRx);
			g_cEstadoComSoft=ESPERA_RX;
			g_cCMD_Soft=SIN_CMD;

		break;
//**************************************************************************************************************
		case EJECT_WR:															   			//ANALIZA DATOS RECIBIDOS
		

			cont(0x80);
			for (j=0; j<16; j++)
			{
				buffer_bus[j]= g_scArrRxComSoft[j];
				vdato(g_scArrRxComSoft[j]);
			}
			

			if (ready==1)
			{
  			
				if (Grabacion==1)
				{
					tx_chr(ACK);
				}


				tx_bus(16);

				g_cEstadoTxSoft &=~LECTURA_COD_TX;
				g_cEstadoComSoft=ESPERA_RX;	
				g_cCMD_Soft=SIN_CMD;
			}
			
  
																
		break;
//**************************************************************************************************************
		case EJECT_WR2:															   			
		
			
   			if (ready==1)
			{

				for (j=0; j<21; j++)
				{
					buffer_bus[j]= g_scArrRxComSoft[j];
				}
				cont(0x80);
				for (j=12; j<20; j++)
				{
					vdato(g_scArrRxComSoft[j]);
				}

				tx_bus(21);

				g_cEstadoTxSoft &=~LECTURA_COD_TX;
				g_cEstadoComSoft=ESPERA_RX;	
				g_cCMD_Soft=SIN_CMD;
				if (Grabacion==1)
				{
					tx_chr(ACK);
				}
			}
			
  
																
		break;
//**************************************************************************************************************
//**************************************************************************************************************
		case EJECT:															   			
		
  			if (ready==1)
			{
						
				buffer_bus[0]=STX;
				buffer_bus[1]='0';
				buffer_bus[2]=0x05;
				buffer_bus[3]='O';
				buffer_bus[4]=ETX;
	
				Rta_Send=tx_bus(0x05);
				if (Rta_Send==1)
				{
	 
					cont(0x80);
					for (j=0; j<16; j++)
					{
						vdato(' ');
					}
					cont(0xc0);
					for (j=0; j<16; j++)
					{
						vdato(CMD_Out[j]);
					}			 
	 
	 				Rta_Send=0;
					g_cEstadoTxSoft &=~LECTURA_COD_TX;
					g_cCMD_Soft=SIN_CMD;
	 				g_cEstadoComSoft=ESPERA_RX;
				}
				else
				{
					g_cCMD_Soft=EJECT;
				}
  			}

																
		break;
//**************************************************************************************************************
		case USUARIOT:															   			//ANALIZA DATOS RECIBIDOS
		
		    if ((NIVEL>0X30)&&(NIVEL<=0X39))
			{
	 			cont(0x80);
	 			for (j=0; j<16 ;j++)
				{
	  				vdato(' ');
				}
	
	
				g_cEstadoTxSoft &=~LECTURA_COD_TX;
				buffer_bus[0]=STX;
				buffer_bus[1]=0x30;
				buffer_bus[2]=6;
				buffer_bus[3]='U';
				buffer_bus[4]=NIVEL;
				buffer_bus[5]=ETX;
				tx_bus(6);
				buffer_bus[3]=0x00;
				g_cCMD_Soft=SIN_CMD;
			}
 																
		break;
//**************************************************************************************************************
		case EXIT:															   			//ANALIZA DATOS RECIBIDOS
		

 			cont(0x80);
 			for (j=0; j<16 ;j++)
			{
  				vdato(' ');
			}

 			cont(0xc0);
 			for (j=0;linea[j]!='\0';j++)
			{
  				vdato(linea[j]);
			}

		   	FueraServicio=0;
			g_cEstadoTxSoft &=~LECTURA_COD_TX;

			buffer_bus[0]=STX;
			buffer_bus[1]=0x30;
			buffer_bus[2]=0x05;
			buffer_bus[3]='E';
			buffer_bus[4]=ETX;
			tx_bus(0x05);
			buffer_bus[3]=0x00;
			g_cCMD_Soft=SIN_CMD;

																
		break;
//**************************************************************************************************************
		case FUERASERV:															   			//ANALIZA DATOS RECIBIDOS
		

 			cont(0x80);
 			for (j=0; j<16 ;j++)
			{
  				vdato(' ');
			}

 			cont(0xc0);
 			for (j=0;FueraServ[j]!='\0';j++)
			{
  				vdato(FueraServ[j]);
			}


			g_cEstadoTxSoft =0;

			buffer_bus[0]=STX;
			buffer_bus[1]=0x30;
			buffer_bus[2]=0x05;
			buffer_bus[3]='F';
			buffer_bus[4]=ETX;
			tx_bus(0x05);
			buffer_bus[3]=0x00;
			g_cCMD_Soft=SIN_CMD;

 
																
		break;
//**************************************************************************************************************
		case CANCELO:															   			//ANALIZA DATOS RECIBIDOS
		
			cont(0x80);
			for (j=0; j<16 ;j++)
			{
				vdato(' ');
			}
			cont(0xc0);
			for (j=0; j<16 ;j++)
			{
				vdato(linea[j]);
			}
		
			buffer_bus[0]=STX;
			buffer_bus[1]=0x30;
			buffer_bus[2]=0x05;
			buffer_bus[3]='o';
			buffer_bus[4]=ETX;
			tx_bus(0x05);

			Electroiman=1;

			g_cEstadoComSoft=ESPERA_RX;	
			g_cEstadoTxSoft=SIN_LECTURA_TX;
			g_cCMD_Soft=SIN_CMD;						
 															
		break;
//***********************************************************************************************************
		case WR_DISPLAY:
 			
			if (g_cContByteRx>5)						// min 1 dato para el display
			{
			 	prg_disp();
				if (g_cContByteRx>21)
				{
					cont(0x80);
					for (j=0; j<16; j++)
					{
						vdato(g_scArrRxComSoft[4+j]);
					}	
					cont(0xc0);
				 	num_chrL2=g_cContByteRx-21;
					for (j=0; j<num_chrL2; j++)
					{
						vdato(g_scArrRxComSoft[20+j]);
					}
				}
				else
				{
 				 	num_chrL1=g_cContByteRx-5;
					cont(0x80);
					for (j=0; j<num_chrL1; j++)
					{
						vdato(g_scArrRxComSoft[4+j]);
					}	
				}
 				txACK=1;
			}
			else
			{
				g_cCMD_Soft=SIN_CMD;
			}
			buffer_bus[3]=0x00;
			

		break;
//------------------------------------------------------------------------------------------*
		default:
			g_cCMD_Soft=SIN_CMD;
		break;
	
	}
} //void AtencComSoft(void)
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//																							*
// Function to initialize the serial port and the UART baudrate.							*
//																							*
//*******************************************************************************************
void com_initialize (void) 
{
  	sendactive = 0; 			        // transmitter is not active  						*
  	SCON = 0x50;			         	// serial port MODE 1, enable serial receiver		*
	PCON |= 0x80; 			         	// 0x80=SMOD: set serial baudrate doubler			*
  	TMOD |= 0x20; 			         	// put timer 1 into MODE 2							*
 	TCON = 0x40;    					// TCON 											*
//------------------------------------------------------------------------------------------*
// 	TH1 =  0xFF;    					// TH1 	115000 Bps @ 22.148MHZ						*
// 	TL1 =  0xFF;    					// TH1 												*
//------------------------------------------------------------------------------------------*
 	TH1 =  0xF4;    					// TH1 9600 Bps @ 22.148MHZ							*					 
 	TL1 =  0xF4;    					// TH1 												*
//------------------------------------------------------------------------------------------*
//  TH1 = (unsigned char) (256 - (XTAL / (16L * 12L * baudrate)));							*
//------------------------------------------------------------------------------------------*
   	TR1 = 1; 							// start timer 1
  	ES = 1; 							// enable serial interrupts
}
//*******************************************************************************************

//*******************************************************************************************	
//	PROGRAMA PRINCIPAL																		*
//*******************************************************************************************
//*******************************************************************************************
void main (void)  
{

 unsigned char msgdir  	[]= "   Addr:        " ;  
 unsigned char lect_cod []= "LECT.           " ;

 extern unsigned char rx_wiegand_l1(void);

// unsigned char confirmacion []= "  LIBERE PULSADOR" ;
// unsigned char borrando     []= "  BORRANDO MEMORIA" ;

 unsigned char msgformato   []= "GRABACION USUARIOS" ;
 unsigned char msgCOMPRA   []= "COMPRA DESCUENTO" ;
 unsigned char clr_display []= "                ";

 unsigned int k;
 unsigned char Pos_Coma;
//*******************************************************************************************

//*******************************************************************************************
 	cond_ini();	 						// CONDICIONES INICIALES							*
  	txEOT=0;
	txACK=0;
	Electroiman=1;
//*******************************************************************************************
//*******************************************************************************************	
    TF2=0; 								//  Bandera de Timer								*
	TH2=0X00;							//													*						
	TL2=0X00;							//													*
	TR2=1;								// Run Timer 0 										*
//*******************************************************************************************	
  	EA = 1;                             // Enable global interrupts 						*
  	com_initialize ();                  // Initialize interrupt driven serial I/O 			*
//*******************************************************************************************	
	TMOD=(TMOD & 0xf0) | 0x01;			//  Coloca el temporizador 0 y 1 en modo 1.  16bITS	*
 	TF0=0; 								//  Bandera de Timer								*
	TH0=0X00;							//													*						
	TL0=0X00;							//													*
	TR0=1;								// Run TM2											*
//******************************************************************************************* 
//*******************************************************************************************
 	prg_disp()	;									// PROGRAMA DISPALY						*					
//------------------------------------------------------------------------------------------*
//	PROCESO DE RESET																		*
//------------------------------------------------------------------------------------------*
 	msg1=0;
	audio1=0;

	msg2=0;
	audio2=0;

	msg3=0;
	audio3=0;

	msg4=0;
	audio4=0;

	Estado_Luz=0x01;
	Time_Devolucion=0;
	Grabacion=0;
//------------------------------------------------------------------------------------------*
//	  MENSAJE INICIAL	   																	*
//------------------------------------------------------------------------------------------*
						
//   cont(0x80);
// 	for (k=0;expedidor[k]!='\0';k++)		
//	{
//		vdato(expedidor[k]);
//	}


	flag_segundo=0;						//Debe actualizar display en 1 segundo
	contador=0;
	seg=cte_seg;
	seg2=0;
	byte=0xff;

	byte=8;
	TimeOut_Codigo=0;
	retry=0;
	TimeOutLinea=TIMEW;
	g_cCMD_Soft=SIN_CMD;
	g_cEstadoComSoft=ESPERA_RX;
	txRTA=0;
	FueraServicio=0;
	g_cEstadoTxSoft=SIN_LECTURA_TX;
//*******************************************************************************************
//	USE_LPR=0;
//*******************************************************************************************
//			RECIBE DIR																		*
//*******************************************************************************************
	cont(0xc0);
 	for (k=0; linea[k]!='\0';k++)
	{
 		vdato(linea[k]);
	}				    				//													*
//*******************************************************************************************
//			LOOP PRINCIPAL		 															*
//*******************************************************************************************
	busy=1;
	bus_clk=1;
//*******************************************************************************************
  while (1) 										// Loop Principal						*	
  {                         				
 		P2=0xff;
		temp=P2;
//------------------------------------------------------------------------------------------*
//------------------------------------------------------------------------------------------*
		AtencComSoft();
//------------------------------------------------------------------------------------------*
//		INTERRUPCION DEL AUXILIAR
//------------------------------------------------------------------------------------------*
		if (ready==0)										//(Interrupcion generada del Aux)
		{
			buffer_bus[0]=0xff;
			rx_bus();
	
			if (num_data!=0)
			{
//------------------------------------------------------------------------------------------*
				len_buffer=num_data;
//				for (k=0; k<len_buffer; k++)
//				{
//					tx_chr(buffer_bus[k]);
//	 			}
//------------------------------------------------------------------------------------------*	
				seg2=cte_seg*4;

				if ((buffer_bus[0]==0x02))	 
				{

					
					if ((buffer_bus[3]=='L')||(buffer_bus[3]=='T')||(buffer_bus[3]=='U')||(buffer_bus[3]=='A')||(buffer_bus[3]=='p')||(buffer_bus[3]=='Q'))//adicione Q
					{
						
  						Estado_Luz=0x02;
					    Grabacion=0;

						if (buffer_bus[3]=='U')
						{
						  	cont(0x80);
							for (k=0;lect_cod[k]!='\0';k++)
							{
								vdato(lect_cod[k]);
							}
							cont(0x86);
							ve_id('U',buffer_bus[4],buffer_bus[5],buffer_bus[6],buffer_bus[7]);
 						}
						else if ((buffer_bus[3]=='T')&&(buffer_bus[5]=='D'))					// T:D	 TARJETA COMPRA DESCUENTO
						{
							cont(0xc0);
 							for (k=0;msgCOMPRA[k]!='\0';k++)
							{
 								vdato(msgCOMPRA[k]);
							}
							
						  	cont(0x80);
							for (k=0;lect_cod[k]!='\0';k++)
							{
								vdato(lect_cod[k]);
							}
							cont(0x86);							
							ve_id('D',buffer_bus[6],buffer_bus[7],buffer_bus[8],buffer_bus[9]);

						
						}
						else if ((buffer_bus[3]=='T')&&(buffer_bus[5]=='0'))					// T:0	 CREA TARJETAS
						{
							cont(0xc0);
 							for (k=0;msgformato[k]!='\0';k++)
							{
 								vdato(msgformato[k]);
							}
							
						  	cont(0x80);
							for (k=0;lect_cod[k]!='\0';k++)
							{
								vdato(lect_cod[k]);
							}
							cont(0x86);							
							ve_id('0',buffer_bus[6],buffer_bus[7],buffer_bus[8],buffer_bus[9]);
						}
 						else if ((buffer_bus[3]=='T')&&(buffer_bus[5]!='0'))		   			// T0:  NO DESARMA !!!!	   T:0 CREAR TARJETAS EN CAJERO
						{
							Electroiman=0;
							cont(0xc0);
 							for (k=0;offlinea[k]!='\0';k++)
							{
 								vdato(offlinea[k]);
							}
							cont(0x80);
							for (k=0;lect_cod[k]!='\0';k++)
							{
								vdato(lect_cod[k]);
							}
							cont(0x86);	   // buffer [5] = tipo
							ve_id(buffer_bus[5],buffer_bus[6],buffer_bus[7],buffer_bus[8],buffer_bus[9]);
						}
						else if (buffer_bus[3]=='A')		   // ALARMA !!!!
						{

							cont(0xc0);
 							for (k=0;err_alarma[k]!='\0';k++)
							{
 								vdato(err_alarma[k]);
							}
							cont(0xc7);
							vdato(buffer_bus[4]);
						}

						else if ((buffer_bus[3]=='L')	||	(buffer_bus[3]=='Q'))   // LECTURA !!!! modificado jp cmd Q
						{
  						
							for (k=0; k<num_data; k++)
							{
							  if (buffer_bus[k]==':')
							  {
							  	Pos_Coma=k;
								k=num_data+1;
							  }
							}


							cont(0x80);
 							for (k=0; ingreso[k]!='\0';k++)
							{
 								vdato(ingreso[k]);
							}
							cont(0x84);
							for (k=Pos_Coma+1; k<Pos_Coma+13 ;k++)
							{
 								vdato(buffer_bus[k]);
							}
							
							cont(0xc0);
							for (k=0;k<16;k++)
							{
								vdato(' ');
							}
						}
						else if (buffer_bus[3]=='p')		   // LECTURA !!!!
						{
								cont(0x80);
								for (k=0;k<16;k++)
								{
									vdato(' ');
								}
								cont(0x80);
	 							for (k=0; k<8;k++)
								{
	 								vdato(buffer_bus[4+k]);
								}
								for (k=0; k<8;k++)
								{
	 								vdato(buffer_bus[12+k]);
								}
	
								cont(0xc0);
								for (k=0;k<16;k++)
								{
									vdato(' ');
								}
								cont(0xc0);
								for (k=0; k<9;k++)
								{
	 								vdato(buffer_bus[21+k]);
								}


						}
	
						g_cEstadoTxSoft |= LECTURA_COD_TX;
						len_buffer=num_data;
						for (k=0; k<len_buffer; k++)
						{
							buffer_ticket[k]=buffer_bus[k];
//							tx_chr(buffer_ticket[k]);
	 					}
						TimeOut_Codigo=cte_seg*3;
						retry=0;

					}
					if (buffer_bus[3]=='p')
					{
						cont(0x80);
						for (k=0;k<16;k++)
						{
							vdato(' ');
						}
						cont(0x80);
						for (k=0; k<16;k++)
						{
							vdato(buffer_bus[4+k]);
						}


						cont(0xc0);
						for (k=0;k<16;k++)
						{
							vdato(' ');
						}
						cont(0xc0);
						for (k=0; k<9;k++)
						{
							vdato(buffer_bus[21+k]);
						}

						g_cEstadoTxSoft |= LECTURA_COD_TX;
						len_buffer=num_data;
						for (k=0; k<len_buffer; k++)
						{
							buffer_ticket[k]=buffer_bus[k];
	 					}
						TimeOut_Codigo=cte_seg*3;
						retry=0;


					}
					if ((buffer_bus[3]=='?'))
					{
						g_scArrTxComSoft[0]=STX;	 									//No hay Novedad 
						g_scArrTxComSoft[1]=g_cDirBoard;
						g_scArrTxComSoft[2]=0X06;
						g_scArrTxComSoft[3]='?';
						g_scArrTxComSoft[4]='V';
						g_scArrTxComSoft[5]=ETX;
						EscribirCadenaSoft(6);
						buffer_bus[3]=0xff;
	 				}
					else if ((buffer_bus[3]=='X')&&(buffer_bus[4]==ETX))
					{

						g_cEstadoTxSoft = SIN_LECTURA_TX;

						buffer_bus[3]=0x00;
						g_cCMD_Soft=SIN_CMD;
						FueraServicio=0;
						Electroiman=1;
   			 			cont(0x80);
			 			for (k=0; k<16 ;k++)
						{
			  				vdato(' ');
						}
			
			 			cont(0xc0);
			 			for (k=0;linea[k]!='\0';k++)
						{
			  				vdato(linea[k]);
						}
//---------------------------------------------------------------						
//						len_buffer=buffer_bus[2];
//						for (k=0; k<len_buffer; k++)
//						{
//							tx_chr(buffer_bus[k]);
//	 					}
//---------------------------------------------------------------
 					}
					else if ((buffer_bus[3]=='S')&&(buffer_bus[5]==ETX))
					{

						if (buffer_bus[4]=='1')
						{
							g_cEstadoTxSoft |= APERTURA_PUERTA;
						}
						else if (buffer_bus[4]=='2')
						{
							g_cEstadoTxSoft |= RETIRO_COFRE_B2B;
						}
						else if (buffer_bus[4]=='3')
						{
							g_cEstadoTxSoft |= RETIRO_COFRE_COIN;
 
						}
//						buffer_bus[3]=0x00;
						buffer_bus[5]=0X00;
						g_cCMD_Soft=SIN_CMD;
  
 					}
//					else if ((buffer_bus[3]=='G')&&(buffer_bus[6]==ETX))
//					{
//
// 					}
					else if (buffer_bus[3]=='e')
					{
						
						
//						g_cEstadoTxSoft |= ERROR_INFO;
//						len_buffer=num_data;
//						for (k=0; k<len_buffer; k++)
//						{
//							buffer_ticket[k]=buffer_bus[k];
//	 					}
//						TimeOut_Codigo=cte_seg*3;
//						retry=0;

//						tx_chr(buffer_bus[3]);
//						tx_chr(buffer_bus[4]);
  
/*
#define SIN_ERROR		0x30		// NO ERROR
#define ERR_AUTH		0x31		// ERROR AUTHENTICATION
#define ERR_RD			0x32		// NO SE PUDO LEER INFO
#define ERR_WR			0x33		//
#define ERR_EEPROMK		0x34 		// ERROR AL GRABAR EN LECTOR
#define ERR_LECTOR		0x35		// RESPUESTA INCORRECTA DEL LECTOR
#define ERR_EJECT		0x36		// ERROR EN EXPULSAR
#define ERR_APB			0x37		// NO POSEE INGRESO
#define ERR_COD			0x38		// TARJETA NO ES DE ESTE PARQUEADERO
#define DESARM			0x39 		// CAJERO DESARMADO / NO PUEDE LIQUIDAR TARJETA DE ROTACION
#define ROTACION		0x3A	  	// TARJETA DE ROTACION
#define ERR_CARD_TIPO	0X3B   		// ERROR TIPO DE TARJETA (STATUS)
#define EXPULSADA		0X3C   		// Expulsada Sin Grabacion 
#define EXPULSADA_WR	0X3D   		// Expulsada con Grabacion
#define CARD_EJECT_WR	0X3E   		// ERROR TIPO DE TARJETA (STATUS)
#define ERR_SELECT		0X3F   		// ERROR TIPO DE TARJETA (STATUS) 
#define ERR_TARJETA		0X40   		// ERROR TARJETA MAL INGRESADA
*/
						switch (buffer_bus[4])
						{
						 	case 0x31:
								cont(0xC0);
								for (k=0; k<16; k++)
								{
									vdato(err_mifare[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}
								Estado_Luz=0x01;

							break;
								
						 	case 0x33:
								cont(0xC0);
								for (k=0; k<16; k++)
								{
									vdato(err_wr[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}
								Estado_Luz=0x01;
																									
							break;


								
						 	case 0x34:
								cont(0xC0);
								for (k=0; k<16; k++)
								{
									vdato(err_lect[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}
								Estado_Luz=0x01;
																									
							break;

						 	case 0x37:
								cont(0xC0);
								for (k=0; k<16; k++)
								{
									vdato(err_in[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}
								Estado_Luz=0x01;
																	
							break;

						 	case 0x38:
								cont(0xC0);
								for (k=0; k<16; k++)
								{
									vdato(err_cod[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}
								Estado_Luz=0x01;
																	
							break;

						 	case 0x39:
								cont(0xC0);
								for (k=0; k<16; k++)
								{
//									vdato(err_uso[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}
								
																	
							break;


						 	case 0x3B:
								cont(0xC0);
//								for (k=0; k<16; k++)
//								{
//									vdato(err_tipo[k]);
//								}

								vdato('E');
								vdato('r');
								vdato('r');
								vdato('o');
								vdato('r');
								vdato(' ');
								vdato('S');
								vdato('t');
								vdato('a');
								vdato('t');
								vdato('u');
								vdato('s');

								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}									
							break;

						 	case 0x3D:
								cont(0xc0);
//								for (k=0; k<16; k++)
//								{
//									vdato(CMD_Out[k]);
//								}
//								
						
								vdato('E');
								vdato('r');
								vdato('r');
								vdato('.');
								vdato(' ');
								
								vdato('C');
								vdato('a');
								vdato('r');
								vdato('d');
								vdato(' ');
								vdato('E');
								vdato('j');
								vdato('e');
								vdato('c');
								vdato('t');
								vdato(' ');

								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}	
							break;

						 	case 0x3E:
								cont(0xc0);
								for (k=0; k<16; k++)
								{
									vdato(OutWR[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}	
							break;

						 	case 0x3F:
								cont(0xc0);
								for (k=0; k<16; k++)
								{
									vdato(OutSEL[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}	
							break;


						 	case 0x40:

								Estado_Luz=0x01;
//				 				g_scArrTxComSoft[0]=STX;
//								g_scArrTxComSoft[1]='0';
//				  				g_scArrTxComSoft[2]=0X06;
//								g_scArrTxComSoft[3]='?';
//								g_scArrTxComSoft[4]='F';
//				  				g_scArrTxComSoft[5]=ETX;
//								EscribirCadenaSoft(6);
//								g_cEstadoComSoft=ESPERA_RX;
//								g_cCMD_Soft=SIN_CMD;

								g_cEstadoTxSoft |= ERROR_WR_CARD;
								buffer_bus[3]=0xff;
  								g_cCMD_Soft=SIN_CMD;


								cont(0xc0);
//								for (k=0; k<16; k++)
//								{
//									vdato(err_Tarjeta[k]);
//								}
								vdato('E');
								vdato('r');
								vdato('r');
								vdato('o');
								vdato('r');
								vdato(' ');
								vdato('C');
								vdato('A');
								vdato('R');
								vdato('D');

								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}	
							break;

						 	case 0x41:


								g_cEstadoTxSoft &=~LECTURA_COD_TX;
								g_cEstadoComSoft=ESPERA_RX;	
								g_cCMD_Soft=SIN_CMD;


								Estado_Luz=0x01;

								buffer_bus[3]=0xff;
  								g_cCMD_Soft=SIN_CMD;


								cont(0xc0);
								for (k=0; k<16; k++)
								{
//									vdato(err_TIME[k]);
								}
								cont(0x80);
			 					for (k=0; k<16 ;k++)
								{
			  						vdato(' ');
								}	
							break;
 
						}
						
					}

				}
				num_data=0;
			}
			buffer_bus[0]=0x00;
 		}				
//------------------------------------------------------------------------------------------*
//		FIN ATENCION INT
//------------------------------------------------------------------------------------------*
		if (txACK==1)
		{
			g_scArrTxComSoft[0]=STX;	 									//No hay Novedad 
			g_scArrTxComSoft[1]=g_cDirBoard;
			g_scArrTxComSoft[2]=0X05;
			g_scArrTxComSoft[3]=ACK;
			g_scArrTxComSoft[4]=ETX;
			EscribirCadenaSoft(5);
			g_cEstadoComSoft=ESPERA_RX;
			g_cCMD_Soft=SIN_CMD;
			g_cEstadoTxSoft=SIN_LECTURA_TX;
			txACK=0;		
 		}
//------------------------------------------------------------------------------------------*
//		TIMERS
//------------------------------------------------------------------------------------------*
		if (TF2==1)
		{
			TF2=0;
 			if (seg2!=0)
			{
	 			seg2--;
				if (seg2==0)
				{
					seg2=cte_seg;


				}
			}
 		}
//----------------------------------------------------------------------------------------------------------*

//**********************************************************************************************
		if(TF0==1)
		{
			TF0=0;
			seg--;
//----------------------------------------------------------------------------------------------*
			if (SignalAcceso==0)
			{
				if ((seg==cte_seg/2)||(seg==0))
				{
					if (toogle==1)
					{
						ledv=0;
						toogle=0;
					}
					else
					{
						ledv=1;
						toogle=1;
					}
				}
			}
			else
			{
			 	ledv=1;
			}
//----------------------------------------------------------------------------------------------*
			if ((seg==cte_seg/2)||(seg==0))
			{
				if (Estado_Luz==0x01)
				{
					audio1=~audio1;
					audio2=0;
					audio3=0;
					audio4=0;
				}
				else if (Estado_Luz==0x02)
				{
					audio1=0;
					audio2=~audio2;
					audio3=0;
					audio4=0;

				}
				else if (Estado_Luz==0x03)
				{
					audio1=0;
					audio2=0;
					audio3=~audio3;
					audio4=0;

				}
			}
//----------------------------------------------------------------------------------------------*
			if (seg==0x00)
 	 		{
				seg=cte_seg;				//Base de segundo con eltimer 0x1c = 28				*
				if (TimeOutLinea!=0)
				{
					TimeOutLinea--;
	
				}
				if (Time_Devolucion!=0)
				{
			   		Time_Devolucion--;
					if (Time_Devolucion==0)
					{
						Estado_Luz=0x01;
					}

				}
				
				if (TimeOutLinea==0)
				{
	
					TimeOutLinea=TIMEW;
					cont(0x80);
	 				for (k=0;k<16;k++)
					{
	  					vdato(' ');
					}
	
	  				FueraLinea=1;
	
				}

 


	 			lock1=0;
				lock2=0;
				flag_segundo=0;
				seg=cte_seg;
				TH0=0X00;								//Inicializa timer										*						
				TL0=0X00;


 			}
			if (TimeOut_Codigo!=0)
			{
			 	TimeOut_Codigo--;
				if (TimeOut_Codigo==0x0001)
				{
//---------------------------------------------------------
//					g_cEstadoTxSoft &=~LECTURA_COD_TX;
//					buffer_wie[0]=0x00;
//					buffer_wie[1]=0x00;
//					buffer_wie[2]=0x00;
//					buffer_wie[3]=0x00;
//					nbitsW=0;
//----------------------------------------------------------
				}
			}
			else
			{
 				retry=0;				
			}
		}
//----------------------------------------------------------------------------------------------*
		relevos_aux();
//----------------------------------------------------------------------------------------------*
   }												//Cierra While Principal					* 
}													//Cierra main								*
//***********************************************************************************************

