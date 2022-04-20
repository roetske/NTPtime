# NTPtime
 NTPtime get time from internet.
 So much code around. Want to update from internet and then close connection Wifi. Update at choosen intervals.
 Found shorter code to update NTP. Just one line yes. Thanks to opensource code contributions of many available on the net.
 Time was an issue get systemclock. time.h Time.h Time_lib.h ok started digging and got more and more confused.
 Returned to ESP32time.h. to get time for esp32.
 And than started trying and checking.
 Important and nice settings posix time. Automatic adjustment time of your country. Includes offset GMT summer-wintertime.
 This is maincode updating. 
 
 
#Remark
Getting and setting systemclock on esp32 via arduino ide. Was trial and error not clear at times. configtime or configTztime (update clearly) but the next step retrieving clock info  from update was trial and error for me to get it right. Info or correction to this code are always welcome.
