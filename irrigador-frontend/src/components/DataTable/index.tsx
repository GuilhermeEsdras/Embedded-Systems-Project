import { useEffect, useState } from "react";

import axios from "axios";
import { Table } from "flowbite-react";

import { BASE_URL } from "~/api/requests";
import { IrrigationPage } from "~/types/irrigation";
import { formatLocalDate } from "~/utils/format";

import LoadingBars from "../LoadingBars";
import IrrigationPagination from "../Pagination";

interface TableRowProps {
  id: number;
  status: string;
  data: string;
  umidity: number;
}

const TableRow = ({ id, status, data, umidity }: TableRowProps) => {
  return (
    <Table.Row className="bg-white dark:border-gray-700 dark:bg-gray-800">
      <Table.Cell>{id}</Table.Cell>
      <Table.Cell className="whitespace-nowrap font-medium text-gray-900 dark:text-white">
        {status}
      </Table.Cell>
      <Table.Cell>{data}</Table.Cell>
      <Table.Cell>{umidity}%</Table.Cell>
    </Table.Row>
  );
};

const DataTable = () => {
  const [loading, setLoading] = useState(true);
  const [activePage, setActivePage] = useState(0);
  const [page, setPage] = useState<IrrigationPage>({
    first: true,
    last: true,
    number: 0,
    totalElements: 0,
    totalPages: 0,
  });

  useEffect(() => {
    setLoading(true);
    axios
      .get(`${BASE_URL}/status/page?page=${activePage}&size=20&sort=date,desc`)
      .then((response) => {
        console.log(response.data);
        setPage(response.data);
        setLoading(false);
      })
      .catch((response) => {
        console.log(response);
      });
  }, [activePage]);

  const changePage = (index: number) => {
    setActivePage(index);
  };

  return loading ? (
    <LoadingBars />
  ) : (
    <>
      <Table hoverable={true}>
        <Table.Head>
          <Table.HeadCell>ID</Table.HeadCell>
          <Table.HeadCell>Status</Table.HeadCell>
          <Table.HeadCell>Data e Hora</Table.HeadCell>
          <Table.HeadCell>Umidade</Table.HeadCell>
        </Table.Head>
        <Table.Body className="divide-y">
          {/* <TableRow
            key={1}
            data="22/12/2022"
            id={1}
            status="Irrigamento feito com sucesso"
            umidity={50}
          /> */}
          {page.content?.map((pageContent) => {
            return (
              <TableRow
                key={pageContent.id}
                data={formatLocalDate(pageContent.date, "dd/MM/yyy")}
                id={pageContent.id}
                status={pageContent.status}
                umidity={pageContent.umitidy}
              />
            );
          })}
        </Table.Body>
      </Table>
      <IrrigationPagination page={page} onPageChange={changePage} />
    </>
  );
};

export default DataTable;
