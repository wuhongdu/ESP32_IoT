const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8888 });

const bufferDevice = [], bufferUser = {};
let userID = null;

wss.on('connection', function connection(ws) {
  ws.on('open', () => {
    console.log('connection ... ');
  });

  ws.on('message', function incoming(message) {
    const msg = JSON.parse(message);
    switch (msg.type) {
      case 'LOGIN':
        let temp = [];
        // 登录分为两种 
        // 一种是 设备登录
        // 第二种是 用户登录 （用户登录需要考虑从数据库获取关联的设备信息）
        if (msg.deviceTYPE != 'USER') {
          // 将设备信息存储， 并通过数据库获得设备对应的用户信息
          // 然后在 用户缓存中 找到对应用户 将设备登录通知用户
          bufferDevice[msg.deviceID] = {deviceTYPE: msg.deviceTYPE, ws: ws, deviceName: msg.name};
        } else {
          // 将用户信息存储后 获取用户对应的所有设备信息 
          // 然后将在线的设备信息发送给 用户 
          userID = msg.userID;
          bufferUser[userID] = ws;
        }
        if (Object.keys(bufferDevice).length > 0) {
          for (let i in bufferDevice) {
            temp.push({deviceID: i, name: bufferDevice[i].deviceName, deviceTYPE: bufferDevice[i].deviceTYPE});
          }
        }
        
        if (Object.keys(bufferUser).length > 0 && temp.length > 0) {
          bufferUser[userID].send(JSON.stringify({ type: 'DEVICE', data: temp}));
        }

        break;
      case 'CTRL':
        const to = bufferDevice[msg.toID].ws;
        to.send(JSON.stringify({type: 'CTRL', value: msg.value}));
        break;
    }

  });

  ws.on('close', (code, reason) => {
    console.log('error code ' + code);
  });
});