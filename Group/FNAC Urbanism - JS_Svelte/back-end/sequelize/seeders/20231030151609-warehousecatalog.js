'use strict';

const { getRandomArbitrary } = require('../utils/generateCatalog');

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    // Récupère les données de la table WarehouseCatalog et remplis les champs max_quantity, min_quantity et stock qui sont NULL et enregistre les données dans la table
    const warehouseCatalog = await queryInterface.sequelize.query(
      `SELECT * FROM "warehousecatalogModels" WHERE max_quantity IS NULL AND min_quantity IS NULL AND stock IS NULL;`,
      { type: Sequelize.QueryTypes.SELECT }
    );

    for (const row of warehouseCatalog) {
      const { id } = row;
      let max_quantity = getRandomArbitrary(100, 1000);
      let min_quantity = Math.floor(max_quantity / getRandomArbitrary(2, 10));

      await queryInterface.sequelize.query(
        `UPDATE "warehousecatalogModels" SET max_quantity = ${max_quantity}, min_quantity = ${min_quantity}, stock = ${max_quantity} WHERE id = ${id};`
      );
    }
  },

  async down(queryInterface, Sequelize) {
    /**
     * Add commands to revert seed here.
     *
     * Example:
     * await queryInterface.bulkDelete('People', null, {});
     */
  },
};
