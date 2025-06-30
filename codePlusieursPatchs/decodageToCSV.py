import binascii
import csv
import os
import math
# Chemin vers le fichier binaire
file_path = '/home/eplanson/Documents/dataSemelle2.bin' # A adapter

# Ouvrir le fichier en mode binaire et lire son contenu
with open(file_path, 'rb') as file:
    content = file.read()

# Convertir le contenu en une représentation hexadécimale
hex_representation = binascii.hexlify(content).decode('utf-8')

# Afficher le contenu hexadécimal

int_list = [int(hex_str, 16) for hex_str in hex_representation]

def write_list_of_lists_to_csv(list_of_lists, filepath):
    # Assurez-vous que le répertoire existe
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    # Ouvrir le fichier CSV en mode écriture
    with open(filepath, mode='w', newline='') as file:
        writer = csv.writer(file)
        # Parcourir chaque sous-liste et l'écrire comme une ligne dans le fichier CSV
        for sublist in list_of_lists:
            writer.writerow(sublist)

def combine_elements(lst): # Sert à créer une liste d'octets à partir d'une liste de 4 bits par 4 bits
    combined_list = []
    for i in range(0, len(lst) - 1, 2):
        combined_list.append(lst[i] + lst[i + 1])
    return combined_list
def hex_to_decimal(hex_str):
    # Convertir la chaîne hexadécimale en un entier décimal
    decimal_int = int(hex_str, 16)
    return decimal_int

def getUint8bits(lst,indice):
    nb = 2**4*lst[indice]+lst[indice+1]
    return nb
def get16bits(lst,indice,uint = True):
    nb = 2**12*lst[indice]+2**8*lst[indice+1]+2**4*lst[indice+2]+lst[indice+3]
    if uint == False and nb>32767:
        nb=nb-65536
    return nb
def getUint24bits(lst,indice):
    nb = 2**20*lst[indice]+2**16*lst[indice+1]+2**12*lst[indice+2]+2**8*lst[indice+3]+2**4*lst[indice+4]+lst[indice+5]
    return nb
L=[]
def decode(lstStart):
    lstEnd=[]
    signal =lstStart[0]
    ID1=lstStart[1]
    ID2=lstStart[3]
    CPT=16*(lstStart[4])+(lstStart[5])
    F1=get16bits(lstStart,6)
    F2=get16bits(lstStart,10)
    F3=get16bits(lstStart,14)
    F4=get16bits(lstStart,18)
    accX=get16bits(lstStart,22,False)
    accY=get16bits(lstStart,26,False)
    accZ=get16bits(lstStart,30,False)
    gyX=get16bits(lstStart,34,False)
    gyY=get16bits(lstStart,38,False)
    gyZ=get16bits(lstStart,42,False)
    press=getUint24bits(lstStart,46)
    x0 = getUint8bits(lstStart,52)
    x1 = getUint8bits(lstStart,54)
    y0 = getUint8bits(lstStart,56)
    y1 = getUint8bits(lstStart,58)
    z0 = getUint8bits(lstStart,60)
    z1 = getUint8bits(lstStart,62)
    xyz_ext = getUint8bits(lstStart,64)
    x_raw = (x0 * 2**10) | (x1 * 2**2) | ((xyz_ext // 2**6) & 0x03)
    y_raw = (y0 * 2**10) | (y1 * 2**2) | ((xyz_ext // 2**4) & 0x03)
    z_raw = (z0 * 2**10) | (z1 * 2**2) | ((xyz_ext // 2**2) & 0x03)
    mx = x_raw - 131072
    my = y_raw - 131072
    mz = z_raw - 131072
    fx = mx * 0.00625
    fy = my * 0.00625
    fz = mz * 0.00625
    norm = math.sqrt(fx * fx + fy * fy + fz * fz)
    lstEnd.append(signal)
    lstEnd.append(ID1)
    lstEnd.append(ID2)
    lstEnd.append(CPT)
    lstEnd.append(F1)
    lstEnd.append(F2)
    lstEnd.append(F3)
    lstEnd.append(F4)
    lstEnd.append(accX)
    lstEnd.append(accY)
    lstEnd.append(accZ)
    lstEnd.append(gyX)
    lstEnd.append(gyY)
    lstEnd.append(gyZ)
    lstEnd.append(press)
    lstEnd.append(norm)
    return lstEnd

entete=["Signal","ID1","ID2","CPT","F1","F2","F3","F4","accX","accY","accZ","gyX","gyY","gyZ","Press","Norme"]
L.append(entete)
oldSignalState=0
for i in range(len(hex_representation)-66):
    if int_list[i]==0 and int_list[i+1]==1 and int_list[i+2]==0 and int_list[i+3]==2: # On cherche une trame du patch 1
        L1=[]
        oldSignalState=int_list[i-1]
        for j in range(66): # Pour tous les groupes de 4 bits
           L1.append(hex_to_decimal(hex_representation[i+j])) 
        L1[0]=oldSignalState
        L1=decode(L1)
        L.append(L1)
    if int_list[i]==0 and int_list[i+1]==3 and int_list[i+2]==0 and int_list[i+3]==4: # On cherche une trame du patch 2
        L2=[]
        for j in range(66): # Pour tous les groupes de 4 bits
           L2.append(hex_to_decimal(hex_representation[i+j])) 
        L2[0]=oldSignalState
        L2=decode(L2)
        L.append(L2)

    if int_list[i]==0 and int_list[i+1]==5 and int_list[i+2]==0 and int_list[i+3]==6: # On cherche une trame du patch 3
        L3=[]
        for j in range(66): # Pour tous les groupes de 4 bits
           L3.append(hex_to_decimal(hex_representation[i+j])) 
        L3[0]=oldSignalState
        L3=decode(L3)
        L.append(L3)

    if int_list[i]==0 and int_list[i+1]==7 and int_list[i+2]==0 and int_list[i+3]==8: # On cherche une trame du patch 4
        L4=[]
        for j in range(66): # Pour tous les groupes de 4 bits
           L4.append(hex_to_decimal(hex_representation[i+j])) 
        L4[0]=oldSignalState
        L4=decode(L4)
        L.append(L4)

chemin_acces = '/home/eplanson/Documents/decodageOutput.csv'
write_list_of_lists_to_csv(L, chemin_acces)