

void config_relay()
{
	pinMode(External_Relay_1_pin, OUTPUT);
	pinMode(External_Relay_2_pin, OUTPUT);

	digitalWrite(External_Relay_1_pin, !External_Relay_1_state);
	digitalWrite(External_Relay_2_pin, !External_Relay_2_state);
}



void set_relay()
{
	digitalWrite(External_Relay_1_pin, !External_Relay_1_state);
	digitalWrite(External_Relay_2_pin, !External_Relay_2_state);

	if(External_Relay_1_state || External_Relay_2_state) 
	{
		delay(500);
		External_Relay_1_state = 0;
		External_Relay_2_state = 0;
		set_relay();
	}
}



