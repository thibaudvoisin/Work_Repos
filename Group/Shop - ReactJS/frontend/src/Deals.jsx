import CardTemplate from "./CardTemplate";
import ProductCard from "./ProductCard";

function Deals() {
  return (
    <>
      <div className="deals">
        <h1>Promotions</h1>
        <div className="row">
          <div className="col">
            <ProductCard
              title="Carotte"
              price={1.42}
              discount={0.23}
              imageSrc={
                "/vegetable/carrots.jpg"
              }
            />
          </div>
          <div className="col">
            <ProductCard
              title="Entretcôte de boeuf"
              price={9.23}
              discount={3.45}
              imageSrc={
                "/red-meat/beef-entrecote.png"
              }
            />
          </div>
          <div className="col">
            <ProductCard
              title="Gigot d'agneau"
              price={8.32}
              discount={1.32}
              imageSrc={
                "/red-meat/raw-lamb-leg.png"
              }
            />
          </div>
        </div>
      </div>
    </>
  );
}

export default Deals;
