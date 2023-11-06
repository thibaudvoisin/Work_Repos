import React from "react";

export const initialState = {
  basket: [],
  gardens: [],
};

export const reducer = (state, action) => {
  switch (action.type) {
    case "NEW_BASKET_ITEM":
      const { item } = action;
      return {
        ...state,
        basket: [...state.basket, item],
      };
    case "REMOVE_BASKET_ITEM":
      const { item: toRemove } = action;
      return {
        ...state,
        basket: state.basket.filter((item) => item.id !== toRemove.id),
      };
    case "GARDENS_LOADED":
      const { gardens } = action;
      return {
        ...state,
        gardens,
      };
    default:
      return state;
  }
};

export const AppContext = React.createContext();

function useAppContext() {
  return React.useContext(AppContext);
}

export default useAppContext;