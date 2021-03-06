/*
  NOKIA 1202 LCD
  Driver for  Arduino.
  Interfacing with the Nokia 1202 LCD, STE2007
  08.07.2013
  SerPanRC@yandex.ru
  http://forum.rcdesign.ru/f90/thread327590.html
   http://youtu.be/PWgo0LkuSA0
*/
 // Подключение к пинам индикатора NOKIA 1202
#define PIN_SCE 5 //  XCS
#define PIN_SDIN 4 // SDA
#define PIN_RESET 6 // XRES
#define PIN_SCLK 3 // SCLK

#define LCD_C LOW // Command
#define LCD_D HIGH // Data

byte lcd_x = 0;
byte lcd_y = 0;
byte column=0; // cтолбец
byte line=0; // строка
byte contrast=10; // контрастность
float t=0;

// Массив для вывода демонстрационной картинки

static const byte LOGO[][96] ={
{0x00,0x00,0x00,0xC0,0x30,0x08,0x08,0x0C,0x04,0x04,0x04,0x04,0x0C,0x08,0x10,0x38,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x04,0x06,0x02,0x02,0x02,0x04,0x0C,0x08,0x10,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x03,0x03,0x02,0x02,0x02,0x02,0x06,0x04,0x9C,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

{0x00,0x00,0x00,0x0F,0x38,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0xC0,0x60,0x10,0x18,0x0F,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x80,0xC0,0x60,0x20,0x30,0x18,0x0C,0x07,0x00,0x00,0xC0,0x70,0x18,0x04,0x04,0x04,0x0C,0x70,0x00},

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x02,0x06,0x04,0x0C,0x78,0x80,0x00,0xE0,0xF8,0x8C,0x84,0x84,0x82,0x82,0x82,0xC2,0x4C,0x78,0x00,0x00,0x02,0x02,0x02,0x02,0xC2,0x72,0x1A,0x06,0x04,0x00,0x00,0x00,0xFF,0x02,0x02,0x03,0x01,0x01,0x00,0x7C,0xC6,0x82,0x02,0x02,0x02,0x03,0x03,0x1F,0x70,0xC0,0x00,0x00,0x00,0xFE,0x0F,0x04,0x02,0x03,0x81,0xE7,0x3C,0x00,0x00,0x00,0x00,0xFF,0x01,0x01,0x01,0x01,0x03,0x06,0x0C,0x18,0xF0,0xC0,0x00,0x3F,0xE0,0x80,0x00,0x00,0x80,0x80,0x00,0x00},

{0x00,0x00,0x00,0x1E,0x3C,0x20,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x30,0x0C,0x07,0x00,0x00,0x01,0x03,0x0C,0x08,0x10,0x10,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x30,0x3C,0x23,0x20,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x06,0x04,0x04,0x04,0x06,0x03,0x01,0x01,0x03,0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x01,0x03,0x02,0x03,0x01,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00}
};

// массив символов

static const byte ASCII[][5] ={
{0x00, 0x00, 0x00, 0x00, 0x00} // 20
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};


// отображение символа

void LcdCharacter(char character){
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }}
// очистка экрана 

void LcdClear(void){
for (int index = 0; index < 864 ; index++)
LcdWrite(LCD_D, 0x00);
}
// Установить позицию на экране c x r в символах

void Lcd_set_pos(byte c, byte r){
  c = c*6;
  r = r*1;
  Lcd_set_pos_pix(c,r);
}

// Установить позицию на экране c x r в пикселях

void Lcd_set_pos_pix(byte c, byte r){
  lcd_x = c;
  lcd_y = r;
  LcdWrite(LCD_C,0xB0 | (r & 0x0F));
  LcdWrite(LCD_C,0x10 | ((c >> 4) & 0x07));
  LcdWrite(LCD_C,0x00 | (c & 0x0F));
}

// Перенос строки

void lcd_crlf(void){
  lcd_x = 0;
  lcd_y = lcd_y+1;
  if(lcd_y>=8) lcd_y = 0;
  Lcd_set_pos_pix(lcd_x,lcd_y);}
  
 // установка контраста 
 
  void lcd_contrast(byte contrast){
    contrast = contrast & 0x1F;
    LcdWrite(LCD_C,0x80 + contrast); // контрастность (0-31)
  }
  
  // отображаем значение как длину вертикальной линии от нижнего края индикатора, удобно использовать как спектрограмму
  
void Lcd_drawColumn(byte sline, byte value){  //строк пустых-оставленных наверху ,уровень что отражаем

    byte lines=9-sline;
    if (value > lines*8) value = lines*8;
    byte mrk = (lines*8 - 1 - value)/8;
    for (byte line = 0; line < mrk; line++) { 
        Lcd_set_pos_pix(column, sline + line);
        LcdWrite(LCD_D, 0x00);}
        
    byte b = 0xff;
    for (byte i = 0; i < lines*8 - mrk*8 - value; i++) {
        b <<= 1;
    }
    Lcd_set_pos_pix(column, sline + mrk);
    LcdWrite(LCD_D, b);
    // заполняем строки от маркера
    for (unsigned char line = mrk + 1; line < lines; line++) {
        Lcd_set_pos_pix(column, sline + line);
        LcdWrite(LCD_D, 0xff);
    } }

//инициализация индикатора

void LcdInitialise(void){
//digitalWrite(PIN_RESET, LOW);
digitalWrite(PIN_RESET, HIGH);
LcdWrite(LCD_C, 0xE2); // сброс програмный
LcdWrite(LCD_C, 0x3D); // Charge pump ON
LcdWrite(LCD_C, 0x01); // Charge pump=4
LcdWrite(LCD_C, 0xA4); //
LcdWrite(LCD_C, 0x2F); //
//LcdWrite(LCD_C, 0xC8);// зеркально верх-низ
LcdWrite(LCD_C, 0xC0); //нормальное верх-низ
//LcdWrite(LCD_C, 0xA1);// зеркально лево-право
LcdWrite(LCD_C, 0xC0); //нормальное лево-право
LcdWrite(LCD_C, 0xAF); // Display ON
}

// печатаем символ

void LcdString(char *characters){
while (*characters){
LcdCharacter(*characters++);
}
}

// печатаем четырехзначное число

void LcdString_f(unsigned int func){
byte k = 0;
if (func>9999) func=9999;
unsigned int n=10000;
for ( byte i = 0; i < 4; i++){
 n=n/10; 
 byte tetra= func/n;
 if (tetra>0)  k = 1;
 byte bukva=tetra+0x30; // переводим цифру в код
 if (k>0|| i==3) LcdCharacter(bukva);
 else LcdCharacter(0x20);// печатаем пустое место если впереди ноль
 func=func-tetra*n;}

}

// программная реализация быстрого SPI

void LcdWrite(byte dc, byte data){
 if (dc==1) PORTD |= 0b00010000; // бит команда\данные на ноге данных ( PD4)
 else PORTD &= ~0b00010000;
 PORTD &= ~0b00100000;        //Сбрасывает  бит(RD5) порта SCE
 PORTD |= 0b00001000;// SCLK (PD3) дрыгаем
 PORTD &= ~0b00001000;
    //цикл по битам
        for (char i=0; i<8; i++) {
        if(data &(0b10000000>>i)) PORTD |= 0b00010000;
        else PORTD &= ~0b00010000;
        //обозначим clock
    PORTD |= 0b00001000;// 1
    PORTD &= ~0b00001000;}// 0
    PORTD |= 0b00100000;//(RD5) порта SCE      
}

// рисуем вертикальную линию от значения у0 до значения у1, удобно применять для осцилограмм

 void DrawVlin (byte y0, byte y1)
{ byte lin_y1=0;
  byte lin_y0=0;
  byte data_l=0;
  if  (y1==y0)                                // Случай
    { lin_y1=y1/8 ; data_l=1<< (y1-8*lin_y1) ;// совпадения 
      Lcd_set_pos_pix(column,lin_y1);         //значений
        LcdWrite(LCD_D, data_l);              //
    }
   else {lin_y0=y0/8; lin_y1=y1/8;    // Если не совпадает определяем строки в каких находятся
          if (lin_y0==lin_y1) {                                                    // рассматриваем если в одной строке
             if (y1>y0) {byte n=7-(y1-y0); byte m=255>>n; data_l=m<<(y0-8*lin_y0);}// если вторая точка выше n - количество нулей добавить
             else {byte n=7-(y0-y1); byte m=255>>n; data_l=m<<(y1-8*lin_y1);} // если вторая ниже
              Lcd_set_pos_pix(column,lin_y1); LcdWrite(LCD_D, data_l); }
          else {if (y1>y0) {byte n=7-(y1-8*lin_y1); byte m=255>>n; data_l=m; // не в одной линии
                  Lcd_set_pos_pix(column,lin_y1); LcdWrite(LCD_D, data_l); // расчитываем и печатаем верхнюю строку
                  n=y0-8*lin_y0;  m=255<<n; data_l=m;
                  Lcd_set_pos_pix(column,lin_y0); LcdWrite(LCD_D, data_l); //расчитываем и печатаем нижнюю строку
                  if ((lin_y1-lin_y0)>1) { 
                    for ( byte i=(lin_y0+1); i <lin_y1; i++) {
                      Lcd_set_pos_pix(column,i); LcdWrite(LCD_D, 255); }  }}
                else {                                               // не в одной линии и первая точка выше второй       
                  
                  byte n=7-(y0-8*lin_y0); byte m=255>>n; data_l=m; // не в одной линии
                  Lcd_set_pos_pix(column,lin_y0); LcdWrite(LCD_D, data_l); // расчитываем и печатаем верхнюю строку
                  n=y1-8*lin_y1;  m=255<<n; data_l=m;
                  Lcd_set_pos_pix(column,lin_y1); LcdWrite(LCD_D, data_l); //расчитываем и печатаем нижнюю строку
                  if ((lin_y0-lin_y1)>1) { 
                    for ( byte i=(lin_y1+1); i <lin_y0; i++) {
                      Lcd_set_pos_pix(column,i); LcdWrite(LCD_D, 255); }  }}
                                                                           }  }
}


void setup()  
{ 
pinMode(PIN_SCE, OUTPUT);
pinMode(PIN_RESET, OUTPUT);
pinMode(PIN_SDIN, OUTPUT);
pinMode(PIN_SCLK, OUTPUT);
digitalWrite(PIN_RESET, LOW);
digitalWrite(PIN_SCE, HIGH);
digitalWrite(PIN_SCLK, LOW);
digitalWrite(PIN_SDIN, LOW);

LcdInitialise();
LcdClear();
//LcdWrite(LCD_C, 0xA7); // Inverted display
LcdWrite(LCD_C, 0xA6); // Normal display
lcd_contrast(contrast);

// напечатаем для демонстрации слова и знаки
 Lcd_set_pos(0,0);
 LcdString(" LCD NOKIA1202");
 Lcd_set_pos(0,2);
 LcdString(" RECIVER scaner");
 Lcd_set_pos(0,4);
 LcdString("  1234567890");
 Lcd_set_pos(0,6);
 LcdString("   <>{}.,;-+");
 
 delay (3000); // пауза
 
 LcdClear();  // очистим экран
 
 // демонстрация режима спектрограммы
 
   for ( byte m = 0; m < 95; m++)
    {
      column=m;
      Lcd_drawColumn(1, (4+m/2));
      column=m+1;
      Lcd_drawColumn(1,0);
     };
    
 delay (3000); // пауза
 LcdClear();  // очистим экран 
 
 // Загрузим картинку на индикатор
 
for (int z = 0; z < 4; z++){
  Lcd_set_pos_pix(0, z+2);
for (int index = 0; index < 96; index++)
  {LcdWrite(LCD_D, LOGO[z][index]);}}
delay(1000);

// Загрузим картинку в движении

for (int u = 0; u < 96; u++){
 for (int z = 0; z < 4; z++){
  Lcd_set_pos_pix(0, z+2);
for (int index = u; index < 96; index++)
  {LcdWrite(LCD_D, LOGO[z][index]);}}
delay(20); 
 }
 
 }
  
void loop()
{  
   // демонстрация режима для осцилограммы, отобразим функцию синуса
   
   t=t+0.6;
   if (t>96) t=0;
   for (byte  i = 0; i <96; i++) {
   column=i; 
   for ( byte p=0; p <9; p++) { Lcd_set_pos_pix(column,p); LcdWrite(LCD_D, 0); }
   DrawVlin ((33+30*sin((i+t)/8)),(33+30*sin((i+t+0.7)/8)));};
  
  }
   

