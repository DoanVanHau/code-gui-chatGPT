#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Bounce2.h>
#include <WatchdogManager.h>
// Tạo đối tượng WatchdogManager với timeout mặc định là 2 giây
WatchdogManager wdtManager;

#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
#define ledBao 2         //ESP_GPIO2_D4
#define latchPin  16      //ESP_GPIO16_D0 / arduino 11 / ST_CP
#define clockPin  5      //ESP_GPIO5_D1  / arduino 12 / SH_CP
#define dataPin   4      //ESP_GPIO4_D2  / arduino 10 / DS
#define BUTTON_Serial 14  //ESP_GPIO14_D5 / arduino 3
#define delays  1000
// Set your new MAC Address
uint8_t newMACAddress[] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x11};//Dia chi moi bang den
//////////////////////////////////////////////////////////////////////////////////// bang den {
bool TinHieu = false;
bool Wifi_On = false;
String noiChuoi = "";
String tempTong;
String temp1;
String temp2;
String temp3;
bool DauAm = false;
const int Serial_eepromAdress = 1;
unsigned int GiaTriTruyen = 0;
int value_Serial = 0;
Bounce debouncer1 = Bounce();
//////////////////////////////////////////////////////////////////////////////////// bang den }
// Structure example to receive data
// Must match the sender structure
typedef struct test_struct  
  {
    char chuoi[100];
  } test_struct ;
// Create a struct_message called myData
test_struct  myData;
////////////////////////////////////////////////////////////////////////////////////bang den {
byte c1 = 11;
byte c2 = 11;
byte c3 = 11;
byte c4 = 11;
byte c5 = 11;
byte c6 = 11;

const byte Seg1[12] = 
{
  0b01101111, // so 0         //0b,dp, a, b, g, c, d, e, f  // chan boar led 7 doan cu
  0b00101000, // so 1         //0b, dp, a, b, c, d, e, f, g      74hc595
  0b01110110, // so 2
  0b01111100, // so 3
  0b00111001, // so 4
  0b01011101, // so 5 
  0b01011111, // so 6
  0b01101000, // so 7
  0b01111111, // so 8
  0b01111101, // so 9
  0b00010000, // dau -
  0b00000000, // khong hien thi gi
};
const byte DaoSeg2[12] = 
{
  0b10010000, // so 0         //0b,dp, a, b, g, c, d, e, f  // chan boar led 7 doan cu
  0b11010111, // so 1         //0b, dp, a, b, c, d, e, f, g      74hc595
  0b10001001, // so 2
  0b10000011, // so 3
  0b11000110, // so 4
  0b10100010, // so 5
  0b10100000, // so 6
  0b10010111, // so 7
  0b10000000, // so 8
  0b10000010, // so 9
  0b11101111, // dau -
  0b11111111, // khong hien thi gi
};
const byte Seg2[12] = 
{
  0b01111001, // q        //0b,dp, a, b, g, c, d, e, f  // chan boar led 7 doan cu
  0b00000100, // _         //0b, dp, a, b, c, d, e, f, g      74hc595
  0b00111011, // H
  0b00101111, // U
  0b01101011, // N
  0b01011111, // G 
};
void ChuQH(void)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg2[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg2[1]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg2[2]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg2[3]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg2[4]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg2[5]);
  digitalWrite(latchPin,HIGH);
}
void TestLedTruocKhiChay(void)
{ 
  ChuQH();
  digitalWrite(ledBao,LOW);
  delay(delays);
  HienThiSo(0,0,0,0,0,0);
  digitalWrite(ledBao,HIGH);
  delay(delays);
  HienThiSo(1,1,1,1,1,1);
  digitalWrite(ledBao,LOW);
  delay(delays);
  HienThiSo(2,2,2,2,2,2);
  digitalWrite(ledBao,HIGH);
  delay(delays);
  HienThiSo(3,3,3,3,3,3);
  digitalWrite(ledBao,LOW);
  delay(delays);
  HienThiSo(4,4,4,4,4,4);
  digitalWrite(ledBao,HIGH);
  delay(delays);
  HienThiSo(5,5,5,5,5,5);
  digitalWrite(ledBao,LOW);
  delay(delays);
  HienThiSo(6,6,6,6,6,6);
  digitalWrite(ledBao,HIGH);
  delay(delays);
  HienThiSo(7,7,7,7,7,7);
  digitalWrite(ledBao,LOW);
  delay(delays);
  HienThiSo(8,8,8,8,8,8);
  digitalWrite(ledBao,HIGH);
  delay(delays);
  HienThiSo(9,9,9,9,9,9);
  digitalWrite(ledBao,LOW);
  delay(delays);
  HienThiSo(11,11,11,11,11,11);
  digitalWrite(ledBao,HIGH);
  delay(delays);
  HienThiLedSerial();
  digitalWrite(ledBao,LOW);
  delay(delays);
}
void HamChuyenSangSoNguyen(void)
{
  int i, len;
  int result = 0;
  len = temp1.length();
  for(i=0; i<len; i++)
    {
      result = result*10 + (temp1[i] - '0');
    }
  temp2 = String(result);
  //Serial.println(temp2); 
}
void Serial_9600(void)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[9]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[6]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  digitalWrite(latchPin,HIGH);
}
void Serial_4800(void)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[4]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[8]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  digitalWrite(latchPin,HIGH);
}
void Serial_2400(void)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[2]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[4]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  digitalWrite(latchPin,HIGH);
}
void Serial_1200(void)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[11]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[1]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[2]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[0]);
  digitalWrite(latchPin,HIGH);
}
void HienThiLedSerial(void)
{
  if(EEPROM.read(Serial_eepromAdress) == 0)
    {
      Serial_2400();
    }
  else if(EEPROM.read(Serial_eepromAdress) == 1)
    {
      Serial_4800();
    }
  else if(EEPROM.read(Serial_eepromAdress) == 2)
    {
      Serial_9600();
    }
  else if(EEPROM.read(Serial_eepromAdress) == 3)
    {
      Serial_1200();
    }
}
void HamKhoiTaoSerial(void)
{
  
  if(EEPROM.read(Serial_eepromAdress) == 0)
    {
      Serial.begin(2400);
    }
  else if(EEPROM.read(Serial_eepromAdress) == 1)
    {
      Serial.begin(4800);
    }
  else if(EEPROM.read(Serial_eepromAdress) == 2)
    {
      Serial.begin(9600);
    }
  else if(EEPROM.read(Serial_eepromAdress) == 3)
    {
      Serial.begin(1200);
    }
  else
    {
      GiaTriTruyen = 0;
      EEPROM.write(Serial_eepromAdress, GiaTriTruyen); // ghi 1 byte vao dia chi
      EEPROM.commit();
      delay(50);            
      HienThiLedSerial();
      delay(1000);
      Serial.begin(2400);
    }
}
void DocTrangThaiNutNhan(void)
{
  static unsigned long time4 = 0UL;
  if((unsigned long) millis() - time4 >= 100UL)
    {
      time4 = millis();
      // Update trang thai nut nhan
      debouncer1.update();
      // gan gia tri cho bien value
      value_Serial = debouncer1.read();
    }
}
void HieuChinhSerial(void)
{
  static bool Serial_true = false;
  if(value_Serial == LOW)
    {
      if(Serial_true == false)
        {
          GiaTriTruyen = GiaTriTruyen + 1;
          if(GiaTriTruyen >= 4)
            {
              GiaTriTruyen = 0;
            }
          //Serial.println(GiaTriTruyen);
          EEPROM.write(Serial_eepromAdress, GiaTriTruyen);
          EEPROM.commit();
          delay(50);
          HamKhoiTaoSerial();            
          HienThiLedSerial();
          delay(1000);
          Serial_true = true;
        }  
    }
  else
    {
      Serial_true = false;
    }
}
void HamPhanTichInRaLed7Doan(void)
{
  unsigned int DoDaiChuoiCan = temp2.length();
  Serial.println(temp2);
  if(DauAm == true)
    {
      if(DoDaiChuoiCan == 0)
        {
          
        }
      else if(DoDaiChuoiCan == 1)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c1 = 9;
            }
          ///////////////////////////////////////////////////////// khong hien thi / dau am
          c2 = 11;
          c3 = 11;
          c4 = 11;
          c5 = 11;
          c6 = 10;
        }
      else if(DoDaiChuoiCan == 2)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c2 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi / dau am
          c3 = 11;
          c4 = 11;
          c5 = 11;
          c6 = 10;
        }
      else if(DoDaiChuoiCan == 3)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c3 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c2 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi / dau am
          c4 = 11;
          c5 = 11;
          c6 = 10;
        }
      else if(DoDaiChuoiCan == 4)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c4 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c4 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c4 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c4 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c4 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c4 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c4 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c4 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c4 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c4 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c3 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c2 = 9;
            }
          //////////////////////////////////////////nghin
          if(temp2.charAt(3) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(3) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(3) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(3) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(3) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(3) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(3) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(3) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(3) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(3) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi / dau am
          c5 = 11;
          c6 = 10;
        }
      else if(DoDaiChuoiCan == 5)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c5 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c5 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c5 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c5 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c5 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c5 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c5 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c5 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c5 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c5 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c4 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c4 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c4 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c4 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c4 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c4 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c4 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c4 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c4 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c4 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c3 = 9;
            }
          //////////////////////////////////////////nghin
          if(temp2.charAt(3) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(3) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(3) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(3) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(3) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(3) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(3) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(3) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(3) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(3) == '9')
            {
              c2 = 9;
            }
          ////////////////////////////////////////// chuc nghin
          if(temp2.charAt(4) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(4) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(4) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(4) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(4) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(4) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(4) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(4) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(4) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(4) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi / dau am
          c6 = 10;
        }
    }
  else
    {
      if(DoDaiChuoiCan == 0)
        {
          
        }
      else if(DoDaiChuoiCan == 1)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c1 = 9;
            }
          ///////////////////////////////////////////////////////// khong hien thi
          c2 = 11;
          c3 = 11;
          c4 = 11;
          c5 = 11;
          c6 = 11;
        }
      else if(DoDaiChuoiCan == 2)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c2 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi
          c3 = 11;
          c4 = 11;
          c5 = 11;
          c6 = 11;
        }
      else if(DoDaiChuoiCan == 3)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c3 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c2 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi
          c4 = 11;
          c5 = 11;
          c6 = 11;
        }
      else if(DoDaiChuoiCan == 4)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c4 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c4 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c4 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c4 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c4 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c4 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c4 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c4 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c4 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c4 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c3 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c2 = 9;
            }
          //////////////////////////////////////////nghin
          if(temp2.charAt(3) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(3) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(3) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(3) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(3) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(3) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(3) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(3) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(3) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(3) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi
          c5 = 11;
          c6 = 11;
        }
      else if(DoDaiChuoiCan == 5)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c5 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c5 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c5 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c5 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c5 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c5 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c5 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c5 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c5 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c5 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c4 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c4 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c4 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c4 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c4 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c4 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c4 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c4 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c4 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c4 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c3 = 9;
            }
          //////////////////////////////////////////nghin
          if(temp2.charAt(3) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(3) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(3) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(3) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(3) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(3) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(3) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(3) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(3) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(3) == '9')
            {
              c2 = 9;
            }
          ////////////////////////////////////////// chuc nghin
          if(temp2.charAt(4) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(4) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(4) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(4) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(4) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(4) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(4) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(4) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(4) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(4) == '9')
            {
              c1 = 9;
            }
          //////////////////////////////////////////////////////Khong hien thi
          c6 = 11;
        }
      else if(DoDaiChuoiCan == 6)
        {
          //////////////////////////////////////////don vi
          if(temp2.charAt(0) == '0')
            {
              c6 = 0;
            }
          else if(temp2.charAt(0) == '1')
            {
              c6 = 1;
            }
          else if(temp2.charAt(0) == '2')
            {
              c6 = 2;
            }
          else if(temp2.charAt(0) == '3')
            {
              c6 = 3;
            }
          else if(temp2.charAt(0) == '4')
            {
              c6 = 4;
            }
          else if(temp2.charAt(0) == '5')
            {
              c6 = 5;
            }
          else if(temp2.charAt(0) == '6')
            {
              c6 = 6;
            }
          else if(temp2.charAt(0) == '7')
            {
              c6 = 7;
            }
          else if(temp2.charAt(0) == '8')
            {
              c6 = 8;
            }
          else if(temp2.charAt(0) == '9')
            {
              c6 = 9;
            }
          //////////////////////////////////////////chuc
          if(temp2.charAt(1) == '0')
            {
              c5 = 0;
            }
          else if(temp2.charAt(1) == '1')
            {
              c5 = 1;
            }
          else if(temp2.charAt(1) == '2')
            {
              c5 = 2;
            }
          else if(temp2.charAt(1) == '3')
            {
              c5 = 3;
            }
          else if(temp2.charAt(1) == '4')
            {
              c5 = 4;
            }
          else if(temp2.charAt(1) == '5')
            {
              c5 = 5;
            }
          else if(temp2.charAt(1) == '6')
            {
              c5 = 6;
            }
          else if(temp2.charAt(1) == '7')
            {
              c5 = 7;
            }
          else if(temp2.charAt(1) == '8')
            {
              c5 = 8;
            }
          else if(temp2.charAt(1) == '9')
            {
              c5 = 9;
            }
          //////////////////////////////////////////tram
          if(temp2.charAt(2) == '0')
            {
              c4 = 0;
            }
          else if(temp2.charAt(2) == '1')
            {
              c4 = 1;
            }
          else if(temp2.charAt(2) == '2')
            {
              c4 = 2;
            }
          else if(temp2.charAt(2) == '3')
            {
              c4 = 3;
            }
          else if(temp2.charAt(2) == '4')
            {
              c4 = 4;
            }
          else if(temp2.charAt(2) == '5')
            {
              c4 = 5;
            }
          else if(temp2.charAt(2) == '6')
            {
              c4 = 6;
            }
          else if(temp2.charAt(2) == '7')
            {
              c4 = 7;
            }
          else if(temp2.charAt(2) == '8')
            {
              c4 = 8;
            }
          else if(temp2.charAt(2) == '9')
            {
              c4 = 9;
            }
          //////////////////////////////////////////nghin
          if(temp2.charAt(3) == '0')
            {
              c3 = 0;
            }
          else if(temp2.charAt(3) == '1')
            {
              c3 = 1;
            }
          else if(temp2.charAt(3) == '2')
            {
              c3 = 2;
            }
          else if(temp2.charAt(3) == '3')
            {
              c3 = 3;
            }
          else if(temp2.charAt(3) == '4')
            {
              c3 = 4;
            }
          else if(temp2.charAt(3) == '5')
            {
              c3 = 5;
            }
          else if(temp2.charAt(3) == '6')
            {
              c3 = 6;
            }
          else if(temp2.charAt(3) == '7')
            {
              c3 = 7;
            }
          else if(temp2.charAt(3) == '8')
            {
              c3 = 8;
            }
          else if(temp2.charAt(3) == '9')
            {
              c3 = 9;
            }
          ////////////////////////////////////////// chuc nghin
          if(temp2.charAt(4) == '0')
            {
              c2 = 0;
            }
          else if(temp2.charAt(4) == '1')
            {
              c2 = 1;
            }
          else if(temp2.charAt(4) == '2')
            {
              c2 = 2;
            }
          else if(temp2.charAt(4) == '3')
            {
              c2 = 3;
            }
          else if(temp2.charAt(4) == '4')
            {
              c2 = 4;
            }
          else if(temp2.charAt(4) == '5')
            {
              c2 = 5;
            }
          else if(temp2.charAt(4) == '6')
            {
              c2 = 6;
            }
          else if(temp2.charAt(4) == '7')
            {
              c2 = 7;
            }
          else if(temp2.charAt(4) == '8')
            {
              c2 = 8;
            }
          else if(temp2.charAt(4) == '9')
            {
              c2 = 9;
            }
          ////////////////////////////////////////// tram nghin
          if(temp2.charAt(5) == '0')
            {
              c1 = 0;
            }
          else if(temp2.charAt(5) == '1')
            {
              c1 = 1;
            }
          else if(temp2.charAt(5) == '2')
            {
              c1 = 2;
            }
          else if(temp2.charAt(5) == '3')
            {
              c1 = 3;
            }
          else if(temp2.charAt(5) == '4')
            {
              c1 = 4;
            }
          else if(temp2.charAt(5) == '5')
            {
              c1 = 5;
            }
          else if(temp2.charAt(5) == '6')
            {
              c1 = 6;
            }
          else if(temp2.charAt(5) == '7')
            {
              c1 = 7;
            }
          else if(temp2.charAt(5) == '8')
            {
              c1 = 8;
            }
          else if(temp2.charAt(5) == '9')
            {
              c1 = 9;
            }
        }
    }
}
void HienThiSo(byte c_1, byte c_2, byte c_3, byte c_4, byte c_5, byte c_6)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[c_6]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[c_5]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[c_4]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[c_3]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[c_2]);
  shiftOut(dataPin, clockPin, LSBFIRST, Seg1[c_1]);
  digitalWrite(latchPin,HIGH);
}
////////////////////////////////////////////////////////////////////////////////////bang den }
void DoiDiaChiMac(void)
{
  // For Soft Access Point (AP) Mode
  //wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) 
{
  if(Wifi_On == false)
    {
      memcpy(&myData, incomingData, sizeof(myData));
      TinHieu = true;
      noiChuoi = noiChuoi + myData.chuoi;
      //Serial.print(myData.chuoi);
      if(noiChuoi.length() >= 50 )
        {
          while((noiChuoi.charAt(0) == '0') || (noiChuoi.charAt(0) == '1') || (noiChuoi.charAt(0) == '2') || (noiChuoi.charAt(0) == '3') || (noiChuoi.charAt(0) == '4') ||
                (noiChuoi.charAt(0) == '5') || (noiChuoi.charAt(0) == '6') || (noiChuoi.charAt(0) == '7') || (noiChuoi.charAt(0) == '8') || (noiChuoi.charAt(0) == '9'))
            {
              noiChuoi.remove(0,1);
            }
          tempTong = noiChuoi;
          //Serial.println(noiChuoi);
          noiChuoi = "";
        }
    }
}
void XuliChuoi(void)
{
   ///////////////////////////////////////////////////////////////////////// chuoi 4 VT200 cam ung thuong, chuoi tf 0 VT200D-IOT số
   if((tempTong.indexOf(0x02) != -1) && (tempTong.indexOf(0x03) != -1 ))
      {
        while((tempTong.charAt(0) != 0x03) && (tempTong.charAt(1) != 0x02) && (tempTong.length() >= 9))
          {
            tempTong.remove(0,1);
          }
        if(tempTong.length() >= 9)
          {
            tempTong = tempTong.substring(0,9);
            if(tempTong.charAt(2)== '-')
              {
                if((tempTong.charAt(3)!= 0x02) && (tempTong.charAt(4)!= 0x02)  && (tempTong.charAt(5)!= 0x02) && (tempTong.charAt(6)!= 0x02) && (tempTong.charAt(7)!= 0x02) && (tempTong.charAt(8)!= 0x02)
                && (tempTong.charAt(3)!= 0x03) && (tempTong.charAt(4)!= 0x03)  && (tempTong.charAt(5)!= 0x03) && (tempTong.charAt(6)!= 0x03) && (tempTong.charAt(7)!= 0x03) && (tempTong.charAt(8)!= 0x03)
                && (tempTong.charAt(3)!= '+') && (tempTong.charAt(4)!= '+')  && (tempTong.charAt(5)!= '+') && (tempTong.charAt(6)!= '+') && (tempTong.charAt(7)!= '+') && (tempTong.charAt(8)!= '+')
                && (tempTong.charAt(3)!= '-') && (tempTong.charAt(4)!= '-')  && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-') && (tempTong.charAt(7)!= '-') && (tempTong.charAt(8)!= '-')
                && (tempTong.charAt(3)!= ';') && (tempTong.charAt(4)!= ';')  && (tempTong.charAt(5)!= ';') && (tempTong.charAt(6)!= ';') && (tempTong.charAt(7)!= ';') && (tempTong.charAt(8)!= ';')
                && (tempTong.charAt(3)!= '>') && (tempTong.charAt(4)!= '>')  && (tempTong.charAt(5)!= '>') && (tempTong.charAt(6)!= '>') && (tempTong.charAt(7)!= '>') && (tempTong.charAt(8)!= '>'))
                  {
                    DauAm = true;
                    tempTong = tempTong.substring(3,9);
                    temp1 = tempTong;
                    //Serial.println(tempTong);
                    temp1.trim(); // loai bo khoan trang
                    HamChuyenSangSoNguyen();
                  }
              }
            else
              {
                if((tempTong.charAt(3)!= 0x02) && (tempTong.charAt(4)!= 0x02)  && (tempTong.charAt(5)!= 0x02) && (tempTong.charAt(6)!= 0x02) && (tempTong.charAt(7)!= 0x02) && (tempTong.charAt(8)!= 0x02)
                && (tempTong.charAt(3)!= 0x03) && (tempTong.charAt(4)!= 0x03)  && (tempTong.charAt(5)!= 0x03) && (tempTong.charAt(6)!= 0x03) && (tempTong.charAt(7)!= 0x03) && (tempTong.charAt(8)!= 0x03)
                && (tempTong.charAt(3)!= '+') && (tempTong.charAt(4)!= '+')  && (tempTong.charAt(5)!= '+') && (tempTong.charAt(6)!= '+') && (tempTong.charAt(7)!= '+') && (tempTong.charAt(8)!= '+')
                && (tempTong.charAt(3)!= '-') && (tempTong.charAt(4)!= '-')  && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-') && (tempTong.charAt(7)!= '-') && (tempTong.charAt(8)!= '-')
                && (tempTong.charAt(3)!= ';') && (tempTong.charAt(4)!= ';')  && (tempTong.charAt(5)!= ';') && (tempTong.charAt(6)!= ';') && (tempTong.charAt(7)!= ';') && (tempTong.charAt(8)!= ';')
                && (tempTong.charAt(3)!= '>') && (tempTong.charAt(4)!= '>')  && (tempTong.charAt(5)!= '>') && (tempTong.charAt(6)!= '>') && (tempTong.charAt(7)!= '>') && (tempTong.charAt(8)!= '>'))
                  {
                    DauAm = false;
                    tempTong = tempTong.substring(3,9);
                    temp1 = tempTong;
                    //Serial.println(tempTong);
                    temp1.trim(); // loai bo khoan trang
                    HamChuyenSangSoNguyen();
                  }
              }
          }
      }
    ///////////////////////////////////////////////////////////////////////// chuoi x VT200 dau sat giong tan quoc hung 0/11/2024
    else if((tempTong.indexOf(0x02) != -1) && (tempTong.indexOf('K') != -1 ) && (tempTong.indexOf('G') != -1 ))
      {
        while((tempTong.charAt(0) != 0x02) && (tempTong.length() >= 12))
            {
              tempTong.remove(0,1);
            }
        if(tempTong.length() >= 12)
          {
            tempTong = tempTong.substring(0,12);
            //Serial.println(tempTong);
            if(tempTong.charAt(1)== '-')
                {
                  if((tempTong.charAt(3)!= 0x02)  && (tempTong.charAt(4)!= 0x02) && (tempTong.charAt(5)!= 0x02) && (tempTong.charAt(6)!= 0x02) && (tempTong.charAt(7)!= 0x02) && (tempTong.charAt(8)!= 0x02) &&
                     (tempTong.charAt(3)!= 'K')  && (tempTong.charAt(4)!= 'K') && (tempTong.charAt(5)!= 'K') && (tempTong.charAt(6)!= 'K') && (tempTong.charAt(7)!= 'K') && (tempTong.charAt(8)!= 'K') &&
                     (tempTong.charAt(3)!= 'G')  && (tempTong.charAt(4)!= 'G') && (tempTong.charAt(5)!= 'G') && (tempTong.charAt(6)!= 'G') && (tempTong.charAt(7)!= 'G') && (tempTong.charAt(8)!= 'G') &&
                     (tempTong.charAt(3)!= 'M')  && (tempTong.charAt(4)!= 'M') && (tempTong.charAt(5)!= 'M') && (tempTong.charAt(6)!= 'M') && (tempTong.charAt(7)!= 'M') && (tempTong.charAt(8)!= 'M'))
                    {
                      DauAm = true;
                      tempTong = tempTong.substring(3,9);
                      temp1 = tempTong;
                      //Serial.println(tempTong);
                      temp1.trim(); // loai bo khoan trang
                      HamChuyenSangSoNguyen();
                    }
                }
            else
              {
                  if((tempTong.charAt(3)!= 0x02)  && (tempTong.charAt(4)!= 0x02) && (tempTong.charAt(5)!= 0x02) && (tempTong.charAt(6)!= 0x02) && (tempTong.charAt(7)!= 0x02) && (tempTong.charAt(8)!= 0x02) &&
                     (tempTong.charAt(3)!= 'K')  && (tempTong.charAt(4)!= 'K') && (tempTong.charAt(5)!= 'K') && (tempTong.charAt(6)!= 'K') && (tempTong.charAt(7)!= 'K') && (tempTong.charAt(8)!= 'K') &&
                     (tempTong.charAt(3)!= 'G')  && (tempTong.charAt(4)!= 'G') && (tempTong.charAt(5)!= 'G') && (tempTong.charAt(6)!= 'G') && (tempTong.charAt(7)!= 'G') && (tempTong.charAt(8)!= 'G') &&
                     (tempTong.charAt(3)!= 'M')  && (tempTong.charAt(4)!= 'M') && (tempTong.charAt(5)!= 'M') && (tempTong.charAt(6)!= 'M') && (tempTong.charAt(7)!= 'M') && (tempTong.charAt(8)!= 'M'))
                    {
                      DauAm = false;
                      tempTong = tempTong.substring(3,9);
                      temp1 = tempTong;
                      //Serial.println(tempTong);
                      temp1.trim(); // loai bo khoan trang
                      HamChuyenSangSoNguyen();
                    }
              }
          }
      }
    ///////////////////////////////////////////////////////////////////////// chuoi 3 VT200 cam ung thuong
    else if((tempTong.indexOf('=') != -1) && (tempTong.indexOf('.') != -1 ) && (tempTong.indexOf('(') != -1 ) && 
            (tempTong.indexOf('k') != -1) && (tempTong.indexOf('g') != -1 ) && (tempTong.indexOf(')') != -1 ))
      {
        while((tempTong.charAt(0) != '=') && (tempTong.length() >= 12))
            {
              tempTong.remove(0,1);
            }
        if(tempTong.length() >= 12)
          {
            tempTong = tempTong.substring(0,12);
            if(tempTong.charAt(1)== '-')
                {
                  if((tempTong.charAt(2)!= '=') && (tempTong.charAt(3)!= '=')  && (tempTong.charAt(4)!= '=') && (tempTong.charAt(5)!= '=') && (tempTong.charAt(6)!= '=') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '-') && (tempTong.charAt(3)!= '-')  && (tempTong.charAt(4)!= '-') && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '.') && (tempTong.charAt(3)!= '.')  && (tempTong.charAt(4)!= '.') && (tempTong.charAt(5)!= '.') && (tempTong.charAt(6)!= '.') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '(') && (tempTong.charAt(3)!= '(')  && (tempTong.charAt(4)!= '(') && (tempTong.charAt(5)!= '(') && (tempTong.charAt(6)!= '(') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'k') && (tempTong.charAt(3)!= 'k')  && (tempTong.charAt(4)!= 'k') && (tempTong.charAt(5)!= 'k') && (tempTong.charAt(6)!= 'k') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'g') && (tempTong.charAt(3)!= 'g')  && (tempTong.charAt(4)!= 'g') && (tempTong.charAt(5)!= 'g') && (tempTong.charAt(6)!= 'g') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= ')') && (tempTong.charAt(3)!= ')')  && (tempTong.charAt(4)!= ')') && (tempTong.charAt(5)!= ')') && (tempTong.charAt(6)!= ')') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')'))
                    {
                      DauAm = true;
                      tempTong = tempTong.substring(2,7);
                      temp1 = tempTong;
                      //Serial.println(tempTong);
                      temp1.trim(); // loai bo khoan trang
                      HamChuyenSangSoNguyen();
                    }
                }
            else
              {
                  if((tempTong.charAt(2)!= '=') && (tempTong.charAt(3)!= '=')  && (tempTong.charAt(4)!= '=') && (tempTong.charAt(5)!= '=') && (tempTong.charAt(6)!= '=') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '-') && (tempTong.charAt(3)!= '-')  && (tempTong.charAt(4)!= '-') && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '.') && (tempTong.charAt(3)!= '.')  && (tempTong.charAt(4)!= '.') && (tempTong.charAt(5)!= '.') && (tempTong.charAt(6)!= '.') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '(') && (tempTong.charAt(3)!= '(')  && (tempTong.charAt(4)!= '(') && (tempTong.charAt(5)!= '(') && (tempTong.charAt(6)!= '(') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'k') && (tempTong.charAt(3)!= 'k')  && (tempTong.charAt(4)!= 'k') && (tempTong.charAt(5)!= 'k') && (tempTong.charAt(6)!= 'k') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'g') && (tempTong.charAt(3)!= 'g')  && (tempTong.charAt(4)!= 'g') && (tempTong.charAt(5)!= 'g') && (tempTong.charAt(6)!= 'g') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= ')') && (tempTong.charAt(3)!= ')')  && (tempTong.charAt(4)!= ')') && (tempTong.charAt(5)!= ')') && (tempTong.charAt(6)!= ')') && (tempTong.charAt(7)== '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')'))
                    {
                      DauAm = false;
                      tempTong = tempTong.substring(1,7);
                      temp1 = tempTong;
                      //Serial.println(tempTong);
                      temp1.trim(); // loai bo khoan trang
                      HamChuyenSangSoNguyen();
                    }
                }
          }
      }
    ///////////////////////////////////////////////////////////////////////// chuoi 6 VT200 cam ung thuong
    else if((tempTong.indexOf('S') != -1) && (tempTong.indexOf('T') != -1 ) && (tempTong.indexOf('G') != -1 ) && (tempTong.indexOf(',') != -1 ) &&
            (tempTong.indexOf('k') != -1) && (tempTong.indexOf('g') != -1 ) && (tempTong.indexOf('.') != -1 ))
      {
        while((tempTong.charAt(0) != 'S') && (tempTong.charAt(1) != 'T') && (tempTong.charAt(2) != ',') && (tempTong.charAt(3) != 'G') && 
              (tempTong.charAt(4) != 'S') && (tempTong.charAt(5) != ',') && (tempTong.length() >= 16))
          {
            tempTong.remove(0,1);
          }
        if((tempTong.length() >= 16) && (tempTong.charAt(13) == '.') && (tempTong.charAt(14) == 'k') && (tempTong.charAt(15) == 'g'))
          {
            tempTong = tempTong.substring(6,13);
            if(tempTong.charAt(0) == '-')
              {
                if((tempTong.charAt(1)!= 'G') && (tempTong.charAt(2)!= 'G')  && (tempTong.charAt(3)!= 'G') && (tempTong.charAt(4)!= 'G') && (tempTong.charAt(5)!= 'G') && (tempTong.charAt(6)!= 'G')
                && (tempTong.charAt(1)!= 'T') && (tempTong.charAt(2)!= 'T')  && (tempTong.charAt(3)!= 'T') && (tempTong.charAt(4)!= 'T') && (tempTong.charAt(5)!= 'T') && (tempTong.charAt(6)!= 'T')
                && (tempTong.charAt(1)!= 'S') && (tempTong.charAt(2)!= 'S')  && (tempTong.charAt(3)!= 'S') && (tempTong.charAt(4)!= 'S') && (tempTong.charAt(5)!= 'S') && (tempTong.charAt(6)!= 'S')
                && (tempTong.charAt(1)!= 'k') && (tempTong.charAt(2)!= 'k')  && (tempTong.charAt(3)!= 'k') && (tempTong.charAt(4)!= 'k') && (tempTong.charAt(5)!= 'k') && (tempTong.charAt(6)!= 'k')
                && (tempTong.charAt(1)!= 'g') && (tempTong.charAt(2)!= 'g')  && (tempTong.charAt(3)!= 'g') && (tempTong.charAt(4)!= 'g') && (tempTong.charAt(5)!= 'g') && (tempTong.charAt(6)!= 'g')
                && (tempTong.charAt(1)!= ',') && (tempTong.charAt(2)!= ',')  && (tempTong.charAt(3)!= ',') && (tempTong.charAt(4)!= ',') && (tempTong.charAt(5)!= ',') && (tempTong.charAt(6)!= ',')
                && (tempTong.charAt(1)!= '.') && (tempTong.charAt(2)!= '.')  && (tempTong.charAt(3)!= '.') && (tempTong.charAt(4)!= '.') && (tempTong.charAt(5)!= '.') && (tempTong.charAt(6)!= '.')
                && (tempTong.charAt(1)!= '+') && (tempTong.charAt(2)!= '+')  && (tempTong.charAt(3)!= '+') && (tempTong.charAt(4)!= '+') && (tempTong.charAt(5)!= '+') && (tempTong.charAt(6)!= '+'))
                  {
                    DauAm = true;
                    tempTong = tempTong.substring(1,7);
                    temp1 = tempTong;
                    //Serial.println(tempTong);
                    temp1.trim(); // loai bo khoan trang
                    HamChuyenSangSoNguyen();
                  }
              }
            else
              {
                if((tempTong.charAt(1)!= 'G') && (tempTong.charAt(2)!= 'G')  && (tempTong.charAt(3)!= 'G') && (tempTong.charAt(4)!= 'G') && (tempTong.charAt(5)!= 'G') && (tempTong.charAt(6)!= 'G')
                && (tempTong.charAt(1)!= 'T') && (tempTong.charAt(2)!= 'T')  && (tempTong.charAt(3)!= 'T') && (tempTong.charAt(4)!= 'T') && (tempTong.charAt(5)!= 'T') && (tempTong.charAt(6)!= 'T')
                && (tempTong.charAt(1)!= 'S') && (tempTong.charAt(2)!= 'S')  && (tempTong.charAt(3)!= 'S') && (tempTong.charAt(4)!= 'S') && (tempTong.charAt(5)!= 'S') && (tempTong.charAt(6)!= 'S')
                && (tempTong.charAt(1)!= 'k') && (tempTong.charAt(2)!= 'k')  && (tempTong.charAt(3)!= 'k') && (tempTong.charAt(4)!= 'k') && (tempTong.charAt(5)!= 'k') && (tempTong.charAt(6)!= 'k')
                && (tempTong.charAt(1)!= 'g') && (tempTong.charAt(2)!= 'g')  && (tempTong.charAt(3)!= 'g') && (tempTong.charAt(4)!= 'g') && (tempTong.charAt(5)!= 'g') && (tempTong.charAt(6)!= 'g')
                && (tempTong.charAt(1)!= ',') && (tempTong.charAt(2)!= ',')  && (tempTong.charAt(3)!= ',') && (tempTong.charAt(4)!= ',') && (tempTong.charAt(5)!= ',') && (tempTong.charAt(6)!= ',')
                && (tempTong.charAt(1)!= '.') && (tempTong.charAt(2)!= '.')  && (tempTong.charAt(3)!= '.') && (tempTong.charAt(4)!= '.') && (tempTong.charAt(5)!= '.') && (tempTong.charAt(6)!= '.')
                && (tempTong.charAt(1)!= '-') && (tempTong.charAt(2)!= '-')  && (tempTong.charAt(3)!= '-') && (tempTong.charAt(4)!= '-') && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-'))
                  {
                    DauAm = false;
                    tempTong = tempTong.substring(1,7);
                    temp1 = tempTong;
                    //Serial.println(tempTong);
                    temp1.trim(); // loai bo khoan trang
                    HamChuyenSangSoNguyen();
                  }
              }              
          }
      }
    ///////////////////////////////////////////////////////////////////////// chuoi 3,4 VDI01
    else if((tempTong.indexOf('=') != -1) && (tempTong.indexOf('(') != -1) && (tempTong.indexOf('k') != -1) && 
            (tempTong.indexOf('g') != -1) && (tempTong.indexOf(')') != -1) && (tempTong.indexOf('.') == -1))
      {
        while((tempTong.charAt(0) != '=') && (tempTong.length() >= 13) && (tempTong.charAt(8) != '(') && (tempTong.charAt(9) != 'k') && (tempTong.charAt(10) != 'g') && (tempTong.charAt(11) != ')'))
            {
              tempTong.remove(0,1);
            }
        if(tempTong.length() >= 13)
          {
            tempTong = tempTong.substring(0,13);
            if(tempTong.charAt(1)== '-')
                {
                  if((tempTong.charAt(2)!= '=') && (tempTong.charAt(3)!= '=')  && (tempTong.charAt(4)!= '=') && (tempTong.charAt(5)!= '=') && (tempTong.charAt(6)!= '=') && (tempTong.charAt(7)!= '=') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '-') && (tempTong.charAt(3)!= '-')  && (tempTong.charAt(4)!= '-') && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-') && (tempTong.charAt(7)!= '-') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '.') && (tempTong.charAt(3)!= '.')  && (tempTong.charAt(4)!= '.') && (tempTong.charAt(5)!= '.') && (tempTong.charAt(6)!= '.') && (tempTong.charAt(7)!= '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '(') && (tempTong.charAt(3)!= '(')  && (tempTong.charAt(4)!= '(') && (tempTong.charAt(5)!= '(') && (tempTong.charAt(6)!= '(') && (tempTong.charAt(7)!= '(') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'k') && (tempTong.charAt(3)!= 'k')  && (tempTong.charAt(4)!= 'k') && (tempTong.charAt(5)!= 'k') && (tempTong.charAt(6)!= 'k') && (tempTong.charAt(7)!= 'k') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'g') && (tempTong.charAt(3)!= 'g')  && (tempTong.charAt(4)!= 'g') && (tempTong.charAt(5)!= 'g') && (tempTong.charAt(6)!= 'g') && (tempTong.charAt(7)!= 'g') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= ')') && (tempTong.charAt(3)!= ')')  && (tempTong.charAt(4)!= ')') && (tempTong.charAt(5)!= ')') && (tempTong.charAt(6)!= ')') && (tempTong.charAt(7)!= ')') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')'))
                    {
                      DauAm = true;
                      tempTong = tempTong.substring(2,8);
                      temp1 = tempTong;
                      //Serial.println(tempTong);
                      temp1.trim(); // loai bo khoan trang
                      HamChuyenSangSoNguyen();
                    }
                }
            else
              {
                  if((tempTong.charAt(2)!= '=') && (tempTong.charAt(3)!= '=')  && (tempTong.charAt(4)!= '=') && (tempTong.charAt(5)!= '=') && (tempTong.charAt(6)!= '=') && (tempTong.charAt(7)!= '=') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '-') && (tempTong.charAt(3)!= '-')  && (tempTong.charAt(4)!= '-') && (tempTong.charAt(5)!= '-') && (tempTong.charAt(6)!= '-') && (tempTong.charAt(7)!= '-') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '.') && (tempTong.charAt(3)!= '.')  && (tempTong.charAt(4)!= '.') && (tempTong.charAt(5)!= '.') && (tempTong.charAt(6)!= '.') && (tempTong.charAt(7)!= '.') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= '(') && (tempTong.charAt(3)!= '(')  && (tempTong.charAt(4)!= '(') && (tempTong.charAt(5)!= '(') && (tempTong.charAt(6)!= '(') && (tempTong.charAt(7)!= '(') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'k') && (tempTong.charAt(3)!= 'k')  && (tempTong.charAt(4)!= 'k') && (tempTong.charAt(5)!= 'k') && (tempTong.charAt(6)!= 'k') && (tempTong.charAt(7)!= 'k') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= 'g') && (tempTong.charAt(3)!= 'g')  && (tempTong.charAt(4)!= 'g') && (tempTong.charAt(5)!= 'g') && (tempTong.charAt(6)!= 'g') && (tempTong.charAt(7)!= 'g') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')') &&
                     (tempTong.charAt(2)!= ')') && (tempTong.charAt(3)!= ')')  && (tempTong.charAt(4)!= ')') && (tempTong.charAt(5)!= ')') && (tempTong.charAt(6)!= ')') && (tempTong.charAt(7)!= ')') && (tempTong.charAt(8)== '(') && (tempTong.charAt(9)== 'k') && (tempTong.charAt(10)== 'g') && (tempTong.charAt(11)== ')'))
                    {
                      DauAm = false;
                      tempTong = tempTong.substring(2,8);
                      temp1 = tempTong;
                      //Serial.println(tempTong);
                      temp1.trim(); // loai bo khoan trang
                      HamChuyenSangSoNguyen();
                    }
                }
          }
      }
    ///////////////////////////////////////////////////////////////////////// Dau TQH
    else if((tempTong.indexOf('+') != -1) || (tempTong.indexOf('-') != -1))
      {
        if(tempTong.indexOf('+') != -1)
          {
            while((tempTong.charAt(0) != '+') && (tempTong.length() >= 17))
              { 
                tempTong.remove(0,1);
              }
            if((tempTong.length() >= 17) && (tempTong.charAt(0) == '+') && (tempTong.charAt(9) == '+'))
              {
                tempTong = tempTong.substring(1,7);
                if(((tempTong.charAt(0) == '0') || (tempTong.charAt(0) == '1') || (tempTong.charAt(0) == '2') || (tempTong.charAt(0) == '3') || (tempTong.charAt(0) == '4') || (tempTong.charAt(0) == '5') || (tempTong.charAt(0) == '6') || (tempTong.charAt(0) == '7') || (tempTong.charAt(0) == '8') || (tempTong.charAt(0) == '9'))
                && ((tempTong.charAt(1) == '0') || (tempTong.charAt(1) == '1') || (tempTong.charAt(1) == '2') || (tempTong.charAt(1) == '3') || (tempTong.charAt(1) == '4') || (tempTong.charAt(1) == '5') || (tempTong.charAt(1) == '6') || (tempTong.charAt(1) == '7') || (tempTong.charAt(1) == '8') || (tempTong.charAt(1) == '9'))
                && ((tempTong.charAt(2) == '0') || (tempTong.charAt(2) == '1') || (tempTong.charAt(2) == '2') || (tempTong.charAt(2) == '3') || (tempTong.charAt(2) == '4') || (tempTong.charAt(2) == '5') || (tempTong.charAt(2) == '6') || (tempTong.charAt(2) == '7') || (tempTong.charAt(2) == '8') || (tempTong.charAt(2) == '9'))
                && ((tempTong.charAt(3) == '0') || (tempTong.charAt(3) == '1') || (tempTong.charAt(3) == '2') || (tempTong.charAt(3) == '3') || (tempTong.charAt(3) == '4') || (tempTong.charAt(3) == '5') || (tempTong.charAt(3) == '6') || (tempTong.charAt(3) == '7') || (tempTong.charAt(3) == '8') || (tempTong.charAt(3) == '9'))
                && ((tempTong.charAt(4) == '0') || (tempTong.charAt(4) == '1') || (tempTong.charAt(4) == '2') || (tempTong.charAt(4) == '3') || (tempTong.charAt(4) == '4') || (tempTong.charAt(4) == '5') || (tempTong.charAt(4) == '6') || (tempTong.charAt(4) == '7') || (tempTong.charAt(4) == '8') || (tempTong.charAt(4) == '9'))
                && ((tempTong.charAt(5) == '0') || (tempTong.charAt(5) == '1') || (tempTong.charAt(5) == '2') || (tempTong.charAt(5) == '3') || (tempTong.charAt(5) == '4') || (tempTong.charAt(5) == '5') || (tempTong.charAt(5) == '6') || (tempTong.charAt(5) == '7') || (tempTong.charAt(5) == '8') || (tempTong.charAt(5) == '9')))
                  {
                    DauAm = false;
                    temp1 = tempTong;
                    //Serial.println(tempTong);
                    temp1.trim(); // loai bo khoan trang
                    HamChuyenSangSoNguyen();
                  }
              }
          }
        else if(tempTong.indexOf('-') != -1)
          {
            while((tempTong.charAt(0) != '-') && (tempTong.length() >= 17))
              { 
                tempTong.remove(0,1);
              }
            if((tempTong.length() >= 17) && (tempTong.charAt(0) == '-') && (tempTong.charAt(9) == '-'))
              {
                tempTong = tempTong.substring(1,7);
                if(((tempTong.charAt(0) == '0') || (tempTong.charAt(0) == '1') || (tempTong.charAt(0) == '2') || (tempTong.charAt(0) == '3') || (tempTong.charAt(0) == '4') || (tempTong.charAt(0) == '5') || (tempTong.charAt(0) == '6') || (tempTong.charAt(0) == '7') || (tempTong.charAt(0) == '8') || (tempTong.charAt(0) == '9'))
                && ((tempTong.charAt(1) == '0') || (tempTong.charAt(1) == '1') || (tempTong.charAt(1) == '2') || (tempTong.charAt(1) == '3') || (tempTong.charAt(1) == '4') || (tempTong.charAt(1) == '5') || (tempTong.charAt(1) == '6') || (tempTong.charAt(1) == '7') || (tempTong.charAt(1) == '8') || (tempTong.charAt(1) == '9'))
                && ((tempTong.charAt(2) == '0') || (tempTong.charAt(2) == '1') || (tempTong.charAt(2) == '2') || (tempTong.charAt(2) == '3') || (tempTong.charAt(2) == '4') || (tempTong.charAt(2) == '5') || (tempTong.charAt(2) == '6') || (tempTong.charAt(2) == '7') || (tempTong.charAt(2) == '8') || (tempTong.charAt(2) == '9'))
                && ((tempTong.charAt(3) == '0') || (tempTong.charAt(3) == '1') || (tempTong.charAt(3) == '2') || (tempTong.charAt(3) == '3') || (tempTong.charAt(3) == '4') || (tempTong.charAt(3) == '5') || (tempTong.charAt(3) == '6') || (tempTong.charAt(3) == '7') || (tempTong.charAt(3) == '8') || (tempTong.charAt(3) == '9'))
                && ((tempTong.charAt(4) == '0') || (tempTong.charAt(4) == '1') || (tempTong.charAt(4) == '2') || (tempTong.charAt(4) == '3') || (tempTong.charAt(4) == '4') || (tempTong.charAt(4) == '5') || (tempTong.charAt(4) == '6') || (tempTong.charAt(4) == '7') || (tempTong.charAt(4) == '8') || (tempTong.charAt(4) == '9'))
                && ((tempTong.charAt(5) == '0') || (tempTong.charAt(5) == '1') || (tempTong.charAt(5) == '2') || (tempTong.charAt(5) == '3') || (tempTong.charAt(5) == '4') || (tempTong.charAt(5) == '5') || (tempTong.charAt(5) == '6') || (tempTong.charAt(5) == '7') || (tempTong.charAt(5) == '8') || (tempTong.charAt(5) == '9')))
                  {
                    DauAm = true;
                    temp1 = tempTong;
                    //Serial.println(tempTong);
                    temp1.trim(); // loai bo khoan trang
                    HamChuyenSangSoNguyen();
                  }
              }
          }
      }
}
void DocSoCanSerial(void)
{             
     while(Serial.available()> 0)
      {
        Wifi_On = true;
        char buffer[5] = "";
        Serial.readBytes(buffer,3);  
        //Serial.print(buffer); 
        TinHieu = true;
        temp3 = temp3 + buffer;
        if(temp3.length() >= 50 )
          {
            while((temp3.charAt(0) == '0') || (temp3.charAt(0) == '1') || (temp3.charAt(0) == '2') || (temp3.charAt(0) == '3') || (temp3.charAt(0) == '4') ||
                  (temp3.charAt(0) == '5') || (temp3.charAt(0) == '6') || (temp3.charAt(0) == '7') || (temp3.charAt(0) == '8') || (temp3.charAt(0) == '9'))
              {
                temp3.remove(0,1);
              }
            tempTong = temp3;
            //Serial.println(temp3);
            temp3 = "";
          }
      }
}
void setup() 
{
  //Serial.begin(2400);
  EEPROM.begin(512);
  // Khởi tạo Watchdog Timer
  wdtManager.begin(); // chuc nang chong treo VDK neu 2s bị treo se reset VDK
  pinMode(ledBao,OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(BUTTON_Serial,INPUT_PULLUP);
  debouncer1.attach(BUTTON_Serial);
  debouncer1.interval(10);
  HamKhoiTaoSerial();
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  DoiDiaChiMac();
  // Init ESP-NOW
  if (esp_now_init() != 0) 
    {
      //Serial.println("Error initializing ESP-NOW");
      return;
    }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  TestLedTruocKhiChay();
}
void DenTinHieu(void)
{
  static unsigned long time3 = 0UL;
  if((unsigned long)millis() - time3 >= 120000UL)
    {
      time3 = millis();
      TinHieu = false;
    }
  static unsigned int dem = 0;
  if(TinHieu == true)
    {
      static unsigned long time2 = 0UL;
      if((unsigned long)millis() - time2 >= 100UL)
        {
          time2 = millis();
          dem = dem + 1;
        }
      if(dem <= 3)
        {
          digitalWrite(ledBao,LOW);
        }
      else if((dem > 3 )&& (dem <= 7))
        {
          digitalWrite(ledBao,HIGH);
        }
      else if (dem > 7 )
        {
          dem = 0;
        }
    }
  else
    {
      digitalWrite(ledBao,LOW);
      dem = 0;
    }
}
void loop() 
{
  // Đảm bảo Watchdog Timer được reset trong mỗi vòng lặp
  wdtManager.feed(); // chuc nang chong treo VDK neu 2s bị treo se reset VDK
  DocTrangThaiNutNhan();
  HieuChinhSerial();
  DenTinHieu();
  DocSoCanSerial();
  XuliChuoi();
  HamPhanTichInRaLed7Doan();
  static unsigned long time1 = 0UL; 
  if((unsigned long) millis() - time1 >= 10)
    {
      time1 = millis();
      if(TinHieu == true)
        {
          HienThiSo(c1,c2,c3,c4,c5,c6);
          //Serial.println(temp2);
        }
      else
        {
          ChuQH();
        }
    }    
}
