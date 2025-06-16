import binascii
# Chemin vers le fichier binaire
file_path = '/home/eplanson/Documents/dataSemelle2.bin'

# Ouvrir le fichier en mode binaire et lire son contenu
with open(file_path, 'rb') as file:
    content = file.read()

# Convertir le contenu en une représentation hexadécimale
hex_representation = binascii.hexlify(content).decode('utf-8')

# Afficher le contenu hexadécimal

int_list = [int(hex_str, 16) for hex_str in hex_representation]

t1=0
t2=0
t3=0
t4=0


def combine_elements(lst):
    combined_list = []
    for i in range(0, len(lst) - 1, 2):
        combined_list.append(lst[i] + lst[i + 1])
    return combined_list


for i in range(len(hex_representation)-3):
    if int_list[i]==0 and int_list[i+1]==1 and int_list[i+2]==0 and int_list[i+3]==2:
        L=[]
        for j in range(46):
            L.append(hex_representation[i+j])
        print(combine_elements(L))
        t1+=1
    if int_list[i]==0 and int_list[i+1]==3 and int_list[i+2]==0 and int_list[i+3]==4:
        L=[]
        for j in range(46):
            L.append(hex_representation[i+j])
        print(combine_elements(L))
        t2+=1
    if int_list[i]==0 and int_list[i+1]==5 and int_list[i+2]==0 and int_list[i+3]==6:
        L=[]
        for j in range(46):
            L.append(hex_representation[i+j])
        print(combine_elements(L))
        t3+=1
    if int_list[i]==0 and int_list[i+1]==7 and int_list[i+2]==0 and int_list[i+3]==8:
        L=[]
        for j in range(46):
            L.append(hex_representation[i+j])
        print(combine_elements(L))
        t4+=1
    
print(t1)
print(t2)
print(t3)
print(t4)
listeT=[t1,t2,t3,t4]
print(max(listeT)-min(listeT))
print((1-min(listeT)/max(listeT))*100)

