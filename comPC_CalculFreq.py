import serial
from datetime import datetime

# Initialiser le port série
ser = serial.Serial('COM8', 2000000)

# Variables pour calculer la fréquence
last_time = None
intervals = []

try:
    while True:
        # Lecture de 30 octets depuis le port série
        data = ser.read(30)
        current_time = datetime.now()

        # Calcul de l'intervalle si ce n'est pas la première réception
        if last_time is not None:
            interval = (current_time - last_time).total_seconds()
            intervals.append(interval)
            # Calcul de la fréquence moyenne
            frequency = 1 / (sum(intervals) / len(intervals))
            print(f"Fréquence moyenne : {frequency:.2f} Hz")
        
        last_time = current_time

        # Affichage des données reçues
        print("Données reçues :", list(data))

except KeyboardInterrupt:
    print("Arrêt du programme")
    if intervals:
        frequency = 1 / (sum(intervals) / len(intervals))
        print(f"Fréquence moyenne finale : {frequency:.2f} Hz")
finally:
    ser.close()
