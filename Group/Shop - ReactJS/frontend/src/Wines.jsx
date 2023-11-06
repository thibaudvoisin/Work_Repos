import useAppContext from "./AppContext";
import ProductCard from "./ProductCard";
import imgWineGlass from "./images/wine.jpg";
import { getWinesFromGarden } from "./helpers";

function EmptyDrink() {
  return <span>Aucun vin disponible dans un rayon de 15 km</span>;
}

function Wines() {
  const {
    state: { gardens },
  } = useAppContext();
  const wines = getWinesFromGarden(gardens);
  return (
    <>
      <h4>Viande rouge</h4>
      <div className="product-items">
        {wines.length ? (
          wines.map(function (wine, i) {
            const productId = `${wine.gardenId}-${wine.name}`;

            return (
              <ProductCard
                key={i}
                id={productId}
                productName={wine.name}
                price={wine.price}
                distance={wine.distance}
                gardenTitle={wine.gardenTitle}
                imageSrc={imgWineGlass}
              />
            );
          })
        ) : (
          <EmptyDrink />
        )}
      </div>
    </>
  );
}

export default Wines;