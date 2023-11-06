import { getShopCatalogs } from '../services/getCatalogs';
import { Request, Response, NextFunction } from 'express';
import { ApiError } from '../utils/ApiError';

export async function shopCatalogController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    const shopCatalog = await getShopCatalogs();
    res.status(200).json({ shops: shopCatalog });
  } catch (error) {
    next(error);
  }
}
