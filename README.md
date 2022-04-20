# NTPtime
 NTPtime get time from internet.
 So much code around. Want to update from internet and then close connection Wifi. Update NTP at choosen intervals.
 Found shorter code to update NTP. Just one line yes. Thanks to opensource code contributions of many available on the net.
 Time was an issue get systemclock. time.h Time.h Time_lib.h ok started digging and got more and more confused.
 Returned to ESP32time.h. to get time for esp32.
 And than started trying and checking.
 Important and nice settings posix time. Automatic adjustment time of your country. Includes offset GMT summer-wintertime.
 Tested the code I posted extensive works :).
 
 rtc yes. But poweroff and no wifi connection time = 0 time 1970. It is no rtc when no Wifi => cannot reload real time from net.
 However no wifi and no poweroff clock keeps going just needs wifi at startup. when wifi is back ok it will reconnect and update.
 Real rtc you have to use separate module with battery. Only this way when no wifi and poweroff you will keep time. 
 
# Remark
Getting and setting systemclock on esp32 via arduino ide was trial and error not clear at times. configtime or configTztime (update clearly) but the next step retrieving clock info from update was trial and error for me to get it right. Info or correction to this code are always welcome.
Code is good till 2038. Epochtime is 32bitvar will overflow in 2038 lol.

# Help appreciated
There must be a way to update time from your router instead of getting it from pool.org.
Have not figured out this method.  Suggestions how to do this would be welcome.
