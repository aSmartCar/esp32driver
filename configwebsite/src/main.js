import Vue from 'vue'
import App from './App.vue'
import router from './router'
import {LayoutPlugin,ModalPlugin,NavPlugin,ButtonPlugin,SpinnerPlugin,TablePlugin, FormCheckboxPlugin, FormInputPlugin, InputGroupPlugin, FormTextareaPlugin} from 'bootstrap-vue'
// import BootstrapVue from 'bootstrap-vue'
import 'bootstrap/dist/css/bootstrap.min.css'          
import axios from 'axios'
import VueAxios from 'vue-axios'

Vue.config.productionTip = false

Vue.use(LayoutPlugin)
Vue.use(ModalPlugin)
Vue.use(NavPlugin)
Vue.use(ButtonPlugin)
Vue.use(SpinnerPlugin)
Vue.use(TablePlugin)
Vue.use(FormCheckboxPlugin)
Vue.use(FormInputPlugin)
Vue.use(InputGroupPlugin)
Vue.use(FormTextareaPlugin)

Vue.use(VueAxios, axios.create({
  baseURL:'',//set to '' before build
  withCredentials:false,
  validateStatus:(status) => {return status < 500}
}))


new Vue({
  render: h => h(App),
  router,
}).$mount('#app')
