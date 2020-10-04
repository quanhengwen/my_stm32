#include "main.h"
#include "pic.h"

extern _calendar_obj calendar;

void Put_hello(void);
void WeatherShow(void);
u32 GetHttpCnt=0;
u8 Beijing_flag = 0;
u8 parse_now_weather(void);
vu32 SELStatus = 0;
void IntExtOnOffConfig(FunctionalState NewState);
void InterruptConfig(void);
u8 min_buf;


u16 write_counter;

u16 read_counter;

u8 *test_readbuf;

u8 test_writebuf[1024]={0x01,0x02,0x03,0x04,0x05,0x06};

char *string;

KFIFO *test_fifo_buffer = NULL;

 int main(void)
 { 
	u8 data;
	u16 i;
	char* Timebuf;
	delay_init();	    	 		//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	uart_init(115200);	 			//���ڳ�ʼ��Ϊ115200
	 
	TIM2_Int_Init(199,7199);		//20ms
	 
	TIM3_Int_Init(99,7199);			//10ms
	 
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
	 
	gpio_init();
	 
//	RTC_Init();
	 
	mem_init();						
	 
//	KEY_Init();
	 
	GUI_Initialize();				/*ZLG GUIӲ����ʼ��*/		
	GUI_SetColor(WHITE,BLUE);		//���ñ���ɫΪ��ɫ  ������ɫΪ��ɫ
	while( font_init())
	{
		GUI_PutString8_8(2,30, "Font Update Failed!");	/*�ֿ����*/		
	}
	Put_hello();//��ʾ��ӭ��Ϣ
//	
	delay_ms(1000);
	delay_ms(1000);
		GUI_ClearSCR();	
//	AIR302_Init();
//	
//	Connect_weather_server();

//	Connect_BeiJtime_server();	
//	
//	WeatherShow();
	
//	Menu_Init();
//	EnterMenu();
//	weather_ask = 1;
	
		test_fifo_buffer = kfifo_alloc(512);
	
		if(!test_fifo_buffer)
		GUI_PutString8_8(2,30, "û�д����ɹ�!");	/*�ֿ����*/
				test_readbuf = mymalloc(1024);
			string = mymalloc(100);		
	
  while(1) 
	{	

			write_counter=__kfifo_put(test_fifo_buffer, test_writebuf, 1024 );
//      if(write_counter != 8)
//		  GUI_PutString8_8(2,30, "д�����!");	/*�ֿ����*/	

      read_counter=__kfifo_get(test_fifo_buffer, test_readbuf,1024);	

			sprintf(string,"Nub1:%03d ,Nub2:%03d ,Nub3:%03d .",test_readbuf[0],test_readbuf[1],test_readbuf[2]);
		  GUI_PutHZ(2,50,(u8*)string,12);				
			sprintf(string,"buffer->size:%03d",test_fifo_buffer->size);
		  GUI_PutHZ(2,70,(u8*)string,12);	
					sprintf(string,"buffer->out:%03d,buffer->in:%03d",test_fifo_buffer->out,test_fifo_buffer->in);
		  GUI_PutHZ(2,90,(u8*)string,12);	
//			myfree(test_readbuf);
//			myfree(string);			
//		if(Usart1ReadFlage == 1)//���ڽ��յ�һ������
//		{
//			Usart1ReadFlage = 0;
//			for(i=0;i<RINGBUFF_LEN;i++)
//			{
//				if(Read_RingBuff(&data))
//				Usart1ReadBuff[i]=data;
//				else  break;
//			}
//				parse_now_weather();	
//		}		
		
//		if(GetHttpCnt > 9000)//���3min����һ��http��getЭ��
//		{
//			GetHttpCnt = 0;		
//			weather_Ask();				
//		}

//		if(min_buf != calendar.min)
//		{
//			min_buf = calendar.min;
//			Timebuf = mymalloc(40);
//			sprintf(Timebuf,"%4d-%02d-%02d %02d:%02d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min);//��ȡʪ������ �浽Humidity����			
//			GUI_PutHZ(15,17,(u8*)Timebuf,24); //��ʾʪ��
//			myfree(Timebuf);		
//		}
		
//		delay_ms(1000);
		
//		/* If SEL pushbutton is pressed */
//		if(SELStatus == 1)
//		{
//		  /* External Interrupt Disable */
//		  IntExtOnOffConfig(DISABLE);

//		  /* Execute Sel Function */  //ִ��ѡ�����
//		  SelFunc();

//		  /* External Interrupt Enable */
//		  IntExtOnOffConfig(ENABLE);
//		  /* Reset SELStatus value */
//		   SELStatus = 0;
//		} 		
	} 
}

/*******************************************************************************
* Function Name  : IntExtOnOffConfig
* Description    : Enables or disables EXTI for the menu navigation keys :
*                  EXTI lines 3, 7 and 15 which correpond respectively
*                  to "DOWN", "SEL" and "UP".
* Input          : NewState: New state of the navigation keys. This parameter
*                  can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void IntExtOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 3, 7 and 15 on falling edge */
  if(NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line5 | EXTI_Line15;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 3, 7 and 15 on falling edge */
  else
  {
    /* Clear the the EXTI line 3, 7 and 15 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0 | EXTI_Line5 | EXTI_Line15);

    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line15;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 ;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	  
  }
}

/*ͨ���豸������ʾ����*/
void Put_hello(void)
{
	GUI_PutHZ(4,1,  "***************************************",16);
	GUI_PutHZ(14,30,      "   �����Ǳ�����������   ",24);
	/*������ҳ����*/
    GUI_LoadPic(45,80,(uint8*)TurnONlogo,220,66);	
	GUI_PutHZ(10,170,"            �豸������...          ",16);	
	GUI_PutHZ(10,200,"   ����˹�������ȫ�Ƽ��ɷ����޹�˾  ",16);
	GUI_PutHZ(4,223,"***************************************",16);
}

/*ͨ���豸������ʾ����*/
void WeatherShow(void)
{
	u16 i;
	u8 data;
	u8 Tflag=0;
	char* Timebuf;
	
	BeiJtime_Ask();
	
	GUI_ClearSCR();	
	
	while(Tflag == 0)
	{
		if(Usart1ReadFlage == 1)//���ڽ��յ�һ������
		{
			Usart1ReadFlage = 0;
			for(i=0;i<RINGBUFF_LEN;i++)
			{
				if(Read_RingBuff(&data))
				Usart1ReadBuff[i]=data;
				else  break;
			}
			/*������������*/
			if(parse_now_BeiJtime())
			{
				Tflag=1;
			}
		}		
	}
	GUI_PutHZ(4,1,  "***************************************",16);	
	GUI_PutHZ(4,41,"***************************************",16);	

	GUI_PutHZ(160,63,  "�¶ȣ�    ",24);
	GUI_PutHZ(160,98,  "ʪ�ȣ�    ",24);
	GUI_PutHZ(160,133, "������      ",24);
	GUI_PutHZ(160,168, "����      ",24);
	GUI_PutHZ(24,180,  "����: ",24);		
	GUI_PutHZ(4,223,"***************************************",16);	
	GUI_PutHZ(40,208,"<LastUpdate>",16);
	
	weather_Ask();				
	TIM_Cmd(TIM2,ENABLE);   //ʹ�ܶ�ʱ��2	

}

