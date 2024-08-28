<?php
    
    // Connessione al database
    $dbHost = 'localhost';
    $dbUsername = 'root';
    $dbPassword = '';
    $dbName = 'iot project';
   
   $conn = new mysqli($dbHost, $dbUsername, $dbPassword, $dbName);

    // Verifica della connessione
    if ($conn->connect_error) {
        die("Connessione fallita: " . $conn->connect_error);
    }

    // Se il form è stato inviato
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Prendi i valori immessi dall'utente
        $ID_cod = $_POST['ID_cod'];
        $password = $_POST['Password'];

        // Query per cercare l'ID Operatore nel database
        $sql = "SELECT * FROM users WHERE ID_cod = '$ID_cod'";
        $result = $conn->query($sql);

        // Verifica se l'utente è stato trovato nel database
        if ($result->num_rows > 0) {
            // L'ID Operatore è stato trovato, ora verifichiamo la password
            $row = $result->fetch_assoc();
            $stored_password = $row['Password'];

             // Verifica la password
            //if (password_verify($password, $stored_password)) {
            if ($password === $stored_password) {
                // Password corretta, l'utente è autenticato
                // Avvia la sessione e memorizza l'ID Operatore
                session_start();
                $_SESSION['ID_cod'] = $row['ID_cod']; // Supponendo che 'ID_cod' sia l'ID che vuoi memorizzare in sessione
                // Reindirizza alla dashboard
                header("Location: dashboard.php");
                exit();
            } else {
                // Password errata
                require_once("index.php");
                echo "Password errata. Riprova.";
            }
        } else {
            // L'ID Operatore non è stato trovato nel database
            echo "ID Operatore non trovato.";
        }
    } 

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Ricezione dei dati dalla richiesta HTTP
        $data = file_get_contents('php://input');
        //$data = "2:1:90: 37.0:120";

        
        if(!empty($data)){

            $pattern = explode(":", $data);


            $insert = "INSERT INTO bracelet_data (anchor_id, humidity, temperature, gyroscope, high_values) VALUES ('" . $pattern[0] . "', '" . $pattern[1] . "', '" . $pattern[2] . "', '" . $pattern[3] . "', '" . $pattern[4] . "')";

            if ($conn->query($insert) === TRUE) {
                echo "<p>Dati inseriti correttamente nel database</p><br>";
            } else {
                echo "<p>Errore durante l'inserimento dei dati nel database: " . $conn->error . "</p><br>";
            }

        }else{
            echo "<p>Nessun dato ricevuto</p><br>";
        }  
    }
?>


<!DOCTYPE html>
<html>
<head>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <title>Dashboard</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #e5f2f6;
            overflow-x: hidden;
        }

        #logo {
            width: 7%;
            height: 7%;
            background-image: url("Images/Enel_X_Logo_Violet_RGB.png"); /* Inserisci qui l'URL del logo dell'azienda */
            background-size: cover;
            background-position: center;
            position: absolute;
            top: 0;
            left: 1%;
        }

        #menu {
            width: 150px;
            height: 30px;
            border: 1px solid #007799;
            border-radius: 5px;
            background-color: rgba(255,255,255,0.7);
            color: #007799;
            font-size: 18px;
            position: absolute;
            top: 120px;
            left: 25px;
        }

        #dati {
            width: 80%;
            height: 60%;
            border: 2px solid #007799;
            border-radius: 10px;
            background-color: #ffffff;
            box-shadow: 5px 5px 10px rgba(0, 0, 0, 0.1);
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            overflow: auto; /* Aggiunto per abilitare lo scrolling */
        }
        #opzioni{
            width: 30%;
            height: 30%;
            border: 2px solid #007799;
            border-radius: 10px;
            background-color: #ffffff;
            box-shadow: 5px 5px 10px rgba(0, 0, 0, 0.1);
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            overflow: auto; /* Aggiunto per abilitare lo scrolling */
        }

        #dati h2 {
            text-align: center;
            color: #007799;
        }

        .dati-row {
            display: flex;;
            margin-bottom: 10px;
        }

        .dati-item {
            flex-direction: row;
            flex: 1 0  3%; /*almeno due elementi per riga */
            margin-right: 0;
            padding: 5px;
            font-weight: bold;
            background-color: rgba(255,255,250,0.2);
            margin-top: 1%;
            margin-left: 2%;
            color: #007799;
            position: sticky;
            top: 0;
        }

        .dati-elem {
            flex-direction: column;
            flex: 1 0  3%; 
            font-weight: bold;
            background-color: rgba(255,255,250,0.2);
            padding: 5px;
            margin-top: 1%;
            margin-left: 2%;
            margin-bottom: 15px;
            color: #045c9c;
        } 

        #opzioni h2 {
            text-align: center;
            margin-bottom: 15px;
            color: #007799;
        }

        #opzioni button {
            width: 200px;
            height: 50px;
            border: 1px solid #007799;
            border-radius: 5px;
            background-color: #ffffff;
            color: #007799;
            font-size: 18px;
            cursor: pointer;
            margin: 20px auto;
            display: block;
        }

        #opzioni button:hover {
            background-color: #007799;
            color: #ffffff;
            transition: 0.3s;
        }

        .Refresh {
            text-align: center;
            margin-top: 20px;
        }

        .Refresh button {
            width: 100px;
            height: 50px;
            border: 1px solid #007799;
            border-radius: 5px;
            background-color: #ffffff;
            color: #007799;
            font-size: 18px;
            cursor: pointer;
            margin: 20px auto;
        }

        .Refresh button:hover {
            background-color: #007799;
            color: #ffffff;
            transition: 0.3s;
        }

        p {
            margin-left: 80%;
            font-weight: bold;
            color: #007799;
        }

        @media only screen and (max-width: 500px){
            p {
            margin-left: 50%;
        }
        }
    </style>
</head>
<body>
    <select id="menu">
        <option value="operatore">Prodotto</option>
        <option value="opzioni">Opzioni</option>
    </select>
    <div id="dati">
        <h2>Dati Prodotto</h2>
        <div class="dati-row">
            <div class="dati-item">N°:</div>
            <div class="dati-item">Anchor ID:</div>
            <div class="dati-item">Humidity:</div>
            <div class="dati-item">Temperature:</div>
            <div class="dati-item">Stability:</div>
        </div>
        <?php
        $select = "SELECT * FROM bracelet_data";
        $result = $conn->query($select);
        if($result->num_rows > 0){
            while($row = $result->fetch_assoc()){
                echo "<div class='dati-row'>";

                if($row["high_values"]  == "1"){
                    echo "<div class='dati-elem' style='color: red'>" . $row["id"] . "</div>";
                    echo "<div class='dati-elem' style='color: red'>" . $row["anchor_id"] . "</div>";
                    echo "<div class='dati-elem' style='color: red'>" . $row["humidity"] . "</div>";
                    echo "<div class='dati-elem' style='color: red'>" . $row["temperature"] . "</div>";
                    echo "<div class='dati-elem' style='color: red'>" . $row["gyroscope"] . "</div>";
                    }
                    else{
                        echo "<div class='dati-elem'>" . $row["id"] . "</div>";
                        echo "<div class='dati-elem'>" . $row["anchor_id"] . "</div>";
                        echo "<div class='dati-elem'>" . $row["humidity"] . "</div>";
                        echo "<div class='dati-elem'>" . $row["temperature"] . "</div>";
                        echo "<div class='dati-elem'>" . $row["gyroscope"] . "</div>";
                    }
        
                echo "</div>";
            }
        }

        // Query per eliminare i dati più vecchi della data limite
        // $sql = "DELETE FROM bracelet_data";
        // if ($conn->query($sql) === TRUE) {
        //     echo "<p>Dati vecchi eliminati con successo.</p>";
        // } else {
        //     echo "Errore durante l'eliminazione dei dati vecchi: " . $conn->error;
        // }
        ?>
    </div>
        <div id="opzioni" style="display: none;">
            <h2>Opzioni</h2>
            <button onclick="cambiaLingua()">Cambia Lingua</button>
            <button onclick="contattaSicurezza()">Segnala un problema</button>
        </div>
    
    <div class="Refresh">
        <a href="<?php echo htmlspecialchars($_SERVER['PHP_SELF']); ?>"><button>Refresh</button></a>
    </div>


    <script>
        $(document).ready(function(){
            $('#menu').change(function(){
                if($(this).val() == 'operatore'){
                    $('#dati').show();
                    $('.Refresh').show();
                    $('#opzioni').hide();
                }else if($(this).val() == 'opzioni'){
                    $('#dati').hide();
                    $('.Refresh').hide();
                    $('#opzioni').show();
                }
            });
        });

        function cambiaLingua(){
            // Da fare se abbiamo tempo
        }

        function contattaSicurezza(){
            // Da fare se abbiamo tempo
        }
    </script>
    
</body>
</html>