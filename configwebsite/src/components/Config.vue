<template>
  <div class="Config">
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
      <div class="mt-3 mx-auto" style="max-width:500px;" v-show="!loading">
        <b-form-textarea
          id="textarea"
          v-model="text"
          rows="15"
        ></b-form-textarea>
        <b-button class="mt-4" style="max-width:500px;" block variant="success" @click="setConfig">Set config</b-button>
      </div>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "Config",
  components: {
    menuComponent: Menu
  },
  data() {
    return {
      config: null,
      text: null,
      loading: true
    };
  },
  methods: {
    async setConfig() {
      this.config = JSON.parse(this.text);
      const response = await this.$http.post("/api/config", this.config);
      const responseData = response.data;

      if (responseData.code != 200) {
        this.msg = "Error send Interfaces configuration";
      } else {
        this.msg = "Success";
      }
    },
    async getConfig() {
      const configData = await this.$http.get("/api/config");
      this.config = configData.data;
      this.text = JSON.stringify(this.config);
      this.loading = false;
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
