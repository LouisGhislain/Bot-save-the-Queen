import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Lecture des données
data_file = "/Users/hugomaleve/Documents/PlatformIO/Mecatro_git/PAMI/Arduino/PAMI_New_arduino/Analyse_data/Open_loop_7_5s.txt"
data = pd.read_csv(data_file, sep=",", header=None)
data.columns = ["time_ms", "left_ticks", "left_speed", "right_ticks", "right_speed"]
data["time_s"] = data["time_ms"] / 1000  # Convertir en secondes

time_first = data["time_s"].iloc[0]

# Conversion des ticks en vitesse
ticks_one_rotation = 13
Gear_ratio = 42
ticks_per_rotation = ticks_one_rotation * Gear_ratio
data["left_speed"] = data["left_speed"] / ticks_per_rotation 
data["right_speed"] = data["right_speed"] / ticks_per_rotation

# Conversion en m/s
WHEEL_DIAMETER = 6.0325/100  # en cm
WHEEL_RADIUS = WHEEL_DIAMETER / 2  # en m
data["left_speed"] = data["left_speed"] * np.pi * WHEEL_DIAMETER  # en m/s
data["right_speed"] = data["right_speed"] * np.pi * WHEEL_DIAMETER  # en m/s

# Rendre les courbes plus lisses
data["left_speed"] = data["left_speed"].rolling(window=7).mean()
data["right_speed"] = data["right_speed"].rolling(window=7).mean()

# Calcul de la vitesse moyenne entre 4s et 5s
start_time = 4
end_time = 5
mask = (data["time_s"] >= start_time) & (data["time_s"] <= end_time)
max_left_speed = data.loc[mask, "left_speed"].mean()
max_right_speed = data.loc[mask, "right_speed"].mean()

# Calcul de 90% de la vitesse maximale
response_threshold_left = 0.63 * max_left_speed
response_threshold_right = 0.63 * max_right_speed

# Trouver le temps où la vitesse atteint 90% de la vitesse maximale pour la première fois
response_time_left = data[data["left_speed"] >= response_threshold_left]["time_s"].iloc[0] - time_first
response_time_right = data[data["right_speed"] >= response_threshold_right]["time_s"].iloc[0] - time_first

print(f"Temps de réponse approximatif (gauche): {response_time_left} secondes")
print(f"Temps de réponse approximatif (droite): {response_time_right} secondes")

# Application de la méthode de Ziegler-Nichols pour déterminer Kp et Ki
# Trouver le point d'oscillation stable dans les données (temps de réponse et amplitude de l'oscillation)
# Cela pourrait être fait en cherchant les pics dans les données de vitesse

# Exemple d'oscillation (en supposant qu'il y a une période stable de vitesse)
# Calculez l'amplitude et la période de l'oscillation

# Trouver les pics dans les données (juste pour illustrer, ce sera à ajuster selon votre signal)
peaks_left = data["left_speed"].loc[data["left_speed"] > max_left_speed * 0.5]
peaks_right = data["right_speed"].loc[data["right_speed"] > max_right_speed * 0.5]

# Période d'oscillation (période = temps entre deux pics)
period_left = peaks_left.index[-1] - peaks_left.index[0]
period_right = peaks_right.index[-1] - peaks_right.index[0]

# Estimer le gain Kp à partir de la méthode de Ziegler-Nichols
# Kp = 0.6 * (max_value / amplitude)
Kp_left = 0.6 * (max_left_speed / period_left)
Kp_right = 0.6 * (max_right_speed / period_right)

# Estimer Ki à partir de la méthode de Ziegler-Nichols
# Ki = 2 * Kp / period
Ki_left = 2 * Kp_left / period_left
Ki_right = 2 * Kp_right / period_right

# Afficher les résultats
print(f"Gain Kp (gauche): {Kp_left}")
print(f"Gain Ki (gauche): {Ki_left}")
print(f"Gain Kp (droite): {Kp_right}")
print(f"Gain Ki (droite): {Ki_right}")

# Visualisation des courbes de vitesse
plt.figure(figsize=(10, 5))
plt.plot(data["time_s"], data["left_speed"], label="Vitesse gauche")
plt.plot(data["time_s"], data["right_speed"], label="Vitesse droite")
plt.axhline(max_left_speed, color="blue", linestyle="--", label="Vitesse max gauche")
plt.axhline(max_right_speed, color="orange", linestyle="--", label="Vitesse max droite")
plt.xlabel("Temps (s)")
plt.ylabel("Vitesse (m/s)")
plt.title("Test en boucle ouverte")
plt.legend()
plt.grid()
plt.show()
