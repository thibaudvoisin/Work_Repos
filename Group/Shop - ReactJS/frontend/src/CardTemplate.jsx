function CardTemplate({ short }) {
  return <div className={`${short ? "short-" : ""}card-template`}></div>;
}

export default CardTemplate;
