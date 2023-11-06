import { Elysia, t } from 'elysia'
import { cors } from '@elysiajs/cors'
import { PrismaClient } from '@prisma/client'

let text = ''
const clients = [];
const cursors = [];

const db = new PrismaClient()

const app = new Elysia()
	.use(cors())
	.model({ 
        'user.sign': t.Object({ 
            username: t.String(), 
        }) 
    })
	.post( 
        '/sign-up', 
        async ({ body }) => db.user.create({ 
            data: body 
        }),
		{
            error({ code }) {  
                switch (code) {  
                    case 'P2002':  
                        return {  
                            error: 'Username must be unique'  
                        }  
                }  
            },  
            body: 'user.sign',
        } 
    )
    .model({
        'user.data': t.Object({
            username: t.String(),
            data: t.String()
        })
    })
    .post(
        '/saveContent',
        async ({ body }) => {
            const { username, data } = body;
            try {
                const user = await db.user.findUnique({
                    where: {
                        username: username
                    }
                });
        
                if (user) {
                    const saveContent = await db.saveContent.create({
                        data: {
                            author: {
                                connect: {
                                    id: user.id
                                }
                            },
                            data: data
                        }
                    });
        
                    return {
                        status: 'success',
                        message: 'Contenu sauvegardé avec succès',
                        data: saveContent
                    };
                } else {
                    return {
                        status: 'error',
                        message: 'Utilisateur non trouvé'
                    };
                }
            } catch (error) {
                return {
                    status: 'error',
                    message: 'Une erreur s\'est produite lors de la sauvegarde du contenu'
                };
            } finally {
                await db.$disconnect();
            }
        }
    )

	.get('/', () => 'Hello Elysia')
	.get('/text', () => ({data : `${text}`}))
    .ws('/ws', {
        open(ws) {
            clients.push(ws);
            ws.send({ type : "editor" , editorContent : text});
            for (const cursor of cursors) {
                ws.send({ type: 'mouse', mouseX: cursor.mouseX, mouseY: cursor.mouseY, pseudo: cursor.pseudo });
            }
            console.log('Nouvelle connexion WebSocket.');
        }, 
        message(ws, message) {
            try {        
                if (message.type === 'editor') {
                    text = message.editorContent;
                    for (const client of clients) {
                        client.send(message);
                    }
                }
                if (message.type === 'mouse') {
                    const existingCursor = cursors.find((cursor) => cursor.pseudo === message.pseudo);
                    if (existingCursor) {
                        existingCursor.mouseX = message.mouseX;
                        existingCursor.mouseY = message.mouseY;
                    } else {
                        cursors.push({ pseudo: message.pseudo, mouseX: message.mouseX, mouseY: message.mouseY });
                    }
                    for (const client of clients) {
                        if (client.data.headers["sec-websocket-key"] !== ws.data.headers["sec-websocket-key"]) {
                            client.send(message);
                        }
                    }
                }
            } catch (error) {
                console.error('Erreur de parsing JSON :', error);
            }
        },
        close(ws) {
            let index = 0;
            for (; index < clients.length; index++) {
                if (clients[index].data.headers["sec-websocket-key"] === ws.data.headers["sec-websocket-key"]) {
                    break;
                }
            }
            if (index !== -1) {
                clients.splice(index, 1);
            }
            for (const client of clients) {
                client.send({ type : "destroy", pseudo : cursors.at(index).pseudo });
            }
            cursors.splice(index, 1);
            console.log('Connexion WebSocket fermée.');
        }
    })
	.listen(8080)
	 
console.log(`🦊 Elysia is running at ${app.server?.hostname}:${app.server?.port}`)