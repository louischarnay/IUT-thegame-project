import { createApp } from 'vue'
import App from './App.vue'
import {Socket} from 'socket.io';
import VueSocketIO from 'vue-socket.io';

export const SocketInstance = Socket('http://localhost:4113');

createApp(App).mount('#app')
App.use(VueSocketIO, SocketInstance)