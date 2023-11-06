import CardTemplate from "./CardTemplate";

function generateCardTemplate(n) {
  return Array(n)
    .fill(0)
    .map((_, idx) => <CardTemplate key={idx} />);
}

function ProductList({ categoryId, children }) {
  let data = [];
  switch (categoryId) {
    case "vegetable":
      data = generateCardTemplate(4);
      break;
    case "chicken":
      data = generateCardTemplate(2);
      break;
    case "red-meat":
      data = generateCardTemplate(5);
      break;
    case "drink":
      data = generateCardTemplate(6);
      break;
    case "wine":
      data = generateCardTemplate(3);
      break;
    default:
      data = [];
  }
  return (
    <>
      <h4>{children}</h4>
      <div className="product-items">{data}</div>
    </>
  );
}

export default ProductList;