//const {retrieveUser} = require('..queries')
const retrieveUser  = require('../src/queries');

describe('retrieveUser funtion', () => {
  test('retreiving a user', async() => {
    console.log("retreiving a use TEST")
    const resutlt =  await retrieveUser('validUser','valid@gmail.com')
    console.log(resutlt[0]);
    const rst1 = resutlt[0] //false
    expect(rst1).toBe(false)
});
})


  
