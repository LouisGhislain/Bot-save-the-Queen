import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

# Open the file
f = open("encoder_data.txt", "r")

# Read the file
data = f.readlines()

# Initialize lists to store data
times = []
left_encoder = []
right_encoder = []

# Process each line of data
for line in data:
    # Skip lines that don't have numeric data
    line = line.strip()
    if not line or 'Temps' in line or 'Avant' in line:
        continue
        
    # Split by comma and strip any whitespace
    values = [val.strip() for val in line.split(',')]
    if len(values) == 3:  # Ensure we have all 3 values
        try:
            times.append(int(values[0]))
            left_encoder.append(float(values[1]))
            right_encoder.append(float(values[2]))
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
    
    # Calculate sampling frequency
    if len(times) > 1:
        # Convert microseconds to seconds for frequency calculation
        time_seconds = times / 1e6
        sampling_rate = 1 / np.mean(np.diff(time_seconds))
        print(f"Estimated sampling rate: {sampling_rate:.2f} Hz")
    else:
        sampling_rate = 1000  # Default if we can't calculate
    
    # Design filter
    # Cutoff frequency (Hz) - adjust this based on your signal characteristics
    # Lower values = more aggressive filtering
    cutoff_freq = 1 
    
    # Normalized cutoff frequency (must be between 0 and 1)
    nyquist = 0.5 * sampling_rate
    normal_cutoff = cutoff_freq / nyquist
    
    # Create a 4th order Butterworth low-pass filter
    b, a = signal.butter(4, normal_cutoff, btype='low', analog=False)
    
    # Apply the filter to the encoder data using filtfilt (zero-phase filtering)
    # filtfilt applies the filter twice, forward and backward, for zero phase delay
    left_encoder_filtered = signal.filtfilt(b, a, left_encoder)
    right_encoder_filtered = signal.filtfilt(b, a, right_encoder)

    # Take the mean value of left and right filtered encoder values
    mean_encoder_filtered = (left_encoder_filtered + right_encoder_filtered) / 2
        
    # Create the plot
    plt.figure(figsize=(12, 8))
    
    # Plot raw data
    plt.plot(times, left_encoder, 'b', alpha=0.5, label='Left Encoder (raw)')
    plt.plot(times, right_encoder, 'r', alpha=0.5, label='Right Encoder (raw)')
    
    # Plot filtered data
    #plt.plot(times, left_encoder_filtered, 'g', linewidth=2, label='Left Encoder (filtered)')
    #plt.plot(times, right_encoder_filtered, 'y', linewidth=2, label='Right Encoder (filtered)')
    
    # Plot mean filtered data
    plt.plot(times, mean_encoder_filtered, 'k', linewidth=2, label='Mean Encoder (filtered)')

    # Computes the value in steady state
    steady_state_value = mean_encoder_filtered[-1]

    # Get the time to reach 95% of the steady state value
    time_95 = 0
    for i in range(len(mean_encoder_filtered)):
        if mean_encoder_filtered[i] >= 0.95*steady_state_value:
            time_95 = times[i]*1e-6
            break
    
    # Plot 95% of the steady state value and the time to reach it
    plt.axhline(y=0.95*steady_state_value, color='r', linestyle='--', label='95% of Steady State Value')
    plt.axvline(x=time_95*1e6, color='r', linestyle='--', label='Time to reach 95% of Steady State Value')
    
    # Computes the time response of the system by dividing the time to reach 95% of the steady state value by 3
    time_response = time_95/3

    # Computes the gain of the system by dividing the steady state value by the input value
    gain = steady_state_value/50

    # Computes Kp & Ki
    imposed_time_response = 3*time_response
    Kp = 1/gain * (time_response/imposed_time_response)
    Ki = Kp/time_response

    # Print the results
    print(f"Steady state value: {steady_state_value}")
    print(f"Time to reach 95% of the steady state value: {time_95}")
    print(f"Time response: {time_response}")
    print(f"Imposed time response: {imposed_time_response}")
    print(f"Gain: {gain}")
    print(f"Kp: {Kp}")
    print(f"Ki: {Ki}")

    # Add labels and title
    plt.xlabel('Time (µs)')
    plt.ylabel('Encoder Value')
    plt.title(f'Encoder Data Over Time (Butterworth Low-Pass Filter, Cutoff: {cutoff_freq} Hz)')
    plt.grid(True)
    plt.legend()
    
    # Show the plot
    plt.tight_layout()
    plt.savefig('encoder_data_plot.png')

else:
    print("No valid data found to plot")
