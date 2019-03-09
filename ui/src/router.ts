import Vue from 'vue';
import Router from 'vue-router';
import Home from './views/Home.vue';
import Config from './views/Config.vue';

Vue.use(Router);

export default new Router({
  mode: 'hash',
  base: process.env.BASE_URL,
  routes: [
    {
      path: '/',
      name: 'status',
      component: Home,
    },
    {
      path: '/config',
      name: 'config',
      component: Config,
    },
  ],
});
