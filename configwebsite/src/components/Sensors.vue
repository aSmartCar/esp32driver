<template>
  <div class="Sensors">
    <menuComponent />
    <div class="container mt-5 text-center">
      <div class="text-center">
        <b-spinner
          v-show="loading"
          style="width: 5rem; height: 5rem;"
          variant="success"
          label="Loading..."
        ></b-spinner>
      </div>
      <div class="mt-3 mx-auto w-50" v-show="!loading" v-if="!loading">

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.sharp0.enabled" switch>Sharp 0</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.sharp0.clearance" placeholder="Clearance" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.sharp1.enabled" switch>Sharp 1</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.sharp1.clearance" placeholder="Clearance" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.sharp2.enabled" switch>Sharp 2</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.sharp2.clearance" placeholder="Clearance" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.sharp3.enabled" switch>Sharp 3</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.sharp3.clearance" placeholder="Clearance" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.us0.enabled" switch>Ultraschall 0</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.us0.clearance" placeholder="Clearance" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.us1.enabled" switch>Ultraschall 1</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.us1.clearance" placeholder="Clearance" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.akku0.enabled" switch>Akku 0</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.akku0.voltage" placeholder="Voltage" />
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="sensors.akku1.enabled" switch>Akku 1</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="sensors.akku1.voltage" placeholder="Voltage" />
        </b-input-group>

        <b-button class="mt-3" block variant="success" @click="setConfig">Set config</b-button>
      </div>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "Sensors",
  components: {
    menuComponent: Menu
  },
  data() {
    return {
      loading: true,
      sensors: {}
    };
  },
  methods: {
    async setConfig() {
      const response = await this.$http.post("/api/sensors", this.sensors);
      const responseData = response.data;

      if (responseData.code != 200) {
        this.msg = "Error send Sensors configuration";
      } else {
        this.msg = "Success";
      }
    },
    async getConfig() {
      const configData = await this.$http.get("/api/sensors");
      this.sensors = configData.data;
      if (Object.keys(this.sensors).length === 0) this.loading = true;
      else this.loading = false;
    }
  },
  created() {
    this.getConfig();
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
