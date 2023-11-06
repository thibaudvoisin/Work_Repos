import express from 'express';
import { exposeWarehouseController } from '../controllers/exposeWarehouseController';
import { ApiError } from '../utils/ApiError';
const router = express.Router();

/**
 * @swagger
 * /ask-warehouses:
 *   post:
 *     tags : [Commands]
 *     summary: Get the list of warehouses that have every item from the order
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               product_ids:
 *                 type: array
 *                 items:
 *                   type: integer
 *             example:
 *               product_ids: [1, 4, 15]
 *     responses:
 *       200:
 *         description: Normal Response
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 products:
 *                   type: array
 *                   items:
 *                     type: object
 *                     properties:
 *                       id:
 *                         type: integer
 *                       warehouses:
 *                         type: array
 *                         items:
 *                           type: object
 *                           properties:
 *                             id:
 *                               type: integer
 *                             stock:
 *                               type: integer
 *             example:
 *               products:
 *                 - product_id: 1
 *                   warehouses:
 *                     - warehouse_id: 1
 *                       stock: 2
 *                     - warehouse_id: 2
 *                       stock: 4
 *                 - product_id: 2
 *                   warehouses:
 *                     - warehouse_id: 1
 *                       stock: 4
 *                     - warehouse_id: 2
 *                       stock: 1
 */

router.post('/ask-warehouses', async function (_req, res, next) {
  try {
    await exposeWarehouseController(_req, res, next);
  } catch (error) {
    next(error);
  }
});

export default router;
