<template>
  <div class="Interfaces">
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

        <!-- Future usage -->
        <!-- <b-form-checkbox class="mt-2" v-model="interfaces.can.enabled" switch>CAN</b-form-checkbox> -->
        <b-form-checkbox class="mt-2" v-model="interfaces.uart.enabled" switch>UART</b-form-checkbox>

        <!-- <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox id v-model="interfaces.i2c.enabled" switch>I²C</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="interfaces.i2c.clock" placeholder="Clock" style="width:200px;"/>
        </b-input-group> -->

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox v-model="interfaces.ws2812.enabled" switch>WS2812</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" v-model="interfaces.ws2812.count" v-on="changeLEDArray()" placeholder="Count"/>
        </b-input-group>

         <b-container fluid v-if="interfaces.ws2812.count" class="mt-3">
              <b-row class="my-1" v-for="(key, index) in tmpLED" :key="index">
                <b-col sm="3">
                  <label :for="`type-${index}`">LED {{ index + 1 }}:</label>
                </b-col>
                <b-col sm="3">
                  <b-form-input :id="`type-${index}`" type="number" placeholder="Red" v-model="tmpLED[index].r"></b-form-input>
                </b-col>
                <b-col sm="3">
                  <b-form-input :id="`type-${index}`" type="number" placeholder="Green" v-model="tmpLED[index].g"></b-form-input>
                </b-col>
                <b-col sm="3">
                 <b-form-input :id="`type-${index}`" type="number" placeholder="Blue" v-model="tmpLED[index].b"></b-form-input>
                </b-col>
              </b-row>
          </b-container>
         
        <b-button class="mt-4" style="max-width:500px;" block variant="success" @click="setConfig">Set config</b-button>
      </div>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "Interfaces",
  components: {
    menuComponent: Menu
  },
  data() {
    return {
      interfaces: {},
      loading: true,
      tmpLED: []
    };
  },
  methods: {
    async setConfig() {
      this.interfaces.ws2812.colors = this.tmpLED
      const response = await this.$http.post(
        "/api/interfaces",
        this.interfaces
      );
      const responseData = response.data;

      if (responseData.code != 200) {
        this.msg = "Error send Interfaces configuration";
      } else {
        this.msg = "Success";
      }
    },
    async getConfig() {
      const configData = await this.$http.get("/api/interfaces");
      this.interfaces = configData.data;
      

      if(Object.keys(this.interfaces).length === 0)
        this.loading = true
      else
      {
        this.tmpLED = this.interfaces.ws2812.colors
        this.loading = false
      }
    },
    changeLEDArray()
    {
      if(this.interfaces.ws2812.count > this.interfaces.ws2812.max) // Set hard limit for maximum ammount
        this.interfaces.ws2812.count = this.interfaces.ws2812.max
      if(this.tmpLED.length < this.interfaces.ws2812.count)
      {
        const tmp = this.interfaces.ws2812.count - this.tmpLED.length
        for(let i = 0; i < tmp; i++)
          this.tmpLED.push({"r":0, "g":0, "b":0})
      }
      else if(this.tmpLED.length > this.interfaces.ws2812.count)
      {
          const tmp = this.tmpLED.length - this.interfaces.ws2812.count
          for(let i = 0; i < tmp; i++)
            this.tmpLED.pop()
      }
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
