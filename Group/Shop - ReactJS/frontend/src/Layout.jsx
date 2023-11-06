import React from "react";
import { createBrowserRouter, Outlet } from "react-router-dom";
import Banner from "./Banner";
import Basket from "./Basket";
import Chickens from "./Chickens";
import Wines from "./Wines";
import MainMenu from "./MainMenu";
import Product from "./Product";
import ProductList from "./ProductList";
import RedMeatList from "./RedMeatList";
import Vegetables from "./Vegetables";
import Juices from "./Juices";

export const router = createBrowserRouter([
  {
    path: "/",
    element: <Layout />,
    errorElement: <h1>Oups, something wrong happened</h1>,
    children: [
      {
        path: "/product",
        element: <Product />,
        children: [
          {
            path: "/product/vegetable",
            element: <Vegetables />,
          },
          {
            path: "/product/chicken",
            element: <Chickens />,
          },
          {
            path: "/product/red-meat",
            element: <RedMeatList />,
          },
          {
            path: "/product/drink",
            element: <Juices />,
          },
          {
            path: "/product/wine",
            element: <Wines />,
          },
        ],
      },
      {
        path: "/command",
        element: <h1>Commands</h1>,
      },
      {
        path: "/basket",
        element: <Basket />,
      },
    ],
  },
]);

function Layout() {
  return (
    <div className="App">
      <MainMenu />
      <Banner />
      <Outlet />
    </div>
  );
}