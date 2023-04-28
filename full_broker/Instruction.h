/* struct to hold actuator instructions */ 

struct instruction
{
  char* heater_instruction; 
  char* humidifier_instruction; 
}; 
/* helper function to compute actuator function and return two appliance instructions to publish */
struct instruction compute_inst(float curr_temp, float curr_hum, float temp_target, float hum_target, float t_variance, float h_variance) 
{
    
    struct instruction ai; 
    if (curr_temp < (temp_target - t_variance))
    {
      ai.heater_instruction = "On"; 
    }
    else if (curr_temp > temp_target + t_variance){
      ai.heater_instruction = "Off";   
    } else {
      ai.heater_instruction = "PREV"; 
    }

    
    if (curr_hum < hum_target - h_variance)
    {
      ai.humidifier_instruction = "On"; 
    }
    else if (curr_hum > hum_target + h_variance){
      ai.humidifier_instruction = "Off";   
    } else {
      ai.humidifier_instruction = "PREV"; 
    }
    
    return ai; 
}
