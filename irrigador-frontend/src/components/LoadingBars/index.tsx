import Loader from "react-loader-spinner";

const LoadingBars = () => {
  return (
    <div className="flex items-center">
      <Loader type="ThreeDots" color="#00BFFF" height={80} width={80} />
      <p>Carregando dados...</p>
    </div>
  );
};

export default LoadingBars;
