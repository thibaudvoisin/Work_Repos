import express from 'express';
import { generateshopCatalogController } from '../controllers/generateCatalogController';
const router = express.Router();

/**
 * @swagger
 * /send-stores:
 *   post:
 *     tags : [Catalog]
 *     summary: Generate and send shops catalog.
 *     requestBody:
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               stores:
 *                 type: array
 *                 items:
 *                   type: object
 *                   properties:
 *                     id:
 *                       type: integer
 *                     name:
 *                       type: string
 *                     location:
 *                       type: string
 *                     createdAt:
 *                       type: string
 *                     updatedAt:
 *                       type: string
 *           example:
 *             stores:
 *               - id: 1
 *                 name: "Fnac KB"
 *                 location: "12 rue KB"
 *                 createdAt: "2023-10-08T10:31:57.221Z"
 *                 updatedAt: "2023-10-08T10:31:57.221Z"
 *               - id: 2
 *                 name: "Fnac Montparnasse"
 *                 location: "34 rue Montparnasse"
 *                 createdAt: "2023-10-08T10:31:57.221Z"
 *                 updatedAt: "2023-10-08T10:31:57.221Z"
 *     responses:
 *       200:
 *         description: Normal Response
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 catalogs:
 *                   type: array
 *                   items:
 *                     type: object
 *                     properties:
 *                       shopId:
 *                         type: integer
 *                       products:
 *                         type: array
 *                         items:
 *                           type: object
 *                           properties:
 *                             id:
 *                               type: integer
 *                             name:
 *                               type: string
 *                             description:
 *                               type: string
 *                             image:
 *                               type: string
 *                             price:
 *                               type: number
 *                             warranty_duration:
 *                               type: integer
 *                             createdAt:
 *                               type: string
 *                               format: date-time
 *                             updatedAt:
 *                               type: string
 *                               format: date-time*
 *             example:
 *                  catalogs:
 *                  - shopId: 1
 *                    products:
 *                      - id: 1
 *                        name: "Smartphone Galaxy S22"
 *                        description: "Un smartphone Android puissant avec un appareil photo de pointe"
 *                        image: "https://example.com/images/galaxy_s22.jpg"
 *                        price: 799.99
 *                        warranty_duration: 2
 *                        createdAt: "2023-09-25T12:56:56.345Z"
 *                        updatedAt: "2023-09-25T12:56:56.345Z"
 *                      - id: 2
 *                        name: "Product 2"
 *                        description: "Description 2"
 *                        image: "https://example.com/images/product2.jpg"
 *                        price: 1499.99
 *                        warranty_duration: 3
 *                        createdAt: "2023-09-25T12:56:56.347Z"
 *                        updatedAt: "2023-09-25T12:56:56.347Z"
 *       400:
 *         description: Invalid body format
 */

router.post('/send-stores', async function (_req, res, next) {
  try {
    await generateshopCatalogController(_req, res, next);
  } catch (error) {
    next(error);
  }
});

export default router;
