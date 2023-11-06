import { getGlobalCatalog } from '../services/getCatalogs';
import { Request, Response, NextFunction } from 'express';
import { ApiError } from '../utils/ApiError';

export async function globalCatalogController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    const globalCatalog = await getGlobalCatalog();
    res.status(200).json({ catalog: globalCatalog });
  } catch (error) {
    next(error);
  }
}
