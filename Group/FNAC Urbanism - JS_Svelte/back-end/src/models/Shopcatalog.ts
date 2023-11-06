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
export class shopcatalogModel extends Model {
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
  shop_id!: number;

  @Column
  max_quantity!: number;

  @Column
  min_quantity!: number;

  @Column
  stock!: number;
}
