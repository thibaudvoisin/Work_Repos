<script>
  import NavMenu from '$lib/NavMenu.svelte';
  import '../app.css';

  import { onMount } from 'svelte';
  import { themeChange } from 'theme-change';
  onMount(() => {
    themeChange(false);
  });
</script>

<nav>
  <NavMenu />
</nav>

<slot />
