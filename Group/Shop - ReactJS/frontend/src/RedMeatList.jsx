import useAppContext from "./AppContext";
import ProductCard from "./ProductCard";
import imgRedMeat from "./images/red-meat.jpg";
import { getRedMeatFromGarden } from "./helpers";

function EmptyRedMeat() {
  return <span>Aucune viande rouge disponible dans un rayon de 15 km</span>;
}

function RedMeatList() {
  const {
    state: { gardens },
  } = useAppContext();
  const redMeatList = getRedMeatFromGarden(gardens);
  return (
    <>
      <h4>Viande rouge</h4>
      <div className="product-items">
        {redMeatList.length ? (
          redMeatList.map(function (redMeat, i) {
            const productId = `${redMeat.gardenId}-${redMeat.name}`;

            return (
              <ProductCard
                key={i}
                id={productId}
                productName={redMeat.name}
                price={redMeat.price}
                distance={redMeat.distance}
                gardenTitle={redMeat.gardenTitle}
                imageSrc={imgRedMeat}
              />
            );
          })
        ) : (
          <EmptyRedMeat />
        )}
      </div>
    </>
  );
}

export default RedMeatList;