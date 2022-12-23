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
