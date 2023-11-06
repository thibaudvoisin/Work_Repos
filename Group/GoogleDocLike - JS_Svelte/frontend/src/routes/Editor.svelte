<script>
    import { onMount, afterUpdate } from 'svelte';

    export let editorContent = '';
    /**
	 * @type {any[]}
	 */
    let cursors = [];
    let mouseX = 0;
    let mouseY = 0;

    /**
	 * @param {number} max
	 */
    function getRandomInt(max) {
        return Math.floor(Math.random() * max);
    }


    onMount(() => {
        const socket = new WebSocket('ws://localhost:8080/ws');
        socket.addEventListener('open', (event) => {
            console.log('Connexion WebSocket établie.');
        });
        
        socket.addEventListener('message', (event) => {
            const message = event.data;
            try {
                const parsedMessage = JSON.parse(message);
                if (parsedMessage.type === 'editor') {
                    editorContent = parsedMessage.editorContent;
                } else if (parsedMessage.type === 'mouse') {
                    const existingCursor = cursors.find((cursor) => cursor.pseudo === parsedMessage.pseudo);
                    
                    if (existingCursor) {
                        existingCursor.x = parsedMessage.mouseX;
                        existingCursor.y = parsedMessage.mouseY;
                    } else {
                        cursors.push({ pseudo: parsedMessage.pseudo, x: parsedMessage.mouseX, y: parsedMessage.mouseY });
                        cursors = cursors;
                        
                    }

                    let el = document.getElementById(`cursor-${parsedMessage.pseudo}`)
                    if (el) {
                        el.style.left = `${parsedMessage.mouseX * window.innerWidth}px`;
                        el.style.top = `${parsedMessage.mouseY * window.innerHeight}px`;
                        if (!el.style.backgroundColor) {
                            let colorCursor = `rgb(${getRandomInt(255)}, ${getRandomInt(255)}, ${getRandomInt(255)})`;
                            el.style.backgroundColor = colorCursor;
                            el.style.color = colorCursor;
                        }
                    }
                } else if (parsedMessage.type === 'destroy') {
                    const index = cursors.findIndex((cursor) => cursor.pseudo === parsedMessage.pseudo);
                    if (index !== -1) {
                        cursors.splice(index, 1);
                    }
                    const cursorElement = document.getElementById(`cursor-${parsedMessage.pseudo}`);
                    if (cursorElement) {
                        cursorElement.remove();
                    }
                }
            }
            catch (error) {
                console.error('Erreur de parsing JSON :', error);
            }
        });

        const editorTextarea = document.getElementById('editor-textarea');
        const pseudoElement = document.querySelector('.account p');
        
        editorTextarea?.addEventListener('input', () => {
            socket.send(JSON.stringify({ type: 'editor', editorContent }));
        });

        editorTextarea?.addEventListener('mousemove', (event) => {
            mouseX = event.pageX / window.innerWidth;
            mouseY = event.pageY / window.innerHeight;
            
            socket.send(JSON.stringify({type: 'mouse', mouseX, mouseY, pseudo: pseudoElement?.textContent }));
        });
    })
</script>

<style>

.editable {
    left: 0;
    right: 0;
    margin-left: auto;
    margin-right: auto;
    margin-top: 90px;
    border: none;
    background-color: #ffffff;
    width: 21cm;
    min-height: 29.7cm;
    line-height: 1.5;
    box-shadow: 0 0 5px 2px rgba(95, 95, 95, 0.5);
    padding: 30px;
    resize: none;
  }
</style>
<div>
    <div id="editor-textarea" contenteditable="true" class="editable" bind:innerHTML={editorContent}></div>
    {#each cursors as cursor (cursor.pseudo)}
        <div id={`cursor-${cursor.pseudo}`} style="position: absolute;
        width: 10px;
        height: 10px;
        border-radius: 50%;">&nbsp;&nbsp;&nbsp;{cursor.pseudo}</div>
    {/each}
</div>