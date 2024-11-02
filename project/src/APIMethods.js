//const { Pool } = require('pg');
//const pool = require("./db");
const usersTable = "users";
const notesTable = "notes";
const categoryTable = "category";
const pool = require("./db");
const jwt = require('jsonwebtoken');
const bcrypt = require('bcrypt');

//const express = require("express");
//const router = express.Router();
// const pool = new Pool({
//     user: "database user",
//     host: "database host",
//     database: "my database name",
//     password: "pass",
//     port: 5432,
// });
//TODO note usernames are unique
async function registerUser(body){
    try {
        //TODO add functionality to implement avatar_url
        //console.log("try one");
        const { user_name,avatar_url ,email_address, u_password} = body;
        //const {user_name , password} = body;
        const accessToken = getAcessToken(user_name , u_password);
        // Check if the user already exists
        const existingUser = await userExist(body); 
        if (existingUser) {
          return "User already exists";
        }else{
            // Hash the password before saving it to the database
            const saltRounds = 10;    
            
            try {
            const salt = await bcrypt.genSalt(saltRounds);
            const hashedPassword = await bcrypt.hash(u_password, salt);
            const body_reg = {
                user_name: user_name,
                email_address: email_address,
                avatar_url: avatar_url,
                u_password: hashedPassword
            };

            await saveNewUser(body_reg);
            
            return accessToken;
            } catch (error) {
            console.error("Error hashing or saving user:", error);
            return "An error occurred during registration";
            }
        }
    
    } catch (error) {
        console.error(error)
        console.log("some error in register")
        return "Internal server error";
    } 
};
module.exports = registerUser;

//TODO tested and it works
async function loginUser(body) {
    try {
        // Check if the user is a valid user
        const accessToken = getAcessToken(user_name , u_password);
        let user = await isValidUser(body);
        if (user) {
          return "Authentication successful";
        }
        return "Authentication failed";
        
    } catch (error) {
        
    }
    return null;
};
module.exports = loginUser; 

//TODO function tested via register function and it works
async function userExist(body) {
    const { user_name } = body
    try {
        let query = `SELECT * FROM ${usersTable} WHERE user_name = $1`;
        const rst = await pool.query(query,[user_name]);
        if(rst && rst.rows && rst.rows.length > 0){
            return true
        }else{
            return false
        }
        
      
    } catch (error) {
        //TODO remove logging
        console.log("error in user exist")
        console.log(error.message);
    } 
};

//TODO function tested and it works
async function updateUser(body) {
    //body eamil
    const { user_name, u_password } = body;

    const { email_address, avatar_url } = body;
    
    try {
        //if user exist 
        const valid = isValidUser({ user_name, u_password })
        if (valid) {
            let query = `UPDATE ${usersTable} SET email_address = $1, avatar_url = $2 WHERE user_name = $3`;
            const rst = await pool.query(query,[email_address,avatar_url,user_name]);
            return "User Updated Sucessfully";
        } else {
            return "User Authentication failed: Not updated";
        }
        
    } catch (error) {
        console.log(error.message) //TODO logging
    } 
};


//TODO tested via registeruser and it works 
async function saveNewUser(body) {
    const { user_name, email_address, avatar_url, u_password } = body;
   try {
        let query = `INSERT INTO ${usersTable} (user_name, email_address, avatar_url, u_password) VALUES ($1, $2, $3, $4)`;
        const rst = await pool.query(query,[user_name,email_address,avatar_url,u_password]);
   } catch (error) {
    console.log("error in saveNewuser")
        //TODO remove logging
        console.log(error.message);
   }
};


//TODO tested via loginuser and it works
async function isValidUser(body) {
    const { user_name, u_password } = body;
    try {
        let query = `SELECT * FROM ${usersTable} WHERE user_name = $1`;
        const rst = await pool.query(query,[user_name]);
        if(rst && rst.rows && rst.rows.length > 0){
            //zero should change to the index of the email column 
            const user = rst.rows[0];
            const userPassword = user.u_password;
            console.log(userPassword + " this is the pass")
            //TODo if hashing works use the following
            const passwordMatch = await bcrypt.compare(u_password, userPassword);
            if (!passwordMatch) {
                return false;
            }
        }else{
            //THE user name was not found in the users table
            console.log("returning false isvalid user")
            return false;
        }
        console.log("returnign true")
        return true;
    } catch (error) {
        console.log(error.message) //TODO logging
    }
};




async function getUserNotes(reqParams) {
    const { userName, category, sort} = reqParams;
    //const user_name = userName;
    try {
        //const valid = isValidUser({ user_name, u_password });
        //if (valid){
            let query = "";
            let rst = "";
            if (sort && !category) {
                query = `SELECT * FROM ${notesTable} WHERE username = $1`;
                rst = await pool.query(query,[userName]); 
            } else if (sort && category){
                //TODO sort by TIME STAMP
                query = `SELECT * FROM ${usersTable} WHERE username = $1 ORDER BY note_time DSC`;

                rst = await pool.query(query,[userName]); 
            }
           
            
            let notesArr = []
            for (let i = 0; i < rst.rows.length; i++) {
                const user = rst.rows[i];
                //let noteContent = user.content;
                //notesArr.push(noteContent)

                let noteTitle = user.note_title;
                notesArr.push(noteTitle)
            }
            return notesArr
        // }else{
        //     return "User Authentication failed"
        // }
    } catch (error) {
        console.error(error.message);
        return "";
    }
    
}
module.exports = getUserNotes;

// async function getUserNotesSorted(userName) {
//     try {
//         let query = `SELECT * FROM ${usersTable} WHERE username = $1 ORDER BY note_time DSC`;
//         const rst = await pool.query(query,[userName]);
//         return rst
//     } catch (error) {
//         console.error(error.message);
//     }
//     return null;
// }
// module.exports = getUserNotesSorted;

// async function getPass(username){
//     try {
//         let query = `SELECT Password FROM ${usersTable} WHERE username = $1`;
//         const rst = await pool.query(query,[username]);
//     } catch (error) {
        
//     }
    
// }



//TODO CREATE THE NOTE
async function createNote(body){

    const {note_title,shared, content, category, username} = body;
    try {
        let query = `INSERT INTO ${notesTable} (note_title, shared, content, username) VALUES ($1, $2, $3, $4)`;
        rst = await pool.query(query,[note_title,shared,content,username]); 
        if (rst) {
            //get the note_id
            let id = await getNote_Id(note_time)
            console.log("freate 2")
            let query2 = `INSERT INTO ${categoryTable} (c_type, fk_note) VALUES ($1, $2)`
            console.log("no tgfdgfd")
            rst2 = await pool.query(query2,[category,id])
            return "Note Created Successfully"
        }
       return " Note not created" 
    } catch (error) {
        console.log(error)
    }
}
module.exports = createNote;

//function to get the note id of a note
async function getNote_Id(note_time,username){
    console.log("getting note id")
    try {
        let query = `SELECT note_id FROM ${notesTable} WHERE note_time = $1 AND username = $2`
        console.log("getting2 note id")
        let rst = await pool.query(query,[note_time,username]);
        console.log("getting3 note id")
        if (rst.rows.length > 0) {
            const user = rst.rows[0];
            let id = user.note_id;
            return id;
        }
        console.log("getting4 note id")
    } catch (error) {
        console.log(error.message);
    }
}
//TODO notes filetered
async function NotesFiltered(category){
    try {
        let query = `SELECT  n.note_title, n.content 
        FROM ${notesTable} n JOIN ${categoryTable} c ON n.note_id = c.fk_note 
        WHERE c.c_type = $1`
        let rst = await pool.query(query,[category]);
        //ON n.note_id = c.fk_note //TODO not necessary
        if(rst.rows.length>0){
            const note = rst.rows[0];
            return rst.rows //an array with a dictionary inside
        }
        
    } catch (error) {
        return error  
    }
}
module.exports = NotesFiltered;

//TODO +++++++++++++++++++++++++++++++++TESTING PURPOSES+++++++++++++++++++++++++++++++++++++++++++++++
// let query = `SELECT  n.note_title, n.content 
        // FROM  ${usersTable} u JOIN ${notesTable} n JOIN ${categoryTable} c ON n.note_id = c.fk_note 
        // WHERE c.c_type = $1, u.username = $2`
        //let rst = await pool.query(query,[c_type,userName]);
async function main() {
    // Your program logic goes here
    console.log("Hello, world!");

    //TODO testing register user
    let body_reg = {
        user_name: 'Ronaldo',
        email_address: 'Ronaldo@gmail.com',
        avatar_url: 'my_avatar_url_Ronaldo',
        u_password: 'Ronaldo123'
      };
      
      try {
        let msg = await registerUser(body_reg);
        console.log(msg);
    } catch (error) {
        console.error(error);
    }

    //TODO testing login user
    // let body_log = {
    //     user_name: 'tom',
    //     u_password: 'Tom123'
    // };
    //   try {
    //     let msg = await loginUser(body_log);
    //     console.log(msg);
    // } catch (error) {
    //     console.error(error);
    // }
    
     //TODO testing update user
     /*
     let body_updt = {
        user_name: 'koni',
        email_address: 'neweamil.com',
        avatar_url: 'new_avatar',
        u_password: 'Koni123'
      };
      try {
        let msg = await getUserNotes(body_updt);
        console.log(msg);
    } catch (error) {
        console.error(error);
    }*/

    /*
      //TODO testing  getusernotes
      let body_getnts = {
        username: 'koni',
        u_password: 'Koni123'
      };
      try {
        let msg = await getUserNotes(body_getnts);
        console.log(msg);
    } catch (error) {
        console.error(error);
    }*/
    //TODO test filterd notes
    // let userName = "koni";
    // let params = {
    //     c_type: 'work',
    //   };
    // try {
    //     let msg = await NotesFiltered(userName,params)
    //     console.log(msg);
    // } catch (error) {
    //     console.error(error);
    // } 

    //TODO create a note note the user must alserady be in the users table else it will throw an error
    // let body_crtNt = {
    //     note_time:"10 OCT 2023",
    //     shared: false,
    //     note_title: "my notes",
    //     content: "this is notes ",
    //     username:"john_doe"
    //   };
    //   try {
    //     let msg = await createNote(body_crtNt)
    //     console.log(msg)
    //   } catch (error) {
        
    //   }
}
//TODO++++++++++++++++RESULT ROWS[0]++++++++++++++++++++++++++++++++++
// Hello, world!
// {
//   note_title: 'Personal NoteK 3',
//   content: 'my notes for module first'
// }
// /TODO++++++++++++++++RESULT ROWS++++++++++++++++++++++++++++++++++
// Hello, world!
// [
//   {
//     note_title: 'Personal NoteK 3',
//     content: 'my notes for module first'
//   }
// ]

// Call the entry point function when the page is loaded
main();
    //const saltRounds = 10;    
        /*
        try {
          const salt = await bcrypt.genSalt(saltRounds);
          const hashedPassword = await bcrypt.hash(password, salt);
          
          // Now you can use the hashedPassword to save the user to the database
          const newUser = new User({
            username,
            email,
            password: hashedPassword,
          });
          console.log("new user I am saving: " + newUser);
        
          // Save the user to the database
          await newUser.saveNewUser(body);
          
          return "User registered successfully";
        } catch (error) {
          console.error("Error hashing or saving user:", error);
          return "An error occurred during registration";
        }
        */
        // const newUser = new User({
        //     username,
        //     email,
        //     password: hashedPassword,
        // })