const express = require("express");
const app = express(); 
const cors = require("cors");
const cookieParser = require('cookie-parser');
app.use(cookieParser());

/**Methods from Quiries */
const {registerUser} = require('./queries');
const login = require('./queries');
const deleteAccount = require('./queries');

const retrieveNotes = require('./queries');
const retrieveUser = require('./queries');
const InsertNotes = require('./queries');

const shareNote = require('./queries');
const filterNotes = require('./queries');
const searchNotes = require('./queries');

const sortNotes = require('./queries');
const updateEmail = require('./queries.js');
const updateUsername = require('./queries');

const updateAvatar = require('./queries');
const updatePassword = require('./queries');

const deleteNote = require('./queries');
const updateNotes = require('./queries');
const validateUser = require('./queries.js')


app.use(cors())
app.use(express.json());

// Reviewed
app.post("/register",async(req,res)=>{
    try {
        const {email, username, password ,rememberMe} = req.body
        const userFound = await retrieveUser(username,email) 
        if (userFound[0]) {
            const result = await registerUser(username,email, password);
            if (result) {
                if (rememberMe) {
                    const accessToken = getRememberMeToken(username);
                    res.cookie('rememberMe', accessToken, { httpOnly: true, secure: true });
                    return res.status(200).json({status: 200 , RememberMeToken:accessToken});
                } else {
                    const accessToken = getAcessToken(username);
                    return res.status(200).json({status: 200 , Token:accessToken});
                }
            } else {
                return res.status(400).json({status: 400 , message:"Error registering the user"});
            }
        }else{
            return res.status(400).json({status: 400 , message:userFound[1]});
        }
    } catch (error) {
        console.error(error.message);
        return res.status(500).json({status:500, message: 'Internal server error' });    
    }
});

// Reviewed
app.post("/login",async(req, res) => {
    const {username, password, rememberMe} = req.body
    try {
        if (rememberMe){
            const token = req.cookies.rememberMe;
            let tokenVerified = validateToken(token,username)
            if(tokenVerified) {
                return res.status(201).json({status:201,message: "User successfully logged in", token : token});
            } 
        }

        //if the user did not do the remember me functionality check their log in details
       const result = await login(username, password); 
       if (result[0]) {
        // generate token for this user
        const token = getAcessToken(username)
        return res.status(201).json({status:201,message: "User successfully logged in", token : token});
       }else{
        //error or user not logged in
        return res.status(400).json({status:400,message: result[1]});
       }

    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }
});

// Reviewed
app.delete("/deleteUser/:username", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username } = req.params;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const user = await deleteAccount(username) 
            if (user) {
                return res.status(200).json({status:200, message: "User deleted successfully"});
            } else {
                return res.status(400).json({status:400, message: "User does not exist"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }
});

// Reviewed
app.put("/updateUserEmail", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, email } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const user = await updateEmail(username,email) 
            if (user) {
                return res.status(200).json({status:200, message: "User email successfully updated"});
            } else {
                return res.status(400).json({status:400, message: "failed to update User email"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    } 
});

// Reviewed
app.put("/updatePassword", async(req, res) => {
    try {
       // const token = req.header('Authorization');
        const { username, new_password } = req.body;
       // let tokenVerified = validateToken(token,username)
        //if(tokenVerified) {
            const user =  updatePassword(username,new_password) //TODO removed await
            if (user) {
                return res.status(200).json({status:200, message: "User password successfully to updated"});
            } else {
                return res.status(400).json({status:400, message: "failed to update User password"});
            }
        //}
        //return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    } 
});


app.put("/updateUsername", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, new_username } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const user =  updateUsername(username,new_username) //TODO removed await
            if (user) {
                return res.status(200).json({status:200, message: "User password successfully to updated"});
            } else {
                return res.status(400).json({status:400, message: "failed to update User password"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    } 
});

//Reviewed
app.put("/updateAvatar", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, new_avatar } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const user = await updateAvatar(username,new_avatar);
            if (user) {
                return res.status(200).json({status:200, message: `User Avatar successfully to updated`});
            } else {
                return res.status(400).json({status:400, message: "failed to update User Avatar"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    } 
});

// get the notes 
app.get("/getNotes",async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const response = await retrieveNotes(username) 
            if (response) {
                return res.json(response); 
            }else{
                return res.status(400).json({status:400, message: "User does not exist"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }   
});


// search the notes based on a given title
app.get("/search",async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, note_title } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const response = await searchNotes(username, note_title) 
            if (response) {
                return res.json(response); 
            }else{
                return res.status(400).json({status:400, message: "User does not exist"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }   
});

//sorting the notes by timestamp recent notes first
app.get("/sort",async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const response = await sortNotes(username) 
            if (response) {
                return res.json(response); 
            }else{
                return res.status(400).json({status:400, message: "User does not exist"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }   
});

//filtering based on the category
app.get("/filter", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, c_type } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const response = await filterNotes(username,c_type)
            if (response) {
                return res.json(response);
            } else {
                return res.status(400).json({status:400, message: "User does not exist"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }
});

//share the notes with the given id
app.put("/shareNote/:noteId", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username } = req.body;
        const {noteId} = req.params;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const rst = await shareNote(username, noteId);
            if (rst) {
                return res.status(200).json({status:200, message: "Note shared successfully"});
            } else {
                return res.status(400).json({status:400, message: "Note not shared"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }
});

// add the note created
app.post("/addNote", async (req, res) => {
    try {
        const token = req.header('Authorization');
        const {title, category, username, body } = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const newNote = await InsertNotes(username, title, body, category);
            if (newNote) {
                return res.status(200).json({status:200, message: "Note saved successfully"});
            } else {
                return res.status(4000).json({status:400, message: "Note not saved"});
            }

        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({ error: "Internal Server Error" });
    }
});

//update the note with the given id
app.post("/updateNote/:noteId", async (req, res) => {
    try {
        const token = req.header('Authorization');
        const {new_title, username, new_content, note_id} = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const newNote = await updateNotes(username, note_id, new_title, new_content);
            if (newNote) {
                return res.status(200).json({status:200, message: "Note updated successfully"});
            } else {
                return res.status(200).json({status:200, message: "Note not updated"});
            }
        }
        return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
    } catch (error) {
        return res.status(500).json({ error: "Internal Server Error" });
    }
});

//delete a note with a given note id
app.delete("/deleteNote/:noteId", async (req, res) => {
    try {
        const token = req.header('Authorization');
        const {noteId } = req.params;
        const {username} = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            // Validate the noteId parameter
            if (noteId) {
                // Find the note by ID and delete it from the database
                const deletedNote = deleteNote(username, noteId);
                if (deletedNote) {
                    return res.status(200).json({status:200, message: "Note deleted successfully"});
                }else{
                    return res.status(400).json({status:400, message: `Note of id: ${noteId} not found`});
                }  
            }else{
                return res.status(400).json({status:400, message: "No NoteId specified"});
            }
        }
        return res.status(401).json({ message: 'Error: Invalid Credentials' });
    } catch (error) {
      res.status(500).json({ error: "Internal Server Error" });
    }
});
  

function getAcessToken(username){
    try {
        // Generate a JWT token
        const payload = {
            username: username,
        };
        const token = jwt.sign(payload, username, {
            expiresIn: '24h', 
        });
    return token;
    } catch (error) {
        return null;
    }
};

// Reviewed
function validateToken(token,username) {
    //check if the token is valid
    try {
        const decoded = jwt.verify(token, username);
        let decodedUsername = decoded.username;

        let rst = validateUser(decodedUsername)
        return rst;
      } catch (error) {
        return false;
    }
}

// Reviewed
function getRememberMeToken(username) {
    try {
        // Generate a JWT token
        const payload = {
            username: username,
        };
        const token = jwt.sign(payload, username);
       
        return token;
    } catch (error) {
        console.log(error.message); 
    }
    return null;
}

//deleteUser
app.delete('/deleteAccount', async (req, res) => {
    try {
        const token = req.header('Authorization');
        const {username} = req.body;
        let tokenVerified = validateToken(token,username)
        if(tokenVerified) {
            const user = await deleteAccount(username);
            if (user) {
                return res.status(200).json({ message: 'User deleted sucessfully' });
            } else {
                return res.status(400).json({ message: 'User not deleted' });
            }
        }else{
            return res.status(401).json({ message: 'Error: Invalid Credentials' });
        }
    } catch (error) {
        res.status(500).json({ error: "Internal Server Error" });
    }
});

// app.listen(8080, () => {
//     console.log("running on port  http://localhost:8080/");
// });

async function main() {
	try {
		const msg = await validateUser("ValidUser")
		console.log(msg)

		const msg2 = await retrieveUser("ValidUser")//in database
		console.log(msg2)

		const msg3 = await retrieveUser("tom")
		console.log(msg3)
	} catch (error) {
		console.log(error)
	}
}
main()