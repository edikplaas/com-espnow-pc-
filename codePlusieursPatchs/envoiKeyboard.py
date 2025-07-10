import serial
from pynput import keyboard

# Configurez le port série en fonction du port auquel votre ESP32 est connecté
serial_port = '/dev/ttyUSB0'  # Changez ceci selon votre configuration
baud_rate = 2000000  # Taux de baud pour ESP32

try:
    # Initialisez la connexion série
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print(f"Connexion série établie sur {serial_port} à {baud_rate} bauds")

    def on_press(key):
        try:
            # Envoyer '1' si la touche 's' est pressée
            if key.char == 's':
                ser.write(b'1\n')
                print("Envoyé : 1")
            # Envoyer '0' si la touche 'q' est pressée
            elif key.char == 'q':
                ser.write(b'0\n')
                print("Envoyé : 0")
        except AttributeError:
            # Gérer les touches spéciales qui n'ont pas de caractère
            pass

    def on_release(key):
        # Arrêter le listener si la touche Échap est pressée
        if key == keyboard.Key.esc:
            return False

    # Démarrer le listener pour les événements clavier
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
