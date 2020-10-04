#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK  STM32������
//ϵͳ�жϷ������û�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/10
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************  
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
/*��λоƬ*/
void Reset_MCU(void)
{
  __disable_fault_irq();   
	NVIC_SystemReset();
}
u32 char_num(u8* str,u8 len)//�ַ���ת����
{
	u16 i = 0;
	u32 num = 0;
	u32 num_value=1;
	for(;len>0;len--)
	{
		for(i=0;i<10;i++)
		{
			if(str[len-1] == ('0'+i))
			{
				num+=(i*num_value);
				break;
			}
		}
		num_value=num_value * 10;
	}
	return num;	
}
