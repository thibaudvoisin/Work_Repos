import express from "express";
import cors from "cors";
import distance from "./distance.mjs";
import gardens from "./data/gardens.json" assert { type: "json" };

const app = express();
const port = 3000;

const EIFFEL_TOWER_POSITION = { lat: 48.8583145, lng: 2.292334 };

// configure your endpoints to expect JSON format for payloads
app.use(express.json());

// Makes sure CORS are implemented
app.use(cors({ origin: "*" }));

// A simple status check route, to be queried to make sure your web API is running
app.get("/", function (request, response) {
  response.send({ status: "healthy" });
});

// Returns back the fly distance between the provided position
// and the eiffel tower (in km)
// Call it from curl!:
//   curl -XPOST -H "Content-Type: application/json" -d '{"position": {"lat": 48.856614,"lng": 2.3522219}}' http://localhost:3000/v1/distance
app.post("/v1/distance", function (request, response) {
  const inputPostion = request.body.position;
  if (inputPostion && inputPostion.lat && inputPostion.lng) {
    const distanceToEiffelTower = distance(inputPostion, EIFFEL_TOWER_POSITION);
    response.send({ distance: distanceToEiffelTower });
  } else {
    response.status(400).send({
      error: `wrong input. Please specify a payload like { "position": { "lat": 46.123, "lng": 2.123 } }`,
    });
  }
});

// TODO: you have to implement this service!
//
// Right now, it only echos what you've called the service with, and
// adds the two first "potagers" of your JSON gardens data
// Try it out!:
// From your terminal, (considering you've run this serivce):
//  curl -XPOST  -H "Content-Type: application/json" -d '{"position": {"lat": 48.856614,"lng": 2.3522219}}' http://localhost:3000/v1/product\?radius=15
app.post("/v1/product", function (request, response) {
  const inputPosition = request.body.position;
  const radius = +request.query.radius;

  // Calculate the distances between the input position and each potager's position
  const distances = gardens.map((potager) => ({
    potager,
    distance: distance(inputPosition, potager.position)
  }));

  var all_potag = [];
  for (var i in distances){
    var potag = distances[i];
    if (potag.distance <= radius)
    {
      var tmpotager = potag.potager;
      tmpotager.distance = potag.distance;
      all_potag.push(tmpotager);
    }
  }
    
  response.send(all_potag);
});

app.listen(port, function () {
  console.log(`Server started on localhost:${port}`);
});
