## plot the data from Closed_loop.txt
#the data are in the format:
# time_ms left_speed right_speed

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Lecture des données
data_file = "/Users/hugomaleve/Documents/PlatformIO/Mecatro_git/PAMI/Arduino/PAMI_New_arduino/Analyse_data/Closed_loop.txt"
data = pd.read_csv(data_file, sep=" ", header=None)
data.columns = ["time_ms", "left_speed", "right_speed"]
data["time_s"] = data["time_ms"] - 5  # Convertir en secondes
time_first = data["time_s"].iloc[0]

#Plots des données
plt.figure(figsize=(10, 5))
plt.plot(data["time_s"], data["left_speed"], label="Left Speed")
plt.plot(data["time_s"], data["right_speed"], label="Right Speed")
plt.xlabel("Time (s)")
plt.ylabel("Speed (m/s)")
plt.title("Closed Loop Speed Data")
plt.legend()
plt.grid()
plt.show()


