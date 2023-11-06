import React from "react";
import { Outlet } from "react-router-dom";
import useAppContext from "./AppContext";
import { API_URL } from "./constant";
import Deals from "./Deals";
import ProductMenu from "./ProductMenu";
import useGeoLocation from "./useGeoLocation";

function Product() {
  const { dispatch } = useAppContext();
  const { latitude, longitude, loading, error } = useGeoLocation();
  React.useEffect(
    function () {
      async function fetchGardens() {
        const userPosition = { position: { lat: latitude, lng: longitude } };
        const url = `${API_URL}/v1/product?radius=15`;
        const response = await fetch(url, {
          method: "POST",
          body: JSON.stringify(userPosition),
          headers: {
            "Content-Type": "application/json",
          },
        });
        const data = await response.json();
        dispatch({ type: "GARDENS_LOADED", gardens: data });
      }
      if (!loading) {
        fetchGardens();
      }
    },
    [loading]
  );

  return loading && !error ? (
    <span>Patience, nous analysons votre position...</span>
  ) : (
    <>
      <Deals />
      <h1>Produits</h1>
      <div className="products">
        <ProductMenu />
        <div>
          <Outlet />
        </div>
      </div>
    </>
  );
}

export default Product;