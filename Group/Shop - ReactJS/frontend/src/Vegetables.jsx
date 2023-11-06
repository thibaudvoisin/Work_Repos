import useAppContext from "./AppContext";
import ProductCard from "./ProductCard";
import imgVegetable from "./images/vegetables.jpg";
import { getVegetablesFromGarden } from "./helpers";

function EmptyVegetable() {
  return <span>Aucun Fruit et Légumes disponibles dans un rayon de 15 km</span>;
}

function Vegetables() {
  const {
    state: { gardens },
  } = useAppContext();
  const vegetables = getVegetablesFromGarden(gardens);
  return (
    <>
      <h4>Fruit & Légumes</h4>
      <div className="product-items">
        {vegetables.length ? (
          vegetables.map(function (vegetable, i) {
            const productId = `${vegetable.gardenId}-${vegetable.name}`;

            return (
              <ProductCard
                key={i}
                id={productId}
                productName={vegetable.name}
                price={vegetable.price}
                distance={vegetable.distance}
                gardenTitle={vegetable.gardenTitle}
                imageSrc={imgVegetable}
              />
            );
          })
        ) : (
          <EmptyVegetable />
        )}
      </div>
    </>
  );
}

export default Vegetables;