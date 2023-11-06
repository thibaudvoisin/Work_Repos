# ChickenRun
Chicken Run is a Node JS API for Clac des Doigts.

The application uses Express and PostGres frameworks, it provides endpoints to create, modify, delete and make run a chicken.

The endpoints are the following :
<pre>
- GET    ('/chicken') -> Return all chicken details (Name, BirthDay, Weight, Steps, isRunning)
- POST   ('/chicken') -> Create a new chicken if none exists; Minimal parameters required are Name and Weight
- PUT    ('/chicken') -> Create a new Chicken if none exist, else update it
- PATCH  ('/chicken') -> Update the existing chicken with the given parameters
- DELETE ('/chicken') -> Delete the existing chicken
- *  ('/chicken/run') -> Can take all differents HTTP methods, set the isRunning boolean on True and increment the steps of the chicken
</pre>

### USAGE 

To use the application you must start by getting all the required modules, you can install them by typing the `npm install` command on the src folder.

When this is done, you must create a PostGres database. To do that, install and configure PostGres on your machine, then, you can open init_database.js. At the end of the file, you will see this following line `main("postgres", "localhost", "rootpass", 5432)`, replace these informations with the ones of you PostGres session.
Then, run this script with `node init_database.js`, this will create the database and the table needed by the application. 

Once your database is set up, you can start the main app by doing the same process, you will find `chicken_run("postgres", "localhost", "rootpass", 5432, 3000)` at the end of the Chicken_run_API.js file, enter your informations and you would then be able to run the ChickenRun app with `node Chicken_run_API.js`
