Ceci est le guide qui explique l'acquisition des données d'une ou plusieurs semelles
  
## Etapes
  
Pour acquérir les données des semelles, procédez de cette manière :  
1. Allumer les 2 semelles (peu importe l'ordre)  
2. Brancher au PC l'ESP32 de réception (privilégiez un cable court)  
3. Ecrivez la commande d'acquisition des données dans un terminal (voir partie commandes)  
4. Attendre que les données s'affichent (< 5 secondes)  
5. Faites CTRL+A puis CTRL+Q dans le terminal pour arrêter l'acquisition des données  
6. Convertissez vos données en lançant le programme python decodageToCSV.py (voir partie commandes)  

## Commandes
  
Installez picocom  
Dans un terminal, allez de préférence dans /com-espnow-pc/codePlusieursPatchs/  
Ecrivez la ligne suivante : `picocom -b 921600 /dev/ttyUSB0 --log dataSemelle.bin`  
`921600` correspond au baud rate, mettez celui présent dans le code RX4Patchs (921600 pour l'ESP WROOM 32 et 2000000 pour l'ESP32C3).  
`/dev/ttyUSB0` est le port USB utilisé. Si vous cherchez le port utilisé par l'ESP32 de réception, faites cette commande après avoir branché l'ESP32 : `ls /dev/ttyACM* /dev/ttyUSB**`  
`dataSemelle.bin` est le fichier binaire créé qui contient toutes les données à acquérir, vous pouvez le renommer comme vous voulez.  
Faites CTRL+A puis CTRL+Q dans le terminal pour arrêter l'acquisition des données.  
  
Pour convertir les données acquises, executez le programme decodageToCSV.py dans le terminal.  
L'appel possède des arguments : input_file et output_file qui correspondent au fichier d'entrée et au fichier de sortie respectivement.  
Exemple d'appel : `python3 decodageToCSV.py dataSemelleEXEMPLE.bin feuilleOutputEXEMPLE.csv`  
Faites attention au chemin du fichier, dans l'exemple le fichier d'entrée est situé dans le même dossier que le programme python, et le fichier de sortie est créé dans le même dossier.  

## Bugs
  
Si jamais l'acquisition des données ne se fait pas, vérifiez le baud rate utilisé en consultant le programme `RX4Patchs`, vous pouvez également vérifier le niveau de charge des batteries.  
Si des pauses apparaissent dans l'acquisition des données, baissez la fréquence.  
Si des trames n'apparaissent finalement pas dans la feuille .CSV finale (par exemple les orteils gauche), c'est peut être qu'un patch s'est déssoudé.  