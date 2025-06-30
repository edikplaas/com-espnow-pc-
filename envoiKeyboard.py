import serial
from pynput import keyboard

# Configurez le port série en fonction du port auquel votre ESP32 est connecté
serial_port = '/dev/ttyACM0'
baud_rate = 2000000

try:
    # Initialisez la connexion série
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print("Connexion série établie")

    def on_press(key):
        try:
            if key.char == 's':
                ser.write(b'1\n')
                print("Envoyé : 1")
            elif key.char == 'q':
                ser.write(b'0\n')
                print("Envoyé : 0")
        except AttributeError:
            pass

    def on_release(key):
        if key == keyboard.Key.esc:
            # Arrêter le listener
            return False

    # Collecter les événements jusqu'à ce que l'utilisateur appuie sur Échap
    with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()

except serial.SerialException as e:
    print(f"Impossible d'ouvrir le port série: {e}. Vérifiez la connexion et le port.")
except Exception as e:
    print(f"Une erreur s'est produite: {e}")
finally:
    if 'ser' in locals():
        ser.close()
        print("Connexion série fermée")
