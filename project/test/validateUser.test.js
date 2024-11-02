//const {validateUser} = require('..queries')
const validateUser = require('../src/queries');

describe('validateUser', () => {
    test('should return true for a valid user', async () => {
      const result =   await validateUser('tom');
      expect(result).toBe(true);
    });
  
    test('should return false for an invalid user', async () => {
      const result2 =  await validateUser('======++++');
      expect(result2).toBe(false);
    });
  
  });