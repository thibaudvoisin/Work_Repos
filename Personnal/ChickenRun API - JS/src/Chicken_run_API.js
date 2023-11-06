//Loading modules

//Back End framework, allow the implementation of endpoints.
const express = require('express') 

//Database framework, simplify the interactions with the postgres DataBase
const { Client } = require('pg'); //D

const pgp = require('pg-promise')();
const app = express()


//Main function, takes the databases parameters and a port | Returns the running Application
async function chicken_run(user_, host_, password_, db_port_, back_end_port) {

//Connection to the database
    const client = new Client({
        user: user_,
        host: host_,
        password: password_,
        port: db_port_,
        database: 'chicken_db',
    });
    await client.connect();

//Get endpoint, get and send the chicken from the database, nothing if there is none.
    app.get('/chicken', async (request, response) => {
        const query = `SELECT * FROM chicken;`;
        const res = await client.query(query)
        response.send(res.rows[0])
    })

//Post endpoint, insert a new chicken in the database after verifying that it is empty.
    app.post('/chicken', async (request, response) => {

        //Verification that the request contains the necessary parameters
        let error_str = ''
        if (!request.query.name) { error_str += 'name is missing; ' }
        if (!request.query.weight) { error_str += 'weight is missing; ' }

        if (error_str !== '') {

            //Error due to the absence of required parameters
            response.status(400).send({ error: error_str });
        }
        else {

            //Checking that the database is empty
            const query = `SELECT 1 FROM chicken;`;
            const res = await client.query(query)
            if (res && res.rowCount !== 0) {

                //Error, a chicken already exists 
                response.status(409).send('A Chicken already exists');
            }
            else {

                //Creation of the insert query using pgp
                const ins_query = pgp.helpers.insert(request.query, null, 'chicken');
                let ins_res;
                try {
                    ins_res = await client.query(ins_query)
                    //Success
                    response.status(201).send('Chicken successfully created');
                }
                catch (error) {

                    //Error due to unvalid parameters rejected by the database
                    response.status(400).send('Invalid parameters');
                }
            }
        }
    })

//Put endpoint, insert a new chicken in the database or update the provided parameters if one already exists
    app.put('/chicken', async (request, response) => {

        //Checking that the request query is not empty
        if (Object.keys(request.query).length === 0) {

            //Error due to the fact that the user did not provide parameters
            response.status(400).send('Expecting at least one parameter');
        }
        else {
            //Checking if a chicken already exists
            const query = `SELECT 1 FROM chicken;`;
            const res = await client.query(query)
            let ins_query;
            if (res && res.rowCount !== 0) {

                //Building a query to update the specified parameters on the existing chicken
                ins_query = pgp.helpers.update(request.query, null, 'chicken');
            }
            else {
                //Checking if provided parameters allow us to create a new chicken
                let error_str = ''
                if (!request.query.name) { error_str += 'name is missing; ' }
                if (!request.query.weight) { error_str += 'weight is missing; ' }

                if (error_str !== '') {

                    //Error due to the absence of required parameters
                    response.status(400).send({ error: error_str });
                }
                else {

                    //Creation of the insert query
                    ins_query = pgp.helpers.insert(request.query, null, 'chicken');
                }
            }

            let ins_res;
            if (ins_query !== undefined) {
                try {
                    ins_res = await client.query(ins_query)

                    //Success
                    response.status(201).send('Chicken successfully created');
                }
                catch (error) {
                    //Error due to unvalid parameters rejected by the database
                    response.status(400).send('Invalid parameters');
                }
            }
        }
    })

//Patch endpoint, modify parameters of an existing chicken
    app.patch('/chicken', async (request, response) => {
        //Checking that the request query is not empty
        if (Object.keys(request.query).length === 0) {

            //Error due to the fact that the user did not provide parameters
            response.status(400).send('Expecting at least one parameter');
        }
        else {

            //Checking if a chicken already exists
            const query = `SELECT 1 FROM chicken;`;
            const res = await client.query(query)
            let ins_query;
            if (res && res.rowCount !== 0) {

                //Building a query to update the specified parameters on the existing chicken
                ins_query = pgp.helpers.update(request.query, null, 'chicken');
                let ins_res;
                if (ins_query !== undefined) {
                    try {
                        ins_res = await client.query(ins_query)

                        //Success
                        response.status(201).send('Chicken successfully updated');
                    }
                    catch (error) {

                        //Error due to unvalid parameters rejected by the database
                        response.status(400).send('Invalid parameters');
                    }
                }
            }
            else {
                //Error caused by the emptiness of the database
                response.status(400).send('No chicken to patch');
            }
        }
    })

//Delete endpoints; delete the existing chicken if it exists, do nothing if it doesn't
    app.delete('/chicken', async (request, response) => {
        const query = `DELETE FROM chicken;`;
        const res = await client.query(query)
        response.send('Chicken successfully deleted')
    })

//'/Chicken/run' endpoint, increase by one the steps of the existing chicken
    app.all('/chicken/run', async (request, response) => {

        //Checking if a chicken already exists
        const query = `SELECT * FROM chicken;`;
        const res = await client.query(query)
        if (!res.rows[0]) {

            //Error caused by the emptiness of the database
            response.status(400).send('No chicken available');
        }
        else {
            
            //Creating an incremented steps and object and setting the isRunning boolean on true 
            let steps_ = { steps: res.rows[0].steps + 1, isrunning: true}
            let ins_query = pgp.helpers.update(steps_, null, 'chicken');
            let ins_res;
            try {
                ins_res = await client.query(ins_query)
                //Success
                response.status(201).send('The chicken has run ' + (res.rows[0].steps + 1) +' steps now' );
            }
            catch (error) {
                //Error due to unvalid parameters rejected by the database
                response.status(400).send('Invalid parameters');
            }
        }
    })

    //Sarting the Application on the provided port and returning it
    return app.listen(back_end_port, () => { console.log('app listening on port ' + back_end_port) })
}

//Calling the function with the parameters of my computer.
chicken_run("postgres", "localhost", "rootpass", 5432, 3000)