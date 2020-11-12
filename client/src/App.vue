<template>
  <div id="app">
    <div v-if="deviceList.length <= 0" class="device-item">
      还没有添加的设备或者没有在线设备
    </div>
    <template v-else>
      <div v-for="(item,index) in deviceList" :key="index" class="device-item">
        <div>{{ item.name }}</div>
        <el-switch
          v-model="value"
          active-color="#13ce66"
          inactive-color="#808080"
          active-value="2"
          inactive-value="1"
          @change="handleTurn(item.deviceID)" />
      </div>
    </template>
  </div>
</template>
<script>
export default {
  data () {
    return {
      value: '1',
      loginStatus: false,
      deviceList: [],
      ws: null
    }
  },
  created () {
    this.initWS();
  },
  destroyed () {
    this.ws.close();
  },
  methods: {
    initWS () {
      const url = 'ws://192.168.1.108:8888/'; // 设置 websocket 服务器的ip地址和端口
      this.ws = new WebSocket(url);
      this.ws.onopen = this.handleOpen;
      this.ws.onmessage = this.handleMessage;
      this.ws.onclose = this.handleClose;
    },
    handleTurn (id) {
      const message = { type: 'CTRL', toID: id, value: this.value };
      this.handleSend(JSON.stringify(message));
    },
    handleOpen (evt) {
      console.log('Connection open ... ');
      const login = JSON.stringify({ type: 'LOGIN', deviceTYPE: 'USER', userID: '1b2j3n2g3hk1' })
      this.handleSend(login);
    },
    handleMessage (event) {
      const message = JSON.parse(event.data);
      switch (message.type) {
        case 'DEVICE':
          this.deviceList = message.data;
          break;
      }
    },
    handleSend (data) {
      this.ws.send(data);
    },
    handleClose (event) {
      console.log('close : ' + event);
    }
  }
}
</script>

<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}
html,body {
  width: 100%;
  height: 100%;
}
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
.device-item{
  width: 100%;
  height: 3rem;
  border-bottom: 1px solid #999999;
  display: flex;
  flex-wrap: wrap;
  justify-content: space-around;
  align-items: center;
  background-color: #dddddd;
}
</style>
