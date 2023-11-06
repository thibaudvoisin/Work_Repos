export function getVegetablesFromGarden(gardens) {
    return gardens.reduce(function (vegetables, garden) {
      const gardenVegetables = garden.products.vegetable;
      const items = gardenVegetables.map(function (vegetable) {
        const item = {
          name: vegetable.name,
          price: vegetable.price,
          distance: garden.distance,
          gardenTitle: garden.title,
          gardenId: garden.id,
        };
        return item;
      });
      return [...vegetables, ...items];
    }, []);
  }
  
  export function getChickensFromGarden(gardens) {
    return gardens.reduce(function (chickens, garden) {
      const gardenChickens = garden.products.chicken;
      const items = gardenChickens.map(function (chicken) {
        const item = {
          name: `${chicken.name} - ${chicken.description}`,
          price: chicken.price,
          distance: garden.distance,
          gardenTitle: garden.title,
          gardenId: garden.id,
        };
        return item;
      });
      return [...chickens, ...items];
    }, []);
  }
  
  export function getRedMeatFromGarden(gardens) {
    return gardens.reduce(function (meatParts, garden) {
      const gardenRedMeats = garden.products.red_meat;
      const items = gardenRedMeats.reduce(function (redMeatList, redMeat) {
        const beefMeatParts = redMeat.parts.map(function (meatPart) {
          return {
            name: `${redMeat.beef.name} - ${meatPart.name}`,
            price: meatPart.price,
            distance: garden.distance,
            gardenTitle: garden.title,
            gardenId: garden.id,
          };
        });
        return [...redMeatList, ...beefMeatParts];
      }, []);
      return [...meatParts, ...items];
    }, []);
  }
  
  export function getJuiceFromGarden(gardens) {
    return gardens.reduce(function (juices, garden) {
      const gardenJuices = garden.products.juice;
      const items = gardenJuices.map(function (juice) {
        const item = {
          name: juice.name,
          price: juice.price,
          distance: garden.distance,
          gardenTitle: garden.title,
          gardenId: garden.id,
        };
        return item;
      });
      return [...juices, ...items];
    }, []);
  }
  
  export function getWinesFromGarden(gardens) {
    return gardens.reduce(function (wines, garden) {
      const gardenWines = garden.products.wine;
      const items = gardenWines.map(function (wine) {
        const item = {
          name: wine.name,
          price: wine.price,
          distance: garden.distance,
          gardenTitle: garden.title,
          gardenId: garden.id,
        };
        return item;
      });
      return [...wines, ...items];
    }, []);
  }