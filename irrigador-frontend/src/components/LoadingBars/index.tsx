import { ThreeDots } from "react-loader-spinner";

const LoadingBars = () => {
  return (
    <div className="grid place-items-center">
      <ThreeDots
        height="80"
        width="80"
        radius="9"
        color="#3b82f6"
        ariaLabel="three-dots-loading"
        wrapperStyle={{}}
        visible={true}
      />
      <p className="text-sm">Carregando dados...</p>
    </div>
  );
};

export default LoadingBars;
