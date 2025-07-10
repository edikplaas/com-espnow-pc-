import csv
import os
import math
import struct
# Chemin vers le fichier binaire
file_path = '/home/eplanson/Documents/dataSemelle4.bin' # A adapter
lastSignalState=0

# Tous les identifiants possibles en début de trame 
identifiantsDeTrame = [ 
    [1,2],
    [3,4],
    [5,6],
    [7,8],
    [9,10],
    [11,12],
    [13,14],
    [15,1]
]
identifiantsDeTrame=[bytes(id) for id in identifiantsDeTrame] # Adaptation des identifiants pour les octets fournis par le unpack de struct

# Coefficients pour la conversion de données brutes en données physiques
coeffsConversion = {
    "acc_xyz": 1/4096,
    "gy_xyz": 1/16.384,
    "m_xyz": 131072,
    "f_xyz": 0.00625
}

# Ouvrir le fichier en mode binaire et lire son contenu
with open(file_path, 'rb') as file:
    data_bytes = bytes(list(file.read()))

def write_csv(list_of_lists, filepath): # Sert à convertir une liste de listes en une feuille CSV consultable
    # Assurez-vous que le répertoire existe
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    # Ouvrir le fichier CSV en mode écriture
    with open(filepath, mode='w', newline='') as file:
        writer = csv.writer(file)
        # Parcourir chaque sous-liste et l'écrire comme une ligne dans le fichier CSV
        for sublist in list_of_lists:
            writer.writerow(sublist)

def getForce(forceBrute,ID,numero): # Sert à récupérer la force en newtons à partir d'une donnée brute
    global maxForce
    if ID==1 or ID>=9:
        range=100
    elif ID==3 or ID==7:
        range=50
    elif ID==5:
        if numero==4:
            range=100
        else:
            range=50
    else:
        range=0
    force = 4.44822*((forceBrute-1000)*(range/14000)/100)*range
    if force>range*4.44822:
        force=0
    if (ID==7 or ID==15) and numero == 1:
        force=0
    return force

def decode(frame): # Fonction de décodage de trame
    global lastSignalState
    unpacked_data = struct.unpack('>BBBBHHHHhhhhhhBHBHBBBBBBBI', bytes(frame[0:41])) # Récupération des données dans le bon format et dans le bon ordre
    signal,ID1, ID2, CPT, F1_raw, F2_raw, F3_raw, F4_raw, accX_raw, accY_raw, accZ_raw, gyX_raw, gyY_raw, gyZ_raw,press_raw_part1,press_raw_part2,temp_raw_part1,temp_raw_part2,x0,x1,y0,y1,z0,z1,xyz_ext,temps = unpacked_data
    if ID1==1 or ID1==9: # Le signal top départ n'est présent que en amont de la trame du talon gauche et du talon droit, dont les identifiants sont respectivement 1,2  et 9,10
        lastSignalState=signal # Mise à jour de l'état du signal top départ
    signal=lastSignalState
    forces_raw = [F1_raw,F2_raw,F3_raw,F4_raw]
    forces=[getForce(forces_raw[i],ID1,i+1) for i in range(4)]
    accXYZ_raw=[accX_raw,accY_raw,accZ_raw]
    accXYZ=[acc*coeffsConversion["acc_xyz"] for acc in accXYZ_raw]
    gyXYZ_raw=[gyX_raw,gyY_raw,gyZ_raw]
    gyXYZ=[gy*coeffsConversion["gy_xyz"] for gy in gyXYZ_raw]

    # La température et la pression brute sont établies sur 3 octets, ainsi la donnée était décomposée en entier de 8 bits (poids fort) et un entier de 16 bits
    temp_raw = temp_raw_part1*65536+temp_raw_part2 
    press_raw = press_raw_part1*65536+press_raw_part2

    xyz_raw=[(x0 * 2**10) | (x1 * 2**2) | ((xyz_ext // 2**6) & 0x03),
             (y0 * 2**10) | (y1 * 2**2) | ((xyz_ext // 2**4) & 0x03),
             (z0 * 2**10) | (z1 * 2**2) | ((xyz_ext // 2**2) & 0x03)]
    m_xyz=[xyz- coeffsConversion["m_xyz"] for xyz in xyz_raw]
    f_xyz=[m*coeffsConversion["f_xyz"] for m in m_xyz]
    norm = math.sqrt(f_xyz[0] * f_xyz[0] + f_xyz[1] * f_xyz[1] + f_xyz[2] * f_xyz[2])

    if ID1!=7 and ID1!=15:
        # Le temps écoulé entre 2 réceptions pour chacun des modules de com n'est précisé qu'en fin de trame du patch gauche et droit des orteils, dont les identifiants sont respectivement 7,8 et 15,16
        temps=0
    decodedFrame=[signal,ID1,ID2,CPT,forces[0],forces[1],forces[2],forces[3],accXYZ[0],accXYZ[1],accXYZ[2],gyXYZ[0],gyXYZ[1],gyXYZ[2],press_raw,temp_raw,norm,temps]
    return decodedFrame

def recognize(list,ind): # fonction pour détecter si la trame est bonne ou non
    ID1=list[ind]
    if list[ind:ind+2] in identifiantsDeTrame :
        if (ID1==7 or ID1==15) and list[ind+41:ind+43] in identifiantsDeTrame:
            return True
        elif (ID1!=7 and ID1!=15) and list[ind+36:ind+38] in identifiantsDeTrame:
            return True
    return False

def get_converted_data(liste):
    entete=["Signal","ID1","ID2","CPT","F1 (N)","F2 (N)","F3 (N)","F4 (N)","accX (g)","accY (g)","accZ (g)","gyX (°/s)","gyY (°/s)","gyZ (°/s)","Press brute","Temp brute","Norme (mT)","Temps"]
    L=[entete]
    for i in range(len(liste)-41): # Parcours des données
        if recognize(liste,i)==True: # Si identifiant de trame identifié et trame valable (par consultation de l'identifiant de la trame suivante), alors 
            list=[]
            for j in range(41):
                list.append(liste[i-1+j]) # Récupération des données de la trame
            list=decode(list) # Conversion des données
            if liste[i]==15: 
                list[2]=16 # Affichage correct du tout dernier identifiant 16 (car précédemment 16 était reconnu comme 0x10)
            L.append(list) # Ajout de la trame adaptée dans la liste finale
            i+=36 # Saut d'une trame entière, temps gagné
    return L
L=get_converted_data(data_bytes)
print(len(L)) # Pour vérifier le nombre de trames obtenues
chemin_acces = '/home/eplanson/Documents/decodageOutput.csv' # A adapter à vos besoins
write_csv(L , chemin_acces) 

