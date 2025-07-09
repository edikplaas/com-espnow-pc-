import binascii
import csv
import os
import math
# Chemin vers le fichier binaire
file_path = '/home/eplanson/Documents/dataSemelle4.bin' # A adapter
TWO_POWER_65 = 36893488147419103232

# Ouvrir le fichier en mode binaire et lire son contenu
with open(file_path, 'rb') as file:
    content = file.read()

# Convertir le contenu en une représentation hexadécimale
hex_representation = binascii.hexlify(content).decode('utf-8')

# Afficher le contenu hexadécimal
lastSignalState=0
int_list = [int(hex_str, 16) for hex_str in hex_representation]

def write_list_of_lists_to_csv(list_of_lists, filepath): # Sert à convertir une liste de listes en une feuille CSV
    # Assurez-vous que le répertoire existe
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    # Ouvrir le fichier CSV en mode écriture
    with open(filepath, mode='w', newline='') as file:
        writer = csv.writer(file)
        # Parcourir chaque sous-liste et l'écrire comme une ligne dans le fichier CSV
        for sublist in list_of_lists:
            writer.writerow(sublist)

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
def getUint32bits(lst,indice):
    nb = 2**28*lst[indice]+2**24*lst[indice+1]+2**20*lst[indice+2]+2**16*lst[indice+3]+2**12*lst[indice+4]+2**8*lst[indice+5]+2**4*lst[indice+6]+lst[indice+7]
    return nb
def getForce(forceBrute,ID,numero):
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
    elif ID==9:
        NVM_PAR_T1= 27763
        NVM_PAR_T2=18983
    elif ID==11:
        NVM_PAR_T1=27750
        NVM_PAR_T2=19099
    elif ID==13:
        NVM_PAR_T1=27768
        NVM_PAR_T2=18657
    elif ID==15:
        NVM_PAR_T1=27575
        NVM_PAR_T2=18973
    
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
    elif ID==3:
        NVM_PAR_P1 = 230
        NVM_PAR_P2 = -1962
        NVM_PAR_P5 = 25549
        NVM_PAR_P6 = 30655
        NVM_PAR_P9 = 16736
        NVM_PAR_P10 = 15
    elif ID==5:
        NVM_PAR_P1 = 122
        NVM_PAR_P2 = -2459
        NVM_PAR_P5 = 25290
        NVM_PAR_P6 = 30667
        NVM_PAR_P9 = 16368
        NVM_PAR_P10 = 20
    elif ID==7:
        NVM_PAR_P1 = -181
        NVM_PAR_P2 = -2349
        NVM_PAR_P5 = 25681
        NVM_PAR_P6 = 30215
        NVM_PAR_P9 = 16210
        NVM_PAR_P10 = 23
    elif ID==9:
        NVM_PAR_P1 = 121
        NVM_PAR_P2 = -2090
        NVM_PAR_P5 = 25250
        NVM_PAR_P6 = 30200
        NVM_PAR_P9 = 16305
        NVM_PAR_P10 = 15
    elif ID==11:
        NVM_PAR_P1 = -358
        NVM_PAR_P2 = -2628
        NVM_PAR_P5 = 25841
        NVM_PAR_P6 = 30585
        NVM_PAR_P9 = 16330
        NVM_PAR_P10 = 23
    elif ID==13:
        NVM_PAR_P1 = 136
        NVM_PAR_P2 = -2770
        NVM_PAR_P5 = 25196
        NVM_PAR_P6 = 30866
        NVM_PAR_P9 = 15861
        NVM_PAR_P10 = 27
    elif ID==15:
        NVM_PAR_P1 = -8
        NVM_PAR_P2 = -2269
        NVM_PAR_P5 = 25551
        NVM_PAR_P6 = 30619
        NVM_PAR_P9 = 16334
        NVM_PAR_P10 = 15
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
    global lastSignalState
    lstEnd=[]
    ID1=lstStart[2]
    ID2=lstStart[4]
    if ID1==1 or ID1==9:
        lastSignalState=lstStart[0]
    signal=lastSignalState
    CPT=16*(lstStart[5])+(lstStart[6])
    F1=getForce(get16bits(lstStart,7),ID1,1) # ATTENTION PIED DROIT 
    F2=getForce(get16bits(lstStart,11),ID1,2)
    F3=getForce(get16bits(lstStart,15),ID1,3)
    F4=getForce(get16bits(lstStart,19),ID1,4)
    accX=get16bits(lstStart,23,False)/4096
    accY=get16bits(lstStart,27,False)/4096
    accZ=get16bits(lstStart,31,False)/4096
    gyX=get16bits(lstStart,35,False)/ 16.384
    gyY=get16bits(lstStart,39,False)/ 16.384
    gyZ=get16bits(lstStart,43,False)/ 16.384
    temp=getTemp(getUint24bits(lstStart,53),ID1)
    press=getPress(getUint24bits(lstStart,47),temp,ID1)

    x0 = getUint8bits(lstStart,59)
    x1 = getUint8bits(lstStart,61)
    y0 = getUint8bits(lstStart,63)
    y1 = getUint8bits(lstStart,65)
    z0 = getUint8bits(lstStart,67)
    z1 = getUint8bits(lstStart,69)
    xyz_ext = getUint8bits(lstStart,71)
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
    if ID1==7 or ID1==15:
        temps=getUint32bits(lstStart,73)
    else:
        temps=0
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
    lstEnd.append(temps)
    return lstEnd

entete=["Signal","ID1","ID2","CPT","F1 (N)","F2 (N)","F3 (N)","F4 (N)","accX (g)","accY (g)","accZ (g)","gyX (°/s)","gyY (°/s)","gyZ (°/s)","Press (Pa)","Temp (°C)","Norme (mT)","Temps"]
L.append(entete)
identifiants = [
    [0, 1, 0, 2],
    [0, 3, 0, 4],
    [0, 5, 0, 6],
    [0, 7, 0, 8],
    [0, 9, 0, 10],
    [0, 11, 0, 12],
    [0, 13, 0, 14],
    [0,15,1,0]
]
def recognize(list,ind): # fonction pour détecter si la trame est bonne ou non
    num=list[ind+1]
    if [int_list[i],int_list[i+1],int_list[i+2],int_list[i+3]] in identifiants:
        if (num==1 or num==3 or num==5 or num==9 or num==11 or num==13) and [int_list[i+72],int_list[i+1+72],int_list[i+2+72],int_list[i+3+72]] in identifiants:
            return True
        elif (num==7 or num==15) and [int_list[i+82],int_list[i+1+82],int_list[i+2+82],int_list[i+3+82]] in identifiants:
            return True
    else:
        return False

for i in range(len(hex_representation)-81):
    if recognize(int_list,i) == True:
        list=[]
        for j in range(81): # Pour tous les groupes de 4 bits
            list.append(int_list[i-1+j])
        list=decode(list)
        if int_list[i+1]==15:
            list[2]=16
        L.append(list)


print(len(L)) 
chemin_acces = '/home/eplanson/Documents/decodageOutput.csv'
write_list_of_lists_to_csv(L , chemin_acces)
