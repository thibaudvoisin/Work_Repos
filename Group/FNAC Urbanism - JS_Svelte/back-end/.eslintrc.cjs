module.exports = {
  extends: ['eslint:recommended', 'plugin:prettier/recommended'],
  parser: '@typescript-eslint/parser',
  parserOptions: {
    project: './tsconfig.json',
  },
  rules: {
    semi: ['error', 'always'],
    'no-undef': 'off',
    'no-redeclare': 'off',
    'no-unused-vars': 'off',
  },
};
