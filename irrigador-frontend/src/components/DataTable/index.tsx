import { useEffect, useState } from "react";

import axios from "axios";
import { format } from "date-fns";
import { Table } from "flowbite-react";

import { IrrigationPage } from "~/types/irrigation";

import { BASE_URL } from "../../api/requests";
import IrrigationPagination from "../Pagination";

interface TableRowProps {
  id: number;
  status: string;
  data: string;
  umidity: string;
}

export const formatLocalDate = (date: string, pattern: string): string => {
  const dt = new Date(date);
  const dtDateOnly = new Date(
    dt.valueOf() + dt.getTimezoneOffset() * 60 * 1000
  );
  return format(dtDateOnly, pattern);
};

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
    const intervalId = setInterval(() => {
      axios({
        method: "get",
        url: `${BASE_URL}/status/page?page=${activePage}&sort=desc`,
        withCredentials: false,
        headers: {
          "Access-Control-Allow-Origin": "*",
        },
      })
        .then((response) => {
          console.log(response.data);
          setPage(response.data);
          setLoading(false);
        })
        .catch((response) => {
          console.log(response);
        });
    }, 1000);
    return () => clearInterval(intervalId);
  }, [activePage]);

  const changePage = (index: number) => {
    setActivePage(index);
  };

  const tableRendered = (
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
            console.log(pageContent);
            return (
              <TableRow
                key={pageContent.id}
                data={formatLocalDate(pageContent.timestamp, "dd/MM/yyy HH:mm")}
                id={pageContent.id}
                status={pageContent.status}
                umidity={pageContent.umidity}
              />
            );
          })}
        </Table.Body>
      </Table>
      <IrrigationPagination page={page} onPageChange={changePage} />
    </>
  );

  // return loading ? <LoadingBars /> : tableRendered;
  return tableRendered;
};

export default DataTable;
