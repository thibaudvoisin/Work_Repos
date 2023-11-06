import express from 'express';
const router = express.Router();

/**
 * @swagger
 * /health:
 *   get:
 *     tags : [Health]
 *     summary: Get the health of the API
 *     responses:
 *       200:
 *         description: Normal Response
 *         content:
 *          application/json:
 *           schema:
 *               type: json
 *               example: {api : "up"}
 *
 */

router.get('/health', function (_req, res) {
  res.status(200).send({ api: 'up' });
});

export default router;
