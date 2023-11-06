import express from 'express';
import { eshopCatalogController } from '../controllers/eshopCatalogController';
const router = express.Router();

/**
 * @swagger
 * /catalog/eshop:
 *   get:
 *     tags: [Catalog]
 *     summary: Get the eshop catalog
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
 *                   name:
 *                     type: string
 *                   description:
 *                     type: string
 *                   image:
 *                     type: string
 *                   price:
 *                     type: number
 *                   warranty_duration:
 *                     type: integer
 *                   createdAt:
 *                     type: string
 *                   updatedAt:
 *                     type: string
 *             example:
 *                catalog:
 *                 - id: 1
 *                   name: Smartphone Galaxy S22
 *                   description: Un smartphone Android puissant avec un appareil photo de pointe
 *                   image: https://example.com/images/galaxy_s22.jpg
 *                   price: 799.99
 *                   warranty_duration: 2
 *                   createdAt: "2023-09-25T12:56:56.345Z"
 *                   updatedAt: "2023-09-25T12:56:56.345Z"
 *                 - id: 2
 *                   name: Ordinateur portable Dell XPS 15
 *                   description: Un ordinateur portable haut de gamme avec un écran 4K et un processeur rapide
 *                   image: https://example.com/images/dell_xps_15.jpg
 *                   price: 1499.99
 *                   warranty_duration: 3
 *                   createdAt: "2023-09-25T12:56:56.347Z"
 *                   updatedAt: "2023-09-25T12:56:56.347Z"
 *                 - id: 3
 *                   name: TV OLED LG 55 pouces
 *                   description: Une télévision OLED de 55 pouces avec une qualité d'image exceptionnelle
 *                   image: https://example.com/images/lg_oled_tv.jpg
 *                   price: 1199.99
 *                   warranty_duration: 2
 *                   createdAt: "2023-09-25T12:56:56.347Z"
 *                   updatedAt: "2023-09-25T12:56:56.347Z"
 */

router.get('/catalog/eshop', async function (_req, res, next) {
  try {
    await eshopCatalogController(_req, res, next);
  } catch (error) {
    next(error);
  }
});

export default router;
