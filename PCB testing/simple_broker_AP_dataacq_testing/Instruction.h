/* struct to hold actuator instructions */ 

struct instruction
{
  char* heater_instruction; 
  char* humidifier_instruction; 
}; 
/* helper function to compute actuator function and return two appliance instructions to publish */
struct instruction compute_inst(float curr_temp, float curr_hum, float temp_target, float hum_target, float t_variance, float h_variance) 
{

    float t_multiplier = t_variance/100; 
    float h_multiplier = h_variance/100; 
    
    struct instruction ai; 
    if (curr_temp < temp_target - t_multiplier*temp_target)
    {
      ai.heater_instruction = "On"; 
    }
    else if (curr_temp > temp_target + t_multiplier*temp_target){
      ai.heater_instruction = "Off";   
    }

    else 
    {
      ai.heater_instruction = "Off"; 
    }
    
    if (curr_hum < hum_target - h_multiplier*hum_target)
    {
      ai.humidifier_instruction = "On"; 
    }
    else if (curr_hum > hum_target + h_multiplier*hum_target){
      ai.humidifier_instruction = "Off";   
    }

    else 
    {
      ai.humidifier_instruction = "On"; 
    }
    
    return ai; 
}
