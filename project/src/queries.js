const bcrypt = require('bcrypt');
const pool = require('./database')

async function retrieveUser(user_name, email_address) {
	try {
		valid =  await validateUser(user_name)
		if (!valid) {
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
		//console.error(error); //TODO commented out
		return [false, `error occured checking user`];
	}
}

module.exports = retrieveUser;

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
module.exports = validateUser;

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

module.exports = get_userId;

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

module.exports = sortNotes;
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

module.exports = updateAvatar;
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

module.exports = deleteAccount;
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

module.exports = getOwner;
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

module.exports = deleteNote;

