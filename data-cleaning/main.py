def data_cleaning():
  with open('input.txt') as f:
    s = f.read().splitlines()
  # print(len(s))
  # print(s)
  res = []
  for line in s:
    vals = line.split(' -> ')[1]
    res.append(list(map(int,vals.split(',')[:-1])))

  print(len(res))
  print(res[:10])

"""
Line sensor reading A3 white paper default(ANALOGUE):

831,783,795,817,854,
831,782,796,817,854,
831,783,796,817,854,
831,783,795,817,854,
831,782,795,817,854,

maximum separation between both robots 39cm.

"""
import pandas as pd
import matplotlib.pyplot as plt

# Read the data from the file
with open('input_d.txt', 'r') as file:
    data = file.read()

# Split the data into sections (distances)
sections = data.split('\n\n')

# Process each section
for section in sections:
    lines = section.strip().split('\n')
    if lines:
        # Get the distance from the first line
        distance = lines[0].split(':')[0]
        
        # Extract the data values
        values = [line.split(',') for line in lines[1:]]
        
        # Convert to a Pandas DataFrame
        df = pd.DataFrame(values, columns=['Sensor1', 'Sensor2', 'Sensor3', 'Sensor4', 'Sensor5'])
        df = df.astype(int)
        
        # Plot the data
        fig, ax = plt.subplots(figsize=(10, 6))
        df.plot(ax=ax)
        ax.set_title(f'Sensor Readings at {distance} cm')
        ax.set_xlabel('Time')
        ax.set_ylabel('Reading')
        plt.legend(df.columns)
        plt.show()



import matplotlib.pyplot as plt
import numpy as np

# Path to the data file
file_path = '/path/to/your/data/file.txt'

# Read the file contents
with open(file_path, 'r') as file:
    data_lines = file.readlines()

# Extract sensor readings
sensor_readings = [list(map(int, line.split('->')[1].strip().split(',')[:-1])) for line in data_lines]

# Convert to a numpy array for easier manipulation
sensor_readings_array = np.array(sensor_readings)

# Assuming linear decrease in distance from 40cm to 0cm
distances = np.linspace(40, 0, len(sensor_readings_array))

# Plotting
plt.figure(figsize=(14, 8))
for i in range(sensor_readings_array.shape[1]):
    plt.plot(distances, sensor_readings_array[:, i], label=f'Sensor {i+1}')

plt.xlabel('Distance (cm)')
plt.ylabel('Sensor Reading')
plt.title('Sensor Readings vs. Distance')
plt.legend()
plt.grid(True)
plt.show()