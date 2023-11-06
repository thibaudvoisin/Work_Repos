//Loading modules

//Pgttols and pg are Postgres frameworks to initialize a database.
var pgtools = require("pgtools");
const { Client } = require('pg');

//Async function to create the 'chicken_db' database with specified parameters
async function init_db(user_, host_, password_, port_)
{
    const config = {
        user: user_,
        host: host_,
        password: password_,
        port: port_
      };
      await pgtools.createdb(config, "chicken_db", function(err, res) {
        if (err) 
        {
          console.error(err);
          process.exit(-1);
        }

        console.log("Database has been successfully created")
      });
}


//Main function to create the database and the chicken table
async function main(user_, host_, password_, port_)
{
    //Await for the database to be creating before trying to insert a new table in it
    await init_db(user_, host_, password_, port_);

    //Connecting to the newly created database
    const client = new Client({
        user: user_,
        host: host_,
        password: password_,
        port: port_,
        database: 'chicken_db',
    });

    //Creating a command to execute a query and return its success
    const execute = async (query) => {
        try 
        {
            await client.connect();
            await client.query(query);
            return true;
        } 
    
        catch (error) 
        {
            console.error(error.stack);
            return false;
        } 
        finally 
        {
            await client.end();
        }
    };

    //Query to create the 'chicken' table
    const query = `
        CREATE TABLE IF NOT EXISTS chicken (
	        id SERIAL,
	        name VARCHAR(100) NOT NULL,
	        birthday DATE,
            weight integer NOT NULL,
            steps integer DEFAULT 0,
            isRunning BOOLEAN DEFAULT FALSE,
	        PRIMARY KEY (id)
        );`;


    execute(query).then(res => {
        if (res) 
        {
            console.log('Table has been successfully created');
        }
        else
        {
            console.log('Error during the creation of the table; check the log above to fix it or create the table manually using the documentations query.');
        }
    });
}

main("postgres", "localhost", "rootpass", 5432)