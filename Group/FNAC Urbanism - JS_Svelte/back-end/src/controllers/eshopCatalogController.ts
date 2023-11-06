import { getEshopCatalog } from '../services/getCatalogs';
import { Request, Response, NextFunction } from 'express';
import { ApiError } from '../utils/ApiError';

export async function eshopCatalogController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    const eshopCatalog = await getEshopCatalog();
    res.status(200).json({ catalog: eshopCatalog });
  } catch (error) {
    next(error);
  }
}
