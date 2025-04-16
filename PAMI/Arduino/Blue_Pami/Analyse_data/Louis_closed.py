import matplotlib.pyplot as plt
import numpy as np

# Open the file
f = open("/Users/hugomaleve/Documents/PlatformIO/Mecatro_git/PAMI/Arduino/PAMI_New_arduino/Analyse_data/Real_Closed_loop.txt", "r")

# Read the file
data = f.readlines()

# Initialize lists to store data
times = []
left_encoder = []
right_encoder = []
left_voltage = []
right_voltage = []

# Process each line of data
for line in data:
    # Skip lines that don't have numeric data
    line = line.strip()
    if not line or 'Temps' in line or 'Avant' in line:
        continue
        
    # Split by comma and strip any whitespace
    values = [val.strip() for val in line.split(',')]
    if len(values) >= 5:  # Ensure we have at least 5 values (time, encoders, voltages)
        try:
            times.append(int(values[0]))
            left_encoder.append(float(values[1]))
            right_encoder.append(float(values[2]))
            left_voltage.append(float(values[3]))
            right_voltage.append(float(values[4]))
        except ValueError:
            # Skip lines that can't be converted to numbers
            print(f"Skipping invalid data line: {line}")

# Close the file
f.close()

# Only plot if we have data
if times:
    # Convert lists to numpy arrays
    times = np.array(times)
    left_encoder = np.array(left_encoder)
    right_encoder = np.array(right_encoder)
    left_voltage = np.array(left_voltage)
    right_voltage = np.array(right_voltage)

    # Create the plot with dual y-axis
    fig, ax1 = plt.subplots(figsize=(12, 8))
    
    # Plot encoder data on the primary y-axis
    ax1.plot(times, left_encoder, 'b', label='Left Encoder')
    ax1.plot(times, right_encoder, 'r', label='Right Encoder')
    ax1.set_xlabel('Time (µs)')
    ax1.set_ylabel('Encoder Value', color='b')
    ax1.tick_params(axis='y', labelcolor='b')

    # Create a secondary y-axis for voltages
    ax2 = ax1.twinx()
    ax2.plot(times, left_voltage, 'g', label='Left Wheel Voltage')
    ax2.plot(times, right_voltage, 'm', label='Right Wheel Voltage')
    ax2.set_ylabel('Voltage (V)', color='g')
    ax2.tick_params(axis='y', labelcolor='g')

    # Combine legends
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax2.legend(lines1 + lines2, labels1 + labels2, loc='best')

    plt.title('Closed Loop Response: Encoder and Voltage Signals')
    ax1.grid(True)

    # Adjust layout and show plot
    plt.tight_layout()
    plt.show()

else:
    print("No valid data found to plot")