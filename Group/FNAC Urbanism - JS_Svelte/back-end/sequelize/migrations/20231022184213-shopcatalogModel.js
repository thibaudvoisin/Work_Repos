'use strict';

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.sequelize.transaction((t) => {
      return Promise.all([
        queryInterface.addColumn(
          'shopcatalogModels',
          'max_quantity',
          {
            type: Sequelize.INTEGER,
            allowNull: true,
          },
          { transaction: t }
        ),
        queryInterface.addColumn(
          'shopcatalogModels',
          'min_quantity',
          {
            type: Sequelize.INTEGER,
            allowNull: true,
          },
          { transaction: t }
        ),
        queryInterface.addColumn(
          'shopcatalogModels',
          'stock',
          {
            type: Sequelize.INTEGER,
            allowNull: true,
          },
          { transaction: t }
        ),
      ]);
    });
  },
  down: (queryInterface, Sequelize) => {
    return queryInterface.sequelize.transaction((t) => {
      return Promise.all([
        queryInterface.removeColumn('shopcatalogModels', 'max_quantity', {
          transaction: t,
        }),
        queryInterface.removeColumn('shopcatalogModels', 'min_quantity', {
          transaction: t,
        }),
        queryInterface.removeColumn('shopcatalogModels', 'stock', {
          transaction: t,
        }),
      ]);
    });
  },
};
