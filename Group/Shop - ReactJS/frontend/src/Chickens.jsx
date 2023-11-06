import useAppContext from "./AppContext";
import ProductCard from "./ProductCard";
import imgChickens from "./images/chickens.jpg";
import { getChickensFromGarden } from "./helpers";

function EmptyChicken() {
  return <span>Aucune poule disponible dans un rayon de 15 km</span>;
}

function Chickens() {
  const {
    state: { gardens },
  } = useAppContext();
  const chickens = getChickensFromGarden(gardens);
  return (
    <>
      <h4>Poules en vente</h4>
      <div className="product-items">
        {chickens.length ? (
          chickens.map(function (chicken, i) {
            const productId = `${chicken.gardenId}-${chicken.name}`;

            return (
              <ProductCard
                key={i}
                id={productId}
                productName={chicken.name}
                price={chicken.price}
                distance={chicken.distance}
                gardenTitle={chicken.gardenTitle}
                imageSrc={imgChickens}
              />
            );
          })
        ) : (
          <EmptyChicken />
        )}
      </div>
    </>
  );
}

export default Chickens;