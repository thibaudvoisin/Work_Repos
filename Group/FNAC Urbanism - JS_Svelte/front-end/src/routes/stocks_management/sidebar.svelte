<script>
  import { onMount } from 'svelte';
  export let isInputVisible = false;

  // Function to toggle the input visibility
  export function toggleInput() {
    isInputVisible = !isInputVisible;
  }

  onMount(async () => {
    console.log("sdfdsf");
  });
</script>

<aside class="absolute w-[15vw] h-full bg-gray-200 border-r-2 shadow-lg flex justify-center">
  <nav class="p-12 text-xl">
    <button class="w-[13vw] text-[1vw] mb-4 bg-blue-500 text-white p-2 rounded-md" on:click={toggleInput}>
      Expédition de commande
    </button>
  </nav>
</aside>
