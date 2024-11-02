const Pool = require("pg").Pool;

const pool = new Pool({
    user: "konanani",
    host: "localhost",
    port: 5432,
    database: "storage",
    password: "123456"
})

module.exports = pool;