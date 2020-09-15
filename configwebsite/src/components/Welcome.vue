<template>
  <div class="Welcome">
    <menuComponent />
    <div class="container text-center mt-3">
      <h3>ESP32-Driver configuration website</h3>
      <b-button class="mt-4 mx-auto w-50" block variant="danger" @click="reset">RESET</b-button>
      <div class="text-center mt-4">
        <b-spinner
          v-show="loading"
          style="width: 5rem; height: 5rem;"
          variant="success"
          label="Loading..."
        ></b-spinner>
      </div>
      <div class="mt-4 mx-auto w-50" v-show="!loading" v-if="!loading">
        <h3>Staus</h3>
        <p></p>
        <h4>WiFi</h4>

        <b-row>
          <b-col sm="5">Current used WiFi:</b-col>
          <b-col sm="5">{{config.wifi.SSID}}</b-col>
        </b-row>
        <b-row>
          <b-col sm="5">Access Point Enabled:</b-col>
          <b-col sm="5">{{config.wifi.enableAP}}</b-col>
        </b-row>
        <b-row>
          <b-col sm="5">Access Point SSID:</b-col>
          <b-col sm="5">{{config.wifi.APSSID}}</b-col>
        </b-row>
        <b-row>
          <b-col sm="5">Access Point Password:</b-col>
          <b-col sm="5">{{config.wifi.APPass}}</b-col>
        </b-row>

        <h4>Sensors</h4>
        <b-container class="mt-4 mx-auto">
          <b-row class="mb-2">
            <b-col>Sensor</b-col>
            <b-col>Value</b-col>
            <b-col>Status</b-col>
            <b-col>Sensor</b-col>
            <b-col>Value</b-col>
            <b-col>Status</b-col>
          </b-row>
          <b-row>
            <b-col>Akku 0:</b-col>
            <b-col v-if="stats.akku0" v-text="stats.akku0"></b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.akku0.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>Sharp 0:</b-col>
            <b-col v-if="stats.sharp0">{{stats.sharp0}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.sharp0.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col>Akku 1:</b-col>
            <b-col v-if="stats.akku1">{{stats.akku1}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.akku1.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>Sharp 1:</b-col>
            <b-col v-if="stats.sharp1">{{stats.sharp1}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.sharp1.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col>US 0:</b-col>
            <b-col v-if="stats.us0">{{stats.us0}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.us0.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>Sharp 2:</b-col>
            <b-col v-if="stats.sharp2">{{stats.sharp2}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.sharp2.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col>US 1:</b-col>
            <b-col v-if="stats.us1">{{stats.us1}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.us1.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>Sharp 3:</b-col>
            <b-col v-if="stats.sharp3">{{stats.sharp3}}</b-col>
            <b-col v-else>0</b-col>
            <b-col v-if="config.sensors.sharp3.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
        </b-container>

        <p></p>
        <h4>Actors</h4>
        <b-container class="mt-4 mx-auto">
          <b-row class="mb-2">
            <b-col>Actor</b-col>
            <b-col>Status</b-col>
            <b-col>Actor</b-col>
            <b-col>Status</b-col>
          </b-row>
          <b-row>
            <b-col>Motor A:</b-col>
            <b-col v-if="config.actuators.motorA.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>Servo 0:</b-col>
            <b-col v-if="config.actuators.servo0.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col>Motor B:</b-col>
            <b-col v-if="config.actuators.motorB.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>Servo 1:</b-col>
            <b-col v-if="config.actuators.servo1.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col></b-col>
            <b-col></b-col>
            <b-col>Servo 2:</b-col>
            <b-col v-if="config.actuators.servo1.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col></b-col>
            <b-col></b-col>
            <b-col>Servo 3:</b-col>
            <b-col v-if="config.actuators.servo1.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
        </b-container>
        <p></p>
        <h4>Interfaces</h4>
        <b-container class="mt-4 mx-auto">
          <b-row class="mb-2">
            <b-col>Interface</b-col>
            <b-col>Status</b-col>
            <b-col>Interface</b-col>
            <b-col>Status</b-col>
          </b-row>
          <b-row>
            <b-col>CAN:</b-col>
            <b-col v-if="config.interfaces.can.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>I2C:</b-col>
            <b-col v-if="config.interfaces.i2c.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
          <b-row>
            <b-col>UART:</b-col>
            <b-col v-if="config.interfaces.uart.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
            <b-col>WS2812:</b-col>
            <b-col v-if="config.interfaces.ws2812.enabled">enabled</b-col>
            <b-col v-else>disabled</b-col>
          </b-row>
        </b-container>
      </div>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "Welcome",
  components: {
    menuComponent: Menu,
  },
  data() {
    return {
      config: null,
      stats: null,
      loading: true,
    };
  },
  methods: {
    reset() {
      this.sendReset();
    },
    async getConfig() {
      const response = await this.$http.get("/api/config");
      const statsResponse = await this.$http.get("/api/stats");
      this.config = response.data;
      this.stats = statsResponse.data;

      if (Object.keys(this.config).length === 0) this.loading = true;
      else {
        this.loading = false;
      }
    },
    async sendReset() {
      await this.$http.post("/api/reset", null);
    },
  },
  created() {
    this.getConfig();
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
