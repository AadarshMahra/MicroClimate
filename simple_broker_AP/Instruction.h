/* struct to hold actuator instructions */ 

struct instruction
{
  char* heater_instruction; 
  char* humidifier_instruction; 
}; 
/* helper function to compute actuator function and return two appliance instructions to publish */
struct instruction compute_inst(float curr_temp, float curr_hum, float temp_threshold, float hum_threshold, float p_variance) 
{

    float multiplier = p_variance/100; 
    
    struct instruction ai; 
    if (curr_temp < temp_threshold - multiplier*temp_threshold)
    {
      ai.heater_instruction = "On"; 
    }
    else if (curr_temp > temp_threshold + multiplier*temp_threshold){
      ai.heater_instruction = "Off";   
    }

    else 
    {
      ai.heater_instruction = "Off"; 
    }
    
    if (curr_hum < hum_threshold - multiplier*hum_threshold)
    {
      ai.humidifier_instruction = "On"; 
    }
    else if (curr_hum > hum_threshold + multiplier*hum_threshold){
      ai.humidifier_instruction = "Off";   
    }

    else 
    {
      ai.humidifier_instruction = "On"; 
    }
    
    return ai; 
}
