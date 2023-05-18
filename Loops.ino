

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{
    server.handleClient();
    MQTT_loop();  
}



void Mid1_Loop()
{
	
}



void Mid2_Loop()
{
  
}



void Slow_Loop()
{  
  MQTT_publish();           
  RunOnce_30mins();
}


void VerySlow_Loop()
{
   
}



void RunOnce_30mins()
{
  if(millis()/1000 - lastrun_30mins > 30 * 60 || lastrun_30mins == 0)
  {
    lastrun_30mins = millis()/1000;
      
  }
}


