import serial

ser = serial.Serial('COM8', 2000000)

try:
    while True:
        # Lecture de 30 octets depuis le port série
        data = ser.read(30)
        print("Données reçues :", list(data))
except KeyboardInterrupt:
    print("Arrêt du programme")
finally:
    ser.close()