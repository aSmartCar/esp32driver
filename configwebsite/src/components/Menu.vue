<template>
  <div class="Menu">
    <center>
      <div class="mx-auto" style="max-width:800px;">
        <b-container class="bv-row mt-5">
          <b-row class="justify-content-md-center">
            <b-col md="auto"><img src="../assets/logo.png" width="100" sytle="padding:0; margin:0;"/></b-col>
            <b-col md="auto"><h1 sytle="padding:0; margin:0;">ESP32-Driver Board</h1></b-col>
          </b-row>
        </b-container>        
      </div>
    </center>
    <div class="container" id="menu_container">
      <b-nav tabs align="center">
        <b-nav-item v-bind:class="{'active':isActive.welcome}">
          <router-link to="/">Welcome</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.wifi}">
          <router-link to="/wifi">WiFi</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.udp}">
          <router-link to="/udp">UDP</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.remote}">
          <router-link to="/remote">Remote Control</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.sensors}">
          <router-link to="/sensors">Sensors</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.actuators}">
          <router-link to="/actuators">Actuators</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.interfaces}">
          <router-link to="/interfaces">Interfaces</router-link>
        </b-nav-item>
        <b-nav-item v-bind:class="{'active':isActive.config}">
          <router-link to="/config">Config</router-link>
        </b-nav-item>
      </b-nav>
    </div>
  </div>
</template>

<script>
export default {
  name: "Menu",
  data() {
    return {
      isActive: {
        welcome: false,
        wifi: false,
        udp: false,
        sensors: false,
        actuators: false,
        interfaces: false,
        remote: false,
        config: false
      }
    };
  },
  created() {
    for (const key in this.isActive) {
      this.isActive[key] = false;
    }

    switch (this.$route.name) {
      case "welcome": {
        this.isActive.welcome = true;
        break;
      }
      case "wifi": {
        this.isActive.wifi = true;
        break;
      }
      case "sensors": {
        this.isActive.sensors = true;
        break;
      }
      case "actuators": {
        this.isActive.actuators = true;
        break;
      }
      case "interfaces": {
        this.isActive.interfaces = true;
        break;
      }
      case "config": {
        this.isActive.config = true;
        break;
      }
      default: {
        break;
      }
    }
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
