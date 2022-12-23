import { Card } from "flowbite-react";
import { ClockClockwise } from "phosphor-react";

import DataTable from "../DataTable";
import IrrigationForm from "../Form";

const MainCard = () => {
  return (
    <Card>
      <h1 className="text-2xl font-bold text-blue-500">
        Sistema de Irrigação Automática 💧🪴
      </h1>

      <IrrigationForm />

      <div className="grid place-items-center">
        <h3 className="text-center font-medium">Umidade desejada atual:</h3>
        <h5>50%</h5>
      </div>

      <hr />

      <div className="flex items-center">
        <ClockClockwise size={16} weight="bold" />
        <h5 className="ml-2 text-lg font-bold">Histórico de irrigação</h5>
      </div>

      <DataTable />
    </Card>
  );
};

export default MainCard;
