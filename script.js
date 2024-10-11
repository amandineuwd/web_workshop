document.getElementById('toggleLed').addEventListener('click', function() {
    // Envoyer un signal pour allumer/éteindre la LED
    console.log('Toggle LED ON/OFF');
    // Ajouter ici la logique WebSocket pour envoyer au ESP32
});

document.getElementById('brightness').addEventListener('input', function() {
    const brightnessValue = this.value;
    console.log('Brightness set to: ', brightnessValue);
    // Envoyer la valeur d'intensité au ESP32
});

document.getElementById('rgb').addEventListener('change', function() {
    const color = this.value;
    console.log('Color selected: ', color);
    // Envoyer la couleur RGB au ESP32
});

document.getElementById('reset').addEventListener('click', function() {
    console.log('Reset all settings');
    // Envoyer une commande RESET au ESP32
});

// Simuler l'affichage de la température (à remplacer par des données réelles)
setInterval(function() {
    const temp = Math.random() * 30 + 10;  // Générer une température aléatoire
    document.getElementById('tempDisplay').textContent = temp.toFixed(1) + " °C";
}, 1000);
