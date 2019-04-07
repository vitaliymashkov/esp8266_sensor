<template>
  <v-card class="mb-3">
    <v-card-title>
      <h1>Configuration</h1>
    </v-card-title>
    <v-card-text>
      <v-form>
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
        <v-text-field v-model="config.host" label="Host"/>
        <v-select
                v-model="config.ip_mode"
                :items="dhcpModes"
                :return-object="false"
                :autocomplete="false"
                item-text="text"
                item-value="value"
                label="DHCP Mode"
        ></v-select>
        <template v-if="config.ip_mode === 'static'">
          <v-text-field v-model="config.ip" label="IP"/>
          <v-text-field v-model="config.subnet" label="Subnet"/>
          <v-text-field v-model="config.gateway" label="Gateway"/>
          <v-text-field v-model="config.dns1" label="DNS1"/>
          <v-text-field v-model="config.dns2" label="DNS2"/>
        </template>
        <v-text-field v-model="config.hub" label="Hub Address"/>
      </v-form>
    </v-card-text>
    <v-card-actions>
      <v-spacer />
      <v-btn color="success" @click="$emit('save-config', config)">Save</v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
import axios from 'axios';

export default {
  props: {
    config: {
      type: Object,
      default: () => {},
    },
  },
  data() {
    return {
      show1: false,
      dhcpModes: [
        {
          text: 'Static IP',
          value: 'static',
        },
        {
          text: 'Auto',
          value: 'dhcp',
        },
      ],
    };
  },
};
</script>
