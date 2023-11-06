import {
  Model,
  Column,
  Table,
  PrimaryKey,
  AutoIncrement,
  ForeignKey,
  BelongsTo,
} from 'sequelize-typescript';
import { productModel } from './Product';

@Table({
  timestamps: true,
})
export class warehousecatalogModel extends Model {
  @PrimaryKey
  @AutoIncrement
  @Column
  id!: number;

  @ForeignKey(() => productModel)
  @Column
  product_id!: number;

  @BelongsTo(() => productModel)
  product!: productModel;

  @Column
  warehouse_id!: number;

  @Column
  max_quantity!: number;

  @Column
  min_quantity!: number;

  @Column
  stock!: number;
}
