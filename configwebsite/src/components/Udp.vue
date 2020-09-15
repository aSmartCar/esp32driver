<template>
  <div class="UDP">
    <menuComponent/>
    <div class="container mt-5 text-center">
      <div class="text-center">
        <b-spinner
          v-show="loading"
          style="width: 5rem; height: 5rem;"
          variant="success"
          label="Loading..."
        ></b-spinner>
      </div>
      <div class="mt-3 mx-auto" style="max-width:500px;" v-if="!loading" v-show="!loading">

        <b-form-checkbox class="mt-2" switch v-model="udp.broadcast">Enable broadcast</b-form-checkbox>
        <br>
        <b-row class="my-1">
          <b-col sm="2">
            <label for="input-small">Remote port:</label>
          </b-col>
          <b-col sm="10">
            <b-form-input id="input-small" size="sm" v-model="udp.remotePort" type="number"></b-form-input>
          </b-col>
        </b-row>

        <b-row class="my-1">
          <b-col sm="2">
            <label for="input-small">Local port:</label>
          </b-col>
          <b-col sm="10">
            <b-form-input id="input-small" size="sm" v-model="udp.localPort" type="number"></b-form-input>
          </b-col>
        </b-row>
         
        <b-button class="mt-4" style="max-width:500px;" block variant="success" @click="setConfig">Set config</b-button>
      </div>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "UDP",
  components: {
    menuComponent: Menu
  },
  data() {
    return {
      udp: {},
      loading: true
    };
  },
  methods: {
    async setConfig() {
      const response = await this.$http.post("/api/udp", this.udp);
      const responseData = response.data;

      if (responseData.code != 200) {
        this.msg = "Error send Interfaces configuration";
      } else {
        this.msg = "Success";
      }
    },
    async getConfig() {
      const configData = await this.$http.get("/api/udp");
      this.udp = configData.data;
      

      if(Object.keys(this.udp).length === 0)
        this.loading = true
      else
        this.loading = false
    },
    
  },
  created() {
    this.getConfig();
  }
};
</script>
<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
