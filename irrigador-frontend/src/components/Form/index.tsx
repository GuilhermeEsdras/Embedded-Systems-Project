import { useState } from "react";

import axios from "axios";
import { Button, Label, TextInput } from "flowbite-react";

const IrrigationForm = () => {
  const [umidity, setUmidity] = useState<number>();
  const [umidityChosen, setUmidityChosen] = useState<number>();

  const handleSendUmidity = (event: React.SyntheticEvent) => {
    event.preventDefault();

    setUmidityChosen(umidity);

    const data = {
      moisture: umidity?.toString(),
    };

    axios({
      method: "post",
      url: `${import.meta.env.VITE_BACKEND_URL}/status/moisture`,
      data: data,
      headers: {
        "Content-Type": "application/json",
      },
    })
      .then((response) => {
        console.log(response);
      })
      .catch((response) => {
        console.log(response);
      });
  };

  return (
    <>
      <form className="flex flex-col gap-4" onSubmit={handleSendUmidity}>
        <div>
          <div className="mb-2 block">
            <Label
              htmlFor="umidity-desired"
              value="Digite a umidade desejada"
            />
          </div>
          <TextInput
            id="umidity-desired"
            placeholder="Umidade desejada (0 ~ 100)"
            required={true}
            pattern="[0-9]*"
            type="number"
            max={100}
            onChange={(e) => setUmidity(e.target.valueAsNumber)}
            value={umidity}
          />
        </div>
        <Button type="submit">Setar umidade desejada</Button>
      </form>

      <div className="grid place-items-center">
        <h3 className="text-center font-medium">Umidade desejada atual:</h3>
        <h5>{umidityChosen ?? 0}%</h5>
      </div>
    </>
  );
};

export default IrrigationForm;
