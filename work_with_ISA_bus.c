void receiveDiscrets (unsigned char *D)
{
	unsigned char i;
	unsigned char portA,portB, portC;
	
    out8(BaseRMM+15,0x02); //page2
    out8(BaseRMM+6,0);     //port0 (A) input 
	portA=in8(BaseRMM);  
    out8(BaseRMM+6,4);     //port1 (B) input
    portB=in8(BaseRMM+1);
	out8(BaseRMM+6,8);     //port2 (C) input
	portC=in8(BaseRMM+2);
	
	for (i=0;i<8;i++)
	{
		D[i]   =(portA>>i)&1;
		D[7-i+8] =(portB>>i)&1;
		D[7-i+16]=(portC>>i)&1;
	}

}

void GP_out_init (void)
{
   unsigned char tempPort;
   tempPort = in8(BaseElectra+11);
   tempPort = tempPort & 0xEF;
   out8(BaseElectra+11,tempPort);
}

void GP_on (void)
{
   unsigned char tempPort;
   tempPort = in8(BaseElectra+8);
   tempPort = tempPort | 0x01;
   out8(BaseElectra+8,tempPort);
}

void GP_instr_on (void)
{
  
   unsigned char tempPort;
   tempPort = in8(BaseElectra+8);
   tempPort = tempPort | 0x02;
   out8(BaseElectra+8,tempPort);
}


void GP_off (void)
{
   unsigned char tempPort;
   tempPort = in8(BaseElectra+8);
   tempPort = tempPort & 0xFE;
   out8(BaseElectra+8,tempPort);
}

void GP_instr_off (void)
{
  
   unsigned char tempPort;
   tempPort = in8(BaseElectra+8);
   tempPort = tempPort & 0xFD;
   out8(BaseElectra+8,tempPort);
}


void sendDiscrets (unsigned char *D)
{
   unsigned char portA;
   unsigned char i;
   portA=0;

   out8(BaseRMM+11,0x02); //
//   out8(BaseRMM+15,0x02); //page2
//   out8(BaseRMM+6,1);     //port0 output 
   for (i=0;i<=7;i++) portA+=D[i]<<i;
//   out8(BaseRMM,portA);
//   printf(" portA %d", portA);
}

void sendDAC(float *fDAC) // output analog signal
{
	int i;
	int iii;
	short int DAC[16];
	int intDAC[16];
	unsigned char LB, HB;
    unsigned char temp_inputt;
//	short int Signal = 4090,num = 0;
	
	out8(BaseRMM+15,0x01);//page1
	for (i=0;i<=15; i++)
	{
		//DAC[i]=(short int)((fDAC[i]/10.0)*2048+2048); //pereschot iz float v gradacii 
		//if (DAC[i]>4095) DAC[i]=4095;
		//if (DAC[i]<0)    DAC[i]=0;
		//DAC[i]=(int)((fDAC[i]/10.0)*2048+2048); //pereschot iz float v gradacii 
		
		
		/////added
		
		if (fDAC[i]> 5.0) fDAC[i] =  5.0;
		if (fDAC[i]<-5.0) fDAC[i] = -5.0;
//		intDAC[i]=(int)((fDAC[i]/10.0)*2048+2048); //pereschot iz float v gradacii 
		intDAC[i]=(int)((fDAC[i]/ 5.0)*2048+2048); //pereschot iz float v gradacii 
		if (intDAC[i]>4095) intDAC[i]=4095;
		if (intDAC[i]<0)    intDAC[i]=0;		
		DAC[i] = (short int)intDAC[i];
		/////end added
		LB = ((DAC[i] << 4) & 0xf0);
		HB = ((DAC[i] >> 4) & 255);
		out8(BaseRMM+0,LB);
		out8(BaseRMM+1,HB);
		iii = ((0x20 * ((i >> 2) & 0x03))+((i % 4)& 0x03));
		out8(BaseRMM+2,iii); 
	    while ((in8(BaseRMM+7))& 0xF0){};//wait until DABUSY 7 is 0
		temp_inputt = in8(BaseRMM+6);
		temp_inputt = temp_inputt | (0x01<<(i/4));
		out8(BaseRMM+6,temp_inputt);	                                                     
//		printf("\n LB \n %x", LB, "\n HB \n %x", HB, "\n num \n %x",iii, "\n i \n %x", (0x01 << (i/4)));

//	printf("\n i \n %x", ((i>>2)&0x03));
//*		in8(BaseRMM+0);
   }
   
   //Dla dat4i4ikov temperaturi vadaetsa signal na DAC ELK
/*	LB = Signal & 255;
	out8(BaseElectra+6,LB);
	
	num = 0;
	HB = Signal>>8;
	out8(BaseElectra+7,HB+ (num<<6));
	
	for (i=0;i<=15; i++){;}
	out8(BaseElectra+7,HB+(1<<6));
	for (i=0;i<=15; i++){;}
	out8(BaseElectra+7,HB+(2<<6));*/
}

void receiveADC(float *fADC)
{
	int i;
	short int ADC[16];
	unsigned char LB, HB;
	
	out8(BaseElectra,(unsigned char)144); //144 - 10010000 
	//7bit - dayom komandu na priyom dannih
	//4bit - reset FIFO
	while ( (in8(BaseElectra+3))&0x80) //80 -1000 0000 -proveryaem gotovnost' dannih
	{
				;
	}
	for (i=0; i<16; i++)
	{
		LB= (unsigned char)(in8(BaseElectra));
		HB= (unsigned char)(in8(BaseElectra+1));
		ADC[i]=(short int)(LB+HB*256);
		fADC[i]=(float)(ADC[i]/(32767.0/5.0));
//		ap_link(DACVal[i][0]
	}
}