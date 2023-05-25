void USART1_Configuration(void){
	UART_InitTypeDef UART_InitStructure;

	UART_DeInit(MDR_UART1);
	// Set the HCLK division factor = 1 for UART1,UART2
	UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
	
	NVIC_EnableIRQ(UART1_IRQn);
	
	// Initialize UART_InitStructure
	UART_InitStructure.UART_BaudRate                = 115200;
	UART_InitStructure.UART_WordLength              = UART_WordLength8b;
	UART_InitStructure.UART_StopBits                = UART_StopBits1;
	UART_InitStructure.UART_Parity                  = UART_Parity_No;
	UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF;
	UART_InitStructure.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

  // Configure UART1 parameters
	UART_Init (MDR_UART1,&UART_InitStructure);
	
	RS485_DIR(0);	// включение на вход RS485 USART1 <--
  // Enable transmitter interrupt (UARTTXINTR)
	UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);
//	UART_ITConfig (MDR_UART1, UART_IT_OE, ENABLE);
//	UART_ITConfig (MDR_UART1, UART_IT_BE, ENABLE);
//	UART_ITConfig (MDR_UART1, UART_IT_PE, ENABLE);
//	UART_ITConfig (MDR_UART1, UART_IT_FE, ENABLE);
	UART_ITConfig (MDR_UART1, UART_IT_RT, ENABLE);
	
  // Enables UART1 peripheral
	UART_Cmd(MDR_UART1,ENABLE);
}

void USART1_SendPacket(void)
{
	motor_status &= ~0x80;
	tick_counter1 = 0;
	USART1_bufTX[0] = 0x55;
	USART1_bufTX[1] = 0xAA;
	USART1_bufTX[2] = full_angle;//error;
	USART1_bufTX[3] = full_angle>>8;//error>>8;//CURR_200/6;
	USART1_bufTX[4] = full_angle>>16; 
	USART1_bufTX[5] = full_angle>>24;  //
	USART1_bufTX[6] = CURR_200/6;// MOTOR_St;//motor_status;
	USART1_bufTX[7] =	motor_status;// cosi;
	USART1_bufTX[8] = 0;// cosi>>8;
	USART1_bufTX[9] = VEL_actual_f;//a;
	USART1_bufTX[10] = VEL_actual_f>>8;//sinus>>8;
	USART1_bufTX[11] = ~(USART1_bufTX[2] + USART1_bufTX[3] + USART1_bufTX[4] + USART1_bufTX[5] + USART1_bufTX[6] + USART1_bufTX[7] + USART1_bufTX[8] + USART1_bufTX[9] + USART1_bufTX[10]);

	UART_ITConfig (MDR_UART1, UART_IT_TX, ENABLE);
	RS485_DIR(1);
	TxCounter1 = 0;
	MDR_UART1->DR = USART1_bufTX[TxCounter1];
}



void UART1_IRQHandler(void){

	if (UART_GetITStatus (MDR_UART1, UART_IT_RT))
		{
		tmp_dr = MDR_UART1->DR;
		}

    if (UART_GetITStatus (MDR_UART1, UART_IT_RX))
		{
		UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);
		SysTick1 = 0;

		t = MDR_UART1->DR;
		switch (RxCounter1)	
		{
			case 0: if(t == 0x57) RxCounter1++;
							break;
			case 1: if(t == 0x01) RxCounter1++;
							break;
			case 2: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 3: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 4: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 5: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 6: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 7: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 8: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 9: USART1_bufRX[RxCounter1] = t; RxCounter1++;
							break;
			case 10: if(t == ((uint8_t)(0xFF - ((uint8_t)(USART1_bufRX[2] + USART1_bufRX[3] + USART1_bufRX[4] + USART1_bufRX[5] + USART1_bufRX[6] + USART1_bufRX[7] + USART1_bufRX[8] + USART1_bufRX[9])))))
							{
								Work_buf[2] = USART1_bufRX[2];
								Work_buf[3] = USART1_bufRX[3];
								Work_buf[4] = USART1_bufRX[4];
								Work_buf[5] = USART1_bufRX[5];
								Work_buf[6] = USART1_bufRX[6];
								Work_buf[7] = USART1_bufRX[7];
								Work_buf[8] = USART1_bufRX[8];
								Work_buf[9] = USART1_bufRX[9];
								rec_flag = 1;
								RxCounter1 = 0;
								trans_flag = 1;
							}
							else {RxCounter1 = RxCounter1;}
							break;
			default: RxCounter1 = 0;
						break;
		}
		}
		
    if (UART_GetITStatus (MDR_UART1, UART_IT_TX))
		{
		UART_ClearITPendingBit(MDR_UART1, UART_IT_TX);
		if (TxCounter1 < TxBufferSize1)
			MDR_UART1->DR = USART1_bufTX[++TxCounter1];
		if (TxCounter1 == TxBufferSize1)
			{RS485_DIR(0);									// включение на вход RS485 USART1 <--
			UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);}
		}
}

int main(void){
	
	RST_CLK_Configuration();
	
	PORT_Configuration();
	
//	IWDG_Configuration();

	ADC_Configuration();

	DAC_init();

	TIMER1_Configuration();

	if (motor_mode & 1)	
		{TIMER2_Configuration();	// синхронный режим
		VEL_Acc_dV = 3;
		VEL_Dec_dV = 3;	
		CURR_max =50*6;				// токовая защита 5 А
		}
//	TIMER3_Configuration();

//	SysTick_Config(SysTick_ReloadValue);

//	PORT_ResetBits	(Led_RED);
//	PORT_SetBits	(OCR);
	TMR1_BRK(0);	// 



		
	while (1)
		{
//--------------------full-angle-------------------------------			
		full_angle = (angle) + Rot*3600 - angle_shift;		
//----------------velocity-control-by-user-------------------			
//		Vel_Control();			
//----------------position-push-pull-control-by-user-------------------
//		Vel_Rot_Control();	
//	PWM = PWML;			
			
			

//		PORT_ToggleBits(Led_RED);
		if (calc) {calc=0;}
//--------------------reset overcurrent-------------------------		
		if (K4 & 0x40)
			{
	//			K4 &= ~0x40;
				motor_status &= ~0x40;
			}
//--------------------set angle to zero-------------------------
			if (K4 & 0x20)
			{
	//			K4 &= ~0x20;
				SetToZero();

			}
//-----------------------receiving-transmitting---------------------		
			if(rec_flag == 1) // прием по UART
			{
				 USART1_processing();
			}
			if(trans_flag == 1) //передача по UART
			{
				USART1_SendPacket();
				trans_flag = 0;
			}
//-------------------------------------------------------------------			
			if(Stop_flag == 1) SetToZero();
		Alarm_Indication();
		}
} 