var mqtt = require('mqtt');
var client  = mqtt.connect('mqtt://192.168.0.216');
client.on('connect', function () {
    setInterval(function() {
        client.publish('home/bathroom-1/config', JSON.stringify({test: 1}));
        console.log('Message Sent');
    }, 5000);
});