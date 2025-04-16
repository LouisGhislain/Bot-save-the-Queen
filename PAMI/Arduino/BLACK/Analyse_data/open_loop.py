#plot data form open loop test
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


# Head of the data
# 0,0,0.00,0,0.00
# 6,0,0.00,0,0.00
# 24,2,111.11,2,111.11
# 47,7,217.39,8,260.87
# 70,16,391.31,17,391.31
# 95,28,479.99,29,479.99
# 120,44,640.01,45,640.01
# 146,64,759.98,65,759.98
# 172,86,851.87,87,814.83

# time_ms,left_ticks,left_speed,right_ticks,right_speed


# Read the data from txt file
# Chemin complet du fichier
data_file = "/Users/hugomaleve/Documents/PlatformIO/Mecatro_git/PAMI/Arduino/PAMI_New_arduino/Analyse_data/Open_loop_7_5s.txt"
# Lire les données
data = pd.read_csv(data_file, sep=",", header=None)
data.columns = ["time_ms", "left_ticks", "left_speed", "right_ticks", "right_speed"]
data["time_s"] = data["time_ms"] / 1000  # Convert to seconds

time_first = data["time_s"].iloc[0]

#the speed is in ticks/s
# Convert ticks to speed
ticks_one_rotation = 13
Gear_ratio = 42
ticks_per_rotation = ticks_one_rotation * Gear_ratio
data["left_speed"] = data["left_speed"] / ticks_per_rotation 
data["right_speed"] = data["right_speed"] / ticks_per_rotation

# Convert to m/s
WHEEL_DIAMETER = 6.0325/100;  # in cm
WHEEL_RADIUS = WHEEL_DIAMETER / 2  # in m
data["left_speed"] = data["left_speed"] * np.pi * WHEEL_DIAMETER # in m/s
data["right_speed"] = data["right_speed"] * np.pi * WHEEL_DIAMETER  # in m/s
# Convert to m/s

#Rendre les courbes plus lisses
data["left_speed"] = data["left_speed"].rolling(window=7).mean()
data["right_speed"] = data["right_speed"].rolling(window=7).mean()


#Calcul valeur moyenne de left_speed et right_speed entre 4s et 5s
start_time = 4
end_time = 5
mask = (data["time_s"] >= start_time) & (data["time_s"] <= end_time)
max_left_speed = data.loc[mask, "left_speed"].mean()
max_right_speed = data.loc[mask, "right_speed"].mean()

# Calculer 90% de la vitesse maximale
response_threshold_left = 0.63 * max_left_speed
response_threshold_right = 0.63 * max_right_speed

# Trouver le temps où la vitesse atteint 90% de la vitesse maximale pour la première fois
response_time_left = data[data["left_speed"] >= response_threshold_left]["time_s"].iloc[0] - time_first
response_time_right = data[data["right_speed"] >= response_threshold_right]["time_s"].iloc[0] - time_first

# Afficher les temps de réponse
print(f"Temps de réponse approximatif (gauche): {response_time_left} secondes")
print(f"Temps de réponse approximatif (droite): {response_time_right} secondes")


# plot
plt.figure(figsize=(10, 5))
# Plot mean speed
plt.axhline(max_left_speed, color="blue", linestyle="--", label="Max left speed")
plt.axhline(max_right_speed, color="orange", linestyle="--", label="Max right speed")

plt.plot(data["time_s"], data["left_speed"], label="Left speed")
plt.plot(data["time_s"], data["right_speed"], label="Right speed")



plt.xlabel("Time (s)")
plt.ylabel("Speed (m/s)")
plt.title("Open loop test")
plt.legend()
plt.grid()
plt.show()