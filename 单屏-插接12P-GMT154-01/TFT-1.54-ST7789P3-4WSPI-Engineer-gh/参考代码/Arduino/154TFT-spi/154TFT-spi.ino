/********************金逸晨**************************
*****************1.54寸  10PIN SPI TFT FOR Arduino*************
*********ST7789V2
从左往右，从上往下    240*240  16bit 5 6 5 RGB 模式  高位模式
********************************************************/

#define SPI_SCK_0  digitalWrite(2,LOW)   //字库sck与屏复用            
#define SPI_SCK_1  digitalWrite(2,HIGH)
#define SPI_SDA_0  digitalWrite(3,LOW)    //字库SDA与屏复用         
#define SPI_SDA_1  digitalWrite(3,HIGH) 
#define SPI_RST_0  digitalWrite(4,LOW)               
#define SPI_RST_1  digitalWrite(4,HIGH)
#define SPI_DC_0  digitalWrite(5,LOW)               
#define SPI_DC_1  digitalWrite(5,HIGH)
#define SPI_CS_0  digitalWrite(6,LOW)               
#define SPI_CS_1  digitalWrite(6,HIGH)
#define BL_0  digitalWrite(7,LOW)               
#define BL_1  digitalWrite(7,HIGH)


//定义显示区域大小，偏移
#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 240
#define TFT_COLUMN_OFFSET 0
#define TFT_LINE_OFFSET 0
#define PIC_NUM 28800      //图片数据大小

//定义常用颜色
#define     RED          0XF800    //红色
#define     GREEN        0X07E0   //绿色
#define     BLUE         0X001F   //蓝色
#define     WHITE        0XFFFF   //白色
#define PIC_LEN 120
#define PIC_HIG 120
#define PIC_NUMBER 28800

const unsigned char  chines_word[   ][32]=   //汉字码
{
0x00,0x00,0xE4,0x3F,0x28,0x20,0x28,0x25,0x81,0x08,0x42,0x10,0x02,0x02,0x08,0x02,
0xE8,0x3F,0x04,0x02,0x07,0x07,0x84,0x0A,0x44,0x12,0x34,0x62,0x04,0x02,0x00,0x02,/*"深",0*/

0x88,0x20,0x88,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0xBF,0x24,0x88,0x24,0x88,0x24,
0x88,0x24,0x88,0x24,0x88,0x24,0xB8,0x24,0x87,0x24,0x42,0x24,0x40,0x20,0x20,0x20,/*"圳",1*/

0x80,0x00,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x08,0x08,0xF4,0x17,0x83,0x60,
0x80,0x00,0xFC,0x1F,0x80,0x00,0x88,0x08,0x90,0x08,0x90,0x04,0xFF,0x7F,0x00,0x00,/*"金",2*/

0x80,0x00,0x82,0x00,0x84,0x0F,0x44,0x08,0x20,0x04,0xF0,0x3F,0x27,0x22,0x24,0x22,
0xE4,0x3F,0x04,0x05,0x84,0x0C,0x84,0x54,0x44,0x44,0x24,0x78,0x0A,0x00,0xF1,0x7F,/*"逸",3*/

0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x00,0x00,0xFC,0x3F,0x04,0x00,
0xF4,0x1F,0x04,0x00,0xFC,0x7F,0x94,0x10,0x14,0x09,0x12,0x06,0x52,0x18,0x31,0x60,/*"晨",4*/

0x80,0x00,0x80,0x00,0x80,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,
0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x50,0x80,0x40,0x80,0x40,0x00,0x7F,/*"电",5*/

0x00,0x00,0xFE,0x1F,0x00,0x08,0x00,0x04,0x00,0x02,0x80,0x01,0x80,0x00,0xFF,0x7F,
0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xA0,0x00,0x40,0x00,/*"子",6*/
};

void  IO_init(void )
{
pinMode(2,OUTPUT);//设置数字脚为输出
pinMode(3,OUTPUT);//设置数字脚为输出
pinMode(4,OUTPUT);//设置数字脚为输入
pinMode(5,OUTPUT);//设置数字脚为输出
pinMode(6,OUTPUT);//设置数字脚为输出
pinMode(7,OUTPUT);//设置数字脚为输出
pinMode(8,OUTPUT);//设置数字脚为输出
pinMode(9,OUTPUT);//设置数字脚为输出
//pinMode(10,OUTPUT);//设置数字脚为输出
//pinMode(11,OUTPUT);//设置数字脚为输出
//pinMode(12,OUTPUT);//设置数字脚为输出
//pinMode(A0,INPUT);//设置数字脚为输入
SPI_SCK_1;
SPI_CS_1;
SPI_SDA_1;
}
void delay_us(unsigned int _us_time)
{       
  unsigned char x=0;
  for(;_us_time>0;_us_time--)
  {
    x++;
  }
}

/*************SPI配置函数*******************
SCL空闲时低电平，第一个上升沿采样
模拟SPI
******************************************/

/**************************SPI模块发送函数************************************************

 *************************************************************************/
void SPI_SendByte(unsigned char byte)
{
  
  unsigned char counter;
   
  for(counter=0;counter<8;counter++)
  { 
    SPI_SCK_0;
    if((byte&0x80)==0)
    {
      SPI_SDA_0;
    }
    else SPI_SDA_1;
    byte=byte<<1;
    SPI_SCK_1;  
  SPI_SCK_0;
   SPI_SDA_0; 
  } 
}

void TFT_SEND_CMD(unsigned char o_command)
  {
    SPI_DC_0;
    SPI_CS_0;
    SPI_SendByte(o_command);
    SPI_CS_1;
   
    //SPI_DC_1;
  }
void TFT_SEND_DATA(unsigned char o_data)
  { 
    SPI_DC_1;
    SPI_CS_0;
    SPI_SendByte(o_data);
    SPI_CS_1;
    
   }
  void TFT_SET_ADD(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{
  unsigned int x = x_start + TFT_COLUMN_OFFSET,y=x_end+ TFT_COLUMN_OFFSET;
    TFT_SEND_CMD(0x2a);     //Column address set
    TFT_SEND_DATA(x>>8);    //start column
    TFT_SEND_DATA(x); 
    TFT_SEND_DATA(y>>8);    //end column
    TFT_SEND_DATA(y);
  x = y_start + TFT_LINE_OFFSET;
  y=y_end+ TFT_LINE_OFFSET;
    TFT_SEND_CMD(0x2b);     //Row address set
    TFT_SEND_DATA(x>>8);    //start row
    TFT_SEND_DATA(x); 
    TFT_SEND_DATA(y>>8);    //end row
    TFT_SEND_DATA(y);
    TFT_SEND_CMD(0x2C);     //Memory write
    
}
void TFT_clear(void)
  {
    unsigned int ROW,column;
    TFT_SET_ADD(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
    for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
      { 
    
          for(column=0;column<TFT_COLUMN_NUMBER;column++)  //column loop
            {
              
        TFT_SEND_DATA(0xFF);
        TFT_SEND_DATA(0xFF);
            }
      }
  }

void TFT_full(unsigned int color)
  {
    unsigned int ROW,column;
    TFT_SET_ADD(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
    for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
    { 
    
        for(column=0;column<TFT_COLUMN_NUMBER ;column++) //column loop
        {

     TFT_SEND_DATA(color>>8);
     TFT_SEND_DATA(color);
        }
    }
  }
  
void Picture_display(const unsigned char *ptr_pic)
{
    unsigned long  number;
  TFT_SET_ADD(20,20,139,139);
    
  for(number=0;number<PIC_NUM;number++) 
          {
//            data=*ptr_pic++;
//            data=~data;
            TFT_SEND_DATA(*ptr_pic++);
  
      
          }
  }
void display_char16_16(unsigned int x,unsigned int y,unsigned long color,unsigned char word_serial_number)
{
   unsigned int column;
  unsigned char tm=0,temp=0,xxx=0;

   TFT_SEND_CMD(0x2a);    //Column address set
  TFT_SEND_DATA(x>>8);    //start column
  TFT_SEND_DATA(x);
  x=x+15;
  TFT_SEND_DATA(x>>8);    //end column
  TFT_SEND_DATA(x);

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA(y>>8);    //start row
  TFT_SEND_DATA(y); 
  y=y+15;
  TFT_SEND_DATA(y>>8);    //end row
  TFT_SEND_DATA(y);
    TFT_SEND_CMD(0x2C);     //Memory write
    
    
  for(column=0;column<32;column++)  //column loop
          {
        temp=chines_word[  word_serial_number ][xxx];
        for(tm=0;tm<8;tm++)
        {
        if(temp&0x01)
          {
          TFT_SEND_DATA(color>>8);
          TFT_SEND_DATA(color);
          }
        else 
          {
          TFT_SEND_DATA(0XFF);
          TFT_SEND_DATA(0XFF);
          }
          temp>>=1;
        }
        xxx++;
          
      }
}
void TFT_init(void)        ////ST7789V2
  {
  SPI_SCK_0;
  SPI_RST_0;
  delay(1000);
  SPI_RST_1;
  delay(1000);
    TFT_SEND_CMD(0x11);       //Sleep Out
  delay(120);               //DELAY120ms 
  //--------------------------------ST7789S Frame rate setting----------------------------------// 
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA(0x00);    //start column
  TFT_SEND_DATA(0x00); 
  TFT_SEND_DATA(0x00);    //end column
  TFT_SEND_DATA(0xef);

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA(0x00);    //start row
  TFT_SEND_DATA(0x28); 
  TFT_SEND_DATA(0x01);    //end row
  TFT_SEND_DATA(0x17);

  TFT_SEND_CMD(0xb2);     //Porch control
  TFT_SEND_DATA(0x0c); 
  TFT_SEND_DATA(0x0c); 
  TFT_SEND_DATA(0x00); 
  TFT_SEND_DATA(0x33); 
  TFT_SEND_DATA(0x33); 

  TFT_SEND_CMD(0x20);     //Display Inversion Off

  TFT_SEND_CMD(0xb7);     //Gate control
  TFT_SEND_DATA(0x56);      //35
//---------------------------------ST7789S Power setting--------------------------------------// 
  TFT_SEND_CMD(0xbb); //VCOMS Setting
  TFT_SEND_DATA(0x18);  //1f

  TFT_SEND_CMD(0xc0);     //LCM Control
  TFT_SEND_DATA(0x2c); 

  TFT_SEND_CMD(0xc2);     //VDV and VRH Command Enable
  TFT_SEND_DATA(0x01); 

  TFT_SEND_CMD(0xc3); //VRH Set
  TFT_SEND_DATA(0x1f); //12

  TFT_SEND_CMD(0xc4);       //VDV Setting
  TFT_SEND_DATA(0x20); 

  TFT_SEND_CMD(0xc6);       //FR Control 2
  TFT_SEND_DATA(0x0f); 
//TFT_SEND_CMD(0xca); 
//TFT_SEND_DATA(0x0f); 
//TFT_SEND_CMD(0xc8); 
//TFT_SEND_DATA(0x08); 
//TFT_SEND_CMD(0x55); 
//TFT_SEND_DATA(0x90); 
  TFT_SEND_CMD(0xd0);  //Power Control 1
  TFT_SEND_DATA(0xa6);   //a4
  TFT_SEND_DATA(0xa1); 
//--------------------------------ST7789S gamma setting---------------------------------------// 

  TFT_SEND_CMD(0xe0); 
  TFT_SEND_DATA(0xd0); 
  TFT_SEND_DATA(0x0d); 
  TFT_SEND_DATA(0x14); 
  TFT_SEND_DATA(0x0b); 
  TFT_SEND_DATA(0x0b); 
  TFT_SEND_DATA(0x07); 
  TFT_SEND_DATA(0x3a);  
  TFT_SEND_DATA(0x44); 
  TFT_SEND_DATA(0x50); 
  TFT_SEND_DATA(0x08); 
  TFT_SEND_DATA(0x13); 
  TFT_SEND_DATA(0x13); 
  TFT_SEND_DATA(0x2d); 
  TFT_SEND_DATA(0x32); 

  TFT_SEND_CMD(0xe1);         //Negative Voltage Gamma Contro
  TFT_SEND_DATA(0xd0); 
  TFT_SEND_DATA(0x0d); 
  TFT_SEND_DATA(0x14); 
  TFT_SEND_DATA(0x0b); 
  TFT_SEND_DATA(0x0b); 
  TFT_SEND_DATA(0x07); 
  TFT_SEND_DATA(0x3a); 
  TFT_SEND_DATA(0x44); 
  TFT_SEND_DATA(0x50); 
  TFT_SEND_DATA(0x08); 
  TFT_SEND_DATA(0x13); 
  TFT_SEND_DATA(0x13); 
  TFT_SEND_DATA(0x2d); 
  TFT_SEND_DATA(0x32);
  
  TFT_SEND_CMD(0x36);       //Memory data access control
  TFT_SEND_DATA(0x00); 
  
  TFT_SEND_CMD(0x3A);       //Interface pixel format
  TFT_SEND_DATA(0x55);      //65K 
  //TFT_SEND_DATA(0x66);      //262K  RGB 6 6 6

  TFT_SEND_CMD(0xe7);       //SPI2 enable    启用2数据通道模式
  TFT_SEND_DATA(0x00); 


  TFT_SEND_CMD(0x21);     //Display inversion on
  TFT_SEND_CMD(0x29);       //Display on
  }
//void Picture_display(const unsigned char *ptr_pic)
//  {
//    unsigned int ROW,column;
//  TFT_SEND_CMD(0x2a);     //Column address set
//  TFT_SEND_DATA(0x00);    //start column
//  TFT_SEND_DATA(0x00+TFT_COLUMN_OFFSET); 
//  TFT_SEND_DATA(0x00);    //end column
//  TFT_SEND_DATA(PIC_LEN-1+ TFT_COLUMN_OFFSET);
//
//  TFT_SEND_CMD(0x2b);     //Row address set
//  TFT_SEND_DATA(0x00);    //start row
//  TFT_SEND_DATA(0x00+TFT_LINE_OFFSET); 
//  TFT_SEND_DATA(0x00);    //end row
//  TFT_SEND_DATA(PIC_HIG-1+ TFT_LINE_OFFSET);
//    TFT_SEND_CMD(0x2C);     //Memory write
//    for(ROW=0;ROW<PIC_HIG;ROW++)        //ROW loop
//      {   
//    
//  for(column=0;column<PIC_LEN;column++) //column loop
//          {
//            TFT_SEND_DATA(*ptr_pic++);
//      TFT_SEND_DATA(*ptr_pic++);
//          }
//      }
//  }
void  setup(){

IO_init();
BL_1;
TFT_init();
  TFT_clear();
}

void  loop(){
  TFT_full(RED);
  delay(2000);
  TFT_full(GREEN);
  delay(2000);
  TFT_full(BLUE);
  delay(2000);
  TFT_clear();
    display_char16_16(20,160,BLUE,0);
  display_char16_16(36,160,GREEN,1);
  display_char16_16(60,160,RED,2);
  display_char16_16(76,160,BLUE,3);
  display_char16_16(92,160,GREEN,4);
  display_char16_16(118,160,BLUE,5);
  display_char16_16(134,160,RED,6);

//    Picture_display(point);
//    Picture_ReverseDisplay(point);
  delay(10000);
              
}
