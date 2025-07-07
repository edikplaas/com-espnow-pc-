## Guide pour comprendre les différents bus et protocoles de communication utilisés  

=> Voir photo de chaîne de communication  

Sur chaque patch se trouve un microcontrôleur STM32, ce microcontrôleur peut communiquer vers un autre composant en liaison série avec les broches RX/TX.  
Nous utilisons cette liaison RX/TX pour convertir cette liaison série en liaison RS485 grâce à un convertisseur UART / RS485.  
Ce bus RS485 nous permet d'avoir une communication multipoint, rapide, le tout sur une longue distance.  

4 STM32 des 4 patchs ainsi que l'ESP32 du module de communication se trouvent sur le bus RS485. C'est donc 5 composants qui communiquent entre eux grâce à ce bus.  
Néamoins, il faut faire attention à comment nous programmons avec ce bus RS485, en effet quand un des composants "parle" (ou écrit) sur ce bus, il faut que tous les autres composants soient en mode "écoute".  

## Code exemple d'envoi d'un nombre (pour STM32)
``` c
LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); //DE : high, RE : low.1  
Serial.write(nombre);  
Serial.flush();  
LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3); 
```

La première ligne sert à mettre le convertisseur UART / RS485 en mode écriture (il faut veiller à ce que tous les autres composants soient en mode lecture).  
On utilise `Serial.write(nombre)` pour envoyer une donnée, ou bien `Serial.write(trame,tailleDeTrame)` pour envoyer une trame de données.  
'Serial.flush()' est utilisé pour s'assurer que les données sont envoyées sur la liaison série.  
Puis la dernière ligne sert à activer le mode lecture.  

## Alternative (pour ESP32)
```c
digitalWrite(DE_RE_PIN, HIGH); // Mode transmission  
Serial0.write(100);            // Octet spécial pour le top départ  
Serial0.flush(); // SERIAL2 POUR LE PIED DROIT ET SERIAL0 POUR LE PIED GAUCHE  
digitalWrite(DE_RE_PIN, LOW); // Mode réception  
```

## Comment les microcontrôleurs communiquent entre eux ?
Pour que le module de communication récupère toutes les données de la semelle pour ensuite les envoyer vers le PC, il envoie un signal "top départ" (ici un nombre sur un octet) sur le bus RS485, les 4 STM32 étant en mode écoute, ils commutent en mode écriture après avoir reçu le top départ.  
Mais attention : chaque STM32 possède un délai propre à leur numéro de patch, cela veut dire que c'est en premier le talon qui passe en mode écriture pour envoyer les données de la semelle, en un temps inférieur à 100µs. Après le délai imposé pour le deuxième patch, c'est à son tour d'envoyer ses données, ainsi de suite.  
Donc pour avoir une fréquence de communication de 250 Hz, le module de communication envoie son top départ toutes les 4000 µs.  

## Code d'envoi des données avec un délai propre au patch
```c
void loop() {  
  byte octet = 0;  
  while (octet != 100) { // Attente de la détection du top départ envoyé par le module de com  
    if (Serial.available()) { // Si des données disponibles  
      octet = Serial.read(); // On lit l'octet  
    }
  }
  delayMicroseconds(300 + 100 * (patch)); // Délai imposé pour respecter la commutation du mode transmission au mode réception  
  read_send_data();  
}  
```
Le module de communication, après avoir reçu les 4 trames de données (une de chaque patch), envoie d'un coup les 4 trames en sans-fil vers un autre ESP32 connecté au PC grâce au protocole ESP-NOW une fois les 4 trames reçues.    
