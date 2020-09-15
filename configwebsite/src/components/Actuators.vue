<template>
  <div class="Sensors">
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
      <div class="mt-3 mx-auto" style="max-width:500px;" text-center v-if="!loading" v-show="!loading">
        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox :disabled="motor" v-on="toggle()" v-model="actuators.motorA.enabled" switch>Motor A</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" :disabled="motor" v-on="toggle()" v-model="actuators.motorA.frequenz" placeholder="Frequenz"/>
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
           <b-form-checkbox :disabled="motor" v-on="toggle()" v-model="actuators.motorB.enabled" switch>Motor B</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" :disabled="motor" v-on="toggle()" v-model="actuators.motorB.frequenz" placeholder="Frequenz"/>
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
           <b-form-checkbox :disabled="servo" v-on="toggle()" v-model="actuators.servo0.enabled" switch>Servo 0</b-form-checkbox>
          </b-input-group-prepend>
          <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo0.frequenz" placeholder="Frequenz"/>
          <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo0.startDuty" placeholder="Start Dutycycle"/>
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox :disabled="servo" v-on="toggle()" v-model="actuators.servo1.enabled" switch>Servo 1</b-form-checkbox>
          </b-input-group-prepend>
           <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo1.frequenz" placeholder="Frequenz"/>
           <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo1.startDuty" placeholder="Start Dutycycle"/>
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox :disabled="servo" v-on="toggle()" v-model="actuators.servo2.enabled" switch>Servo 2</b-form-checkbox>
          </b-input-group-prepend>
           <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo2.frequenz" placeholder="Frequenz"/>
           <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo2.startDuty" placeholder="Start Dutycycle"/>
        </b-input-group>

        <b-input-group class="mt-2">
          <b-input-group-prepend is-text>
            <b-form-checkbox :disabled="servo" v-on="toggle()" v-model="actuators.servo3.enabled" switch>Servo 3</b-form-checkbox>
          </b-input-group-prepend>
           <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo3.frequenz" placeholder="Frequenz"/>
           <b-form-input type="number" :disabled="servo" v-on="toggle()" v-model="actuators.servo3.startDuty" placeholder="Start Dutycycle"/>
          </b-input-group>
          <b-button class="mt-4" block variant="success" style="max-width:500px;" @click="setConfig">Set config</b-button>
      </div>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "Actuators",
  components: {
    menuComponent: Menu
  },
  data() {
    return {
      loading: true,
      actuators: {},
      motor:false,
      servo: false
    };
  },
  methods:{
      async setConfig() {
        const response = await this.$http.post("/api/actuators", this.actuators)
        const responseData = response.data

        if (responseData.code != 200) {
          this.msg = "Error send Sensors configuration"
        } else {
          this.msg = "Success"
        } 
    },
    async getConfig() {
      const configData = await this.$http.get("/api/actuators")
      this.actuators = configData.data
      this.loading = false
    },
    toggle(){
      if(this.actuators.motorA.enabled == true || this.actuators.motorB.enabled == true)
        this.servo = true;
      
      if(this.actuators.servo0.enabled == true || this.actuators.servo1.enabled == true || this.actuators.servo2.enabled == true || this.actuators.servo3.enabled == true)
        this.motor = true;

      if(this.servo == true && this.actuators.motorA.enabled == false && this.actuators.motorB.enabled == false)
        this.servo = false;
      
      if(this.motor == true && this.actuators.servo0.enabled == false && this.actuators.servo1.enabled == false && this.actuators.servo2.enabled == false && this.actuators.servo3.enabled == false)
        this.motor = false;
    }
  },
  created(){
      this.getConfig()
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
