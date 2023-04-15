import tkinter as tk

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
        self.update_button = tk.Button(master, text="Update", command=self.update_values)
        self.update_button.pack()
        
        # Create button to exit program
        self.exit_button = tk.Button(master, text="Exit", command=master.quit)
        self.exit_button.pack()
    
    def update_values(self):
        # Get current temperature and humidity values (replace with actual values from sensors)
        current_temp = 25.0
        current_humidity = 50.0
        
        # Update current temperature and humidity labels
        self.current_temp_label.config(text="Current Temperature: {:.1f}°C".format(current_temp))
        self.current_humidity_label.config(text="Current Humidity: {:.1f}%".format(current_humidity))
        
        # Check if temperature and/or humidity are above threshold values
        temp_threshold = float(self.temp_threshold_entry.get())
        humidity_threshold = float(self.humidity_threshold_entry.get())
        
        if current_temp > temp_threshold:
            print("Temperature above threshold!")
        
        if current_humidity > humidity_threshold:
            print("Humidity above threshold!")

root = tk.Tk()
app = App(root)
root.mainloop()
