export type Irrigation = {
  id: number;
  status: string;
  date: string;
  umitidy: number;
};

export type IrrigationPage = {
  content?: Irrigation[];
  last: boolean;
  totalElements: number;
  totalPages: number;
  size?: number;
  number: number;
  first: boolean;
  numberOfElements?: number;
  empty?: boolean;
};
