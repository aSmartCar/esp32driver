import Vue from 'vue'
import Router from 'vue-router'
import Welcome from '@/components/Welcome.vue'
import WiFi from '@/components/WiFi.vue'
import Udp from '@/components/Udp.vue'
import Sensors from '@/components/Sensors.vue'
import Actuators from '@/components/Actuators.vue'
import Interfaces from '@/components/Interfaces.vue'
import Remote from '@/components/Remote.vue'
import Config from '@/components/Config.vue'

Vue.use(Router)

const router = new Router({
routes: [
    { path: '/', name: 'welcome', component: Welcome },
    { path: '/wifi', name: 'wifi', component: WiFi },
    { path: '/udp', name: 'udp', component: Udp},
    { path: '/sensors', name: 'sensors', component: Sensors },
    { path: '/actuators', name: 'actuators', component: Actuators },
    { path: '/interfaces', name: 'interfaces', component: Interfaces},
    { path: '/remote', name: 'remote', component: Remote},
    { path: '/config', name: 'config', component: Config}
  ],
  mode:'history'
})

export default router