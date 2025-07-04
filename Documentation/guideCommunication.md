Ceci est le guide pour comprendre les différents bus et protocoles de communication utilisés  

=> Voir photo de chaîne de communication  

Sur chacun des patchs se trouve un microcontrôleur STM32, ce microcontrôleur peut communiquer vers un autre composant en liaison série avec les broches RX/TX.  
Nous utilisons cette liaison RX/TX pour convertir cette liaison série en liaison RS485 grâce à un convertisseur.  
Ce bus RS485 nous permet d'avoir une communication multipoint, rapide et sur une longue distance.  

Sur ce bus RS485 d'une semelle entière se trouve les 4 STM32 des 4 patchs ainsi que l'ESP32 du module de communication.  
Néamoins, il faut faire attention à comment nous programmons avec ce bus RS485, en effet quand un des composants "parle" (ou écrit) sur ce bus, il faut que tous les autres composants soient en mode "écoute".

# Code exemple d'envoi d'un nombre (pour STM32)
LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); //DE : high, RE : low.1  
Serial.write(nombre);  
Serial.flush();  
LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);  

La première ligne sert à mettre le convertisseur UART / RS485 en mode écriture (il faut veiller à ce que tous les autres composants soient en mode lecture).  
On utilise 'Serial.write(nombre)' pour envoyer une donnée, ou bien 'Serial.write(trame,tailleDeTrame)' pour envoyer une trame de données.  
'Serial.flush()' est utilisé pour s'assurer que les données sont envoyées sur la liaison série.  
Puis la dernière ligne sert à activer le mode lecture.  

# Alternative (pour ESP32)
digitalWrite(DE_RE_PIN, HIGH); // Mode transmission
Serial0.write(100);            // Octet spécial pour le top départ
Serial0.flush(); // SERIAL2 POUR LE PIED DROIT ET SERIAL0 POUR LE PIED GAUCHE
digitalWrite(DE_RE_PIN, LOW); // Mode réception

