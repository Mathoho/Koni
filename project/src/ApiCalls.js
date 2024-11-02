const express = require("express");
const app = express(); //takes express library and runs it
const cors = require("cors");

/**Methods from APIMethods */
const registerUser = require('./APIMethods');
const loginUser = require('./APIMethods');
const getAcessToken = require('./APIMethods');
const getRefreshToken = require('./APIMethods');
const getUserNotes = require('./APIMethods');
const NotesFiltered = require('./APIMethods');
const createNote = require('./APIMethods');
const validateToken = require('./APIMethods')

app.use(cors())

//for getting requested data from client
app.use(express.json());

app.get("/",async(req, res) => {
    try {
        return res.status(201).json({message: "Login Page or Sign in Page"});
    } catch (error) {
        console.error(error.message)
    }
    
});

app.post("/register",async(req,res)=>{
    try {
        
        let result = await registerUser(req.body);
        switch (result) {
            case "User already exists":
                return res.status(400).json({ message: "User already exists" });
            case "User registered successfully":
                return res.status(201).json({message: "User registered successfully"});
            default:
                //console.error(error);
                return res.status(500).json({ message: 'Internal server error' });
                //return res.status(201).json({ message: 'KONI posted stuff' }); //TODO debug
        }
    } catch (error) {
        console.error(error.message);
        return res.status(500).json({ message: 'Internal server error' });    
    }
});