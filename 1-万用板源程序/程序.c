//����ͷ����
#include <reg52.h>

//�궨��
#define uint unsigned int 
#define uchar unsigned char
#define Data_ADC0809 P1		   //����P1��ΪData_ADC0809
 
//�ܽ�����
sbit jdq=P3^0;			   //�̵���
sbit Feng = P3^1;		   //������
//ADC0809
sbit ST=P3^3;
sbit EOC=P3^4;
sbit OE=P3^2;
//����
sbit Key1=P3^5;
sbit Key2=P3^6;
sbit Key3=P3^7;
bit shan=0;				   //��˸��־λ

//��ʾ����		0-9+F
uchar Data_[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x71,0x3f};	 //������ʾ����
sbit Wei1 = P2^7;
sbit Wei2 = P2^6;
sbit Wei3 = P2^5;
sbit Wei4 = P2^4;				 //�ĸ�����ܵ�λѡ�˿�
//��������
extern uchar ADC0809();
void Display(uchar X,uchar Data);
void delay(uint t);

//�ƾ���������
uchar temp=0;
//����������
uchar FF=10;
//��ʾģʽ
uchar Mode=0;
uchar p;

void main()				  //������
{
	TH0=0x3c;
	TL0=0xb0;//����ֵ50ms
	TMOD=0x01;//���ù�����ʽ
	EA=1;//���ж��ܿ���
	ET0=1;//��������
	TR0=1;	//�򿪶�ʱ����ʱ����
	while(1)	   //����ѭ����������Ϊ1�����Գ������ѭ���Ͳ�������ˣ����Ծͽ���ѭ��
	{
		//����ģʽ
		if(Mode==0)
		{
			//��ȡADֵ
			temp=ADC0809();
			for(p=0;p<30;p++)	   //������ʮ����ʾ����
			Display(0,temp);
			//�ж��Ƿ񱨾�
			if(temp>FF)				   //���ڱ���ֵ
			{
				jdq=0;					//�̵�������
			}
			if((shan==1)&&(temp>FF))   //��˸����Ϊ1ʱ���Ҵ��ڱ���ֵ
			{
				Feng=0;				   //��������
			}
			if(shan==0)				   //��˸����Ϊ0ʱ
			{
				Feng=1;				   //�رշ�����
			}								   //����˸����shan�ܶ�ʱ�����ƣ�500msȡ��һ�Σ��ﵽ1����һ�µ�Ч����
			if(temp<=FF)			   //С�ڱ���ֵʱ
			{
				Feng=1;				   //�رշ������ͼ̵���
				jdq=1;
			}
		}
		//����ģʽ
		else
		{
			Display(1,FF);		//��ʾ����ֵ����
		}
		//���ܼ�
		if(Key3==0)				   //���ü����£�����һ�˽ӵأ�һ�˽ӵ�Ƭ��IO�ڣ�����������ʱ����Ƭ����IO�ھͻ��⵽�͵�ƽ��
		{
			Feng=0;					//��������
			jdq=1;					 //�Ͽ��̵���
			delay(100);				//��ʱȥ��
			while(Key3==0)			//�жϰ����Ƿ���
			{
				if(Mode==0)			//����ģʽ
					Display(0,temp);//��ʾ��������	
				else				//���ñ���ֵ
					Display(1,FF);	//��ʾ����ֵ����								
			}
			if(Mode==0)				//���������ģʽ���������ü������л�������ģʽ
				Mode=1;
			else					//���������ģʽ���������ü������л�������ģʽ
				Mode=0;
			Feng=1;					//�رշ�����
		}
		//����
		if(Key2==0&&Mode==1)		//�Ӽ�����
		{
			Feng=0;					//��������
			delay(100);
			while(Key2==0)
			{
					Display(1,FF);	//��ʾ���ý���								
			}
			FF++;					//����ֵ��
			if(FF>=251)				//���ӵ�250
				FF=250;	
			Feng=1;					//�رշ�����
		}
		//����
		if(Key1==0&&Mode==1)		//���������£�ע�Ͳο��Ӱ���ע��
		{
			Feng=0;
			delay(100);
			while(Key1==0)
			{
					Display(1,FF);									
			}
			FF--;
			if(FF==0xff)			//����ֵ����С��0
				FF=0;				//��Сֵ����0
			Feng=1;
		}
	}
}
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
	while(EOC==0)
	//��ȡת����ADֵ
	OE=1;
	temp_=Data_ADC0809;
	OE=0;
	return temp_;
}
//��ʱ
void delay(uint t)
{
	uint i,j;
	for(i=0;i<t;i++)
		for(j=0;j<10;j++);
}

//��ʾ       X��ʾ״̬     Data��ʾ����
void Display(uchar X,uchar Data)
{
	Wei1=1;
	Wei2=1;
	Wei3=1;
	Wei4=1;
	P0=0xff;			//�ر���ʾ

	//����ģʽ
	if(X==0)
	{
		P0=~Data_[11];	 //��ʾ0��Ҳ��������ģʽʱ����һ��λ����ʾ0
	}
	//������
	else				 
	{
		P0=~Data_[10];	 //����ģʽ����һλ��ʾ��ĸF
	}
	Wei1=0;				 //ѡ�е�һλ������λ�ر�
	Wei2=1;
	Wei3=1;
	Wei4=1;
	delay(10);			 //��ʱ
	Wei1=1;				 //�ر�����λ
	Wei2=1;
	Wei3=1;
	Wei4=1;

	P0=~Data_[Data/100]; //��ʾ��ֵ�İ�λ������������123������100������1��������ʾ�ľ�������1
	Wei1=1;
	Wei2=0;				 //ѡ�еڶ�λ
	Wei3=1;
	Wei4=1;
	delay(10);			 //��ʱ
	Wei1=1;				 //�ر�����λ
	Wei2=1;
	Wei3=1;
	Wei4=1;


	P0=~Data_[Data%100/10];//��ʾ��ֵ��ʮλ����ֵ��123��ȡ������þ��ǳ���100�����������23,23�ٳ���10�õ�������2��������ʾ�ľ���2
	Wei1=1;
	Wei2=1;
	Wei3=0;
	Wei4=1;
	delay(10);
	Wei1=1;
	Wei2=1;
	Wei3=1;
	Wei4=1;

	P0=~Data_[Data%10];	  //��ʾ��ֵ�ĸ�λ����ֵ��123��ȡ��10������ֵ����10����������3��������ʾ�ľ���3
	Wei1=1;
	Wei2=1;
	Wei3=1;
	Wei4=0;
	delay(10);
	Wei1=1;
	Wei2=1;
	Wei3=1;
	Wei4=1;

}
void time0() interrupt 1//��ʱ������
{
	uchar m;
	TH0=0x3c;
	TL0=0xb0;//���¸���ֵ
	m++;	  //��ʱ������һ����50ms��m��һ��
	if(m==10)  //��10�Σ�Ҳ����500ms
	{
		shan=~shan;	  //����shan����ȡ��������0��1,1��0
		m=0;		  //m����
	}
}
