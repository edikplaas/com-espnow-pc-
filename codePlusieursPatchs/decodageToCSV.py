import binascii
import csv
import os
import math
# Chemin vers le fichier binaire
file_path = '/home/eplanson/Documents/dataSemelle2.bin' # A adapter
TWO_POWER_65 = 36893488147419103232
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
maxForce=0
def getForce(forceBrute,ID,numero):
    global maxForce
    if ID==1:
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
    if force>maxForce and ID==1 and numero==1:
        maxForce=force
    return force
def getTemp(rawTemp,ID):
    NVM_PAR_T3=-12
    if ID==1:
        NVM_PAR_T1= 27590
        NVM_PAR_T2=18750
    elif ID==3:
        NVM_PAR_T1=27662
        NVM_PAR_T2=18973
    elif ID==5:
        NVM_PAR_T1=27511
        NVM_PAR_T2=18794
    elif ID==7:
        NVM_PAR_T1=27807
        NVM_PAR_T2=19371
    PAR_T1 = NVM_PAR_T1 * (1 << 8)
    PAR_T2 = NVM_PAR_T2 / (1 << 30)
    PAR_T3 = NVM_PAR_T3 / (1 << 48)
    partial_data1_temp = rawTemp - PAR_T1
    partial_data2_temp = partial_data1_temp * PAR_T2
    temp = partial_data2_temp + (partial_data1_temp * partial_data1_temp) * PAR_T3
    return temp

def getPress(rawPress,temp,ID):
    NVM_PAR_P1 = 230
    NVM_PAR_P2 = -1962
    NVM_PAR_P5 = 25549
    NVM_PAR_P6 = 30655
    NVM_PAR_P9 = 16736
    NVM_PAR_P10 = 15
    NVM_PAR_P3 = 30
    NVM_PAR_P4 = -3
    
    NVM_PAR_P7 = -13
    NVM_PAR_P8 = 3
    
    NVM_PAR_P11 = -59
    if ID==1:
        return 0
    elif ID==2:
        NVM_PAR_P1 = 230
        NVM_PAR_P2 = -1962
        NVM_PAR_P5 = 25549
        NVM_PAR_P6 = 30655
        NVM_PAR_P9 = 16736
        NVM_PAR_P10 = 15
    elif ID==3:
        NVM_PAR_P1 = 122
        NVM_PAR_P2 = -2459
        NVM_PAR_P5 = 25290
        NVM_PAR_P6 = 30667
        NVM_PAR_P9 = 16368
        NVM_PAR_P10 = 20
    elif ID==4:
        NVM_PAR_P1 = -181
        NVM_PAR_P2 = -2349
        NVM_PAR_P5 = 25681
        NVM_PAR_P6 = 30215
        NVM_PAR_P9 = 16210
        NVM_PAR_P10 = 23
    PAR_P1 = (NVM_PAR_P1 - float(1 << 14)) / float(1 << 20)
    PAR_P2 = (NVM_PAR_P2 - float(1 << 14)) / float(1 << 29)
    PAR_P3 = NVM_PAR_P3 / float(1 << 32)
    PAR_P4 = NVM_PAR_P4 / float(1 << 37)
    PAR_P5 = NVM_PAR_P5 * float(1 << 3)
    PAR_P6 = NVM_PAR_P6 / float(1 << 6)
    PAR_P7 = NVM_PAR_P7 / float(1 << 8)
    PAR_P8 = NVM_PAR_P8 / float(1 << 15)
    PAR_P9 = NVM_PAR_P9 / float(1 << 48)
    PAR_P10 = NVM_PAR_P10 / float(1 << 48)
    PAR_P11 = NVM_PAR_P11 / TWO_POWER_65

    # Application de la compensation
    partial_data1 = PAR_P6 * temp
    partial_data2 = PAR_P7 * temp * temp
    partial_data3 = PAR_P8 * temp * temp * temp
    partial_out1 = PAR_P5 + partial_data1 + partial_data2 + partial_data3

    partial_data1 = PAR_P2 * temp
    partial_data2 = PAR_P3 * temp * temp
    partial_data3 = PAR_P4 * temp * temp * temp
    partial_out2 = rawPress * PAR_P1 + partial_data1 + partial_data2 + partial_data3

    partial_data1 = rawPress * rawPress
    partial_data2 = PAR_P9 + PAR_P10 * temp
    partial_data3 = partial_data1 * partial_data2
    partial_data4 = partial_data3 + rawPress * rawPress * rawPress * PAR_P11

    press = partial_out1 + partial_out2 + partial_data4
    return press
    
L=[]
def decode(lstStart):
    lstEnd=[]
    signal =lstStart[0]
    ID1=lstStart[1]
    ID2=lstStart[3]
    CPT=16*(lstStart[4])+(lstStart[5])
    F1=getForce(get16bits(lstStart,6),ID1,1)
    F2=getForce(get16bits(lstStart,10),ID1,2)
    F3=getForce(get16bits(lstStart,14),ID1,3)
    F4=getForce(get16bits(lstStart,18),ID1,4)
    accX=get16bits(lstStart,22,False)/4096
    accY=get16bits(lstStart,26,False)/4096
    accZ=get16bits(lstStart,30,False)/4096
    gyX=get16bits(lstStart,34,False)/ 16.384
    gyY=get16bits(lstStart,38,False)/ 16.384
    gyZ=get16bits(lstStart,42,False)/ 16.384
    temp=getTemp(getUint24bits(lstStart,52),ID1)
    press=getPress(getUint24bits(lstStart,46),temp,ID1)
    x0 = getUint8bits(lstStart,58)
    x1 = getUint8bits(lstStart,60)
    y0 = getUint8bits(lstStart,62)
    y1 = getUint8bits(lstStart,64)
    z0 = getUint8bits(lstStart,66)
    z1 = getUint8bits(lstStart,68)
    xyz_ext = getUint8bits(lstStart,70)
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
    lstEnd.append(temp)
    lstEnd.append(norm)
    return lstEnd

entete=["Signal","ID1","ID2","CPT","F1 (N)","F2 (N)","F3 (N)","F4 (N)","accX (g)","accY (g)","accZ (g)","gyX (°/s)","gyY (°/s)","gyZ (°/s)","Press (Pa)","Temp (°C)","Norme (mT)"]
L.append(entete)
oldSignalState=0
for i in range(len(hex_representation)-72):
    if int_list[i]==0 and int_list[i+1]==1 and int_list[i+2]==0 and int_list[i+3]==2: # On cherche une trame du patch 1
        L1=[]
        oldSignalState=int_list[i-1]
        for j in range(72): # Pour tous les groupes de 4 bits
           L1.append(hex_to_decimal(hex_representation[i+j])) 
        L1[0]=oldSignalState
        L1=decode(L1)
        L.append(L1)
    if int_list[i]==0 and int_list[i+1]==3 and int_list[i+2]==0 and int_list[i+3]==4: # On cherche une trame du patch 2
        L2=[]
        for j in range(72): # Pour tous les groupes de 4 bits
           L2.append(hex_to_decimal(hex_representation[i+j])) 
        L2[0]=oldSignalState
        L2=decode(L2)
        L.append(L2)

    if int_list[i]==0 and int_list[i+1]==5 and int_list[i+2]==0 and int_list[i+3]==6: # On cherche une trame du patch 3
        L3=[]
        for j in range(72): # Pour tous les groupes de 4 bits
           L3.append(hex_to_decimal(hex_representation[i+j])) 
        L3[0]=oldSignalState
        L3=decode(L3)
        L.append(L3)
    if int_list[i]==0 and int_list[i+1]==7 and int_list[i+2]==0 and int_list[i+3]==8: # On cherche une trame du patch 4
        L4=[]
        for j in range(72): # Pour tous les groupes de 4 bits
           L4.append(hex_to_decimal(hex_representation[i+j])) 
        L4[0]=oldSignalState
        L4=decode(L4)
chemin_acces = '/home/eplanson/Documents/decodageOutput.csv'
write_list_of_lists_to_csv(L, chemin_acces)