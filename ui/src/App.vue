<template>
  <v-app>
    <v-system-bar status color="primary" fixed dark>
      <span class="pr-1">ESP8266 </span>
      <span class="font-weight-light">Control panel </span>
      <v-spacer />
      <span class="font-weight-light body-2">{{ time }}</span>
    </v-system-bar>
    <v-content class="pt-4">
      <router-view :config="config" :data="data" @set-pin-val="setPinVal($event.key, $event.val)" @save-config="saveConfig"/>
    </v-content>
    <v-footer>
      <v-bottom-nav
        :active.sync="bottomNav"
        :value="true"
        color="white"
        fixed
      >
        <v-btn
          color="teal"
          flat
          value="status"
        >
          <span>Status</span>
          <v-icon>home</v-icon>
        </v-btn>

        <v-btn
          color="teal"
          flat
          value="config"
        >
          <span>Config</span>
          <v-icon>settings</v-icon>
        </v-btn>
      </v-bottom-nav>
    </v-footer>
  </v-app>
</template>

<script>
import axios from 'axios';
import moment from 'moment';

export default {
  name: 'App',
  data() {
    return {
      host: 'http://192.168.0.123',
      //host: '',
      bottomNav: this.$route.name,
      time: '',
      config: {},
      data: {},
      pin_map: {},
    };
  },
  watch: {
    bottomNav(val) {
      this.$router.push({name: val});
    },
  },
  beforeMount() {
    this.getMap();
    this.getData();
    //setInterval(() => { this.getData(); }, 5000);
    this.getConfig();
  },
  methods: {
    setVal(data) {
      this.data = data;
      this.time = moment(new Date(parseInt(`${data.time}000`, 10))).format('MMMM D YYYY, HH:mm:ss');
    },
    getData() {
      axios.get(`${this.host}/data.json`).then((resp) => {
        this.setVal(resp.data);
      });
    },
    getConfig() {
      axios.get(`${this.host}/config`).then((resp) => {
        this.config =resp.data;
      });
    },
    getMap() {
      axios.get(`${this.host}/pin_map.json`).then((resp) => {
        this.pin_map = resp.data.pin_map;
      });
    },
    setPinVal(key, val) {
      this.data.digital[key] = val ? 1 : 0;
      axios.get(`${this.host}/set?pin=${this.pin_map[key]}&val=${val ? 1 : 0}`).then((resp) => {
        this.setVal(resp.data);
      });
    },
    saveConfig(config) {
      axios.post(`${this.host}/config`,config).then((resp) => {
        console.log(resp);
      });
    },
  },
};
</script>
