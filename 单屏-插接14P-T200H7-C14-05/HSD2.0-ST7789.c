
	TFT_SEND_CMD(0x11); 			//Sleep Out
	delay_ms(120);               //DELAY120ms 
	//--------------------------------ST7789S Frame rate setting----------------------------------// 
	TFT_SEND_CMD(0x2a); 		//Column address set
	TFT_SEND_DATA(0x00); 		//start column
	TFT_SEND_DATA(0x00); 
	TFT_SEND_DATA(0x00);		//end column
	TFT_SEND_DATA(0xef);

	TFT_SEND_CMD(0x2b); 		//Row address set
	TFT_SEND_DATA(0x00); 		//start row
	TFT_SEND_DATA(0x28); 
	TFT_SEND_DATA(0x01);		//end row
	TFT_SEND_DATA(0x17);

	TFT_SEND_CMD(0xb2); 		//Porch control
	TFT_SEND_DATA(0x0c); 
	TFT_SEND_DATA(0x0c); 
	TFT_SEND_DATA(0x00); 
	TFT_SEND_DATA(0x33); 
	TFT_SEND_DATA(0x33); 

	TFT_SEND_CMD(0x21); 		//Display Inversion Off

	TFT_SEND_CMD(0xb7); 		//Gate control
	TFT_SEND_DATA(0x56);   		//35
//---------------------------------ST7789S Power setting--------------------------------------// 
	TFT_SEND_CMD(0xbb); //VCOMS Setting
	TFT_SEND_DATA(0x18);  //1f

	TFT_SEND_CMD(0xc0); 		//LCM Control
	TFT_SEND_DATA(0x2c); 

	TFT_SEND_CMD(0xc2); 		//VDV and VRH Command Enable
	TFT_SEND_DATA(0x01); 

	TFT_SEND_CMD(0xc3); //VRH Set
	TFT_SEND_DATA(0x1f); //12

	TFT_SEND_CMD(0xc4); 			//VDV Setting
	TFT_SEND_DATA(0x20); 

	TFT_SEND_CMD(0xc6); 			//FR Control 2
	TFT_SEND_DATA(0x0f); 

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

	TFT_SEND_CMD(0xe1); 				//Negative Voltage Gamma Contro
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
	
	TFT_SEND_CMD(0x36); 			//Memory data access control
	TFT_SEND_DATA(0x00); 
	
	TFT_SEND_CMD(0x3A); 			//Interface pixel format
	//TFT_SEND_DATA(0x55);			//65K	
	TFT_SEND_DATA(0x66);			//262K  RGB 6 6 6

	TFT_SEND_CMD(0xe7); 			//SPI2 enable    启用2数据通道模式
	TFT_SEND_DATA(0x00); 


	TFT_SEND_CMD(0x21);			//Display inversion on
	TFT_SEND_CMD(0x29); 			//Display on
	TFT_SEND_CMD(0x2C);			//Memory write
