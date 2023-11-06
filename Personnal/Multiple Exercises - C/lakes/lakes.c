char **color_(char **map, int pos_x, int pos_y)
{
    map[pos_x][pos_y] = '$';

    if (map[pos_x - 1][pos_y] == '.')
        map = color_(map, pos_x - 1, pos_y);

    if (map[pos_x][pos_y - 1] == '.')
        map = color_(map, pos_x, pos_y - 1);

    if (map[pos_x + 1][pos_y] == '.')
        map = color_(map, pos_x + 1, pos_y);

    if (map[pos_x][pos_y + 1] == '.')
        map = color_(map, pos_x, pos_y + 1);

    return map;
}

int lakes(char **map, int width, int height)
{
    int nb_lake = 0;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (map[i][j] == '.')
            {
                map = color_(map, i, j);
                ++nb_lake;
            }
        }
    }
    return nb_lake;
}
