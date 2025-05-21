import serial

def read_serial(port="COM8", baudrate=2000000, timeout=1):
    try:
        # Configure le port série
        ser = serial.Serial(port, baudrate, timeout=timeout)
        print(f"Connexion établie sur {port} à {baudrate} bauds")
        while True:
            if ser.in_waiting > 0:
                # Lis les données disponibles sur le port série
                line = ser.readline().decode('utf-8').strip()
                lineTab = line.split() # Transforme la ligne lue en un tableau qui regroupe l'ID, et les données
                
                tabAffiche = ["ID : ",lineTab[0]," Data : "] # Préparation pour l'affichage
                for i in range(len(lineTab)-1): # Pour chaque donnée (en excluant l'ID)
                    tabAffiche.append(lineTab[i+1]) # Ajout de la donnée dans le tableau d'affichage
                    tabAffiche.append(" ")
                print("".join(map(str, tabAffiche))) # Affichage correct de toute la data
                
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
