# Smart-tracking Warehouse
This is an IOT project made for an Hackathlon organized by Codemotion.  The solution offers the possibility to monitorate the products inside a warehouse in real time, in terms of humidity,  temperature (in degrees) and gyroscope value, only using ESP32 devices (TTGO LoRa32-OLED v2.1.6). Here is how you can test the project! 


IT: 

<h3 align="center"> Simulazione monitoraggio prodotti all'interno di un magazzino</h3>

<p> <strong>Spiegazione Progetto:</strong></p>

<p>Nel problema presentato, vi è un magazzino al cui interno può anche non essere presente alcuna connessione alla rete internet. sono installati al suo interno dei dispositivi anchor per un eventuale scambio di dati. All'entrata e all'uscita è presente rispettivamente un gateway che si occupa di ricevere i dati dagli anchor e inoltrarli ad un server.</p>

<p>Il seguente progetto, dunque, prevede di monitorare in tempo reale dei prodotti che si trovano all'intterno di un magazzino. Il progetto prevede l'uso di un dispositivo BLE Tracker che simula dei valori come umidità, temperatura e stabilità (valore del giroscopio). Questo dispostivo si connetterà al dispositivo Anchor.</p>
<p>E' necessario eseguire il codice di più dispositivi Anchor se si vuole verificare l'inoltro del messaggio ai vari nodi, tramite procollo LoRa.
Il codice del Gateway si comporterà come un "bridge" che si occuperà di ricevere i messaggi dai nodi anchor ed inoltrarli al server.
Per vedere i dati dei prodotti su una dashboard, eseguire il login con le credenziali: id -> "PROVA" 
Password -> "Password"
Una volta effettuato l'accesso alla pagina sulla rete locale "dashboard.php", sarà possibile vedere l'aggiornamento</p>


<p><strong>Cosa installare:</strong></p>
<ul>
  <li>Installare XAMPP dal link -> <a href="https://www.apachefriends.org">https://www.apachefriends.org</a></li>
  <li>Aggiungere come variabile d'ambiente del sistema il path "C:\xampp\php"</li>
  <li>Scaricare l'editor Visual Studio Code al link -> <a href="https://code.visualstudio.com">https://code.visualstudio.com</a>. Al suo interno bisogna importare i codici per i dispositivi ESP.</li>
  <li>Tra le opzioni sulla sinistra, selezionare "Estensioni" ed installare PlatformIO IDE da <a href="https://platformio.org">platformio.org</a>.</li>
  <li>Nella pagina "PIO Home", importare i vari progetti per i dispositivi ESP32, selezionando come board TTGO LoRa32-OLED v2.1.6</li>
  <li>Verificare nel file "platformio.ini", la corretta installazione delle librerie in "lib_deps".</li>i>
</ul>

<p><strong>Come eseguire correttamente il progetto:<p><strong>
<ol>
  <li>Aprire il pannello di controllo di XAMPP e avviare il servizio MySQL e Apache, poi avviare il server usando l'indirizzo IP della rete a cui si è connessi.</li>
  <li>Aprire più finestre di Visual Studio con i vari codici dei dispositivi IoT.</li>
  <li>Collegare i dispositivi ESP32 e assegnare porte (es. COM4, COM5) diverse ai vari progetti.</li>
  <li>Flashare il codice del Gateway e il codice "Tracker". Successivamente eseguire il codice di almeno 2 Anchor.</li>
  <li>Flashare il codice di almeno 2 Anchor (consigliato).</li>
  <li>Controllare nel monitor seriale se Tracker e Anchor comunicano e se il Gateway riceve il messaggio.</li>
  <li>Visualizzare i risultati nella pagina Web all'apposito indirizzo IP.</li>
</ol>

<p><strong>N.B.</strong></p>
<ul>
<li>Affinché il progetto funzioni correttamente, sono necessari almeno 3 dispositivi ESP32.</li>
<li>All'interno del codice del tracker vi sono una serie di righe di codice commentate. Questo perché all'interno del progetto è stata implementata una rete neurale addestrata a riconoscere i valori anomali che protrebbero indicare se un prodotto si è danneggiato o meno. Tuttavia il modello deve essere affinato ulteriormente, quindi innanzitutto viene proposta una soluzione con semplici valori simulati.</li>
</ul>

ENG:

<h3 align="center">Simulation of Product Monitoring Inside a Warehouse</h3>

<p><strong>Project Explanation:</strong></p>

<p>In the presented scenario, there is a warehouse that may not have an internet connection. Inside, there are anchor devices installed for potential data exchange. At the entrance and exit, there are gateways that receive data from the anchors and forward it to a server.</p>

<p>The aim of this project is to monitor products in real-time within a warehouse. The project uses a BLE Tracker device that simulates values such as humidity, temperature, and stability (gyroscope value). This device will connect to the Anchor device.</p>

<p>To verify the forwarding of messages to various nodes via the LoRa protocol, it is necessary to run the code on multiple Anchor devices.
The Gateway code will act as a "bridge," receiving messages from the anchor nodes and forwarding them to the server.</p>

<p>To view the product data on a dashboard, log in with the credentials: ID -> "PROVA" Password -> "Password". Once logged in to the local network page "dashboard.php", you will be able to see the updates.</p>

<p><strong>What to Install:</strong></p>
<ul>
    <li>Install XAMPP from the link -> <a href="https://www.apachefriends.org">https://www.apachefriends.org</a></li>
    <li>Add the "C:\xampp\php" path as a system environment variable.</li>
    <li>Download Visual Studio Code editor from the link -> <a href="https://code.visualstudio.com">https://code.visualstudio.com</a>. Inside it, import the codes for ESP devices.</li>
    <li>Among the options on the left, select "Extensions" and install PlatformIO IDE from <a href="https://platformio.org">platformio.org</a>.</li>
    <li>On the "PIO Home" page, import the various projects for ESP32 devices, selecting TTGO LoRa32-OLED v2.1.6 as the board.</li>
    <li>Check in the "platformio.ini" file for the correct installation of libraries under "lib_deps."</li>
</ul>

<p><strong>How to Execute the Project Correctly:</strong></p>
<ol>
    <li>Open the XAMPP control panel and start the MySQL and Apache services, then start the server using the IP address of the connected network.</li>
    <li>Open multiple Visual Studio windows with the various IoT device codes.</li>
    <li>Connect the ESP32 devices and assign different ports (e.g., COM4, COM5) to the various projects.</li>
    <li>Flash the Gateway code and the "Tracker" code, then execute the code for at least 2 Anchors.</li>
    <li>Flash the code for at least 2 Anchors (recommended).</li>
    <li>Check in the serial monitor if the Tracker and Anchors communicate and the Gateway receives the message.</li>
    <li>View the results on the webpage at the specified IP address.</li>
</ol>

<p><strong>WARNINGS:</strong></p>
<ul>
    <li>The project needs at least 3 ESP32 devices to work correctly.</li>
    <li>Within the tracker code, there are several lines of commented-out code. This is because a neural network has been implemented in the project to recognize anomalous values that could indicate whether a product is damaged or not. However, the model needs further refinement, so initially, a solution with simple simulated values is proposed.</li>
</ul>
