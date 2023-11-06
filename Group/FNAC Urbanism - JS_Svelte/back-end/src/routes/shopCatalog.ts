import express from 'express';
import { shopCatalogController } from '../controllers/shopCatalogController';
const router = express.Router();

/**
 * @swagger
 * /catalog/shops:
 *   get:
 *     tags:
 *       - Catalog
 *     summary: Get the shops catalog
 *     responses:
 *       200:
 *         description: Normal Response
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 type: object
 *                 properties:
 *                   id:
 *                     type: integer
 *                     description: ID du produit
 *                   name:
 *                     type: string
 *                     description: Nom du produit
 *                   description:
 *                     type: string
 *                     description: Description du produit
 *                   image:
 *                     type: string
 *                     description: URL de l'image du produit
 *                   price:
 *                     type: number
 *                     description: Prix du produit
 *                   warrantyDuration:
 *                     type: integer
 *                     description: Durée de la garantie
 *                   createdAt:
 *                     type: string
 *                     description: Date de création du produit
 *                   updatedAt:
 *                     type: string
 *                     description: Date de mise à jour du produit
 *                   maxQuantity:
 *                     type: integer
 *                     description: Quantité maximale disponible du produit
 *                   stock:
 *                     type: integer
 *                     description: Quantité disponible du produit
 *             example:
 *               shops:
 *                 - shopId: 1
 *                   products:
 *                     - id: 41
 *                       name: Balai vapeur Bissell PowerFresh
 *                       description: Un balai vapeur pour un nettoyage en profondeur des sols
 *                       image: https://example.com/images/bissell_powerfresh.jpg
 *                       price: 89.99
 *                       warranty_duration: 1
 *                       createdAt: "2023-10-30T16:06:41.679Z"
 *                       updatedAt: "2023-10-30T16:06:41.679Z"
 *                       max_quantity: 386
 *                     - id: 22
 *                       name: Aspirateur balai Shark DuoClean
 *                       description: Un aspirateur balai polyvalent avec une technologie de nettoyage avancée
 *                       image: https://example.com/images/shark_duoclean.jpg
 *                       price: 349.99
 *                       warranty_duration: 2
 *                       createdAt: "2023-10-30T16:06:41.679Z"
 *                       updatedAt: "2023-10-30T16:06:41.679Z"
 *                       max_quantity: 320
 */

router.get('/catalog/shops', async function (_req, res, next) {
  try {
    await shopCatalogController(_req, res, next);
  } catch (error) {
    next(error);
  }
});

export default router;
