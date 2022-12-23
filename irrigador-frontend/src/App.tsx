import { Button, Card, Label, Table, TextInput } from "flowbite-react";
import { ClockClockwise } from "phosphor-react";

function App() {
  return (
    <div className="flex min-h-screen flex-col items-center justify-center bg-gradient-to-br from-green-400 to-blue-500">
      <Card>
        <h1 className="text-2xl font-bold text-blue-500">
          Sistema de Irrigação Automática 💦🪴
        </h1>
        <form className="flex flex-col gap-4">
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
            />
          </div>
          <Button type="submit">Setar umidade desejada</Button>
        </form>

        <div className="grid place-items-center">
          <h3 className="text-center font-medium">Umidade desejada atual:</h3>
          <h5>50%</h5>
        </div>

        <hr />

        <div className="flex items-center">
          <ClockClockwise size={16} weight="bold" />
          <h5 className="ml-2 text-lg font-bold">Histórico de irrigação</h5>
        </div>

        <Table hoverable={true}>
          <Table.Head>
            <Table.HeadCell>Status</Table.HeadCell>
            <Table.HeadCell>Nível de Água</Table.HeadCell>
            <Table.HeadCell>Umidade do Solo</Table.HeadCell>
          </Table.Head>
          <Table.Body className="divide-y">
            <Table.Row className="bg-white dark:border-gray-700 dark:bg-gray-800">
              <Table.Cell className="whitespace-nowrap font-medium text-gray-900 dark:text-white">
                Testing
              </Table.Cell>
              <Table.Cell>80%</Table.Cell>
              <Table.Cell>50%</Table.Cell>
            </Table.Row>
          </Table.Body>
        </Table>
        {/* <div className="flex items-center justify-center text-center">
          <Pagination
            currentPage={1}
            layout="table"
            totalPages={1000}
            onPageChange={() => {
              return "Hello world";
            }}
          />
        </div> */}
      </Card>
    </div>
  );
}

export default App;
