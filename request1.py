import requests
import matplotlib.pyplot as plt

URL = "http://192.168.1.175/"

def get_sensor_data():
    """
    Retrieves sensor data from the root URL.
    Assumes that the data is returned as plain text with each line formatted as:
        Sensor Name: value
    Returns:
        A dictionary with sensor names as keys and their numeric values.
    """
    try:
        response = requests.get(URL, timeout=2)
        text = response.text
        data = {}
        # Parse each line for sensor data.
        for line in text.splitlines():
            if ":" in line:
                name, value = line.split(":", 1)
                try:
                    # Convert the sensor value to a float.
                    data[name.strip()] = float(value.strip())
                except ValueError:
                    # If conversion fails, default to 0.
                    data[name.strip()] = 0
        return data
    except requests.exceptions.RequestException as e:
        print("Error fetching sensor data:", e)
        return {}

def plot_sensor_data():
    """
    Continuously retrieves sensor data and updates a bar graph.
    """
    plt.ion()  # Enable interactive mode for live updating
    fig, ax = plt.subplots()
    
    # Initial fetch to set up sensor names.
    sensor_data = get_sensor_data()
    if not sensor_data:
        print("No sensor data found. Check the data format at", URL)
        return

    sensor_names = list(sensor_data.keys())
    
    while True:
        sensor_data = get_sensor_data()
        sensor_names = list(sensor_data.keys())
        sensor_values = [sensor_data.get(name, 0) for name in sensor_names]
        
        # Clear and redraw the bar chart to accommodate any changes.
        ax.clear()
        bars = ax.bar(sensor_names, sensor_values, color='skyblue')
        
        # Dynamically set the y-axis limit.
        max_value = max(sensor_values) if sensor_values else 100
        ax.set_ylim(0, max_value * 1.2 if max_value > 0 else 100)
        ax.set_ylabel("Sensor Values")
        ax.set_title("Live Sensor Data")
        
        plt.draw()
        plt.pause(2)  # Update every 2 seconds

if __name__ == "__main__":
    plot_sensor_data()
