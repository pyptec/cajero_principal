sbit D0L1= P3^3;
sbit D1L1= P3^2;

 extern char nbitsW;
 extern unsigned char xdata buffer_wie[10];
 extern unsigned long int byte_wie; 
 extern unsigned char apunta;
 
 bit paridad;							//bandera para indicar tipo de paridad
 bit error_wie;	
 bit lect1_ok;


 unsigned char byte_temp;
 unsigned char byte;
 unsigned char compara;

 unsigned char bcc;
 unsigned char dato_new;
 unsigned char type_lect;
 unsigned char shift;


 char lectura  []= "LECT.           " ;
 char formato  []= "WIEGAND     BITS" ;
 char err_pin  []= " ERROR DE CLAVE " ;
 char apx_err  []= "ERROR DE LECTURA" ;
 char apx_nbits[]= "        BITS    " ;
 
//*******************************************************************************************
//	BORRA BUFFER																			*
//*******************************************************************************************

//*******************************************************************************************
//	COVIERTE DE HEXADECIMAL A DECIMAL 2BYTES												*
//*******************************************************************************************
void ve_id(unsigned char id_h,unsigned char id_l)
{
	unsigned int valor,numero;
	unsigned char temp,dmil, mil, centena, decena, unidad;
	valor=0;
	dmil=0;
	mil=0;
	centena=0;
	decena=0;
	unidad=0;
	 
	temp=id_h;
	temp=temp&0xf0;
	temp>>=4;

	valor=valor+(pow(16,3))*temp;
	temp=id_h;
	temp=temp&0x0f;
	valor=valor+(pow(16,2))*temp;

	temp=id_l;
	temp=temp&0xf0;
	temp>>=4;
	valor=valor+(pow(16,1))*temp;
	temp=id_l;
	temp=temp&0x0f;
	valor=valor+(pow(16,0))*temp;
	numero=valor;

	while (numero>=0x2710)				// resto 10.000 
	{
		numero=numero-0x2710;
		dmil=dmil+1;
	}
	while (numero>=0x03e8) 				// resto 1.000
	{
		numero=numero-0x03e8;
		mil=mil+1;
	}
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

	vdato(dmil|0x30);
	vdato(mil|0x30);
	vdato(centena|0x30);
	vdato(decena|0x30);
	vdato(unidad|0x30);
}
//******************************************************************************************
//*******************************************************************************************
void ve_fc(unsigned char fc)
{
	unsigned int valor;
	unsigned char centena, decena, unidad;
	valor=0;

	centena=0;
	decena=0;
	unidad=0;
	 

	while (fc>=0x064) 				// resto 100
	{
		fc=fc-0x64;
		centena=centena+1;
	}
	while (fc>=0x0a)				// resto 10
	{
		fc=fc-0x0a;
		decena=decena+1;
	}
	unidad=fc;

	vdato(centena|0x30);
	vdato(decena|0x30);
	vdato(unidad|0x30);
}
//***************************************************************************************************************
//***************************************************************************************************************
//void rote (void)		  					/*rota y almacena en xram datos									   */
//{
//	byte_wie=byte_wie|dato_new;				//dato_new trae el valor del bit y se añade a los anteriores
//	byte--;									//byte contiene los 8 bits para armar el byte wiegand (byte_wie)
//	nbitsW++;								//Incrementa numero de bits
//	if (byte!=0)							//Byte va decrementando: llego a cero?
//	{
//		byte_wie<<=1;	
//	}
//	else		 							//byte==0 Si byte llega a cero ya alamceno 8 bits en Byte Wiegand
//	{
//	    buffer_wie[apunta]=byte_wie;		//Almacena el Byte
//		byte=0X08;							//Numero de bits de datos
//		byte_wie=0;							//Borra registro de almacenamiento
//		apunta++;  							//Incrementa direccion de buffer
//	}
//}
//***************************************************************************************************************
//***************************************************************************************************************
//		RECIBE WIEGAND POR LECTOR 1																				*
//***************************************************************************************************************
void  rx_wiegand (void) 			 			/* lectura wiegand LOGICA NEGADA POR ULN2004				   */
{                   			
 
	if (D0L1==0)								//Inicio por D0 Lector 1
	{
		nbitsW++;								//Incrementa numero de bits
		byte_wie=byte_wie&0xfffffffe;
		byte_wie<<=1;

  	}
  	else if (D1L1==0)							//Inicio por D1 Lector 1
  	{
 		nbitsW++;								//Incrementa numero de bits
 		byte_wie=byte_wie|0x00000001;
		byte_wie<<=1;
  	}
}


//*****************************************************************************************************************
bit analiza_wiegand(void)
{
	unsigned char k, mascara;
	bit in_apx;
	long int temp;

	if ((nbitsW==26))
	{
			byte_wie>>=2;	 	//Ajuste Bit Paridad + avance que deja rx_wiegand

			temp= byte_wie;
			temp>>=16;
			mascara=temp;
  			buffer_wie[0]=mascara;

 			temp= byte_wie;
			temp>>=8;
			mascara=temp;
  			buffer_wie[1]=mascara;

 			mascara=byte_wie;
  			buffer_wie[2]=mascara;

 
		cont(0x80);							// Informa de donde proviene la lectura		  	*
		for (k=0;lectura[k]!='\0';k++)
		{
			vdato(lectura[k]);
		}
		cont(0x85);							// Lector 1 o lector 2
		vdato('1'); 						// 												*
		cont(0x87);						   	// Visualiza FC + ID
		ve_fc(buffer_wie[0]);
		cont(0x8b);						   	// Visualiza FC + ID
		ve_id(buffer_wie[1],buffer_wie[2]);
		in_apx=1;
 	}
	else
	{

  		cont(0x80);
 		for (k=0;apx_err[k]!='\0';k++)
		{
  			vdato(apx_err[k]);
		}
  		cont(0xC0);
 		for (k=0;apx_nbits[k]!='\0';k++)
		{
  			vdato(apx_nbits[k]);
		}
		cont(0xC4);
		ve_fc(nbitsW);


		buffer_wie[0]=0;
		buffer_wie[1]=0;
		buffer_wie[2]=0;
		byte_wie=0x00000000;

		nbitsW=0;
		in_apx=0;
	}
return in_apx; 
}

//****************************************************************************************************************
