'use strict';

const products = require('../utils/products');

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up(queryInterface, Sequelize) {
    await queryInterface.bulkInsert('productModels', products, {});
  },

  async down(queryInterface, Sequelize) {
    await queryInterface.bulkDelete('productModels', products, {});
  },
};
