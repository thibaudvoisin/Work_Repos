import useAppContext from "./AppContext";
import ProductCard from "./ProductCard";
import imgJuices from "./images/juice.jpg";
import { getJuiceFromGarden } from "./helpers";

function EmptyJuices() {
  return <span>Aucune boisson disponible dans un rayon de 15 km</span>;
}

function Juices() {
  const {
    state: { gardens },
  } = useAppContext();
  const juices = getJuiceFromGarden(gardens);
  return (
    <>
      <h4>Viande rouge</h4>
      <div className="product-items">
        {juices.length ? (
          juices.map(function (juice, i) {
            const productId = `${juice.gardenId}-${juice.name}`;

            return (
              <ProductCard
                key={i}
                id={productId}
                productName={juice.name}
                price={juice.price}
                distance={juice.distance}
                gardenTitle={juice.gardenTitle}
                imageSrc={imgJuices}
              />
            );
          })
        ) : (
          <EmptyJuices />
        )}
      </div>
    </>
  );
}

export default Juices;