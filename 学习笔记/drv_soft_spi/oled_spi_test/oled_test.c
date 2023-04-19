/* 
配置4线SPI模式的所有引脚
4线SPI模式只能写，不能读
SCLK:PC0  D0 
SDIN:PC1  D1
RES :PG15  拉低时初始化（复位），正常时拉高
DC  :PD3  （0:命令  1:数据）
CS  :PD6
*/
#include <rtdevice.h>
#include <drv_pin.h>
#include <drv_soft_spi.h>

//-----------------OLED端口定义----------------  
//#define BSP_S_SPI1_SCK_PIN 10
//#define BSP_S_SPI1_MISO_PIN 56
//#define BSP_S_SPI1_MOSI_PIN 40

#define CS_PIN 		22		//spi片选	 CS
#define RES_PIN   93		//oled复位 RES
#define DC_PIN		BSP_S_SPI1_MOSI_PIN		//spi数据/命令选择脚 DC	
#define SCK_PIN		BSP_S_SPI1_SCK_PIN		//spi时钟线 D0						clock
#define DIN_PIN		BSP_S_SPI1_MISO_PIN		//spi数据线 D1						data

#define OLED_CS_Clr()  rt_pin_write(CS_PIN,PIN_LOW)//CS
#define OLED_CS_Set()  rt_pin_write(CS_PIN,PIN_HIGH)

#define OLED_RST_Clr() rt_pin_write(RES_PIN,PIN_LOW)//RES
#define OLED_RST_Set() rt_pin_write(RES_PIN,PIN_HIGH)

#define OLED_DC_Clr() rt_pin_write(DC_PIN,PIN_LOW)//DC
#define OLED_DC_Set() rt_pin_write(DC_PIN,PIN_HIGH)

#define OLED_SCLK_Clr() rt_pin_write(SCK_PIN,PIN_LOW)//CLK
#define OLED_SCLK_Set() rt_pin_write(SCK_PIN,PIN_HIGH)

#define OLED_SDIN_Clr() rt_pin_write(DIN_PIN,PIN_LOW)//DIN
#define OLED_SDIN_Set() rt_pin_write(DIN_PIN,PIN_HIGH)

//12*6 ASCII字符集点阵
const unsigned char asc2_1206[95][12]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x00,0x3F,0x40,0x00,0x00,0x00,0x00,0x00,0x00},/*"!",1*/
{0x00,0x00,0x30,0x00,0x40,0x00,0x30,0x00,0x40,0x00,0x00,0x00},/*""",2*/
{0x09,0x00,0x0B,0xC0,0x3D,0x00,0x0B,0xC0,0x3D,0x00,0x09,0x00},/*"#",3*/
{0x18,0xC0,0x24,0x40,0x7F,0xE0,0x22,0x40,0x31,0x80,0x00,0x00},/*"$",4*/
{0x18,0x00,0x24,0xC0,0x1B,0x00,0x0D,0x80,0x32,0x40,0x01,0x80},/*"%",5*/
{0x03,0x80,0x1C,0x40,0x27,0x40,0x1C,0x80,0x07,0x40,0x00,0x40},/*"&",6*/
{0x10,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x20,0x40,0x40,0x20},/*"(",8*/
{0x00,0x00,0x40,0x20,0x20,0x40,0x1F,0x80,0x00,0x00,0x00,0x00},/*")",9*/
{0x09,0x00,0x06,0x00,0x1F,0x80,0x06,0x00,0x09,0x00,0x00,0x00},/*"*",10*/
{0x04,0x00,0x04,0x00,0x3F,0x80,0x04,0x00,0x04,0x00,0x00,0x00},/*"+",11*/
{0x00,0x10,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*",",12*/
{0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x00,0x00},/*"-",13*/
{0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*".",14*/
{0x00,0x20,0x01,0xC0,0x06,0x00,0x38,0x00,0x40,0x00,0x00,0x00},/*"/",15*/
{0x1F,0x80,0x20,0x40,0x20,0x40,0x20,0x40,0x1F,0x80,0x00,0x00},/*"0",16*/
{0x00,0x00,0x10,0x40,0x3F,0xC0,0x00,0x40,0x00,0x00,0x00,0x00},/*"1",17*/
{0x18,0xC0,0x21,0x40,0x22,0x40,0x24,0x40,0x18,0x40,0x00,0x00},/*"2",18*/
{0x10,0x80,0x20,0x40,0x24,0x40,0x24,0x40,0x1B,0x80,0x00,0x00},/*"3",19*/
{0x02,0x00,0x0D,0x00,0x11,0x00,0x3F,0xC0,0x01,0x40,0x00,0x00},/*"4",20*/
{0x3C,0x80,0x24,0x40,0x24,0x40,0x24,0x40,0x23,0x80,0x00,0x00},/*"5",21*/
{0x1F,0x80,0x24,0x40,0x24,0x40,0x34,0x40,0x03,0x80,0x00,0x00},/*"6",22*/
{0x30,0x00,0x20,0x00,0x27,0xC0,0x38,0x00,0x20,0x00,0x00,0x00},/*"7",23*/
{0x1B,0x80,0x24,0x40,0x24,0x40,0x24,0x40,0x1B,0x80,0x00,0x00},/*"8",24*/
{0x1C,0x00,0x22,0xC0,0x22,0x40,0x22,0x40,0x1F,0x80,0x00,0x00},/*"9",25*/
{0x00,0x00,0x00,0x00,0x08,0x40,0x00,0x00,0x00,0x00,0x00,0x00},/*":",26*/
{0x00,0x00,0x00,0x00,0x04,0x60,0x00,0x00,0x00,0x00,0x00,0x00},/*";",27*/
{0x00,0x00,0x04,0x00,0x0A,0x00,0x11,0x00,0x20,0x80,0x40,0x40},/*"<",28*/
{0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x00,0x00},/*"=",29*/
{0x00,0x00,0x40,0x40,0x20,0x80,0x11,0x00,0x0A,0x00,0x04,0x00},/*">",30*/
{0x18,0x00,0x20,0x00,0x23,0x40,0x24,0x00,0x18,0x00,0x00,0x00},/*"?",31*/
{0x1F,0x80,0x20,0x40,0x27,0x40,0x29,0x40,0x1F,0x40,0x00,0x00},/*"@",32*/
{0x00,0x40,0x07,0xC0,0x39,0x00,0x0F,0x00,0x01,0xC0,0x00,0x40},/*"A",33*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x24,0x40,0x1B,0x80,0x00,0x00},/*"B",34*/
{0x1F,0x80,0x20,0x40,0x20,0x40,0x20,0x40,0x30,0x80,0x00,0x00},/*"C",35*/
{0x20,0x40,0x3F,0xC0,0x20,0x40,0x20,0x40,0x1F,0x80,0x00,0x00},/*"D",36*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x2E,0x40,0x30,0xC0,0x00,0x00},/*"E",37*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x2E,0x00,0x30,0x00,0x00,0x00},/*"F",38*/
{0x0F,0x00,0x10,0x80,0x20,0x40,0x22,0x40,0x33,0x80,0x02,0x00},/*"G",39*/
{0x20,0x40,0x3F,0xC0,0x04,0x00,0x04,0x00,0x3F,0xC0,0x20,0x40},/*"H",40*/
{0x20,0x40,0x20,0x40,0x3F,0xC0,0x20,0x40,0x20,0x40,0x00,0x00},/*"I",41*/
{0x00,0x60,0x20,0x20,0x20,0x20,0x3F,0xC0,0x20,0x00,0x20,0x00},/*"J",42*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x0B,0x00,0x30,0xC0,0x20,0x40},/*"K",43*/
{0x20,0x40,0x3F,0xC0,0x20,0x40,0x00,0x40,0x00,0x40,0x00,0xC0},/*"L",44*/
{0x3F,0xC0,0x3C,0x00,0x03,0xC0,0x3C,0x00,0x3F,0xC0,0x00,0x00},/*"M",45*/
{0x20,0x40,0x3F,0xC0,0x0C,0x40,0x23,0x00,0x3F,0xC0,0x20,0x00},/*"N",46*/
{0x1F,0x80,0x20,0x40,0x20,0x40,0x20,0x40,0x1F,0x80,0x00,0x00},/*"O",47*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x24,0x00,0x18,0x00,0x00,0x00},/*"P",48*/
{0x1F,0x80,0x21,0x40,0x21,0x40,0x20,0xE0,0x1F,0xA0,0x00,0x00},/*"Q",49*/
{0x20,0x40,0x3F,0xC0,0x24,0x40,0x26,0x00,0x19,0xC0,0x00,0x40},/*"R",50*/
{0x18,0xC0,0x24,0x40,0x24,0x40,0x22,0x40,0x31,0x80,0x00,0x00},/*"S",51*/
{0x30,0x00,0x20,0x40,0x3F,0xC0,0x20,0x40,0x30,0x00,0x00,0x00},/*"T",52*/
{0x20,0x00,0x3F,0x80,0x00,0x40,0x00,0x40,0x3F,0x80,0x20,0x00},/*"U",53*/
{0x20,0x00,0x3E,0x00,0x01,0xC0,0x07,0x00,0x38,0x00,0x20,0x00},/*"V",54*/
{0x38,0x00,0x07,0xC0,0x3C,0x00,0x07,0xC0,0x38,0x00,0x00,0x00},/*"W",55*/
{0x20,0x40,0x39,0xC0,0x06,0x00,0x39,0xC0,0x20,0x40,0x00,0x00},/*"X",56*/
{0x20,0x00,0x38,0x40,0x07,0xC0,0x38,0x40,0x20,0x00,0x00,0x00},/*"Y",57*/
{0x30,0x40,0x21,0xC0,0x26,0x40,0x38,0x40,0x20,0xC0,0x00,0x00},/*"Z",58*/
{0x00,0x00,0x00,0x00,0x7F,0xE0,0x40,0x20,0x40,0x20,0x00,0x00},/*"[",59*/
{0x00,0x00,0x70,0x00,0x0C,0x00,0x03,0x80,0x00,0x40,0x00,0x00},/*"\",60*/
{0x00,0x00,0x40,0x20,0x40,0x20,0x7F,0xE0,0x00,0x00,0x00,0x00},/*"]",61*/
{0x00,0x00,0x20,0x00,0x40,0x00,0x20,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
{0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10},/*"_",63*/
{0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
{0x00,0x00,0x02,0x80,0x05,0x40,0x05,0x40,0x03,0xC0,0x00,0x40},/*"a",65*/
{0x20,0x00,0x3F,0xC0,0x04,0x40,0x04,0x40,0x03,0x80,0x00,0x00},/*"b",66*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x04,0x40,0x06,0x40,0x00,0x00},/*"c",67*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x24,0x40,0x3F,0xC0,0x00,0x40},/*"d",68*/
{0x00,0x00,0x03,0x80,0x05,0x40,0x05,0x40,0x03,0x40,0x00,0x00},/*"e",69*/
{0x00,0x00,0x04,0x40,0x1F,0xC0,0x24,0x40,0x24,0x40,0x20,0x00},/*"f",70*/
{0x00,0x00,0x02,0xE0,0x05,0x50,0x05,0x50,0x06,0x50,0x04,0x20},/*"g",71*/
{0x20,0x40,0x3F,0xC0,0x04,0x40,0x04,0x00,0x03,0xC0,0x00,0x40},/*"h",72*/
{0x00,0x00,0x04,0x40,0x27,0xC0,0x00,0x40,0x00,0x00,0x00,0x00},/*"i",73*/
{0x00,0x10,0x00,0x10,0x04,0x10,0x27,0xE0,0x00,0x00,0x00,0x00},/*"j",74*/
{0x20,0x40,0x3F,0xC0,0x01,0x40,0x07,0x00,0x04,0xC0,0x04,0x40},/*"k",75*/
{0x20,0x40,0x20,0x40,0x3F,0xC0,0x00,0x40,0x00,0x40,0x00,0x00},/*"l",76*/
{0x07,0xC0,0x04,0x00,0x07,0xC0,0x04,0x00,0x03,0xC0,0x00,0x00},/*"m",77*/
{0x04,0x40,0x07,0xC0,0x04,0x40,0x04,0x00,0x03,0xC0,0x00,0x40},/*"n",78*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x04,0x40,0x03,0x80,0x00,0x00},/*"o",79*/
{0x04,0x10,0x07,0xF0,0x04,0x50,0x04,0x40,0x03,0x80,0x00,0x00},/*"p",80*/
{0x00,0x00,0x03,0x80,0x04,0x40,0x04,0x50,0x07,0xF0,0x00,0x10},/*"q",81*/
{0x04,0x40,0x07,0xC0,0x02,0x40,0x04,0x00,0x04,0x00,0x00,0x00},/*"r",82*/
{0x00,0x00,0x06,0x40,0x05,0x40,0x05,0x40,0x04,0xC0,0x00,0x00},/*"s",83*/
{0x00,0x00,0x04,0x00,0x1F,0x80,0x04,0x40,0x00,0x40,0x00,0x00},/*"t",84*/
{0x04,0x00,0x07,0x80,0x00,0x40,0x04,0x40,0x07,0xC0,0x00,0x40},/*"u",85*/
{0x04,0x00,0x07,0x00,0x04,0xC0,0x01,0x80,0x06,0x00,0x04,0x00},/*"v",86*/
{0x06,0x00,0x01,0xC0,0x07,0x00,0x01,0xC0,0x06,0x00,0x00,0x00},/*"w",87*/
{0x04,0x40,0x06,0xC0,0x01,0x00,0x06,0xC0,0x04,0x40,0x00,0x00},/*"x",88*/
{0x04,0x10,0x07,0x10,0x04,0xE0,0x01,0x80,0x06,0x00,0x04,0x00},/*"y",89*/
{0x00,0x00,0x04,0x40,0x05,0xC0,0x06,0x40,0x04,0x40,0x00,0x00},/*"z",90*/
{0x00,0x00,0x00,0x00,0x04,0x00,0x7B,0xE0,0x40,0x20,0x00,0x00},/*"{",91*/
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF0,0x00,0x00,0x00,0x00},/*"|",92*/
{0x00,0x00,0x40,0x20,0x7B,0xE0,0x04,0x00,0x00,0x00,0x00,0x00},/*"}",93*/
{0x40,0x00,0x80,0x00,0x40,0x00,0x20,0x00,0x20,0x00,0x40,0x00},/*"~",94*/
};  

/* OLED的显存 */
uint8_t OLED_GRAM[128][64];
/*
OLED_GRAM[128][8]中是8位二进制为一个单位
其逻辑排序如示：（高位先行）
7 7
6 6
5 5
4 4  ...
3 3
2 2
1 1
0 0
*/


void OLED_SCLK(uint8_t x)
{
	if( x==1 )
		OLED_SCLK_Set();
	else
		OLED_SCLK_Clr();
}

void OLED_SDIN( uint8_t x )
{
	if( x==0 )
		OLED_SDIN_Set();
	else
		OLED_SDIN_Clr();
}

void OLED_RES( uint8_t x )
{
	if( x==1 )
		OLED_RST_Set();
	else
		OLED_RST_Clr();
}

void OLED_DC( uint8_t x )
{
	if( x==1 )
		OLED_DC_Set();
	else
		OLED_DC_Clr();
}

void OLED_CS( uint8_t x )
{
	if( x==1 )
		OLED_CS_Set();
	else
		OLED_CS_Clr();
}


/* 写一个命令 高位先行 */
void OLED_WR_Cmd( uint8_t cmd )
{
	uint8_t i,k;
	/* 拉低片选CS，写命令拉低DC */
	OLED_CS(0);
	OLED_DC(0);
	
	for( i=0;i<8;i++ )
	{
		/* 时钟线，上升沿有效 */
		OLED_SCLK(0);
		k=cmd&(0x80);
		OLED_SDIN(k);
		OLED_SCLK(1);
		cmd<<=1;
	}
	OLED_CS(1);
	OLED_DC(1);	
}


/* 写一个数据 高位先行 */
void OLED_WR_Data( uint8_t data )
{
	uint8_t i,k;
	/* 拉低片选CS，写数据拉高DC */
	OLED_CS(0);
	OLED_DC(1);
	
	for( i=0;i<8;i++ )
	{
		/* 时钟线，上升沿有效 */
		OLED_SCLK(0);
		k=data&(0x80);
		OLED_SDIN(k);
		OLED_SCLK(1);
		data<<=1;
	}
	OLED_CS(1);
	OLED_DC(1);
}

//OLED_GRAM[128][8]
/* 画点函数，以屏幕像素点为单位，以左上角为原点 x:0~127 y:0~63 
（x,y）坐标换算:OLED_GRAM[x][7-y/8]|=1<<(7-y%8)；

mode取1正显，取0反显
*/
/* 图像刷新函数 */
void OLED_Refresh_GRAM(void)
{
	uint8_t i,j;
	for( i=0;i<8;i++ )
	{
		/* 设置显示的起始地址 */
		OLED_WR_Cmd(0xB0+i);//设置页地址（行）
		OLED_WR_Cmd(0x00);//设置列地址的低四位
		OLED_WR_Cmd(0x10);//设置列地址的高四位
		for( j=0;j<128;j++ )
		{
			OLED_WR_Data(OLED_GRAM[j][i]);//将GRAM中图像信息写入屏幕
		}
	}
}


/* 清屏函数 */
void OLED_Clear(void)
{
	uint8_t i,j;
	for( i=0;i<8;i++ )
		for( j=0;j<128;j++ )
			OLED_GRAM[j][i]=0x00;
	OLED_Refresh_GRAM();
}

/* OLED初始化函数 */
void OLED_Init( void )
{
	rt_err_t result = -RT_ERROR;

	result = rt_hw_softspi_device_attach("sspi1","ssd1306",CS_PIN);
	if(result != RT_ERROR)
	{
		rt_kprintf("sotf_spi device create successful!\n");
	}
	
	
	OLED_CS(1);
	OLED_DC(1);
	
	/* 复位 */
	OLED_RES(0);
	lpc_udelay(1000);
	OLED_RES(1);
	
	/* 开始写入初始化命令 */
	OLED_WR_Cmd(0xAE);//关闭显示
	OLED_WR_Cmd(0xD5);//设置时钟分频因子
	OLED_WR_Cmd(80);
	
	OLED_WR_Cmd(0xA8);//设置驱动路数
	OLED_WR_Cmd(0x3F);//路数默认0x3F（1/64）
	
	OLED_WR_Cmd(0xD3);//设置显示偏移
	OLED_WR_Cmd(0x00);//偏移默认为0
	
	OLED_WR_Cmd(0x40);//设置显示开始行[5:0]
	
	OLED_WR_Cmd(0x8D);//电荷泵设置
	OLED_WR_Cmd(0x14);//bit2，开启/关闭
	
	OLED_WR_Cmd(0x20);//设置内存地址模式
	OLED_WR_Cmd(0x02);//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	
	OLED_WR_Cmd(0xA1);//段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Cmd(0xC0);//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Cmd(0xDA);//设置COM硬件引脚配置
	OLED_WR_Cmd(0x12);//[5:4]配置
	
	OLED_WR_Cmd(0x81);//对比度设置
	OLED_WR_Cmd(0xEF);//默认0x7F（范围1~255，越大越亮）
	
	OLED_WR_Cmd(0xD9);//设置预充电周期
	OLED_WR_Cmd(0xF1);//[3:0],PHASE 1;[7:4],PHASE 2;
	
	OLED_WR_Cmd(0xDB);//设置VCOMH 电压倍率
	OLED_WR_Cmd(0x30);//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	
	OLED_WR_Cmd(0xA4);//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Cmd(0xA6);//设置显示方式;bit0:1,反相显示;0,正常显示
	
	OLED_WR_Cmd(0xAF);//开启显示
	
	/* 清屏函数 */
	OLED_Clear();
}
INIT_APP_EXPORT(OLED_Init);

void OLED_DrawPoint( uint8_t x,uint8_t y,uint8_t mode )
{
	/* 主要是求出y坐标，根据y坐标来位运算OLED_GRAM中相应的位 */
	uint8_t i,j,temp;
	/* 判断坐标是否超出范围 */
	if(x>127||y>63)return;
	
	i=7-y/8;//算出第几页
	j=y%8;
	temp=0x01<<(7-j);//由位运算精确找出坐标像素点
	if(mode==0)
		OLED_GRAM[x][i]&=~temp;
	else
		OLED_GRAM[x][i]|=temp;
}

/* 在（x,y）坐标正显/反显指定大小字符chr
mode：0是反显，1是正常显示
size：12/16/24
ASCII字符集: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
*/
void OLED_Show_Char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
	/* temp是当前对应的一个字节的点集，y0是初始坐标 */
	uint8_t temp,t,t1;
	uint8_t y0=y;
	/* csize是单个字符所占字节的多少 */
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);
	/* 求出偏移的地址差，即得到该字符在字库中的序号（从零开始） */
	/* chr会锁定字符在字库中的的序号 */
	chr=chr-' ';
	
	/* 相应字符点集有多少个字节 */
	for( t=0;t<csize;t++ )
	{
		/* 根据字符的大小选择相应字库，根据chr得到具体的字符地址 */
		switch( size )
		{
			case 12:temp=asc2_1206[chr][t];break;//12x6(行x列)
			default:return;//没有相应字库
		}
		
		/* 画出每一列的点 */
		for( t1=0;t1<8;t1++ )
		{
			if( temp&0x80 )
				OLED_DrawPoint( x,y,mode );
			else
				OLED_DrawPoint( x,y,!mode );
			
			temp<<=1;
			y++;
			
			/* 根据给出每一列的像素点的多少，判断列是否满：
			满了就换列，不满继续在此列画点 （与字库画点原理相关）			
			*/
			if( ( y-y0 )==size )
			{
				y=y0;
				x++;
				/* 一列满，跳出循环，直接转到下一个字节点集的打印 */
				break;
			}
		}
	}
}


/* （x,y）是显示的坐标，*p是字符串的首地址，size是字符点集大小 */
void OLED_Show_String( uint8_t x, uint8_t y, const uint8_t *p, uint8_t size, uint8_t mode )
{
		/* 判断是否合法字符，同时也限定了范围 */
	while( (*p<='~')&&(*p>=' ') )
	{
		/* 如果初始行放不下，移动到下一行 */
		if( x>(128-(size/2)) )
		{
			x=0;
			y=y+size;
		}
		if( y>(64-size) )
		{
			x=y=0;
			OLED_Clear();
		}
		
		OLED_Show_Char( x,y,*p,size,mode );
		/* 移动到下一个字符位置，size/2是因为做点集时就是：行X列，而且 行=2X列，所以size就是行数 */
		x=x+size/2;
		p++;
	}
}

