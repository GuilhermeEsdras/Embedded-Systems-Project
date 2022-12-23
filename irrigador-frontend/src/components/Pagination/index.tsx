import { Pagination } from "flowbite-react";

import { IrrigationPage } from "~/types/irrigation";

interface PaginationProps {
  page: IrrigationPage;
  onPageChange(index: number): void;
}

const IrrigationPagination = ({ page, onPageChange }: PaginationProps) => {
  return (
    <div className="flex items-center justify-center text-center">
      <Pagination
        currentPage={page.number}
        layout="table"
        totalPages={page.totalPages}
        onPageChange={() => onPageChange(page.number + 1)}
      />
    </div>
  );
};

export default IrrigationPagination;
