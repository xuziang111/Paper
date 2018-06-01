//程序头函数
#include <reg52.h>

//宏定义
#define uint unsigned int 
#define uchar unsigned char
#define Data_ADC0809 P1		   //定义P1口为Data_ADC0809
 
//管脚声明
sbit jdq=P3^0;			   //继电器
sbit Feng = P3^1;		   //蜂鸣器
//ADC0809
sbit ST=P3^3;
sbit EOC=P3^4;
sbit OE=P3^2;
//按键
sbit Key1=P3^5;
sbit Key2=P3^6;
sbit Key3=P3^7;
bit shan=0;				   //闪烁标志位

//显示数组		0-9+F
uchar Data_[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x71,0x3f};	 //共阴显示数组
sbit Wei1 = P2^7;
sbit Wei2 = P2^6;
sbit Wei3 = P2^5;
sbit Wei4 = P2^4;				 //四个数码管的位选端口
//函数声明
extern uchar ADC0809();
void Display(uchar X,uchar Data);
void delay(uint t);

//酒精含量变量
uchar temp=0;
//蜂鸣器变量
uchar FF=10;
//显示模式
uchar Mode=0;
uchar p;

void main()				  //主函数
{
	TH0=0x3c;
	TL0=0xb0;//赋初值50ms
	TMOD=0x01;//设置工作方式
	EA=1;//打开中断总开关
	ET0=1;//打开允许开关
	TR0=1;	//打开定时器定时开关
	while(1)	   //进入循环，括号内为1，所以程序进入循环就不会出来了，所以就叫死循环
	{
		//正常模式
		if(Mode==0)
		{
			//读取AD值
			temp=ADC0809();
			for(p=0;p<30;p++)	   //调用三十次显示函数
			Display(0,temp);
			//判断是否报警
			if(temp>FF)				   //大于报警值
			{
				jdq=0;					//继电器吸和
			}
			if((shan==1)&&(temp>FF))   //闪烁变量为1时并且大于报警值
			{
				Feng=0;				   //蜂鸣器响
			}
			if(shan==0)				   //闪烁变量为0时
			{
				Feng=1;				   //关闭蜂鸣器
			}								   //（闪烁变量shan受定时器控制，500ms取反一次，达到1秒响一下的效果）
			if(temp<=FF)			   //小于报警值时
			{
				Feng=1;				   //关闭蜂鸣器和继电器
				jdq=1;
			}
		}
		//调整模式
		else
		{
			Display(1,FF);		//显示报警值界面
		}
		//功能键
		if(Key3==0)				   //设置键按下（按键一端接地，一端接单片机IO口，当按键按下时，单片机的IO口就会检测到低电平）
		{
			Feng=0;					//蜂鸣器响
			jdq=1;					 //断开继电器
			delay(100);				//延时去抖
			while(Key3==0)			//判断按键是否按下
			{
				if(Mode==0)			//正常模式
					Display(0,temp);//显示正常界面	
				else				//设置报警值
					Display(1,FF);	//显示报警值界面								
			}
			if(Mode==0)				//如果是正常模式，按下设置键，就切换到设置模式
				Mode=1;
			else					//如果是设置模式，按下设置键，就切换到正常模式
				Mode=0;
			Feng=1;					//关闭蜂鸣器
		}
		//增加
		if(Key2==0&&Mode==1)		//加键按下
		{
			Feng=0;					//蜂鸣器响
			delay(100);
			while(Key2==0)
			{
					Display(1,FF);	//显示设置界面								
			}
			FF++;					//报警值加
			if(FF>=251)				//最大加到250
				FF=250;	
			Feng=1;					//关闭蜂鸣器
		}
		//减少
		if(Key1==0&&Mode==1)		//减按键按下，注释参考加按键注释
		{
			Feng=0;
			delay(100);
			while(Key1==0)
			{
					Display(1,FF);									
			}
			FF--;
			if(FF==0xff)			//报警值减到小于0
				FF=0;				//最小值就是0
			Feng=1;
		}
	}
}
//ADC0809读取信息
uchar ADC0809()
{
	uchar temp_=0x00;
	//初始化高阻太
	OE=0;
	//转化初始化
	ST=0;
	//开始转换
	ST=1;
	ST=0;
	//外部中断等待AD转换结束
	while(EOC==0)
	//读取转换的AD值
	OE=1;
	temp_=Data_ADC0809;
	OE=0;
	return temp_;
}
//延时
void delay(uint t)
{
	uint i,j;
	for(i=0;i<t;i++)
		for(j=0;j<10;j++);
}

//显示       X表示状态     Data表示数据
void Display(uchar X,uchar Data)
{
	Wei1=1;
	Wei2=1;
	Wei3=1;
	Wei4=1;
	P0=0xff;			//关闭显示

	//正常模式
	if(X==0)
	{
		P0=~Data_[11];	 //显示0，也就是正常模式时，第一个位置显示0
	}
	//非正常
	else				 
	{
		P0=~Data_[10];	 //设置模式，第一位显示字母F
	}
	Wei1=0;				 //选中第一位，其他位关闭
	Wei2=1;
	Wei3=1;
	Wei4=1;
	delay(10);			 //延时
	Wei1=1;				 //关闭所有位
	Wei2=1;
	Wei3=1;
	Wei4=1;

	P0=~Data_[Data/100]; //显示数值的百位，例如数据是123，除以100的商是1，这里显示的就是数字1
	Wei1=1;
	Wei2=0;				 //选中第二位
	Wei3=1;
	Wei4=1;
	delay(10);			 //延时
	Wei1=1;				 //关闭所有位
	Wei2=1;
	Wei3=1;
	Wei4=1;


	P0=~Data_[Data%100/10];//显示数值的十位，数值是123，取余的作用就是除以100后的余数，是23,23再除以10得到的商是2，这里显示的就是2
	Wei1=1;
	Wei2=1;
	Wei3=0;
	Wei4=1;
	delay(10);
	Wei1=1;
	Wei2=1;
	Wei3=1;
	Wei4=1;

	P0=~Data_[Data%10];	  //显示数值的个位，数值是123，取余10就是数值除以10的余数就是3，这里显示的就是3
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
void time0() interrupt 1//定时器函数
{
	uchar m;
	TH0=0x3c;
	TL0=0xb0;//重新赋初值
	m++;	  //定时器进入一次是50ms，m加一次
	if(m==10)  //加10次，也就是500ms
	{
		shan=~shan;	  //控制shan变量取反，就是0变1,1变0
		m=0;		  //m清零
	}
}
