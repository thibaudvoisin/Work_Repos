'use strict';

const { generateCatalog, catalogType } = require('../utils/generateCatalog');

let generatedCatalog = generateCatalog(catalogType.Eshop);

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.bulkInsert('eshopcatalogModels', generatedCatalog, {});
  },

  async down(queryInterface, Sequelize) {
    await queryInterface.bulkDelete('eshopcatalogModels', generatedCatalog, {});
  },
};
