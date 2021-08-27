#include "U8glib.h"
#include <DS3231.h>
#include <Wire.h>

static const uint8_t tonePin = 2;
DS3231 rtc(SDA, SCL);
U8GLIB_PCD8544 u8g(7, 6, 4, 5, 3);  // CLK=7, DIN=6, CE=4, DC=5, RST=3
Time  t;
signed long wdate = 1657737000; // 13.7.21 19:30@ - unix showtime! (Setting time to 1830 Considering local time would be GMT+2 & Italy's GMT+1)

String timeStr;
const uint8_t ramm_bitmap[] PROGMEM = {
  0x00, 0x00, 0x3f, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfc, 0x07, 0xff, 0x00, 0x00, 0x80, 0x00,
  0x04, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00,
  0x01, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x01, 0x00,
  0x00, 0x80, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x04,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80, 0x28, 0x04, 0x00, 0x01,
  0x00, 0x00, 0x80, 0x28, 0x06, 0x00, 0x01, 0x00, 0x00, 0x80, 0x24, 0x01, 0x80, 0x01, 0x00, 0x00,
  0x80, 0x24, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x22, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x22,
  0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x21, 0x00, 0x80, 0x01, 0x00, 0x00, 0xff, 0xe1, 0xff, 0x87,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0xc3, 0xfc, 0x00, 0x00
};

const uint8_t rammtxt_bitmap[] PROGMEM = {
0xf3, 0xce, 0x2e, 0x2f, 0x99, 0xed, 0xc8, 0xf3, 0xcf, 0x2f, 0x2f, 0x99, 0xed, 0xe8, 0xda, 0x6f,
0x6f, 0x68, 0x3d, 0x0d, 0xe8, 0xda, 0x6d, 0xad, 0xa6, 0x3d, 0xed, 0xb8, 0xda, 0x2d, 0xad, 0xa1,
0x99, 0x0d, 0x98, 0xca, 0x2d, 0xad, 0xaf, 0x99, 0xed, 0x98
};

String addZero(int num){  //this adds zeros to the time integers, and turns them to strings before printing.
  if (num < 10){
    String ret = "0" + String(num);
    return ret;
}
return String(num);
}

void draw(void) {
String timeStr = addZero(t.hour) + ":" + addZero(t.min) + ":" + addZero(t.sec);

u8g.drawBitmapP( 15, 0, 7, 6, rammtxt_bitmap);  // I bitmap
u8g.drawBitmapP( 35, 7, 7, 40, ramm_bitmap);  // I bitmap
  u8g.setFont(u8g_font_fixed_v0);  // select a big font
  //u8g.drawStr(0, 16, rtc.getTimeStr());  // put string of display at position X, Y <<<Direct from RTC, new way puts together a string set above:
      u8g.setPrintPos(0, 16);
      u8g.print(timeStr);
  u8g.setFont(u8g_font_u8glib_4);  // select a small font
  u8g.drawStr(0, 25, rtc.getDateStr());
  u8g.setPrintPos(0, 33);
  u8g.print(rtc.getTemp());
  u8g.drawStr(18, 33, " C");
  u8g.setFont(u8g_font_fixed_v0);
  u8g.setPrintPos(0, 44);
  u8g.print((wdate - rtc.getUnixTime(rtc.getTime()))/60/60/24);
  u8g.setFont(u8g_font_u8glib_4);  // select a smaller font
  u8g.drawStr(20, 44, "  Days");
}

void setup(void) {
rtc.begin();
Wire.begin();
pinMode(11, INPUT_PULLUP); //will be defined as "-1" hour setter when grounded

/* this sets time: (Comment out & upload again after setting time)

rtc.setDOW(SUNDAY); // Set Day-of-Week

rtc.setTime(15, 15, 00); // Set the time to 15:15:00 (24hr format)

rtc.setDate(24, 8, 2020); // Day, Month, Year
*/

}

void loop(void) {
t = rtc.getTime();

if (digitalRead(11) == LOW) { //to check if -1 is active? 
  if (t.hour == 0){
    t.hour = 23;
  }
  else{
    t.hour--;
  }
}



/*  // tests all sounds
  for (int i = 0; i < 8; i++) {

   switch (i) {

    case 1:
      duhast ();
      break;

    case 2:
      deutschland (); 
      break;
    
    case 3:
      engel ();
      break;

    case 4:
      dasmodel ();
      break;

    case 5:
      Haifisch();
      break;

    case 6:
      ichwill ();
      break;

    case 7:
      hertz ();
      break;
  }

    delay(1000);

}
*/

if (((t.hour) == 19) && ((t.min) == 30) && ((t.sec) == 0)){
  delay(1000);
  switch (t.dow) {

    case 1:
      duhast ();
      break;

    case 2:
      deutschland (); 
      break;
    
    case 3:
      engel ();
      break;

    case 4:
      dasmodel ();
      break;

    case 5:
      Haifisch();
      break;

    case 6:
      ichwill ();
      break;

    case 7:
      hertz ();
      break;
  }
}

  u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() );
  delay(200);  // Delay of 5sec before accessing DHT11 (min - 2sec)
}

 

 
void deutschland() {

    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 261, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 195, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 261, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 195, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 261, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 195, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 293, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 96.3541666667);
    delay(96.3541666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 220, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 146, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 233, 104.166666667);
    delay(104.166666667);
    tone(tonePin, 195, 104.166666667);
    delay(104.166666667);

}
 
void hertz() {

    tone(tonePin, 349, 120.0);
    delay(120.0);
    tone(tonePin, 329, 120.0);
    delay(120.0);
    tone(tonePin, 293, 120.0);
    delay(120.0);
    delay(720.0);
    tone(tonePin, 554, 120.0);
    delay(120.0);
    tone(tonePin, 293, 120.0);
    delay(120.0);
    tone(tonePin, 659, 120.0);
    delay(120.0);
    tone(tonePin, 349, 120.0);
    delay(120.0);
    delay(360.0);
    tone(tonePin, 391, 120.0);
    delay(120.0);
    tone(tonePin, 349, 120.0);
    delay(120.0);
    tone(tonePin, 329, 120.0);
    delay(120.0);
    delay(720.0);
    tone(tonePin, 440, 120.0);
    delay(120.0);
    tone(tonePin, 391, 120.0);
    delay(120.0);
    tone(tonePin, 349, 120.0);
    delay(120.0);
    tone(tonePin, 329, 120.0);
    delay(120.0);
    delay(120.0);
    tone(tonePin, 554, 120.0);
    delay(120.0);
    tone(tonePin, 698, 120.0);
    delay(120.0);
    tone(tonePin, 659, 120.0);
    delay(120.0);
    tone(tonePin, 587, 120.0);
    delay(120.0);
    delay(720.0);
    tone(tonePin, 277, 120.0);
    delay(120.0);
    tone(tonePin, 587, 120.0);
    delay(120.0);
    tone(tonePin, 329, 120.0);
    delay(120.0);
    tone(tonePin, 698, 120.0);
    delay(120.0);
    delay(360.0);
    tone(tonePin, 783, 120.0);
    delay(120.0);
    tone(tonePin, 698, 120.0);
    delay(120.0);
    tone(tonePin, 659, 120.0);
    delay(120.0);
    delay(720.0);
    tone(tonePin, 880, 120.0);
    delay(120.0);
    tone(tonePin, 783, 120.0);
    delay(120.0);
    tone(tonePin, 698, 120.0);
    delay(120.0);
    tone(tonePin, 659, 120.0);
    delay(120.0);
    delay(120.0);
    tone(tonePin, 277, 120.0);
    delay(120.0);
}

 

void ichwill() {

    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    delay(480.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 698, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 659, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    delay(480.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 587, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
    tone(tonePin, 293, 240.0);
    delay(240.0);
}

 

 

void Haifisch() {

    tone(tonePin, 164, 160.0);
    delay(160.0);
    tone(tonePin, 195, 320.0);
    delay(320.0);
    tone(tonePin, 164, 160.0);
    delay(160.0);
    tone(tonePin, 195, 320.0);
    delay(320.0);
    tone(tonePin, 164, 160.0);
    delay(160.0);
    delay(320.0);
    tone(tonePin, 164, 160.0);
    delay(160.0);
    tone(tonePin, 329, 480.0);
    delay(480.0);
    tone(tonePin, 293, 320.0);
    delay(320.0);
    tone(tonePin, 329, 320.0);
    delay(320.0);
    delay(160.0);
    tone(tonePin, 164, 160.0);
    delay(160.0);
}

 

void dasmodel() {

    tone(tonePin, 1174, 188.0);
    delay(188.0);
    tone(tonePin, 932, 168.0);
    delay(168.0);
    delay(48.0);
    tone(tonePin, 783, 172.0);
    delay(172.0);
    delay(20.0);
    tone(tonePin, 932, 192.0);
    delay(192.0);
    delay(8.0);
    tone(tonePin, 1174, 168.0);
    delay(168.0);
    delay(36.0);
    tone(tonePin, 932, 176.0);
    delay(176.0);
    delay(12.0);
    tone(tonePin, 1174, 120.0);
    delay(120.0);
    delay(76.0);
    tone(tonePin, 1244, 116.0);
    delay(116.0);
    delay(84.0);
    tone(tonePin, 1174, 156.0);
    delay(156.0);
    delay(36.0);
    tone(tonePin, 932, 152.0);
    delay(152.0);
    delay(40.0);
    tone(tonePin, 698, 184.0);
    delay(184.0);
    delay(8.0);
    tone(tonePin, 880, 204.0);
    delay(204.0);
    delay(8.0);
    tone(tonePin, 1174, 336.0);
    delay(336.0);
    delay(32.0);
    delay(372.0);
    tone(tonePin, 1046, 4.0);
    delay(4.0);
    tone(tonePin, 1174, 192.0);
    delay(192.0);
    delay(12.0);
    tone(tonePin, 932, 172.0);
    delay(172.0);
    delay(32.0);
    delay(176.0);
    tone(tonePin, 783, 4.0);
    delay(4.0);
    tone(tonePin, 932, 180.0);
    delay(180.0);
    delay(12.0);
    tone(tonePin, 1174, 152.0);
    delay(152.0);
    delay(28.0);
    tone(tonePin, 932, 172.0);
    delay(172.0);
    delay(16.0);
    tone(tonePin, 1174, 116.0);
    delay(116.0);
    delay(76.0);
    tone(tonePin, 1244, 124.0);
    delay(124.0);
    delay(68.0);
    tone(tonePin, 1174, 176.0);
    delay(176.0);
    delay(28.0);
    tone(tonePin, 932, 128.0);
    delay(128.0);
    delay(48.0);
    tone(tonePin, 698, 184.0);
    delay(184.0);
    delay(4.0);
    tone(tonePin, 880, 192.0);
    delay(192.0);
    delay(16.0);
    tone(tonePin, 1174, 624.0);
    delay(624.0);
}

 

void engel() {

    tone(tonePin, 2349, 1026.0);
    delay(1026.0);
    delay(7.0);
    tone(tonePin, 1760, 377.0);
    delay(377.0);
    delay(7.0);
    tone(tonePin, 2093, 1145.0);
    delay(1145.0);
    delay(7.0);
    tone(tonePin, 1760, 377.0);
    delay(377.0);
    delay(7.0);
    tone(tonePin, 1975, 1145.0);
    delay(1145.0);
    delay(7.0);
    tone(tonePin, 1567, 377.0);
    delay(377.0);
    delay(7.0);
    tone(tonePin, 1760, 953.0);
    delay(953.0);
    delay(800.0);
    tone(tonePin, 2349, 1026.0);
    delay(1026.0);
    delay(7.0);
    tone(tonePin, 1760, 377.0);
    delay(377.0);
    delay(7.0);
    tone(tonePin, 2093, 1145.0);
    delay(1145.0);
    delay(7.0);
    tone(tonePin, 1760, 377.0);
    delay(377.0);
    delay(7.0);
    tone(tonePin, 1975, 1145.0);
    delay(1145.0);
    delay(7.0);
    tone(tonePin, 1567, 377.0);
    delay(377.0);
    delay(7.0);
    tone(tonePin, 1174, 1913.0);
    delay(1913.0);
}

void duhast() {

    tone(tonePin, 195, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 195, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 195, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 195, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 120.0);
    delay(120.0);
    tone(tonePin, 164, 240.0);
    delay(240.0);
    tone(tonePin, 195, 240.0);
    delay(240.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 220, 120.0);
    delay(120.0);
    tone(tonePin, 220, 120.0);
    delay(120.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
    tone(tonePin, 220, 120.0);
    delay(120.0);
    tone(tonePin, 220, 120.0);
    delay(120.0);
    tone(tonePin, 220, 240.0);
    delay(240.0);
}
