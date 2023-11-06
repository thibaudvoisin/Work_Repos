<script>
	/**
	 * @type {boolean}
	 */
	 export let showModal;

	/**
	 * @type {HTMLDialogElement}
	 */
	let dialog;

	$: if (dialog && showModal) dialog.showModal();


	let pseudo = '';
    
    function handleSubmit() {
        fetch('http://localhost:8080/sign-up', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ username : pseudo }),
        }).then(response => {
            if (response.ok) {
				// To change when working dialog.close()
            } else {
                // Gérer les erreurs
            }
        });
    }
	/**
	 * @param {string} pseudo
	 */
	function fctt(pseudo){
		const pElement = document.querySelector('.account p');

    // Vérifiez si l'élément existe
    if (pElement) {
      // Ajoutez le texte à l'élément <p>
      pElement.textContent = pseudo;
    }
	}

</script>

<!-- svelte-ignore a11y-click-events-have-key-events a11y-no-noninteractive-element-interactions -->
<dialog bind:this={dialog} on:close={() => (showModal = false)}>
    <!-- svelte-ignore a11y-no-static-element-interactions -->
    <div on:click|stopPropagation>
        <form on:submit={handleSubmit}>
            <div class="centered_text">
                <label class="titlee" for="pseudo">Entrez votre pseudo</label>
            </div>
            <div style="display: flex; align-items: center;">
                <input type="text" id="pseudo" bind:value={pseudo} />
                <button type="submit" on:click={() => {
                    if (pseudo.trim() !== "") {
                        dialog.close();
						fctt(pseudo);
                    }
                }}>Envoyer</button>
            </div>
            {#if pseudo.trim() === ""}
                <p style="color: red;">Le pseudo ne peut pas être vide.</p>
            {/if}
        </form>
    </div>
</dialog>


<style>
	.titlee{
		font-family: Roboto, sans-serif;
		font-weight: 0;
		font-size: 20px;
	}
	dialog {
		border-radius: 0.75em;
		border: none;
		padding: 0;
	}
	dialog::backdrop {
		background: rgba(0, 0, 0, 0.3);
	}
	dialog > div {
		padding: 1em;
	}
	dialog[open] {
		animation: zoom 0.3s cubic-bezier(0.34, 1.56, 0.64, 1);
	}
	@keyframes zoom {
		from {
			transform: scale(0.95);
		}
		to {
			transform: scale(1);
		}
	}
	dialog[open]::backdrop {
		animation: fade 0.2s ease-out;
	}
	@keyframes fade {
		from {
			opacity: 0;
		}
		to {
			opacity: 1;
		}
	}
	input[type=text], select {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  border-radius: 4px;
  box-sizing: border-box;
}
	button {
		display: block;
	}
	#pseudo {
		margin-left: 5px;
		margin-right: 10px;
	}
	.centered_text{
		text-align: center;
		margin-bottom: 15px;
	}
	button{

font-family: Roboto, sans-serif;
font-weight: 0;
font-size: 14px;
color: #fff;
background-color: #0066CC;
padding: 10px 30px;
border: 2px solid #0066cc;
box-shadow: rgb(0, 0, 0) 0px 0px 0px 0px;
border-radius: 50px;
transition : 516ms;
display: flex;
flex-direction: row;
align-items: center;
cursor: pointer;
}

button:hover{

transition : 516ms;
background-color: #fff;
color: #0066cc;
border: solid 2px #0066cc;
}
</style>
