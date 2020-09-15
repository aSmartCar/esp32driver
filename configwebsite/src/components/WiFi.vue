<template>
  <div class="WiFi">
    <menuComponent />
    <div class="container mt-5">
      <div class="text-center">
        <b-spinner
          v-show="loading"
          style="width: 5rem; height: 5rem;"
          variant="success"
          label="Loading..."
        ></b-spinner>
      </div>
      <div v-show="!loading" class="text-center mt-3">
        Aktuelles W-LAN: {{wifi.SSID}}
         <b-form-checkbox class="mt-2" switch @change="showAPModal" v-model="enableAP">Enable AP Only</b-form-checkbox>
      </div>
      <div class="mt-3 mx-auto w-50" align="center" v-show="!loading">
        <b-table
          selectable
          select-mode="single"
          :items="items"
          @row-selected="rowSelected"
          class="mt-3"
          small
          show-empty
        ></b-table>
        <b-button block variant="success" @click="showModal">Set WiFi</b-button>
      </div>
      <b-modal ref="addWiFi" hide-footer title="Add WiFi">
        <div class="mt-2">SSID: {{selected[0] ? selected[0].SSID : ''}}</div>
        <b-form-input class="mt-2" v-model="password" placeholder="Password" type="password"></b-form-input>
        <b-button class="mt-2" block variant="success" @click="saveModal">Add</b-button>
        <b-button class="mt-2" block variant="danger" @click="hideModal">Close</b-button>
      </b-modal>

      <b-modal ref="setAP" hide-footer title="Set AP">
        <b-form-input class="mt-2" v-model="wifi.APSSID" placeholder="SSID" type="text"></b-form-input>
        <b-form-input class="mt-2" v-model="wifi.APPass" placeholder="Password" type="password"></b-form-input>
        <b-button class="mt-2" block variant="success" @click="saveAPModal">Set</b-button>
        <b-button class="mt-2" block variant="danger" @click="hideAPModal">Close</b-button>
      </b-modal>
    </div>
  </div>
</template>

<script>
import Menu from "./Menu";
export default {
  name: "W-LAN",
  components: {
    menuComponent: Menu
  },
  data() {
    return {
      items: [],
      selected: [],
      loading: true,
      wifi: {},
      password: "",
      enableAP: false
    };
  },
  methods: {
    rowSelected(items) {
      this.selected = items;
    },
    showModal() {
      if (this.selected.length && this.selected[0].Encryption != "OPEN")
        this.$refs["addWiFi"].show();
      else if (this.selected[0].Encryption == "OPEN") this.sendSelectedWiFi();
    },
    hideModal() {
      this.$refs["addWiFi"].hide();
      this.password = ""
    },
    saveModal() {
      this.$refs["addWiFi"].hide();
      this.sendSelectedWiFi();
    },
    showAPModal(){
      if(this.enableAP != true)
        this.$refs["setAP"].show();
      else
      {
        this.enableAP = false
        this.sendSelectedWiFi();
      }
    },
    hideAPModal() {
      this.$refs["setAP"].hide();
      if(!this.wifi.enableAP)
        this.enableAP = false;
    },
    saveAPModal() {
      this.$refs["setAP"].hide();
      this.sendSelectedWiFi();
    },
    async getData() {
      const WiFiData = await this.$http.get("/api/wifi");
      this.items = WiFiData.data;

      const setWIFI = await this.$http.get("/api/wifi/conf");
      this.wifi = setWIFI.data;
      this.enableAP = this.wifi.enableAP

      this.loading = false;
    },
    async sendSelectedWiFi() {
      if (this.selected.length || this.enableAP != this.wifi.enableAP) {
        if(this.selected.length){
          this.wifi.password = this.password;
          this.wifi.SSID = this.selected[0].SSID;
        }
        if(this.enableAP){
          this.enableAP = false
          this.wifi.enableAP = false
        }
        
        const response = await this.$http.post("/api/wifi", this.wifi);
        const responseData = response.data;

        if (responseData.code != 200) {
          this.msg = "Error send WiFi configuration";
        } else {
          this.msg = "Senden erfolgreich";
        }
      }
    }
  },
  created() {
    this.getData();
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.table-success {
  background-color: darkgreen;
}
</style>
