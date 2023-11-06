<script>
  import { PUBLIC_API_URL } from '$env/static/public';
  import { fade } from 'svelte/transition';

  let selectedSource = 'warehouses'; // Default value
  let selectedId = ''; // Default value
  let isElementVisible = true;

  /**
     * @type {string | any[]}
     */
  let catalogData = []; // Store catalog data here

  /**
     * @param {{ shops: any[]; warehouses: any[]; }} dataRaw
     * @param {string} selectedId
     */
  function getProducts(dataRaw, selectedId) {
    if (!Object.keys(dataRaw).length){return;}
    let type = Object.keys(dataRaw)[0];
    let data = null;

    if (type == 'shops') {
      data = dataRaw.shops.find(shop => String(shop.shopId) === String(selectedId));
    } else if ('warehouses' in dataRaw) {
      // @ts-ignore
      data = dataRaw.warehouses.find(warehouse => String(warehouse.warehouseId) === String(selectedId));
    }
    else
    {
      console.log(dataRaw);
    }

    return data ? data.products : [];
  }

  async function fetchCatalog() {
    try {
      const response = await fetch(`${PUBLIC_API_URL}/catalog/${selectedSource}`);
      const responseData = await response.json();

      if (responseData && selectedId === '') {
        catalogData = responseData.catalog;
      } else {
        catalogData = getProducts(responseData, selectedId);
      }

      if (!catalogData.length) {
        console.error('Catalog/shop/warehouse key not found in the JSON response');
      }

      selectedId = '';
    } catch (error) {
      console.error('Error fetching the catalog:', error);
    }
  }

  function handleSelectChange() {
    isElementVisible = selectedSource === 'warehouses' || selectedSource === 'shops';
  }
</script>

<h1 class="text-3xl font-bold underline text-center mt-10">
  Welcome to UBSI - Fonction Achat
</h1>

<div class="flex justify-center content-center mt-10">
  <iframe
    title="Welcome"
    frameBorder="0"
    height="140"
    src="https://cdn.discordapp.com/attachments/512758883513794560/1155856727867146250/joel-spinning.gif"
    width="480"
  ></iframe>
</div>

<div class="flex justify-center items-center mt-10">
  <select
    class="w-40 p-2 border border-gray-300 rounded outline-none"
    bind:value={selectedSource}
    on:change={handleSelectChange}
  >
    <option value="warehouses">Entrepôt</option>
    <option value="shops">Magasin</option>
    <option value="eshop">E-commerce</option>
    <option value="global">Global</option>
  </select>

  {#if isElementVisible}
    <input
      transition:fade
      type="text"
      bind:value={selectedId}
      placeholder="Entrez l'ID"
      class="ml-2 p-2 rounded border border-gray-300"
    />
  {/if}
</div>

<div class="flex justify-center items-center mt-4">
  <button
    class="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded cursor-pointer"
    on:click={fetchCatalog}>Récupérer le catalogue</button>
</div>

<div class="flex flex-wrap justify-between mt-4">
  {#each catalogData as product (product.id)}
    <div class="w-64 border border-gray-300 p-4 m-2 shadow-md bg-white">
      <p class="font-bold mb-2">Name: {product.name}</p>
      <p class="text-blue-500">Price: {product.price}</p>
      <img class="max-w-full h-auto" src={product.image} alt={product.name} />
      <p class="text-gray-700 text-base mt-2">Max Quantity: {product.max_quantity}</p>
    </div>
  {/each}
</div>
