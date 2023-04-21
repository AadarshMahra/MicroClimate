import tkinter as tk
import paho.mqtt.client as mqtt
import datetime
# definitions
broker_address, broker_port = "192.168.1.119", 1883

RPI_topics= ["RPI/targets/temp", "RPI/targets/rh"]

sensor_data_topics = ["data_acq/node0/temp", "data_acq/node0/rh",
                      "data_acq/node1/temp", "data_acq/node1/rh" ]

app_ctrl_status_topics = ["app_control/outlet0/status",
                          "app_control/outlet1/status",
                          "app_control/outlet2/status",
                          "app_control/outlet3/status"]


current_temp, current_humidity = 0.0,0.0

    

def on_message(client, userdata, message):
    # Get the current time
    now = datetime.datetime.now()

    # Convert the current time to a string in the desired format
    time_str = now.strftime("%Y-%m-%d %H:%M:%S")
    print("[{}]: Received message on topic {}: {}".format(time_str,message.topic, str(message.payload)))
    if message.topic == sensor_data_topics[0]:
        current_temp = message.payload
        current_temp_label.config(text="Current Temperature: {}°F".format(float(current_temp)))
            
    elif message.topic == sensor_data_topics[1]:
        current_humidity = message.payload
        current_humidity_label.config(text="Current Humidity: {}%".format(float(current_humidity)))

    elif message.topic == app_ctrl_status_topics[0]:
        heater1_status = str(message.payload)[2:-1]
        heater1_status_label.config(text="Heater 1 Status: " + heater1_status)
        
    elif message.topic == app_ctrl_status_topics[1]:
        humidifier1_status = str(message.payload)[2:-1]
        humidifier1_status_label.config(text="Humidifier 1 Status: " + humidifier1_status)
        
def update_temperature_target_to_mc():
    # Get the current time
    now = datetime.datetime.now()

    # Convert the current time to a string in the desired format
    time_str = now.strftime("%Y-%m-%d %H:%M:%S")
    # create connection and publish inputs to topics
    print("[{}]: Changing Requested Temperature Target".format(time_str))
    client.publish(RPI_topics[0], temp_target_entry.get())
    current_temp_target_label.config(text="Current Temperature Target: " + temp_target_entry.get())
    
def update_humidity_target_to_mc():
    # Get the current time
    now = datetime.datetime.now()

    # Convert the current time to a string in the desired format
    time_str = now.strftime("%Y-%m-%d %H:%M:%S")
    # create connection and publish inputs to topics
    print("[{}]: Changing Requested Humidity Target".format(time_str))
    client.publish(RPI_topics[1], humidity_target_entry.get())
    current_humidity_target_label.config(text="Current Humidity Target: " + humidity_target_entry.get())
  # Create the tkinter window
master = tk.Tk()
master.title("Temperature and Humidity Monitor")

# Create temperature target label and entry widget
temp_target_label = tk.Label(master, text="Temperature Target:")
temp_target_label.pack()
temp_target_entry = tk.Entry(master)
temp_target_entry.pack()

# Create button to update current temperature and humidity values
update_temperature_target_button = tk.Button(master, text="Update", command=update_temperature_target_to_mc)
update_temperature_target_button.pack()

#See current target
current_temp_target_label = tk.Label(master, text="Current Temperature Target:")
current_temp_target_label.pack() 
# Create humidity target label and entry widget
humidity_target_label = tk.Label(master, text="Humidity Target:")
humidity_target_label.pack()
humidity_target_entry = tk.Entry(master)
humidity_target_entry.pack()

#See current humidity
current_humidity_target_label = tk.Label(master, text="Current Humidity Target:")
current_humidity_target_label.pack()
# Create button to update current temperature and humidity values
update_humidity_target_button = tk.Button(master, text="Update", command=update_humidity_target_to_mc)
update_humidity_target_button.pack()


# Create current temperature and humidity labels
current_temp_label = tk.Label(master, text="Current Temperature: ")
current_temp_label.pack()
current_humidity_label = tk.Label(master, text="Current Humidity: ")
current_humidity_label.pack()


# Create status labels for the actuators
heater1_status_label = tk.Label(master, text="Heater 1 Status: ")
heater1_status_label.pack()
humidifier1_status_label = tk.Label(master, text="Humidifier 1 Status: ")
humidifier1_status_label.pack()



# Create button to exit program
exit_button = tk.Button(master, text="Exit", command=master.quit)
exit_button.pack()

    

        
        
# declare an MQTT client 
client = mqtt.Client(client_id="RPI")
client.on_message = on_message

 # create connection and subscribe to topics
try: 
    client.connect(broker_address, broker_port)
except Exception as e:
    print(e)
# subscribe to sensor topics 
client.subscribe(sensor_data_topics[0])
client.subscribe(sensor_data_topics[1])
client.subscribe(app_ctrl_status_topics[0])
client.subscribe(app_ctrl_status_topics[1])
client.loop_start()

master.mainloop()
# ghp_Pjd9n5OGhQAtxUqsMC0LwcUYezIha3uAPPS