import serial
from datetime import datetime
import struct

# Définir la structure des données attendues
data_format = 'B' + 'h' * 49  # 1 octet pour l'ID et 49 entiers de 2 octets pour les blocs
data_size = struct.calcsize(data_format)

def read_serial(port="COM8", baudrate=2000000, timeout=1):
    try:
        # Configure le port série
        ser = serial.Serial(port, baudrate, timeout=timeout)
        print(f"Connexion établie sur {port} à {baudrate} bauds")

        last_time = None  # Variable pour stocker l'heure du dernier message reçu
        message_count = 0  # Compteur de messages
        total_time_elapsed = 0  # Temps total écoulé entre les messages

        while True:
            if ser.in_waiting >= data_size:
                # Lis les données binaires disponibles sur le port série
                binary_data = ser.read(data_size)

                # Dépaqueter les données binaires selon la structure définie
                unpacked_data = struct.unpack(data_format, binary_data)

                current_time = datetime.now()  # Heure actuelle

                if last_time is not None:
                    # Calculer le temps écoulé depuis le dernier message
                    time_elapsed = (current_time - last_time).total_seconds()
                    total_time_elapsed += time_elapsed
                    message_count += 1

                    # Calculer et afficher la fréquence de réception
                    if message_count > 1:  # On attend au moins deux messages pour calculer la fréquence
                        frequency = (message_count - 1) / total_time_elapsed
                        print(f"Fréquence de réception : {frequency:.2f} Hz")

                last_time = current_time  # Mettre à jour l'heure du dernier message

                # Afficher les données reçues
                tabAffiche = ["ID : ", str(unpacked_data[0]), " Data : "]
                for i in range(1, len(unpacked_data)):
                    tabAffiche.append(str(unpacked_data[i]))
                    tabAffiche.append(" ")
                print("".join(tabAffiche))  # Affichage correct de toute la data

    except serial.SerialException as e:
        print(f"Erreur avec le port série : {e}")

    except KeyboardInterrupt:
        print("Programme arrêté par l'utilisateur")

    finally:
        # Ferme la connexion série en quittant
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Connexion série fermée")

if __name__ == "__main__":
    read_serial()
