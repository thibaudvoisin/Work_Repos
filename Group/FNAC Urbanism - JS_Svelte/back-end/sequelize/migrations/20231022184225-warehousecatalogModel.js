'use strict';

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.sequelize.transaction((t) => {
      return Promise.all([
        queryInterface.addColumn(
          'warehousecatalogModels',
          'max_quantity',
          {
            type: Sequelize.INTEGER,
            allowNull: true,
          },
          { transaction: t }
        ),
        queryInterface.addColumn(
          'warehousecatalogModels',
          'min_quantity',
          {
            type: Sequelize.INTEGER,
            allowNull: true,
          },
          { transaction: t }
        ),
        queryInterface.addColumn(
          'warehousecatalogModels',
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
        queryInterface.removeColumn('warehousecatalogModels', 'max_quantity', {
          transaction: t,
        }),
        queryInterface.removeColumn('warehousecatalogModels', 'min_quantity', {
          transaction: t,
        }),
        queryInterface.removeColumn('warehousecatalogModels', 'stock', {
          transaction: t,
        }),
      ]);
    });
  },
};
