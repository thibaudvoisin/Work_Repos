<script lang="ts">
  import LoadingAnimation from '$lib/LoadingAnimation.svelte';
  import { PUBLIC_API_URL } from '$env/static/public';
  import { onMount } from 'svelte';

  let check: any = null;

  onMount(async () => {
    try {
      const response = await fetch(`${PUBLIC_API_URL}/health`);
      //const data = await response.json();
      if (response.ok) {
        check = true;
      } else {
        check = false;
      }
    } catch (error) {
      check = false;
    }
  });
</script>

<h1 class="text-center mt-10">
  Backend API connexion is:
  {#if check === null}
    <LoadingAnimation />
  {:else if check}
    <strong style="color: green;">UP</strong>
  {:else}
    <strong style="color: red;">DOWN</strong>
  {/if}
</h1>
