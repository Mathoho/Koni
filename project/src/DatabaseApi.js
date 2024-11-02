const registerUser = (email, username, password) => {
    //TODO: Api call to register user
    //request.post("url/register-user")
    const requestBody = {
        "email": email,
        "username": username,
        "password": password
    };

    const response = {
        "status": 200,
        "message": "User registered successfully"
    };
};

const signInUser = (username, password) => {
    //TODO: Api call to sign in user
	const requestBody = {
		"username": username,
		"password": password
	};

    const response = {
        "status": 200,
        "message": "User signed in successfully"
    };

};

const deleteUser = (username) => {
    //TODO: Api call to delete user
    //request.delete("url/deleteUser/<username>")

    const response = {
        "status": 200,
        "message": "User deleted successfully"
    };
};

const updateUsername = (oldUsername, username) => {
    //TODO: Api call to update username
    //request.put("url/rename-user")

    const requestBody = {
        "oldUsername": oldUsername,
        "username": username
    };

    const response = {
        "status": 200,
        "message": `User renamed successfully to ${username}`
    }
};

const getNotes = (username) => {
    //TODO: Api call to fetch user's notes

    //request.get("url/get-user-notes/<username>")

    //expected response: array of json note objects.
    const response = {
        "notes": [
            {
                "id": 1,
                "title": "Note title1",
                "body": "This is the note body",
                "category": "Category1",
                "lastEdited": "11 October 2023"
            },
            {
                "id": 2,
                "title": "Note title2",
                "body": "This is the note body",
                "category": "Category2",
                "lastEdited": "11 October 2023"
            },
            {
                "id": 3,
                "title": "Note title3",
                "body": "This is the note body",
                "category": "Category3",
                "lastEdited": "11 October 2023"
            }
        ]
    }

};

const deleteNote = (noteId) => {
    //TODO: Api call to delete note using noteId
    //request.delete("url/delete-user-notes/<noteId>")
    const response = {
        "status": 200,
        "message": "Note deleted successfully"
    };

};

const addNote = (username, note) => {
    //TODO: APi call to add note
    //request.post("url/add-note/<username>")
    const requestBody = {
        "title": note.title,
        "body": note.body,
        "category": note.category,
        "lastEdited": note.lastEdited
    };

    const response = {
        "status": 200,
        "message": "Note added successfully",
        "id": 101 //The id of the note after it's added to the db
    };
};

const updateNote = (noteId, title, body, lastEdited) => {
    //TODO: Api call to update note (title or body)
    //request.put("url/update-note/<noteId>")
    const requestBody = {
        "title": title,
        "body": body,
        "lastEdited": lastEdited
    };

    const response = {
        "status": 200,
        "message": "Note updated successfully",
    };
};

const shareNote = (noteId, username) => {
    //TODO: Api call to bind user to note
    //request.put("url/share-note/<username>")
    const requestBody = {
        "noteId": noteId
    };

    const response = {
        "status": 200,
        "message": `Note shared with ${username}`
    };
};
