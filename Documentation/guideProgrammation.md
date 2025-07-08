Ceci est le guide qui explique les différents fichiers de programmation

## Dossier "Tests RS485"
Ce dossier regroupe les fichiers de programmation qui servent à tester la communication à travers le bus RS485.  
Il y a 2 sous-dossiers : "TRANSMISSION RS485 STM32" et "LECTURE RS485 MODULE COM".  
TRANSMISSION RS485 STM32 : Regroupement des fichiers de programmation pour les STM32 des patchs (s'ouvre avec VSCode avec PlatformIO).  
LECTURE RS485 MODULE COM : Regroupement des fichiers de programmation pour l'ESP32 du module de communication (s'ouvre avec Arduino IDE).  
  
Dans ces sous-dossiers, chaque fichier de programmation correspond à une étape de test.  
_1Octet : Transmission / réception simple d'un octet  
_3Octets : Transmission / réception de 3 octets dont 2 octets d'adressage  
_Talon4Capteurs : Transmission / réception des données des 4 capteurs de force d'un patch  
_Talon5CapAcc : T/R des données des 4 capteurs de force ainsi que de l'accéléromètre dans l'IMU  
_Talon6CapAccGy : T/R des données des 4 capteurs de forces, accéléromètre et gyroscope de l'IMU  
_Talon7CapAccGyBaro : T/R de toutes les données précédentes ainsi que de la valeur brute du baromètre (pas de température ni de coefficients)  

Ces fichiers ne sont que pour la communication d'un seul patch avec le module de com, pas de toute la semelle, ils ne sont là que pour tester et comprendre la transmission et la réception de données à travers le bus RS485.  
  
## Dossier "Tests ESP NOW - PC"
Ce dossier regroupe les fichiers de programmation qui servent à tester la communication ESP NOW entre 2 ESP32 dont un qui est connecté au PC pour de la lecture via le port série.    
  
Il y a 4 fichiers de programmation : TX1, TX2, RX, RX_timestamps.  
TX1 : Code pour l'ESP32 du module de com, envoi de 32 octets dont 2 d'adressage  
TX2 : Code pour un potentiel deuxième module de com, envoi de 32 octets dont 2 d'adresses différents de ceux de TX1  
RX : Code pour l'ESP32 de la réception branché au PC, réception puis écriture sur le port série  
RX_timestamps : Code pour l'ESP32 de la réception branché au PC, écriture du temps écoulé entre 4 réception, sert à vérifier et optimiser la fréquence de transmission  

Attention : Vérifiez l'adresse MAC de l'ESP32 de réception, nécessaire pour la communication ESP NOW.  
  
## Dossier "codePlusieursPatchs"
Ce dossier regroupe le code "final" de tous les microcontrôleurs de la semelle.  
  
envoi4Patchs : Code pour chaque patch des 2 semelles, paramètrable avec le numéro du patch et l'indication de la semelle utilisée  
lecture4Patchs : Code de debug pour le module de com, lecture des données de toute la semelle puis écriture sur le port série (pas de communication ESP NOW), module branché au PC.  
TX4Patchs : Code pour les 2 modules de com, paramétrable selon la semelle utilisée  
RX4Patchs : Code pour l'ESP32 de réception branché au PC, réception puis écriture des données sur le port série  
TX_ESPNOW_vide : Code paramétrable pour les 2 modules de com, transmission de données sans semelle connectée, sert à tester la synchro des 2 semelles depuis l'ESP32 de réception.  
decodage.py : Fichier python de traitement de données, sert surtout à compter les trames pour mesurer le taux d'erreur  
decodageToCSV.py : Fichier python de traitement de données, sert à mettre en forme les données, les convertir, pour les mettre dans un fichier Excel  

