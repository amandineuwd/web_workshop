let socket = new WebSocket("ws://10.1.224.101:81");

socket.onopen = function() {
    console.log("WebSocket is connected.");
};

socket.onmessage = function(event) {
    console.log("Received: " + event.data);
    if (event.data.startsWith("Temperature:")) {
        document.getElementById('tempDisplay').textContent = event.data.split(":")[1] + " °C";
    }
};

document.getElementById('toggleLed').addEventListener('click', function() {
    socket.send('TOGGLE_LED');  // Envoie la commande TOGGLE_LED pour basculer l'état
    console.log('Toggled LED');
});

document.getElementById('brightness').addEventListener('input', function() {
    const brightnessValue = this.value;
    console.log('Brightness set to: ', brightnessValue);
    socket.send('BRIGHTNESS:' + brightnessValue);
});

document.getElementById('rgb').addEventListener('change', function() {
    const color = this.value;
    let colorCode;
    switch(color) {
        case 'blue': colorCode = '#0000FF'; break;
        case 'yellow': colorCode = '#FFFF00'; break;
        case 'red': colorCode = '#FF0000'; break;
    }
    console.log('Color selected: ', colorCode);
    socket.send(colorCode);  // Envoyer la couleur sélectionnée
});

document.getElementById('reset').addEventListener('click', function() {
    console.log('Reset all settings');
    socket.send('RESET');  // Si vous avez une commande RESET à implémenter sur l'ESP32
});

/*
socket.onmessage = function(event) {
    console.log("Message from ESP32: ", event.data);

    // Si le message provient du joystick
    if (event.data.startsWith("JOYSTICK:")) {
        let joystickData = event.data.split(":")[1].split(",");
        let joyX = parseInt(joystickData[0]);  // Valeur X
        let joyY = parseInt(joystickData[1]);  // Valeur Y
        let click = joystickData[2];           // État du switch (CLICK ou NO_CLICK)

        // Déplacer la souris sur la page
        moveMouse(joyX, joyY);

        // Simuler un clic si nécessaire
        if (click === "CLICK") {
            simulateClick();
        }
    }
};

// Fonction pour déplacer la souris (modifie selon la plage des valeurs du joystick)
function moveMouse(x, y) {
    let mouseX = (x / 4095) * window.innerWidth;  // Convertir les valeurs joystick X en pixels
    let mouseY = (y / 4095) * window.innerHeight; // Convertir les valeurs joystick Y en pixels

    // Déplacer la souris en utilisant CSS
    let mousePointer = document.getElementById('mousePointer');
    mousePointer.style.left = mouseX + 'px';
    mousePointer.style.top = mouseY + 'px';
}

// Fonction pour simuler un clic
function simulateClick() {
    console.log("Simulated Mouse Click");
    // Ajoute ici une logique pour déclencher un événement de clic sur la page
}
*/