CREATE TABLE users(
	user_id SERIAL PRIMARY KEY,
	user_name varchar(25) UNIQUE, --user_name can be changed (spec argues change user_profile details)
	email_address varchar(50) UNIQUE,
	avatar_url varchar(128),
	u_password varchar(128)
	-- salt varchar(32) // hash of the password
);

CREATE TABLE notes(
	note_id SERIAL PRIMARY KEY,
	note_time timestamp default current_timestamp,
	note_title varchar(100),
	content text,
	user_id int references users(user_id) on delete cascade
);

CREATE TABLE categories(
	c_id SERIAL PRIMARY KEY,
	c_type varchar(25) UNIQUE
);

CREATE TABLE user_notes(
	user_id int references users(user_id)  on delete cascade, -- The primary key will be a combination of user_id and note_id
	note_id int references notes(note_id) on delete cascade,
	c_id int references categories(c_id), -- Accounts for the fact that user should be able to create their own categories
	PRIMARY KEY (user_id, note_id), -- Accounting for the many - to - many relationship of users and notes
	FOREIGN KEY (c_id) REFERENCES categories(c_id),
	FOREIGN KEY (note_id) REFERENCES notes(note_id)
);