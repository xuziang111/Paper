#include <reg52.h>

sbit ST=P0^0;
sbit EOC=P0^1;
sbit OE=P0^2;
sbit AD_CLK=P0^3;

sbit Feng=P2^3;
uchar k=0x00;
//ADC0809��ȡ��Ϣ
uchar ADC0809()
{
	uchar temp_=0x00;
	//��ʼ������̫
	OE=0;
	//ת����ʼ��
	ST=0;
	//��ʼת��
	ST=1;
	ST=0;
	//�ⲿ�жϵȴ�ADת������
	Feng=0;
	while(EOC==0)
	Feng=1;
	OE=1;
	//temp_=AD_data;
	OE=0;
	return temp_;
}
void main()
{
	while(1)
	{
		ADC0809();
	}
}