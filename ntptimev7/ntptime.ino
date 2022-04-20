bool getNTPtime() 
{   bool ok = false;  
    tm timeinfox;
    Serial.println("check ntp");
    if (check_Wifi())
    {
      configTzTime(TZ_INFO,NTP_SERVER,NTP_SERVER2,NTP_SERVER3);//this updates via ntp
      if (getLocalTime(&timeinfox))
        { rtc.setTimeStruct(timeinfox);
           Serial.println("update NTP clock succeeded");
           Serial.println(rtc.getDateTime());
          ok = true;
        }
     else
      {Serial.println("Error get localtime");} 
    } 
    else
    { Serial.println("--Cannot get NTP--");
      Serial.println("No Wifi");
    } 
    stop_Wifi();
    
    return ok;
}
void f_heartbeat()
{
  if (heartbeat > heartbeatinterval)
  {
    digitalWrite(blinkled,!digitalRead(blinkled));
    heartbeat = 0;
  }  
}
// //internal clock set
//     time_t t = mktime(&timeinfo);
//     struct timeval newtime = { .tv_sec = t };
//     settimeofday(&newtime, NULL);
