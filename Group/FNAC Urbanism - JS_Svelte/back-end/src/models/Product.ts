import {
  Model,
  Column,
  Table,
  PrimaryKey,
  AutoIncrement,
  DataType,
  HasMany,
} from 'sequelize-typescript';
import { eshopcatalogModel } from './Eshopcatalog';
import { shopcatalogModel } from './Shopcatalog';
import { warehousecatalogModel } from './Warehousecatalog';

@Table({
  timestamps: true,
})
export class productModel extends Model {
  @PrimaryKey
  @AutoIncrement
  @Column
  id!: number;

  @Column
  name!: string;

  @Column
  description!: string;

  @Column
  image!: string;

  @Column({
    type: DataType.FLOAT,
  })
  price!: number;

  @Column
  warranty_duration!: number;

  @HasMany(() => eshopcatalogModel)
  eshopCatalogs!: eshopcatalogModel[];

  @HasMany(() => warehousecatalogModel)
  warehouseCatalogs!: warehousecatalogModel[];

  @HasMany(() => shopcatalogModel)
  shopCatalogs!: shopcatalogModel[];
}
