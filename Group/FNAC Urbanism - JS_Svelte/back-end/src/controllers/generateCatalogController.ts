import {
  generateShopCatalog,
  generateWarehouseCatalog,
} from '../services/generateCatalogs';
import { Request, Response, NextFunction } from 'express';
import { ApiError } from '../utils/ApiError';

export async function generateshopCatalogController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    const data = await _req.body;

    if (!data || !data.stores || !Array.isArray(data.stores)) {
      throw ApiError.badRequest('Invalid store data format !');
    }

    const shopCatalogs = [];

    const shopIds = data.stores.map((store: { id: number }) => store.id);
    for (const shopId of shopIds) {
      if (shopId == undefined || isNaN(shopId) || shopId < 0) {
        throw ApiError.badRequest('Invalid store data format (id) !');
      }
      const shopCatalog = await generateShopCatalog(shopId);
      shopCatalogs.push({ shopId: shopId, products: shopCatalog });
    }

    res.status(200).json({ catalogs: shopCatalogs });
  } catch (error) {
    next(error);
  }
}

export async function generatewarehouseCatalogController(
  _req: Request,
  res: Response,
  next: NextFunction
) {
  try {
    const data = await _req.body;

    if (!data || !data.warehouse_list || !Array.isArray(data.warehouse_list)) {
      throw ApiError.badRequest('Invalid warehouse data format !');
    }

    const warehouseCatalogs = [];

    const warehouseIds = data.warehouse_list.map(
      (warehouse: { id: number }) => warehouse.id
    );
    for (const warehouseId of warehouseIds) {
      if (warehouseId == undefined || isNaN(warehouseId) || warehouseId < 0) {
        throw ApiError.badRequest('Invalid warehouse data format (id) !');
      }
      const warehouseCatalog = await generateWarehouseCatalog(warehouseId);
      warehouseCatalogs.push({
        warehouseId: warehouseId,
        products: warehouseCatalog,
      });
    }

    res.status(200).json({ catalogs: warehouseCatalogs });
  } catch (error) {
    next(error);
  }
}
