# Smart-tracking Warehouse
This is an IOT project made for an Hackathlon organized by Codemotion.  The solution offers the possibility to monitorate the products inside a warehouse in real time, in terms of humidity,  temperature (in degrees) and gyroscope value, only using ESP32 devices (TTGO LoRa32-OLED v2.1.6). Here is how you can test the project! 


IT: 

		#Simulazione monitoraggio prodotti all'interno di un magazzino

Spiegazione Progetto:

Nel problema presentato, vi è un magazzino al cui interno può anche non essere presente alcuna connessione alla rete internet. sono installati al suo interno dei dispositivi anchor per un eventuale scambio di dati. All'entrata e all'uscita è presente rispettivamente un gateway che si occupa di ricevere i dati dagli anchor e inoltrarli ad un server.

Il seguente progetto, dunque, prevede di monitorare in tempo reale dei prodotti che si trovano all'intterno di un magazzino. Il progetto prevede l'uso di un dispositivo BLE Tracker che simula dei valori come umidità, temperatura e stabilità (valore del giroscopio). Questo dispostivo si connetterà al dispositivo Anchor.
E' necessario eseguire il codice di più dispositivi Anchor se si vuole verificare l'inoltro del messaggio ai vari nodi, tramite procollo LoRa.
Il codice del Gateway si comporterà come un "bridge" che si occuperà di ricevere i messaggi dai nodi anchor ed inoltrarli al server.
Per vedere i dati dei prodotti su una dashboard, eseguire il login con le credenziali: id -> "PROVA" 
Password -> "Password"
Una volta effettuato l'accesso alla pagina sulla rete locale "dashboard.php", sarà possibile vedere l'aggiornamento 


Cosa installare:
-Installare XAMPP dal link-> https://www.apachefriends.org
-Aggiungere come variabile d'ambiente del sistema il path "C:\xampp\php"
-Scaricare l'editor Visual Studio Code al link -> https://code.visualstudio.com
Al suo interno bisogna importare i codici per i dispositivi ESP.
-Tra le opzioni sulla sinistra, selezionare "Estensioni" ed installare PlatformIO IDE da platformio.org.
-Nella pagina "PIO Home", importare i vari progetti per i dispositivi ESP32, selezionando come board TTGO LoRa32-OLED v2.1.6
-Verificare nel file "platformio.ini", la corretta installazione delle librerie in "lib_deps".


Come eseguire correttamente il progetto:

Step 1: aprire il pannello di controllo di xampp per e avviare il servizio MYSQL e Apache, dopo di che avviare il server usando l'indirizzo ip della rete a cui si è connessi.
Step 2: aprire più finestre di visual studio con i vari codici dei dispositivi iot.
Step 3: collegare i dispostivi ESP32 e assegnare porte (es. COM4, COM5) diverse ai vari progetti.
Step 4: flashare il codice del Gateway ed il codice "Tracker". successivamente eseguire il codice di almeno 2 Anchor.
Step 5: flashare il codice di almeno 2 Anchor (consigliato).
Step 6: controllare nel monitor seriale se Tracker e Anchor comunicano ed il Gateway riceve il messaggio.
Step 7: visualizzare i risultati nella pagina Web all'apposito indirizzo ip.

N.B.
-Affinché il progetto funzioni correttamente, sono necessari almeno 3 dispositivi ESP32.
-All'interno del codice del tracker vi sono una serie di righe di codice commentate. Questo perché all'interno del progetto è stata implementata una rete neurale addestrata a riconoscere i valori anomali che protrebbero indicare se un prodotto si è danneggiato o meno. Tuttavia il modello deve essere affinato ulteriormente, quindi innanzitutto viene proposta una soluzione con semplici valori simulati.


ENG:

		#Simulation of Product Monitoring Inside a Warehouse

Project Explanation:

In the presented scenario, there is a warehouse that may not have an internet connection. Inside, there are anchor devices installed for potential data exchange. At the entrance and exit, there are gateways that receive data from the anchors and forward it to a server.

The aim of this project is to monitor products in real-time within a warehouse. The project uses a BLE Tracker device that simulates values such as humidity, temperature, and stability (gyroscope value). This device will connect to the Anchor device.
To verify the forwarding of messages to various nodes via the LoRa protocol, it is necessary to run the code on multiple Anchor devices.
The Gateway code will act as a "bridge," receiving messages from the anchor nodes and forwarding them to the server.
To view the product data on a dashboard, log in with the credentials: ID -> "PROVA" Password -> "Password". Once logged in to the local network page "dashboard.php", you will be able to see the updates.


What to Install:

-Install XAMPP from the link -> https://www.apachefriends.org
-Add the "C:\xampp\php" path as a system environment variable.
-Download Visual Studio Code editor from the link -> https://code.visualstudio.com
Inside it, import the codes for ESP devices.
-Among the options on the left, select "Extensions" and install PlatformIO IDE from platformio.org.
-On the "PIO Home" page, import the various projects for ESP32 devices, selecting TTGO LoRa32-OLED v2.1.6 as the board.
-Check in the "platformio.ini" file for the correct installation of libraries under "lib_deps."


How to Execute the Project Correctly:

Step 1: Open the XAMPP control panel and start the MYSQL and Apache services, then start the server using the IP address of the connected network.
Step 2: Open multiple Visual Studio windows with the various IoT device codes.
Step 3: Connect the ESP32 devices and assign different ports (e.g., COM4, COM5) to the various projects.
Step 4: Flash the Gateway code and the "Tracker" code, then execute the code for at least 2 Anchors.
Step 5: Flash the code for at least 2 Anchors (recommended).
Step 6: Check in the serial monitor if the Tracker and Anchors communicate and the Gateway receives the message.
Step 7: View the results on the webpage at the specified IP address.

WARNINGS:
-The project needs at least 3 ESP32 devices to work correctly. 
-Within the tracker code, there are several lines of commented-out code. This is because a neural network has been implemented in the project to recognize anomalous values that could indicate whether a product is damaged or not. However, the model needs further refinement, so initially, a solution with simple simulated values is proposed.
