#include "LCDDriver.h"

LCDDriver *LCDDriver::instance=0;

LCDDriver::LCDDriver(uint32_t orient)
{
    this->orientation = orient;
    wiringPiSetupGpio();
    delay(50);
    PortSetup();
    delay(50);
    LCDInit();
    delay(50);
    //disable READ
    digitalWrite(RD,HIGH); 
}

LCDDriver::~LCDDriver(){}

LCDDriver *LCDDriver::GetInstance(uint32_t orient)
{
    if (!instance)
        instance = new LCDDriver(orient);
    return instance;
}

void LCDDriver::PortSetup()
{
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    pinMode(D9, OUTPUT);
    pinMode(D10, OUTPUT);
    pinMode(D11, OUTPUT);
    pinMode(D12, OUTPUT);
    pinMode(D13, OUTPUT);
    pinMode(D14, OUTPUT);
    pinMode(D15, OUTPUT);

    pinMode(CS, OUTPUT);
    pinMode(RS, OUTPUT);
    pinMode(WR, OUTPUT);
    pinMode(RD, OUTPUT);
    pinMode(RESET, OUTPUT); 

    PinArray[0] = D0;
    PinArray[1] = D1;
    PinArray[2] = D2;
    PinArray[3] = D3;
    PinArray[4] = D4;
    PinArray[5] = D5;
    PinArray[6] = D6;
    PinArray[7] = D7;
    PinArray[8] = D8;
    PinArray[9] = D9;
    PinArray[10] = D10;
    PinArray[11] = D11;
    PinArray[12] = D12;
    PinArray[13] = D13;
    PinArray[14] = D14;
    PinArray[15] = D15;
}

void LCDDriver::LCDInit()
{
    digitalWrite(RD,HIGH);
    digitalWrite(RESET,HIGH);
    delay(5);
    digitalWrite(RESET,LOW);
    delay(15);
    digitalWrite(RESET,HIGH);
    delay(15);
    digitalWrite(CS,LOW);
    WriteCmdData(0x0000,0x0001);
    WriteCmdData(0x0003,0xA8A4);
    WriteCmdData(0x000C,0x0000);
    WriteCmdData(0x000D,0x800C);
    WriteCmdData(0x000E,0x2B00);
    WriteCmdData(0x001E,0x00B7);
    WriteCmdData(0x0001,0x2B3F);
    WriteCmdData(0x0002,0x0600);
    WriteCmdData(0x0010,0x0000);
    WriteCmdData(0x0011,0x6070); //left->right, up->down continous fill
    WriteCmdData(0x0005,0x0000);
    WriteCmdData(0x0006,0x0000);
    WriteCmdData(0x0016,0xEF1C);
    WriteCmdData(0x0017,0x0003);
    WriteCmdData(0x0007,0x0233);
    WriteCmdData(0x000B,0x0000);
    WriteCmdData(0x000F,0x0000);
    WriteCmdData(0x0041,0x0000);
    WriteCmdData(0x0042,0x0000);
    WriteCmdData(0x0048,0x0000);
    WriteCmdData(0x0049,0x013F);
    WriteCmdData(0x004A,0x0000);
    WriteCmdData(0x004B,0x0000);
    WriteCmdData(0x0044,0xEF95);
    WriteCmdData(0x0045,0x0000);
    WriteCmdData(0x0046,0x013F);
    WriteCmdData(0x0030,0x0707);
    WriteCmdData(0x0031,0x0204);
    WriteCmdData(0x0032,0x0204);
    WriteCmdData(0x0033,0x0502);
    WriteCmdData(0x0034,0x0507);
    WriteCmdData(0x0035,0x0204);
    WriteCmdData(0x0036,0x0204);
    WriteCmdData(0x0037,0x0502);
    WriteCmdData(0x003A,0x0302);
    WriteCmdData(0x003B,0x0302);
    WriteCmdData(0x0023,0x0000);
    WriteCmdData(0x0024,0x0000);
    WriteCmdData(0x0025,0x8000);
    WriteCmdData(0x004f,0x0000);
    WriteCmdData(0x004e,0x0000);
    WriteCmd(0x0022);
    digitalWrite(CS,HIGH); 
}

void LCDDriver::WriteCmd(uint16_t cmd)
{
    int bits[16] = {D0,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15};
    int i;
    
    //digitalWrite(RD,HIGH);    
    digitalWrite(RS,LOW);
    for (i=0;i<16;i++)
    {
        digitalWrite(bits[i], (cmd&1)?HIGH:LOW);
        cmd >>= 1;
    }
    digitalWrite(WR,LOW);
    digitalWrite(WR,HIGH);    
}

void LCDDriver::WriteData(uint16_t data)
{
    int bits[16] = {D0,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15};
    int i;
    
    //digitalWrite(RD,HIGH);
    digitalWrite(RS,HIGH);
    for (i=0;i<16;i++)
    {
        digitalWrite(bits[i], (data&1)?HIGH:LOW);
        data >>= 1;
    }
    digitalWrite(WR,LOW);
    digitalWrite(WR,HIGH);
}

void LCDDriver::WriteCmdData(uint16_t cmd, uint16_t data)
{
   WriteCmd(cmd);
   WriteData(data);
}

void LCDDriver::LCDSetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{  
    //Column address start2   
    WriteCmdData(0x44,(x2 << 8) + x1 );  
    //Column address start1
    WriteCmdData(0x45,y1);  
    //Column address end2    
    WriteCmdData(0x46,y2);      
    
    //Column address end1
    WriteCmdData(0x4E,x1);  
    //Row address start2
    WriteCmdData(0x4F,y1);
    
    WriteCmd(0x22);    
}

void LCDDriver::LCDFill(uint16_t color)
{
    unsigned int i,j;
    digitalWrite(CS,LOW);
    LCDSetAddress(0,0,239,319);
    WriteData(color);
    for(i = 0; i < 320*240; i++)
    {
        digitalWrite(WR,LOW);
        digitalWrite(WR,HIGH);
    }
    digitalWrite(CS,HIGH);   
}

void LCDDriver::RectFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	int i;
    digitalWrite(CS,LOW);    
	LCDSetAddress(x,y,x+w-1,y+h-1); 	
    WriteData(color);
	for( i = 0; i < w*h; i++ )		
    {
        digitalWrite(WR,LOW); 
        digitalWrite(WR,HIGH);
    }
    digitalWrite(CS,HIGH);        
}

void LCDDriver::Pixel(uint16_t x,uint16_t y,uint16_t color)
{
    digitalWrite(CS,LOW); 
    LCDSetAddress(x,y,x,y);
    WriteData(color);
    digitalWrite(CS,HIGH);  
}

void LCDDriver::Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    int x,y,addx,addy,dx,dy;
    long P;
    unsigned int i;

    dx = (x2-x1>0)?x2-x1:x1-x2;
    dy = (y2-y1>0)?y2-y1:y1-y2;
    x = x1;
    y = y1;

    if(x1 > x2)
        addx = -1;
    else
        addx = 1;

    if(y1 > y2)
        addy = -1;
    else
        addy = 1;


    if(dx >= dy)
    {
        P = (2*dy) - dx;
        for(i = 1; i <= (dx +1); i++)
        {
            Pixel(x,y,color);
            if(P < 0)
            {
                P = P + (2*dy);
                x = (x + addx);
            }
            else
            {
                P = P+(2*dy) - (2*dx);
                x = x + addx;
                y = y + addy;
            }
        }
    }
    else
    {
        P = (2*dx) - dy;
        for(i = 1; i <= (dy +1); i++)
        {
            Pixel(x,y,color);
            if(P<0)
            {
                P = P + (2*dx);
                y = y + addy;
            }
            else
            {
                P = P + (2*dx) - (2*dy);
                x = x + addx;
                y = y + addy;
            }
        }
    }
}

void LCDDriver::HLine(uint16_t x1,uint16_t y1,uint16_t w,uint16_t color)
{
    RectFill(x1,y1,w,1,color);
}

void LCDDriver::VLine(uint16_t x1,uint16_t y1,uint16_t h,uint16_t color)
{
    RectFill(x1,y1,1,h,color);
}

void LCDDriver::Rectangle(uint16_t x1,uint16_t y1,uint16_t w,uint16_t h,uint16_t color)
{
    int i;
    
    HLine(x1,y1,w-1,color);
    HLine(x1,y1+h-1,w-1,color);
    VLine(x1,y1,h-1,color);
    VLine(x1+w-1,y1,h-1,color);
}

void LCDDriver::Circle(uint16_t x,uint16_t y,uint16_t radius,uint16_t fill,uint16_t color)
{
    int a_,b_,P;
    a_ = 0;
    b_ = radius;
    P = 1 - radius;
    while (a_ <= b_)
    {
        if(fill == 1)
        {
            RectFill(x-a_,y-b_,a_,b_,color);
            RectFill(x-b_,y-a_,b_,a_,color);
        }
        else
        {
            Pixel(a_+x, b_+y, color);
            Pixel(b_+x, a_+y, color);
            Pixel(x-a_, b_+y, color);
            Pixel(x-b_, a_+y, color);
            Pixel(b_+x, y-a_, color);
            Pixel(a_+x, y-b_, color);
            Pixel(x-a_, y-b_, color);
            Pixel(x-b_, y-a_, color);
        }
        if (P < 0 )
        {
            P = (P + 3) + (2* a_);
            a_ ++;
        }
        else
        {
            P = (P + 5) + (2* (a_ - b_));
            a_ ++;
            b_ --;
        }
    }
}

void LCDDriver::SetFont( font_t* font )
{
    _font = font;
}

font_t* LCDDriver::GetFont()
{
    return (font_t*)_font;
}

uint8_t LCDDriver::GetFontWidth()
{
    if ( _font != 0 ) 
        return _font->Width;
    return 0;   
}

uint8_t LCDDriver::GetFontHeight()
{
    if ( _font != 0 ) 
        return _font->Height;
    return 0;    
}

/*void LCDDriver::PrintChar( char c, uint16_t x, uint16_t y, uint16_t fgColor)
{
    uint8_t i, ch;
    uint16_t j,k;   
    int16_t position = _font->Position[ c - _font->Offset ];

	if ( position == -1 ) 
		position = 0;
    
    for ( j = 0; j < _font->Height; j++ )
    {
        for (k=0;k<_font->Width/8;k++)
        {
            ch = _font->Data[ position ];
            for ( i = 0; i < 8; i++ )
            {            
                if ( ( ch & ( 1 << ( 7 - i ) ) ) != 0 ) 
                    Pixel(x+8*k+i, y+j,fgColor);
            }
            position++;
        }
    }        
}*/

void LCDDriver::PrintChar( char c, uint16_t x, uint16_t y, uint16_t fgColor, uint16_t bgColor)
{
    uint8_t i, ch;
    uint16_t j,k,q;   
    int16_t position = _font->Position[ c - _font->Offset ];
    uint16_t color;
    uint16_t *representation;

	if ( position == -1 ) 
		position = 0;
    
    representation = (uint16_t*)malloc(_font->Width*_font->Height*sizeof(uint16_t));
    q=0;
    for ( j = 0; j < _font->Height; j++ )
    {
        for (k=0;k<_font->Width/8;k++)
        {
            ch = _font->Data[ position ];
            for ( i = 0; i < 8; i++ )
            {            
                if ( ( ch & ( 1 << ( 7 - i ) ) ) != 0 ) 
                    representation[q++] = fgColor;
                else
                    representation[q++] = bgColor;;
            }
            position++;
        }
    }        
        
    digitalWrite(CS,LOW);        
    LCDSetAddress(x,y,x+_font->Width-1,y+_font->Height-1);
    for (i=0;i<_font->Width * _font->Height;i++)
        WriteData(representation[i]);
    digitalWrite(CS,HIGH);      

    free(representation);
}

/*void LCDDriver::Print( const char *str, uint16_t x, uint16_t y, uint16_t fgColor)
{
    int stl, i;

    stl = strlen( str );

    for ( i = 0; i < stl; i++ )
        PrintChar( *str++, x + ( i * ( _font->Width ) ), y, fgColor );    
}*/

//functioneaza corect cu afisare left->right, top->down, PORTRAIT mode
void LCDDriver::Print( const char *str, uint16_t x, uint16_t y, uint16_t fgColor, uint16_t bgColor)
{
    int stl, i,j,k,l,q;
    char ch;
    uint16_t positions[30]; 
    uint16_t color;

    stl = strlen( str );
    
    //check if string is larger than XRES
    if (x+stl*_font->Width>=RESX)
        stl = stl - (x+stl*_font->Width - RESX)/_font->Width;
    if (y+_font->Height >= RESY)
        return;
    
    for (i=0;i<stl;i++)
        positions[i] = (_font->Position[str[i]-_font->Offset]==-1)?0:_font->Position[str[i]-_font->Offset];
    
    digitalWrite(CS,LOW);        
    LCDSetAddress(x,y,x+stl*_font->Width-1,y+_font->Height-1);
    digitalWrite(RS,HIGH);          
    for (i=0;i<_font->Height;i++)   //for each full text pixel line
    {
        for (j=0;j<stl;j++) //for each different char
        {                 
            for (k=0;k<_font->Width/8;k++)  //for each line in font
            {                              
                ch = _font->Data[positions[j]];
                for ( l = 0; l < 8; l++ )
                {            
                    if ( ( ch & ( 1 << ( 7 - l ) ) ) != 0 ) 
                    {
                        color = fgColor;
                        for (q=0;q<16;q++)
                        {
                            digitalWrite(PinArray[q], (color&1)?HIGH:LOW);
                            color >>= 1;
                        }
                        digitalWrite(WR,LOW);
                        digitalWrite(WR,HIGH);  
                    }
                    else
                    {
                        color = bgColor;
                        for (q=0;q<16;q++)
                        {
                            digitalWrite(PinArray[q], (color&1)?HIGH:LOW);
                            color >>= 1;
                        }
                        digitalWrite(WR,LOW);
                        digitalWrite(WR,HIGH);  
                    }
                }
                positions[j]++;
            }
        }
    }
    digitalWrite(CS,HIGH);     
}

void LCDDriver::DisplayBMP(uint16_t x,uint16_t y,uint8_t *buffer)
{
    uint32_t width,height;
    uint16_t bpp;
    uint32_t linesize,pos,start_pos,raw_bitmapsz;
    uint32_t xx,yy;
    uint32_t rest,padding;
    uint16_t color;
	uint8_t r,g,b;
    uint8_t *pixels;
    int byte_per_pixel;
    int i;    
        
    width=*((uint32_t*)(buffer+0x12));
    height=*((uint32_t*)(buffer+0x16));
    bpp=*((uint16_t*)(buffer+0x1C));
    raw_bitmapsz=*((uint32_t*)(buffer+0x22));
    start_pos=*((uint32_t*)(buffer+0x0A));
    
    byte_per_pixel = bpp/8;
    if (!raw_bitmapsz)
        raw_bitmapsz = width*height*byte_per_pixel;    
    
    linesize=width*byte_per_pixel;
    rest=linesize%4;
    if (rest)
    {
        padding=4-rest;
        linesize+=padding;
    }
    else 
        padding=0;	
    
    pos=raw_bitmapsz-linesize;
    pixels = buffer+start_pos;   
    
    digitalWrite(CS,LOW);        
    LCDSetAddress(x,y,x+width-1,y+height-1); 
    digitalWrite(RS,HIGH);    
    for (yy=y;yy<y+height-2;yy++)
    {
        for (xx=x;xx<x+width;xx++)
        {
            switch (byte_per_pixel)
            {
                case 3:
                    b=((uint8_t*)(pixels))[pos+0];
                    g=((uint8_t*)(pixels))[pos+1];
                    r=((uint8_t*)(pixels))[pos+2];	
                    color=RGB(r,g,b);
                    break;
                case 2:
                    color = *(uint16_t*)(pixels+pos);
                    break;
                default:
                    continue;
            }

            for (i=0;i<16;i++)
            {
                digitalWrite(PinArray[i], (color&1)?HIGH:LOW);
                color >>= 1;
            }
            digitalWrite(WR,LOW);
            digitalWrite(WR,HIGH); 
            
            pos+=byte_per_pixel;
            if ((pos+padding)%linesize==0)
            {
                pos+=padding;					
                pos-=2*linesize;	
            }		                        
        }		
    }
    
    for (xx=x;xx<x+width;xx++)
    {
        switch (byte_per_pixel)
        {
            case 3:
                b=((uint8_t*)(pixels))[pos+0];
                g=((uint8_t*)(pixels))[pos+1];
                r=((uint8_t*)(pixels))[pos+2];	
                color=RGB(r,g,b);
                break;
            case 2:
                color = *(uint16_t*)(pixels+pos);
                break;
            default:
                continue;
        }

        for (i=0;i<16;i++)
        {
            digitalWrite(PinArray[i], (color&1)?HIGH:LOW);
            color >>= 1;
        }
        digitalWrite(WR,LOW);
        digitalWrite(WR,HIGH); 
        
        pos+=byte_per_pixel;
        if ((pos+padding)%linesize==0)
        {
            pos+=padding;					
            pos-=2*linesize;	
        }		                        
    }   
    digitalWrite(CS,HIGH); 
}


