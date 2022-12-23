import { useState } from "react";

import { Button, Label, TextInput } from "flowbite-react";

const IrrigationForm = () => {
  const [umidity, setUmidity] = useState<number>();
  const [currentPage, setCurrentPage] = useState<number>(1);

  const handleSendUmidity = (event: React.SyntheticEvent) => {
    event.preventDefault();
  };

  return (
    <form className="flex flex-col gap-4" onSubmit={handleSendUmidity}>
      <div>
        <div className="mb-2 block">
          <Label htmlFor="umidity-desired" value="Digite a umidade desejada" />
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
  );
};

export default IrrigationForm;
