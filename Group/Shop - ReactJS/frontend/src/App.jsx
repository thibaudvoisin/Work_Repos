import React from "react";
import { RouterProvider } from "react-router-dom";
import "./App.css";
import { AppContext, initialState, reducer } from "./AppContext";
import { router } from "./Layout";

function App() {
  const [state, dispatch] = React.useReducer(reducer, initialState);

  return (
    <AppContext.Provider value={{ state, dispatch }}>
      <RouterProvider router={router} />
    </AppContext.Provider>
  );
}

export default App;
