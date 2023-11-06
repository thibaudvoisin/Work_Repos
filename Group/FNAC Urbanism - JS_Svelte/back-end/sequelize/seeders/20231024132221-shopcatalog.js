'use strict';

const { generateCatalog, catalogType } = require('../utils/generateCatalog');

let generatedCatalog = generateCatalog(catalogType.Shop, 1);

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.bulkInsert('shopcatalogModels', generatedCatalog, {});
  },

  async down(queryInterface, Sequelize) {
    await queryInterface.bulkDelete('shopcatalogModels', generatedCatalog, {});
  },
};
