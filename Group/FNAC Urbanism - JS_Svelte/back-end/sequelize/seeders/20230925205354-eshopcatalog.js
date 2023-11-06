'use strict';

const { generateCatalog, catalogType } = require('../utils/generateCatalog');

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    let generatedCatalog = generateCatalog(catalogType.Eshop);
    await queryInterface.bulkInsert('eshopcatalogModels', generatedCatalog, {});
  },

  async down(queryInterface, Sequelize) {
    await queryInterface.bulkDelete('eshopcatalogModels', null, {});
  },
};
