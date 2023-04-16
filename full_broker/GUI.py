import tkinter as tk
import paho.mqtt.client as mqtt

# definitions
broker_address, broker_port = "192.168.1.119", 1883

RPI_topics= ["RPI/thresholds/temp", "RPI/thresholds/rh"]

sensor_data_topics = ["data_acq/node0/temp", "data_acq/node0/rh",
                      "data_acq/node1/temp", "data_acq/node1/rh" ]
current_temp, current_humidity = 0.0,0.0

def update_values_to_mc():
    # create connection and publish inputs to topics
    try: 
        client.connect(broker_address, broker_port)
    except Exception as e:
        print(e)
    
    print("Changing Requested Temperature Threshold")
    client.publish(RPI_topics[0], temp_threshold_entry.get())
    print("Changing Requested Humidity Threshold")
    client.publish(RPI_topics[1], humidity_threshold_entry.get())

# Create the tkinter window
master = tk.Tk()
master.title("Temperature and Humidity Monitor")

# Create temperature threshold label and entry widget
temp_threshold_label = tk.Label(master, text="Temperature Threshold:")
temp_threshold_label.pack()
temp_threshold_entry = tk.Entry(master)
temp_threshold_entry.pack()

# Create humidity threshold label and entry widget
humidity_threshold_label = tk.Label(master, text="Humidity Threshold:")
humidity_threshold_label.pack()
humidity_threshold_entry = tk.Entry(master)
humidity_threshold_entry.pack()

# Create current temperature and humidity labels
current_temp_label = tk.Label(master, text="Current Temperature: ")
current_temp_label.pack()
current_humidity_label = tk.Label(master, text="Current Humidity: ")
current_humidity_label.pack()

# Create button to update current temperature and humidity values
update_button = tk.Button(master, text="Update", command=update_values_to_mc)
update_button.pack()

# Create button to exit program
exit_button = tk.Button(master, text="Exit", command=master.quit)
exit_button.pack()
    

def on_message(client, userdata, message):
    print("Received message on topic {}: {}".format(message.topic, str(message.payload)))
    if message.topic == sensor_data_topics[0]:
        current_temp = message.payload
        current_temp_label.config(text="Current Temperature: {:.1f}°C".format(current_temp))
            
    elif message.topic == sensor_data_topics[1]:
        current_humidity = message.payload
        current_humidity_label.config(text="Current Humidity: {:.1f}%".format(current_humidity))
    

        

# create GUI
root = tk.Tk()

        
# declare an MQTT client 
client = mqtt.Client()
client.on_message = on_message

 # create connection and subscribe to topics
try: 
    client.connect(broker_address, broker_port)
except Exception as e:
    print(e)
# subscribe to sensor topics 
client.subscribe(sensor_data_topics[0])
client.subscribe(sensor_data_topics[1])

client.loop_start()
root.mainloop()
