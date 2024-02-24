

void config_relay()
{
	pinMode(External_Relay_1_pin, OUTPUT);
	pinMode(External_Relay_2_pin, OUTPUT);

	digitalWrite(External_Relay_1_pin, true);  // Active Low
	digitalWrite(External_Relay_2_pin, true);  // Active Low
}





void handle_grow_light_cmd(bool cmd)
{
	if(cmd==1)
	{			
		digitalWrite(External_Relay_1_pin, false);  // Active Low
		delay(500);
		digitalWrite(External_Relay_1_pin, true);
		grow_light_state=1;
	}
	if(cmd==0)
	{			
		digitalWrite(External_Relay_2_pin, false);  // Active Low
		delay(500);
		digitalWrite(External_Relay_2_pin, true);
		grow_light_state=0;
	}
}


