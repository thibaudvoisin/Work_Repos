<script>
  // @ts-nocheck

  // Import necessary modules
  import { onDestroy, onMount } from "svelte";
  import Editor from "./Editor.svelte";
  import Modal from './Modal.svelte';
  import { enableImageResizeInDiv } from "./imageResize.js";
  
  // Declare variables
  let isPopupVisible = false;
  let isButtonVisible = true;
  let showModal = false;
  let files;
  let editorContent = '';

  // Initialize onMount hook
  onMount(() => {
    showModal = true;
  });

  // Function to download the file
  function downloadFile() {
    const blob = new Blob([editorContent], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'TextFile.alteam6';
    a.click();
    URL.revokeObjectURL(url);
  }

  // Function to load a file
  function loadedfile() {
    const file = files[0];
    const reader = new FileReader();
    reader.onload = (e) => {
      editorContent = e.target.result;
    };
    reader.readAsText(file);
    files = null;
  }

  // Function to show a popup
  function showPopup() {
    isPopupVisible = true;
    isButtonVisible = false;
  }

  // Declare other variables and functions
  let interval;
  let content = '';
  let selectedFontSize = "1";
  let selectedFontName = "1";
  let selectedColorRGB = 'rgb(255, 0, 0)';

  function toggleBold() {
    document.execCommand('bold');
  }

  function toggleItalic() {
    document.execCommand('italic');
  }

  function toggleUnderline() {
    document.execCommand('underline');
  }

  function changeFontSize(event) {
    const fontSize = event.target.value;
    document.execCommand('fontSize', false, fontSize);
    selectedFontSize = fontSize;
  }

  function changeFontName(event) {
    const fontName = event.target.value;
    document.execCommand('fontName', false, fontName);
    selectedFontSize = fontSize; // Note: Should this be selectedFontName?
  }

  function toggleColor(color) {
    document.execCommand('foreColor', false, color);
  }

  // Cleanup on onDestroy
  onDestroy(() => {
    clearInterval(interval);
  });

  let imgurl, fileinput, fileinput2;

  // Function to handle file selection
  const onFileSelected = (e) => {
    const editorTextarea = document.getElementById('editor-textarea');
    let image = e.target.files[0];
    let reader = new FileReader();
    reader.readAsDataURL(image);
    reader.onload = e => {
      imgurl = e.target.result;

      const imgElement = document.createElement('img');
      imgElement.src = imgurl;
      imgElement.id = 'edt';
      imgElement.style.maxWidth = '100%';

      editorTextarea.appendChild(imgElement);
      enableImageResizeInDiv('editor-textarea');
    };
  }

  $: {
  }
</script>

<style>
    .toolbar {
    position: fixed;
    display: flex;
    align-items: center; /* Centre verticalement les éléments */
    justify-content: center; /* Centre horizontalement les éléments */
    background-color: snow;
    border: 1px solid #ccc;
    left: 0;
    right: 0;
    margin-top: 0cm;
    margin-left: auto;
    margin-right: auto;
    width: auto;
    height: 4rem;
}

	.editable {
	  border: none;
	  background-color: #ffffff;
	  border-radius: 10px;
	  width: 21cm;
	  height: 29.7cm;
	  line-height: 1.5;
	  box-shadow: 0 0 10px 3px rgba(143, 229, 231, 0.5);
	  padding: 30px;
	  margin: 200px;
	  resize: none;
	}

	.img_button{
		cursor: pointer;
		height: 2.4em;
		border: 1px solid #ccc;
  	  	background-color: white;
   		border-radius: 5px;
    	cursor: pointer;
    	font-size: 16px;
		margin: 7px;
		padding:2px;
	}
    .img_button:hover {
  border-color: #999;
  box-shadow: 0 0 2px rgba(0, 0, 0, 0.5);
}
    .img_button img {
		padding:5px;
        
	}

    .account{
        position: fixed;
    display: flex;
    align-items: center; /* Centre verticalement les éléments */
    justify-content: center; /* Centre horizontalement les éléments */
    background-color: #0066ccc4;
    border-radius: 10px 10px 10px 10px;
    right: 0;
    margin-right: 5px;
    width: 15rem;
    height: 3.5rem;
    }
    .logo{
        position: fixed;
    display: flex;
    align-items: center; /* Centre verticalement les éléments */
    justify-content: center; /* Centre horizontalement les éléments */
    left: 0;
    height: 3.5rem;
    }
</style>

<div class="toolbar">
  <!-- svelte-ignore a11y-missing-attribute -->
  <img class="logo" src="https://i.ibb.co/x6Ttcp2/alteam-Blason.png" />

  <!-- Buttons for formatting -->
  <img class="img_button" src="https://i.ibb.co/fX42WCP/bold.png" alt="Bold" on:click={toggleBold} />
  <img class="img_button" src="https://i.ibb.co/YNmdZQw/italic.png" alt="Italic" on:click={toggleItalic} />
  <img class="img_button" src="https://i.ibb.co/qFJ2g8q/underline.png" alt="Underline" on:click={toggleUnderline} />

  <!-- Color picker -->
  <input class="img_button" type="color" value="#BE2302" on:change={e => toggleColor(e.target.value)} />

  <!-- Font size selector -->
  <select class="img_button" on:change={changeFontSize}>
    <option value="4">12px</option>
    <option value="5">14px</option>
    <option value="6">16px</option>
    <option value="7">20px</option>
    <option value="8">24px</option>
  </select>

  <!-- Font name selector -->
  <select class="img_button" on:change={changeFontName}>
    <option value="Arial">Arial</option>
    <option value="Calibri">Calibri</option>
    <option value="Courier New">Courier New</option>
    <option value="Helvetica">Helvetica</option>
    <option value="Georgia">Georgia</option>
    <option value="Times New Roman">Times New Roman</option>
  </select>

  <!-- File upload buttons -->
  <img class="img_button" src="https://i.ibb.co/HB3sTqQ/imgg.png" alt="Upload Image" on:click={() => { fileinput.click(); }} />
  <input style="display:none" type="file" accept=".jpg, .jpeg, .png" on:change={(e) => onFileSelected(e)} bind:this={fileinput} />

  <img class="img_button" src="https://cdn-icons-png.flaticon.com/512/0/532.png" alt="Download File" on:click={downloadFile} />

  <img class="img_button" src="https://upload.wikimedia.org/wikipedia/commons/thumb/c/cf/Upload_alt_font_awesome.svg/1200px-Upload_alt_font_awesome.svg.png" alt="Upload File" on:click={() => { fileinput2.click(); }} />
  <input style="display:none" type="file" bind:this={fileinput2} bind:files on:change={loadedfile} />

  <!-- Modal component -->
  <Modal bind:showModal>
  </Modal>

  <!-- User account information -->
  <div class="account">
    <img style="width:2.7rem;" src="https://i.ibb.co/44dDHvG/imageedit-1-5302777497.png" />
    <p style="color:white; font-size:18px;margin-left:5px;"></p>
  </div>
</div>

<!-- Editor component -->
<Editor bind:editorContent={editorContent} />
