/*
  board wemos d1 esp32
  (board like arduino uno but with esp32)
  this board choice arduino ide is not really correct but works
  info
  https://www.youtube.com/watch?v=r2UAmBLBBRM source form the swiss guy
  https://www.youtube.com/watch?v=r2UAmBLBBRM
  settimeofday set internal system clock what a pain in the ass to understand
  https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/time.h.html
  https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-time.c  info configtime
   Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format 
  // formating options  http://www.cplusplus.com/reference/ctime/strftime/  
  remarks
  --------
  check code for restart ESP.restart every 12 hours works
  test example swiss guy 24h clock 2min behind ? did not update ntptime cal
  not with getlocaltime_r no update NTP example swiss guy not working
  configTzTime(esp32) or configtime with extra settings tszet do ntp update
  searching how to set time nightmare
  time.h Time.h conflicts Time_lib.h gave conflicts and when you mix push you to nothing working
  used esp32time
  strange thing
  cycletime 5 s before update and after update 1ms
  configTzTime feature esp32 only works one line to update via NTP great !!!!!
  summer time and wintertime set also
  getting weeknumber did not work 
   Serial.println(rtc.getTime(%W)); does not work
  searched code for weekdate
  after restart and no wifi epoch 0 1970 normal
  no restart or loss of power no wifi ok no wifi does not block clock view
  rtc is no real rtc power off and no wifi =  epochtime 0 1970
  stop restart wifi good for now clock keeps running
*/


#include <WiFi.h>
#include <ESP32Time.h>
#include <elapsedMillis.h>


#define blinkled 2

//wifi
const char* ssid = "your ssid";
const char* password = "your password";
//server time
ESP32Time rtc;
const char* NTP_SERVER = "0.be.pool.ntp.org";
const char* NTP_SERVER2 ="1.be.pool.ntp.org";
const char* NTP_SERVER3 ="2.be.pool.ntp.org";
const char* TZ_INFO    = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  
// enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

//refresh
int heartbeatinterval = 1000;
elapsedMillis heartbeat;
elapsedMillis show;
elapsedMillis cycletime;
long unsigned showinterval = 20000;
int updateinterval = 5; //nmbr of minutes ntp request
int lastupdate = 0;
//esp restart
int lasthour ;
int triggerrestart = 12; //
int hourcounter = 0;
tm timeinfo;
//messages for matrix
#define MAX_MESG  20
char szTime[9];    // mm:ss\0
char szMesg[MAX_MESG+1] = "";

void setup() 
{
  Serial.begin(115200);
  getNTPtime() ;
  timeinfo = rtc.getTimeStruct();
  //cheat check update works
  //rtc.setTime(0,10, 0, 1, 1, 1970);
  lastupdate = rtc.getMinute();
  //blink output
  pinMode(blinkled, OUTPUT);
   Serial.println("------end setup--------");
  delay(200);
}

void loop() {
  cycletime = 0;
  if (show > showinterval)
    { Serial.println("-timerefresh-");
      Serial.println(rtc.getDateTime()); 
      weeknmbr();//ouputs szMesg
      Serial.println(szMesg);
     //Serial.println(ESP.getFreeHeap());//how much memory free
     show = 0;
    }
  check(1,cycletime);
  if ((lastupdate + updateinterval) < rtc.getMinute())
   { getNTPtime();
     lastupdate = rtc.getMinute();
   }
   if (rtc.getMinute() == 0)
    {lastupdate = 0;}

  check(2,cycletime);
  f_heartbeat();
  if (cycletime > 20)
  {Serial.print("cycletime:");
   Serial.println(cycletime);   
  }            
}

//sub
void check(int step, int i_timer)
{
  if (i_timer > 20)
    { Serial.print("Check_cycletime");
      Serial.print(step);
      Serial.print(":");
      Serial.println(i_timer); 
    }
}

int ymdToWeekNumber (int y, int m, int d)
 {
  // reject out-of-range input
  if ((y < 1583)||(y > 25000)) return 0;
  if ((m < 1)||(m > 12)) return 0;
  if ((d < 1)||(d > 31)) return 0;
  // compute correction for year
  //     If Jan. 1 falls on: Mo Tu We Th Fr Sa Su
  // then the correction is:  0 +1 +2 +3 -3 -2 -1
  int corr = ((y + 2 + (y-1)/4 - (y-1)/100 + (y-1)/400) % 7) - 3;
  // compute day of the year (in range 1-366)
  int doy = d;
  if (m > 1) doy += 31;
  if (m > 2) doy += (((y%100) ? (y%4) : (y%400)) ? 28 : 29);
  if (m > 3) doy += 31;
  if (m > 4) doy += 30;
  if (m > 5) doy += 31;
  if (m > 6) doy += 30;
  if (m > 7) doy += 31;
  if (m > 8) doy += 31;
  if (m > 9) doy += 30;
  if (m > 10) doy += 31;
  if (m > 11) doy += 30;
  // compute corrected day number
  int cdn = corr + doy;
  // compute number of weeks, rounding up to nearest whole week
  // this gives us our week number
  int wknum = ((cdn + 6) / 7);
  // check for boundary conditions:
  // if our calculation would give us "week 0",
  // then it is really the final week of the previous year
  // and we need to find out whether that is week 52 or week 53
  if (wknum < 1) {
    // first, compute correction for the previous year
    corr = ((y + 1 + (y-2)/4 - (y-2)/100 + (y-2)/400) % 7) - 3;
    // then, compute day of year with respect to that same previous year
    doy = d + ((((y-1)%100) ? ((y-1)%4) : ((y-1)%400)) ? 365 : 366);
    // finally, re-compute the corrected day number and the week number
    cdn = corr + doy;
    wknum = ((cdn + 6) / 7);
    return wknum;
  }
  // if our calculation would give us "week 53",
  // we need to find out whether week 53 really exists,
  // or whether it is week 1 of the following year
  if (wknum > 52) {
    // check for year beginning on Thurs.
    if (corr==3) return 53;
    // check for leap year beginning on Wed.
    if (((y%4)==0) && (corr==2)) return 53;
    // otherwise, there is no week 53
    return 1;
  }
  return wknum;
}
void weeknmbr()
{ int year = rtc.getYear();
  int month = rtc.getMonth() + 1;// verdomme is van 0 tot 11
  int day = rtc.getDay();
  int week = ymdToWeekNumber(year,month,day);
  //mon 1 sunday 0
  int dayofweek = rtc.getDayofWeek();
  if (dayofweek == 0)
   {dayofweek = 7;}
  year = year -2000;
   sprintf(szMesg, "%dW%dD%d",year, week,dayofweek);
  
}


//restart esp condition
  // if ( lasthour != rtc.getHour()) 
  //  {   lasthour = rtc.getHour();
  //      ++hourcounter;
  //      Serial.print("Hourcounter:");
  //      Serial.println(hourcounter);
  //      if (hourcounter >= triggerrestart)
  //         {ESP.restart();}
  //  }


