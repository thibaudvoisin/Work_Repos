import { getWarehouseCatalogs } from '../services/getCatalogs';
import { Request, Response, NextFunction } from 'express';
import { ApiError } from '../utils/ApiError';

export async function warehouseCatalogController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    const globalCatalog = await getWarehouseCatalogs();
    res.status(200).json({ warehouses: globalCatalog });
  } catch (error) {
    next(error);
  }
}
