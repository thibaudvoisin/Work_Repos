import { NavLink } from "react-router-dom";

function NavItem({ to, children }) {
  return (
    <NavLink className={({ isActive }) => (isActive ? "active" : "")} to={to}>
      {children}
    </NavLink>
  );
}

export default NavItem;
