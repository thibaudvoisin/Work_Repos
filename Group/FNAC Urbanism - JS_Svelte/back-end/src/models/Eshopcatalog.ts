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

@Table
export class eshopcatalogModel extends Model {
  @PrimaryKey
  @AutoIncrement
  @Column
  id!: number;

  @ForeignKey(() => productModel)
  @Column
  product_id!: number;

  @BelongsTo(() => productModel)
  product!: productModel;
}
