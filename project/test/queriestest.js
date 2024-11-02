
const bcrypt = require('bcrypt');
const {pool} = require('../src/queries');

describe("Quiries test", () => {
    test('should return true if user is not in the database', async() =>{
        const username = "newUser";
        const email = "newUser@example.com";
        const result = await retrieveUser(username,email)
        console.log(result, "result: " );
        expect(result[0]).toBe(false); 
});

test('it should validate the user', async() => {
    const user_name = "newUser";
    const result = await validateUser(user_name)
    expect(result).toBe(false);

    const user_name2 = "validUser";
    const result2 = await validateUser(user_name2)
    expect(result2).toBe(false);
  });

  test('registerUser', async() =>{
    try {
        const user_name = "newUser";
        const email_address = "newUser@example.com"
        const u_password = "pass123";
        const result = registerUser(user_name,email_address,u_password);
        expect(result).toBe(true);
    } catch (error) {
        
    }
  });
     
  test('login user', () =>{
    try {
        const user_name = "newUser";
        const pass = "pass123";
        const result = login(user_name,pass)
        expect(result).toBe(false)

    } catch (error) {
        
    }
    });

    test('should return -1 when the user is not in the database', () =>{
      try {
          const user_name = "=====";
          const result = get_userId(user_name)
          expect(result).toBe(-1)
      } catch (error) {
          
      }
      });

      test('should return true when a note is added to the database', () =>{
        try {
          const user_name = "bens"
          const note_title = "Freedom"
          const content ="Freedom is awesome"
          const c_type = "work"
          const result = insertNotes(user_name, note_title, content, c_type);
          expect(result).toBe(true)
        } catch (error) {
        }
      });

      test('login should return true for valid user', () =>{
        try {
          //add 2 users to the database
          const user_name = "burger";
          const email_address = "burger@example.com"
          const u_password = "burger123";
          const adding1 = registerUser(user_name,email_address,u_password);

          const user_name2 = "sam";
          const email_address2 = "sam@example.com"
          const u_password2 = "sam123";
          const adding2 = registerUser(user_name2,email_address2,u_password2);
          const result = login(user_name2,u_password2)
          expect(result).toBe(true)
          const result2 = login("psedo","psudo234")
          expect(result2).toBe(false)

        } catch (error) {
            
        }
        });

        test('should return true when notes are share sucessfullly', () =>{
          try {

            const user_name = "behn"
            const note_title = "Freedom"
            const content ="Freedom is awesome"
            const c_type = "work"
            const result = insertNotes(user_name, note_title, content, c_type);
            
            const user_name2 = "pams";
            const email_address2 = "pam@example.com"
            const u_password2 = "pam456";
            const adding2 = registerUser(user_name2,email_address2,u_password2);
            const c_id = get_categoryId(c_type)

            const result2 = shareNote(user_name2,c_id)

            expect(result2).toBe(true)
          } catch (error) {
          }
        });

      test('should return rpws when a note is added to the database', () =>{
        try {
          
          const result = filterNotes(user_name,c_type);
          if (result) {
            expect(result).toBe(true)
          } 
        } catch (error) {
        }
      });

      test('should return ', () =>{
        try {
          const user_name = "abigails"
          const note_title = "Grocery"
          const content ="shopping is awesome"
          const c_type = "personal"
          const result = insertNotes(user_name, note_title, content, c_type);
          const result2 = searchNotes(user_name,note_title);
          if (result2) {
            expect(result).toBe(true)
          } 
        } catch (error) {
          
        }

      });

      test('should return true if an account was sucessfuly deleted', () =>{
        const user_name2 = "pamas";
        const email_address2 = "paoam@example.com"
        const u_password2 = "pamas456";
        const adding2 = registerUser(user_name2,email_address2,u_password2)
        const result = deleteAccount(user_name2);
        expect(result).toBe(true)
      });

      test('should return true if an account was sucessfuly deleted', () =>{
        const user_name2 = "pamass";
        const email_address2 = "paoasm@example.com"
        const u_password2 = "pamas4563";
        const adding2 = registerUser(user_name2,email_address2,u_password2)
        const result = updatePassword(user_name2,u_password2);
        expect(result).toBe(true)
      
      });

      test('should return true if an account was sucessfuly deleted', () =>{
        const user_name2 = "pamasst";
        const email_address2 = "paoastm@example.com"
        const u_password2 = "pamast4563";
        const adding2 = registerUser(user_name2,email_address2,u_password2)
        const result = updateAvatar(user_name2,u_password2);
        expect(result).toBe(true)
      
      });
});

/** METHODS  From queries*/
async function retrieveUser(user_name, email_address) {
	try {
		valid =  await validateUser(user_name)
		if (valid) {
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
		console.error(error);
		return [false, `error occured checking user`];
	}
}

module.export = retrieveUser;

async function validateUser(user_name) {
	try {
		id = await get_userId(user_name)
		if (id != -1) {
			return true
		}
		return false
	} catch (error) {
		console.error(error);
		return false
	}
}

module.export = validateUser;

async function get_userId(user_name) {
	try {
		const res = await pool.query(
			'SELECT user_id FROM users WHERE user_name = $1',
			[user_name],
		);

		return res.rows[0]['user_id']
		} catch (error) {

		console.error(error);
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
		console.error(error);
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
			console.log(`Failed to register ${user_name}`)
			console.error(error);
			return false
	}
}
module.exports = registerUser;

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
		return flag
	} catch (error) {
		console.error(error)
		return [false, `Could not obtain user ${username} password`]
	}
}
module.exports = login;
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
module.exports = insertNotes;
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
module.exports = shareNote;

async function filterNotes(username, c_type) {
	user_id = get_userId(username)
	c_id = get_categoryId(c_type)
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
module.exports = filterNotes;
async function retrieveNotes(user_name) {
	user_id = await get_userId(user_name)
	try {
		const res = await pool.query(
			`SELECT note_time, note_title, content, c_type
			FROM notes, user_notes
			WHERE note_id IN (SELECT u.note_id FROM user_notes u WHERE u.user_id = $1) AND note_id IN (SELECT no)`,
			[user_id]
		);
		console.log(res.rows);
		return res.rows
	} catch (error) {
		console.error(error);
	}
}
module.exports = retrieveNotes;
async function searchNotes(user_name, note_title) {
	try {
		user_id = await get_userId(user_name);
		const res = await pool.query(Owner
			`SELECT note_title, note_time, content
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
module.exports = searchNotes;
async function sortNotes(user_name) {
	try {
		user_id = get_userId(user_name)
	
		console.log(res.rows);
			return res.rows
	} catch (error) {
		console.error(error);
		return false
	}
}
module.exports = sortNotes
async function updateEmail(username, new_email) {
	try {
		const flag = await retrieveUser(username, new_email)
		if (flag[0]) {
			const res = await pool.query(
			"UPDATE users SET email_address = $1 WHERE user_name = $2", [new_email, username]);
			return true
		}
		console.log(res.rows);
		
	} catch (error) {
		console.error(error);
		return false
	}
}
module.exports = updateEmail;
async function updateUsername(username, new_username) {
	try {
		const flag = await retrieveUser(username, new_email)
		if (flag[0]) {
			const res = await pool.query(
				"UPDATE users SET user_name = $2 WHERE user_name = $1", [username, new_username]);
			console.log(res.rows);
			return true
			}
	} catch (error) {
		console.error(error);
		return false
	}
}

module.exports = updateUsername;
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
		console.error(`Error : Note ${note_id} could not be updated. Either it doesn't exist or the user doesn't have permission.`);
		return false;
	}
}

module.exports = updateNotes;
async function updateAvatar(username, new_avatar) {
	try {
		const res = await pool.query(
			"UPDATE users SET avatar_url = $1 WHERE user_name = $2", [new_avatar, username]);
		console.log(res.rows);
		return true
	} catch (error) {
		console.error(error);
		return false
	}
}
module.exports = updateAvatar
async function updatePassword(username, new_password) {
	try {
		const res = await pool.query(
			"UPDATE users SET u_password = $1 WHERE user_name = $2", [new_password, username]);
		console.log(res.rows);
		return true
	} catch (error) {
		console.error(error);
		return false
	}
}

module.exports = updatePassword;

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
module.exports = deleteAccount
async function getOwner(username, note_id) {
	try {
		user_id = get_userId(username)
		const res = await pool.query(
			`SELECT * FROM notes WHERE note_id =  $1 AND user_id = $2`, 
			[note_id, user_id]);
			console.log(res.rows.length);
			if (res.rows.length > 0) {
				return true
			}
			return false
	} catch (error) {
		console.error(error);
		return false
	}
}

async function deleteNote(user_name, note_id) {
	try {
		owner = await getOwner(user_name)
		if (owner) {
			query = `DELETE FROM notes WHERE note_id = ${note_id}`
		} else {
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
//module.exports = deleteNote;
// async function main() {
// 	try {
// 		// const username = "david_green"; //he is in the database
// 		// const email = "david@example.com"
// 		// const userFound 2= await retrieveUser(username,email) 
//         // console.log(userFound[0], "this is the found")
// 		console.log("now now now")
// 		const username2 = "==============="; //he is in the database
// 		const email2 = "=======@example.com"
// 		const userFound2 = await retrieveUser(username2,email2) 
//         console.log(userFound2[0], "this is the found")
// 	} catch (error) {
		
// 	}
// }
// main()

