import { getProductsFromWarehouses } from '../services/getProducts';
import { ApiError } from '../utils/ApiError';
import { Request, Response, NextFunction } from 'express';

export async function exposeWarehouseController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    if (!_req.body || Object.keys(_req.body).length === 0)
      throw ApiError.badRequest('Body is empty');
    let product_ids = _req.body.product_ids as [];
    const products = await getProductsFromWarehouses(product_ids);
    res.status(200).json({ products });
  } catch (error) {
    next(error);
  }
}
