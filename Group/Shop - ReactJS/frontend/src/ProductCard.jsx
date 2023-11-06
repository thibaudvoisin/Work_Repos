import React from "react";
import useAppContext from "./AppContext";

function ProductCard({
  imageSrc,
  productName,
  price,
  gardenTitle,
  distance,
  id,
}) {
  const {
    state: { basket },
    dispatch,
  } = useAppContext();

  const alreadyAdded = !!basket.find((item) => item.id === id);

  return (
    <div className="card">
      <div className="card-header">
        <img src={imageSrc} alt={productName} />
      </div>

      <div className="card-content">
        <h4>{productName}</h4>
        <p>
          Vendu par {gardenTitle} à {distance} km
        </p>
      </div>
      <div className="card-footer">
        <span>
          <i>
            <span className="price">{price}</span> € / kg
          </i>
        </span>
      </div>
      <div className="card-actions">
        <button
          socra="add-to-basket"
          disabled={alreadyAdded}
          onClick={function () {
            dispatch({
              type: "NEW_BASKET_ITEM",
              item: { id, price },
            });
          }}
        >
          <i className="fa fa-shopping-cart"></i>{" "}
          {alreadyAdded ? "Déjà ajouté au panier" : "Ajouter au panier"}
        </button>
      </div>
    </div>
  );
}

export default ProductCard;