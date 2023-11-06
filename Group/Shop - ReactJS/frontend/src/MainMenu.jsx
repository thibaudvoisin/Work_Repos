import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faShoppingCart } from "@fortawesome/free-solid-svg-icons";
import NavItem from "./NavItem";
import useAppContext from "./AppContext";

function MainMenu() {
  const {
    state: { basket },
  } = useAppContext();
  const numberOfItemsInBasket = basket.length;
  return (
    <header className="menu-horizontal">
      <nav>
        <div className="logo">
          <img width="50" src="/favicon.ico" alt="logo" />
        </div>
        <ul className="links">
          <li>
            <NavItem to="/product">Produits</NavItem>
          </li>
          <li>
            <NavItem to="/command">Commandes</NavItem>
          </li>
          <li>
            <NavItem to="/basket">
              Panier
              {numberOfItemsInBasket > 0 && `(${numberOfItemsInBasket})`}
            </NavItem>
          </li>
        </ul>
      </nav>
    </header>
  );
}

export default MainMenu;
