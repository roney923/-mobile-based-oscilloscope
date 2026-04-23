const dgram = require('dgram');
const WebSocket = require('ws');

// UDP server (ESP32 → server)
const udpServer = dgram.createSocket('udp4');

// WebSocket server (server → browser)
const wss = new WebSocket.Server({ port: 8080 });

console.log("🟢 Server running...");
console.log("📡 UDP listening on port 8888");

// Track last ESP32 data
let lastESP32Time = 0;

// ----------------------
// 🎭 FAKE DATA (DEMO MODE)
// ----------------------
let t = 0;

function generateFakeData() {
    t += 0.05;

    const vrms = 220 + 20 * Math.sin(t);
    const current = 5 + 2 * Math.sin(t - 0.5);
    const voltage = 12; // DC demo
    const power = vrms * current;

    return { voltage, current, power, vrms };
}

// ----------------------
// 📡 SEND TO BROWSER
// ----------------------
function broadcast(data) {
    const msg = JSON.stringify(data);

    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(msg);
        }
    });
}

// ----------------------
// 📥 RECEIVE ESP32 DATA
// ----------------------
udpServer.on('message', (msg, rinfo) => {

    console.log("RAW:", msg.toString()); // debug

    try {
        const json = JSON.parse(msg.toString());

        lastESP32Time = Date.now();

        let voltage = 0;   // DC
        let current = 0;
        let vrms = 0;      // AC

        if (json.rms !== undefined) vrms = json.rms;
        if (json.dc_v !== undefined) voltage = json.dc_v;
        if (json.current !== undefined) current = json.current;

        const power = vrms * current;

        broadcast({ voltage, current, power, vrms });

        console.log(`📥 ESP32 (${rinfo.address}): VRMS=${vrms.toFixed(2)}  I=${current.toFixed(2)}`);

    } catch (e) {
        console.log("❌ Parse error:", msg.toString());
    }
});

// Start UDP
udpServer.bind(8888);

// ----------------------
// 🔁 AUTO MODE (LIVE / DEMO)
// ----------------------
setInterval(() => {
    const now = Date.now();

    if (now - lastESP32Time > 2000) {
        const fake = generateFakeData();
        broadcast(fake);
        console.log("🎭 DEMO mode");
    } else {
        console.log("📡 LIVE mode");
    }

}, 500);

// ----------------------
// 🌐 WebSocket CONNECT
// ----------------------
wss.on('connection', () => {
    console.log("🌐 Browser connected");
});