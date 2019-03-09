<template>
  <v-card class="mb-3">
    <v-card-title>
      <h1>Configuration</h1>
    </v-card-title>
    <v-card-text>
      <v-form>
        <v-text-field v-model="config.host" label="OTA Host"/>
        <v-text-field v-model="config.ssid" label="Wifi Network Name"/>
        <v-text-field
          v-model="config.password"
          :append-icon="show1 ? 'visibility' : 'visibility_off'"
          :type="show1 ? 'text' : 'password'"
          name="wifi-password"
          label="Wifi Network Password"
          counter
          @click:append="show1 = !show1"
        />
      </v-form>
    </v-card-text>
    <v-card-actions>
      <v-spacer />
      <v-btn color="success">Save</v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
import axios from 'axios';

export default {
  props: {
    host: {
      type: String,
      default: '',
    },
  },
  data() {
    return {
      show1: false,
      config: {},
    };
  },
  beforeMount() {
    this.getData();
  },
  methods: {
    setVal(data) {
      this.config = data;
    },
    getData() {
      axios.get(`${this.host}/config`).then((resp) => {
        this.setVal(resp.data);
      });
    },
  },
};
</script>
