<template>
  <div class="home">
    <v-card>
      <v-card-title  primary-title>
        <h1 class="headline mb-0">Dashboard</h1>
      </v-card-title>
      <v-card-text>
        <h3>Analog</h3>
        <template
          v-for="(val, index) in analog"
        >
          <v-text-field :key="`A${index}`" :value="val" :label="`A${index}`" disabled />
        </template>
        <h3>Digital</h3>
        <v-container v-bind="{ [`grid-list-3`]: true }" fluid>
          <v-layout row wrap>
            <v-flex xs4
              v-for="(val, key) in digital"
            >
              <v-switch
                v-model="digital[key]"
                :key="key"
                :label="`${key}`"
                @change="setPinVal(key, $event)"
              ></v-switch>
            </v-flex>
          </v-layout>
        </v-container>
      </v-card-text>
    </v-card>
  </div>
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
      analog: [],
      digital: {},
      time: 0,
      temperature: 0,
      humidity: 0,
      pin_map: 0,
    };
  },
  beforeMount() {
    this.getData();
  },
  methods: {
    setVal(data) {
      this.analog = data.analog;
      this.digital = data.digital;
      this.time = data.time;
      this.temperature = data.temperature;
      this.humidity = data.humidity;
      this.pin_map = data.pin_map;
    },
    getData() {
      axios.get(`${this.host}/data.json`).then((resp) => {
        this.setVal(resp.data);
      });
    },
    setPinVal(key, val) {
      this.digital[key] = val ? 1 : 0;
      axios.get(`${this.host}/set?pin=${this.pin_map[key]}&val=${val ? 1 : 0}`).then((resp) => {
        this.setVal(resp.data);
      });
    },
  },
};
</script>
