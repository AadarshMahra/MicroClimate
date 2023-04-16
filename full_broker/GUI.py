import tkinter as tk
import paho.mqtt.client as mqtt

# definitions
broker_address, broker_port = "192.168.1.119", 1883

RPI_topics= ["RPI/thresholds/temp", "RPI/thresholds/rh"]

sensor_data_topics = ["data_acq/node0/temp", "data_acq/node0/rh",
                      "data_acq/node1/temp", "data_acq/node1/rh" ]
current_temp, current_humidity = 0.0,0.0

def on_message(client, userdata, message):
    print("Received message on topic {}: {}".format(message.topic, str(message.payload)))
    if message.topic == sensor_data_topics[0]:
        current_temp = message.payload
        self.current_temp_label.config(text="Current Temperature: {:.1f}°C".format(current_temp))
            
    elif message.topic == sensor_data_topics[1]:
        current_humidity = message.payload
        self.current_humidity_label.config(text="Current Humidity: {:.1f}%".format(current_humidity))
    
class App:
    def __init__(self, master):
        self.master = master
        master.title("Temperature and Humidity Monitor")
        
        # Create temperature threshold label and entry widget
        self.temp_threshold_label = tk.Label(master, text="Temperature Threshold:")
        self.temp_threshold_label.pack()
        self.temp_threshold_entry = tk.Entry(master)
        self.temp_threshold_entry.pack()
        
        # Create humidity threshold label and entry widget
        self.humidity_threshold_label = tk.Label(master, text="Humidity Threshold:")
        self.humidity_threshold_label.pack()
        self.humidity_threshold_entry = tk.Entry(master)
        self.humidity_threshold_entry.pack()
        
        # Create current temperature and humidity labels
        self.current_temp_label = tk.Label(master, text="Current Temperature: ")
        self.current_temp_label.pack()
        self.current_humidity_label = tk.Label(master, text="Current Humidity: ")
        self.current_humidity_label.pack()
        
        # Create button to update current temperature and humidity values
        self.update_button = tk.Button(master, text="Update", command=self.update_values_to_mc)
        self.update_button.pack()
        
        # Create button to exit program
        self.exit_button = tk.Button(master, text="Exit", command=master.quit)
        self.exit_button.pack()
    
    def update_values_to_mc(self):
        # create connection and publish inputs to topics
        try: 
            client.connect(broker_address, broker_port)
        except Exception as e:
            print(e)
        
        print("Changing Requested Temperature Threshold")
        client.publish(RPI_topics[0], self.temp_threshold_entry.get())
        print("Changing Requested Humidity Threshold")
        client.publish(RPI_topics[1], self.humidity_threshold_entry.get())
    
        
        

# create GUI
root = tk.Tk()
app = App(root)
        
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
