<template>
  <div class="home">
    <v-card>
      <v-card-title  primary-title>
        <h1 class="headline mb-0">Dashboard</h1>
      </v-card-title>
      <v-card-text>
        <h3>Analog</h3>
        <template
          v-for="(val, index) in data.analog"
        >
          <v-text-field :key="`A${index}`" :value="val" :label="`A${index}`" disabled />
        </template>
        <h3>Digital</h3>
        <v-container v-bind="{ [`grid-list-3`]: true }" fluid>
          <v-layout row wrap>
            <v-flex xs4
              v-for="(val, key) in data.digital"
            >
              <v-switch
                v-model="data.digital[key]"
                :key="key"
                :label="`D${key}`"
                @change="$emit('set-pin-val', {key: 'D' + key, val: $event})"
              ></v-switch>
            </v-flex>
          </v-layout>
        </v-container>
      </v-card-text>
    </v-card>
  </div>
</template>

<script>

export default {
  props: {
    config: {
      type: Object,
      default: () => {},
    },
    data: {
      type: Object,
      default: () => {},
    },
  },
};
</script>
