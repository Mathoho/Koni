const express = require("express");
const app = express(); 
const cors = require("cors");
const cookieParser = require('cookie-parser');
app.use(cookieParser());
const jwt = require("jsonwebtoken")
app.use(cors())
app.use(express.json());
const bcrypt = require('bcrypt');
const {pool} = require('./database')

async function retrieveUser(user_name, email_address) {

	try {
		valid =  await validateUser(user_name)
		if (valid) {
			console.log(`User with username ${user_name} already exists`)
			return [false, `User with username ${user_name} already exists`];
		}
		const res = await pool.query(
			'SELECT * FROM users WHERE email_address = $1', 
			[email_address],
		);
		
		if (
			res.rows.length > 0) {
			console.log(`Jaga Yeet`)
			return [false, `Email ${email_address} already exists`];
		} 
		return [true]
	} catch (error) {
		console.error(error); //TODO commented out
		return [false, `error occured checking user`];
	}
}

async function email_exists(email_address){
    try {
		const res = await pool.query(
			'SELECT * FROM users WHERE email_address = $1', 
			[email_address],
		);
		
		if (res.rows.length > 0) {
			return [false, `Email ${email_address} already exists`];
		} 
		return [true]
	} catch (error) {
		console.error(error); //TODO commented out
		return [false, `error occured checking email`];
	}
}
async function validateUser(user_name) {
	try {
		id = await get_userId(user_name)
		if (id != -1) {
			return true
		}
		return false
	} catch (error) {
		//console.error(error); //TODO commented out
		return false
	}
}

async function get_userId(user_name) {
	try {
		const res = await pool.query(
			'SELECT user_id FROM users WHERE user_name = $1',
			[user_name],
		);
        console.log(res.rows[0]['user_id'])
		return res.rows[0]['user_id']
		} catch (error) {
			
		//TODO commented out console.error(error);
		return -1;
	}
}

async function get_categoryId(c_type) {
	try {
		const res = await pool.query(
			'SELECT c_id FROM categories WHERE c_type = $1',
			[c_type],
		);

		return res.rows[0]['c_id']
		} catch (error) {

		// When an Error occurred, we will assume the user is not found ?
	//TODO commented out	console.error(error);
		return -1;
	}
}

async function registerUser(user_name, email_address, u_password) {
	try {
		// Salt the password
		const salt = await bcrypt.genSalt(10);
    	const hashedPassword = await bcrypt.hash(u_password, salt);
		const userResult = await pool.query(
			'INSERT INTO users (user_name, email_address, u_password) VALUES ($1, $2, $3)',
			[user_name, email_address, hashedPassword]
			);

			console.log(`Successfully registered ${user_name}`);
			return true
		} catch (error) {
	//TODO commented out		console.error(error);
			return false
	}
}

async function login(username, password) {
	valid  = await validateUser(username)
	if (!valid){
		return [false, `${username} is not a valid user`]
	}
	try{
		const res = await pool.query(
			`SELECT u_password FROM users WHERE user_name = $1`,
			[username]
		)
		salt_password = res.rows[0]['u_password']
		flag = await bcrypt.compare(password,salt_password)
		console.log(flag)
		return [flag]
	} catch (error) {
		return [false, `Could not obtain user ${username} password`]
	}
}

async function insertNotes(user_name, note_title, content, c_type) {
	try {
		const noteResult = await pool.query(
			`WITH new_note AS (
				INSERT INTO notes (note_title, content, user_id)
				VALUES ($1, $2, (SELECT user_id FROM users WHERE user_name = $3))
				RETURNING note_id
			), 
			new_category AS (
				INSERT INTO categories (c_type)
				SELECT $4::text
				WHERE NOT EXISTS (
					SELECT 1 
					FROM categories 
					WHERE c_type = $4
				)
				RETURNING c_id
			)
			INSERT INTO user_notes (user_id, note_id, c_id) 
			SELECT 
				(SELECT user_id FROM users WHERE user_name = $3), 
				(SELECT note_id FROM new_note), 
				COALESCE((SELECT c_id FROM categories WHERE c_type = $4),(SELECT c_id FROM new_category));`,
			[note_title, content, user_name, c_type]
		);
		
		console.log(`Added the note ${note_title} with category ${c_type}`);
		return true
	} catch (error) {
	    console.error(error);
		return false
	}
}

// Check primary KEYS on user notes before inserting (Avoid sharing one note with same person twice)
// Confirm with front-end how notes will be shared? List current users or type in name of users?
async function shareNote(target_user, note_id) {
	try {
		const sharedNote = await pool.query(
			`WITH target_user AS (
				SELECT user_id
				FROM users
				WHERE user_name = $1
			),
			insert_category AS (
				INSERT INTO categories (c_type)
				SELECT 'sharedWithMe'
				WHERE NOT EXISTS (
					SELECT 1 
					FROM categories 
					WHERE c_type = 'sharedWithMe'
				)
				RETURNING c_id
			)
			INSERT INTO user_notes (user_id, note_id, c_id)
			SELECT
				(SELECT user_id FROM target_user),
				$2,
				COALESCE((SELECT c_id FROM categories WHERE c_type = 'sharedWithMe'), (SELECT c_id FROM insert_category))
				RETURNING *`,
			[target_user, note_id]);

		const sharedNotes = sharedNote.rows;

		if (sharedNotes.length > 0) {
			console.log(`Note ${note_id} shared successfully.`);
			return true;
		} else {
			console.log(`Note ${note_id} could not be shared.`);
			return false;
		}
	} catch (error) {
		console.error(`Note ${note_id} could not be shared.`);
		return false;
	}
}

async function filterNotes(username, c_type) {
	user_id = await get_userId(username)
	c_id = await get_categoryId(c_type)
	try {
		const res = await pool.query(
			`SELECT note_time, note_title, content
			FROM notes WHERE user_id = $1 AND note_id IN (SELECT note_id FROM user_notes WHERE c_id = $2)`,
			[user_id, c_id]);
		console.log(res.rows);
	
		return res.rows
	} catch (error) {
        console.error(error);
	}
}

async function retrieveNotes(user_name) {
	user_id = await get_userId(user_name)
	try {
		const res = await pool.query(
			`SELECT note_time, note_title, content, c_type
			FROM notes, categories
			WHERE note_id IN (SELECT u.note_id FROM user_notes u WHERE u.user_id = $1) AND note_id IN (SELECT note_id FROM user_notes where user_id = $1)`,
			[user_id]
		);
		console.log(res.rows);
		return res.rows
	} catch (error) {
	    console.error(error);
	}
}

async function searchNotes(user_name, note_title) {
	try {
		user_id = await get_userId(user_name);
		const res = await pool.query(`SELECT note_title, note_time, content
			 FROM notes  
			 WHERE 
			 user_id = $1 AND 
			 note_id IN (SELECT u.note_id FROM user_notes u) AND note_title = $2`,
			[user_id, note_title]
		);
		console.log(res.rows);
		return res.rows
	} catch (error) {
		console.error(error);
		return false
	}
}

async function sortNotes(user_name) {
	try {
		user_id = get_userId(user_name)
	
		console.log(res.rows);
			return res.rows
	} catch (error) {
	//TODO commented out	console.error(error);
		return false
	}
}

async function updateEmail(username, new_email) {
	try {
		const flag = await email_exists(username, new_email)
		if (!flag[0]) {
			const res = await pool.query(
			"UPDATE users SET email_address = $1 WHERE user_name = $2", [new_email, username]);
			return true
		}
		console.log(res.rows);
		return false
	} catch (error) {
	//TODO commented out	console.error(error);
		return false
	}
}

async function updateUsername(username, new_username) {
	try {
		const flag = await get_userId(username, new_email)
		if (flag[0]) {
			const res = await pool.query(
				"UPDATE users SET user_name = $2 WHERE user_name = $1", [username, new_username]);
			console.log(res.rows);
			return true
			}
	} catch (error) {
	//TODO commented out	console.error(error);
		return false
	}
}


async function updateNotes(username, note_id, new_note_title, new_content) {
	try {
		const user_id = await get_userId(username);

		// Update the note title or note content
		const res = await pool.query(
			`UPDATE notes 
			 SET note_time = NOW(), note_title = $1, content = $2 
			 WHERE note_id = $3;`,
			[new_note_title, new_content, note_id]
		);

		if (res.rowCount > 0) {
			console.log(`Note ${note_id} updated successfully.`);
			return true;
		} else {
			console.log(`Note ${note_id} could not be updated. Either it doesn't exist or the user doesn't have permission.`);
			return false;
		}
	} catch (error) {
	//TODO commented out	console.error(`Error : Note ${note_id} could not be updated. Either it doesn't exist or the user doesn't have permission.`);
		return false;
	}
}

async function updateAvatar(username, new_avatar) {
	try {
		const res = await pool.query(
			"UPDATE users SET avatar_url = $1 WHERE user_name = $2", [new_avatar, username]);
		console.log(res.rows);
		return true
	} catch (error) {
	//TODO commented out	console.error(error);
		return false
	}
}

async function updatePassword(username, new_password) {
	try {
		const res = await pool.query(
			"UPDATE users SET u_password = $1 WHERE user_name = $2", [new_password, username]);
		console.log(res.rows);
		return true
	} catch (error) {
	//TODO commented out	console.error(error);
		return false
	}
}

async function deleteAccount(username) {
	try {
		const res = await pool.query(
			`DELETE FROM users
			 WHERE user_name = $1`, [username]);
		console.log(res.rows);
		return true
	} catch (error) {
	    console.error(error);
		return true
	}
}

async function getOwner(username, note_id) {
	try {
		user_id = get_userId(username)
		const res = await pool.query(
			`SELECT * FROM notes WHERE note_id = $1 AND user_id = $2`, 
			[note_id, user_id]);
			console.log(res.rows.length);
			if (res.rows.length > 0) {
				return true
			}
			return false
	} catch (error) {
	//TODO commented out	console.error(error);
		return false
	}
}

async function deleteNote(user_name, note_id) {
	try {
		
        owner = await getOwner(user_name)
		if (owner) {
			query = `DELETE FROM notes WHERE note_id = ${note_id}`
		} else {
            user_id = await get_userId(user_name)
			query = `DELETE FROM user_notes WHERE note_id = ${note_id}
			 AND user_id = ${user_id}`
		}
		const res = await pool.query(query);
		console.log(res.rows);
		return true
	} catch (error) {
	    console.error(error);
		return false
	}
}

// Reviewed
app.post("/register",async(req,res)=>{
    try {
        const {email, username, password } = req.body
        const userFound = await retrieveUser(username,email) 
        if (userFound[0]) {
            const result = await registerUser(username,email, password);
            if (result) {
                const accessToken = getAcessToken(username);
                console.log(accessToken)
                return res.status(200).json({status: 200 , Token:accessToken});
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
                return res.status(201).json({status:200,message: "User successfully logged in", token : token});
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
app.delete("/deleteUser", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username } = req.body;
        let tokenVerified = validateToken(token.split(" ")[1],username)
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
        console.log(error)
        return res.status(500).json({status:500, message: 'Internal server error' });  
    }
});

// Reviewed
app.put("/updateUserEmail", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, email } = req.body;
        let tokenVerified = validateToken(token.split(" ")[1], username)
        if(tokenVerified) {
            const user = await updateEmail(username,email) 
            if (user) {
                return res.status(200).json({status:200, message: "User email successfully updated"});
            } else {
                return res.status(400).json({status:400, message: "failed to update User email"});
            }
        } else {
            return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
        }
    } catch (error) {
        return res.status(500).json({status:500, message: 'Internal server error' });  
    } 
});

// Reviewed
app.put("/updatePassword", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, new_password } = req.body;
        let tokenVerified = validateToken(token.split(" ")[1],username)
        if(tokenVerified) {
            const user =  updatePassword(username,new_password) //TODO removed await
            if (user) {
                return res.status(200).json({status:200, message: "User password successfully to updated"});
            } else {
                return res.status(400).json({status:400, message: "failed to update User password"});
            }
        } else {
            return res.status(400).json({status:400, message: "Error: Invalid Credentials"});
        }
        
    } catch (error) {
        console.log(error)
        return res.status(500).json({status:500, message: 'Internal server error' });  
    } 
});


app.put("/updateUsername", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, new_username } = req.body;
        let tokenVerified = validateToken(token.split(" ")[1],username)
        if(tokenVerified) {
            const user =  updateUsername(username,new_username) //TODO removed await
            if (user) {
                return res.status(200).json({status:200, message: "Username successfully to updated"});
            } else {
                return res.status(400).json({status:400, message: "failed to update Username"});
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
        let tokenVerified = validateToken(token.split(" ")[1],username)
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
        let tokenVerified = validateToken(token.split(" ")[1],username)
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
        let tokenVerified = validateToken(token.split(" ")[1],username)
        if(tokenVerified) {
            const response = await searchNotes(username, note_title) 
            console.log(response)
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
        let tokenVerified = validateToken(token.split(" ")[1],username)
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
        let tokenVerified = validateToken(token.split(" ")[1],username)
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
app.put("/shareNote", async(req, res) => {
    try {
        const token = req.header('Authorization');
        const { username, note_id } = req.body;
        
        let tokenVerified = validateToken(token.split(" ")[1],username)
        if(tokenVerified) {
            const rst = await shareNote(username, note_id);
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
        let tokenVerified = await validateToken(token.split(" ")[1],username)
        if(tokenVerified) {
            const newNote = await insertNotes(username, title, body, category);
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
app.post("/updateNote", async (req, res) => {
    try {
        const token = req.header('Authorization');
        const {new_title, username, new_content, note_id} = req.body;
        let tokenVerified = validateToken(token.split(" ")[1],username)
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
app.delete("/deleteNote", async (req, res) => {
    try {
        const token = req.header('Authorization');
        const {username, note_id} = req.body;
        let tokenVerified = validateToken(token.split(" ")[1],username)
        if(tokenVerified) {
            // Validate the noteId parameter
            if (note_id) {
                // Find the note by ID and delete it from the database
                const deletedNote = await deleteNote(username, note_id);
                if (deletedNote) {
                    return res.status(200).json({status:200, message: "Note deleted successfully"});
                }else{
                    return res.status(400).json({status:400, message: `Note of id: ${note_id} not found`});
                }  
            }else{
                return res.status(400).json({status:400, message: "No NoteId specified"});
            }
        }
        return res.status(401).json({ message: 'Error: Invalid Credentials' });
    } catch (error) {
        console.log(error)
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
    console.log(token)
    return token;
    } catch (error) {
        console.log(error)
        return null;
    }
};

// Reviewed
async function validateToken(token,username) {
    //check if the token is valid
    try {
        const decoded = jwt.verify(token, username);
        let decodedUsername = decoded.username;
        let rst = await get_userId(decodedUsername)
        return rst;
      } catch (error) {
        console.log('error in validate token')
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

app.get("/", (req, res) => {
    res.send({message: "Welcome to my Api"});
});
// app.listen(5000, () => {
//     console.log("running on port  http://localhost:5000/");
// });

app.listen(process.env.PORT || 5000);