var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://192.168.0.216')
client.on('connect', function () {
    client.subscribe('home/bathroom-1/state')
})
client.on('message', function (topic, message) {
    context = message.toString();
    console.log(topic, context)
})