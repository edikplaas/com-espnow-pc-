import serial
L=[]
min = 0
max=100
import pygame
import random
import sys

# Initialiser Pygame
pygame.init()

# Définir les dimensions de la fenêtre
window_width, window_height = 800, 600
screen = pygame.display.set_mode((window_width, window_height))
pygame.display.set_caption("Cercle avec couleur aléatoire")

# Boucle principale

def read_from_serial_port(port_name='/dev/ttyACM0', baud_rate=2000000):
    global max,min
    try:
        # Ouvrir le port série avec le débit spécifié
        ser = serial.Serial(port_name, baud_rate)
        print(f"Lecture du port série {port_name} à {baud_rate} bauds...")

        while True:
            # Lire une ligne du port série
            line = ser.readline()
            try:
                # Essayer de décoder la ligne en UTF-8
                print(line.decode('utf-8').strip())
            except UnicodeDecodeError:
                # En cas d'erreur de décodage, afficher les données brutes en hexadécimal
                L = line.hex()
                if len(L)>20:
                    for i in range(len(L)-8):
                        if L[i]=="1" and L[i+2]=="2" :
                            str = L[i+5]+L[i+6]+L[i+7]+L[i+8]
                hex_parts = str.split()
                combined_hex = ''.join(hex_parts)
                # Convertir la chaîne hexadécimale combinée en un entier
                force = int(combined_hex, 16)
                if force < min :
                    min = force
                elif force > max and force<5000:
                    max=force
                if force>5000:
                    force=1000
                rouge=(255/(max-min))*force
                vert=(-255/(max-min))*force+255
                if(rouge>255):
                    rouge=255
                elif rouge<0:
                    rouge=0
                if vert>255:
                    vert=255
                elif vert<0:
                    vert=0
                screen.fill((255, 255, 255))
                # Choisir une couleur aléatoire pour le cercle
                color = (rouge, vert, 0)
                # Dessiner un cercle au centre de l'écran
                pygame.draw.circle(screen, color, (window_width // 2, window_height // 2), 50)
                # Mettre à jour l'affichage
                pygame.display.flip()

    except serial.SerialException as e:
        print(f"Erreur lors de l'ouverture du port série {port_name}: {e}")
    except KeyboardInterrupt:
        print("Arrêt de la lecture.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Port série fermé.")

if __name__ == "__main__":
    read_from_serial_port()
pygame.quit()
sys.exit()
"""

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Remplir l'écran avec une couleur de fond (blanc dans ce cas)
    screen.fill((255, 255, 255))

    # Choisir une couleur aléatoire pour le cercle
    color = (0,0,0)

    # Dessiner un cercle au centre de l'écran
    pygame.draw.circle(screen, color, (window_width // 2, window_height // 2), 50)

    # Mettre à jour l'affichage
    pygame.display.flip()


"""