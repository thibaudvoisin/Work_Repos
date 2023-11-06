<script lang="ts">
  import { PUBLIC_API_URL } from '$env/static/public';
  import Sidebar from './sidebar.svelte';
  import Fuse from 'fuse.js';
  import { onMount } from 'svelte';

  // Define the structure of WarehouseResult
  type WarehouseResult = {
    products: {
      id: number;
      name: string;
      image: string;
      price: number;
      warranty_duration: number;
      number: number;
      warehouses: {
        id: number;
        stock: number;
      }[];
    }[];
  };

  // Declare and initialize variables with meaningful names
  let isInputVisible = false;
  let searchQuery = '';
  let globalCatalogData;
  let productNumberToAdd = 1;
  let listOfProducts: any[] = [];
  let fuseCatalog: Fuse<string> | null = null;
  let searchResults: any[] = [];
  let warehouseResults: WarehouseResult | null = null;
  let selectedItem: any = null;
  let isCommandLaunched = false;

  // Define the public API URL

  onMount(async () => {
    try {
      // Fetch global catalog data from the public API
      const response = await fetch(`${PUBLIC_API_URL}/Catalog/global`);
      const responseData = await response.json();

      if ('catalog' in responseData) {
        globalCatalogData = responseData.catalog;
        // Initialize the fuseCatalog for searching
        fuseCatalog = new Fuse(globalCatalogData, {
          keys: ['name', 'description'],
          includeScore: true,
        });
      } else {
        console.error('Catalog key not found in the JSON response');
      }
    } catch (error) {
      console.error('Error fetching the catalog:', error);
    }
  });

  // Function to toggle input visibility
  function toggleInput() {
    searchQuery = '';
    productNumberToAdd = 1;
    listOfProducts = [];
    searchResults = [];
    warehouseResults  = null;
    selectedItem = null;
    isCommandLaunched = false;
    isInputVisible = !isInputVisible;
  }

  // Function to add a product to the list
  function addProduct() {
    if (selectedItem) {
      const existingProduct = listOfProducts.find(product => product.id === selectedItem.id);

      if (existingProduct) {
        // If the product already exists in the list, update its quantity
        existingProduct.number += productNumberToAdd;
        listOfProducts = [...listOfProducts];
      } else {
        // If the product doesn't exist in the list, add it
        listOfProducts = [...listOfProducts, { ...selectedItem, number: productNumberToAdd }];
      }

      // Clear the input fields
      searchQuery = '';
      productNumberToAdd = 1;
      selectedItem = null;
    }
  }

  // Function to remove a product from the list
  function removeProduct(product: any) {
    listOfProducts = listOfProducts.filter(p => p !== product);
  }

  // Function to search for warehouses
  async function searchForWarehouses() {

    // Extract product IDs from the list of products
    const productIds = listOfProducts.map(product => product.id);

    // Create the payload for the POST request
    const payload = {
      product_ids: productIds
    };

    try {
      // Make a POST request to the /ask-warehouses/ endpoint

      const response =  await fetch(`${PUBLIC_API_URL}/ask-warehouses`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(payload),
      });

      if (response.ok) {
        const warehouseResultsTmp = await response.json();

        // Populate the warehouse results with product data
        warehouseResults = {
          products: listOfProducts.map(product => {
            const productWithWarehouses = {
              ...product,
              warehouses: warehouseResultsTmp.products.find((warehouseProduct: { id: any; }) => warehouseProduct.id === product.id)?.warehouses || []
            };
            return productWithWarehouses;
          })
        };

        // Set default image for products
        warehouseResults.products.forEach(product => {
          if (product.image.includes('example')){
            product.image = "https://media.istockphoto.com/id/1399859917/fr/vectoriel/pas-de-symbole-vectoriel-dimage-ic%C3%B4ne-disponible-manquante-pas-despace-r%C3%A9serv%C3%A9-de.jpg?s=170667a&w=0&k=20&c=5-gyzMzpmi-M4QDI2rweAYALoVGoXvaDnmjjaVb1U9Y=";
          }
        });

        // Clear the list of products and set the command as launched
        listOfProducts = [];
        isCommandLaunched = true;
      } else {
        console.error('Error in POST request to /ask-warehouses/');
      }
    } catch (error) {
      console.error('Error making POST request:', error);
    }
  }

  // Function to perform a search
  function performSearch() {
    const results = fuseCatalog?.search(searchQuery);

    if (results) {
      searchResults = Array.isArray(results) ? results : [];
      searchResults = searchResults.filter(result => result.score < 0.5);

      // Create an object to keep track of unique item names
      const uniqueNames: { [key: string]: boolean } = {};

      // Filter searchResults to keep only items with unique names
      searchResults = searchResults.filter(result => {
        if (!uniqueNames[result.item.name]) {
          uniqueNames[result.item.name] = true;
          return true;
        }
        return false;
      });
    }
  }
</script>

<!-- Sidebar component -->
<Sidebar bind:isInputVisible toggleInput={toggleInput} />

<div class="w-full flex justify-center">
  {#if isInputVisible}
    <div class="absolute bg-gray-100 border-r-2 shadow-lg rounded-xl w-[50vw] h-full items-center mx-auto">
      <button class="absolute text-[1.5vw] top-2 right-2 text-gray-500 hover:text-gray-800" on:click={toggleInput}>
        X
      </button>
      <h1 class="text-3xl font-bold underline text-center mt-10 mb-5">
        Expédition de commande
      </h1>
      {#if !isCommandLaunched}
        <p class="mt-10 ml-10 mb-7">
          Entrez le nom et le nombre de produits à commander
        </p>
        <div class="flex justify-center">
          <input type="text" bind:value={searchQuery} on:input={performSearch} placeholder="Rechercher un produit..."
            class="w-[15vw] h-[3vw] m-1 p-2 rounded border border-gray-300" />
          <input type="number" bind:value={productNumberToAdd}
            class="w-[4vw] h-[3vw] m-1 p-2 rounded border border-gray-300" />
          <button class="w-[13vw] h-[3vw] bg-blue-500 text-white text-[1vw] m-1 p-2 rounded border border-gray-300"
            on:click={addProduct}>
            Ajouter à la commande
          </button>
        </div>
        <div class="mt-2 mx-auto max-w-sm">
          <ul class="bg-white shadow-md rounded-md overflow-hidden">
            {#each searchResults as result}
              <li>
                <button class="w-full py-2 text-left px-4 hover:bg-blue-100 focus:outline-none focus:ring focus:ring-blue-400"
                  on:click={() => { selectedItem = result.item; searchQuery = result.item.name; searchResults = []; }}>
                  {result.item.name}
                </button>
              </li>
            {/each}
          </ul>
        </div>

        <div class="flex flex-wrap">
          {#each listOfProducts as product (product.name)}
            <div class="w-30 border border-gray-300 p-2 m-2 shadow-md bg-white rounded-md">
              <p class="text-[1vw]">{product.name} x {product.number}</p>
              <button class="text-[1vw] top-2 right-2 bg-red-500 hover:bg-red-800 text-white p-1 rounded-md"
                on:click={() => removeProduct(product)}>
                Remove
              </button>
            </div>
          {/each}
        </div>
        {#if listOfProducts.length > 0}
          <button class="absolute right-2 w-[13vw] h-[3vw] bg-blue-500 text-white text-[1vw] m-1 p-2 rounded border border-gray-300"
            on:click={searchForWarehouses}>
            Rechercher les entrepôts
          </button>
        {/if}
      {/if}

      {#if warehouseResults}
        {#each warehouseResults.products as product (product.id)}
          <div class="mb-4 p-4 border border-gray-300 rounded-lg flex">
            <div class="mr-4 w-[24vw]">
              <h3 class="text-xl font-bold mb-2">{product.name}</h3>
              <img src={product.image} alt={product.name} class="w-20 object-cover mb-2 rounded" />
              <p class="text-green-600 text-lg font-semibold mb-2">Price: ${product.price}</p>
              <p class="text-gray-700 mb-2">Warranty Duration: {product.warranty_duration} years</p>
              <p class="text-gray-700 mb-2">Quantity: {product.number}</p>
            </div>

            <div>
              <h4 class="text-lg font-semibold mt-4 mb-2">Warehouses:</h4>
              {#each product.warehouses as warehouse (warehouse.id)}
                <div class="ml-6 mb-2 border-l-2 border-gray-300 pl-4">
                  <p class="text-gray-700">Warehouse ID: {warehouse.id}</p>
                  <p class="text-green-600">Stock: {warehouse.stock}</p>
                </div>
              {/each}
            </div>
          </div>
        {/each}
      {/if}
    </div>
  {/if}
</div>

<svelte:head>
  <link href="https://unpkg.com/tailwindcss@^1.0/dist/tailwind.min.css" rel="stylesheet" />
</svelte:head>
