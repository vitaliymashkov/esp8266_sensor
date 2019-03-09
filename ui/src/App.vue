<template>
  <v-app>
    <v-system-bar status color="primary" fixed dark>
      <span class="pr-1">ESP8266 </span>
      <span class="font-weight-light">Control panel </span>
      <v-spacer />
      <span class="font-weight-light body-2">{{ time }}</span>
    </v-system-bar>
    <v-content class="pt-4">
      <router-view :host="host"/>
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
      //host: 'http://192.168.0.124',
      host: '',
      bottomNav: this.$route.name,
      time: '',
    };
  },
  watch: {
    bottomNav(val) {
      this.$router.push({name: val});
    },
  },
  beforeMount() {
    this.getData();
    setInterval(() => { this.getData(); }, 5000);
  },
  methods: {
    setVal(data) {
      this.time = moment(new Date(parseInt(`${data.time}000`, 10))).format('MMMM D YYYY, HH:mm:ss');
    },
    getData() {
      axios.get(`${this.host}/data.json`).then((resp) => {
        this.setVal(resp.data);
      });
    },
  },
};
</script>
