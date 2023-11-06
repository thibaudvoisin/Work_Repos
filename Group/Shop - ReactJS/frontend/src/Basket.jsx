import useAppContext from "./AppContext";
import "./Basket.css";

function EmptyBasket() {
  return <h4>Your basket is empty</h4>;
}

function Basket() {
  const {
    state: { basket },
    dispatch,
  } = useAppContext();

  const handleRemoveItem = (item) => {
    dispatch({ type: "DELETE_BASKET_ITEM", item });
  };

  return (
    <div className="basket">
      <span />
      <table>
        <thead>
          <tr>
            <th>Produit</th>
            <th>Prix</th>
            <th>Action</th>
          </tr>
        </thead>
        <tbody>
          {basket.length > 0 ? (
            basket.map((item, idx) => (
              <tr key={idx} className="product-list">
                <td className="product">{item.title}</td>
                <td>{item.price} € / kg</td>
                <td>
                  <button className="delete-button" onClick={() => handleRemoveItem(item)}>Remove</button>
                </td>
              </tr>
            ))
          ) : (
            <tr>
              <td colSpan="3">
                <EmptyBasket />
              </td>
            </tr>
          )}
        </tbody>
      </table>
      <span />
    </div>
  );
}

export default Basket;
