module.exports = {
  preset: 'ts-jest',
  transform: {
    '^.+\\.(ts|tsx)?$': 'ts-jest',
  },
  coveragePathIgnorePatterns: [
    '/node_modules/',
    'src/logger.ts',
    'src/app.ts',
    'src/database.ts',
    'src/utils/ApiError.ts',
  ],
  coverageThreshold: {
    global: {
      branches: 80,
      functions: 80,
      lines: 80,
      statement: 80,
    },
  },
};
