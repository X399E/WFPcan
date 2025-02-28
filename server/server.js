const express = require('express');
const mysql = require('mysql');
const app = express();
const methodOverride = require('method-override')
const cookieParser = require('cookie-parser');
const crypto = require('crypto');

app.use(express.urlencoded({extended: true})); 
app.use(express.static('public'));
app.use(cookieParser());
app.use(methodOverride('_method'));
app.use(express.json());

const port = 3000;

const con = mysql.createConnection({host: 'localhost',user: 'root',password: 'Kitsf1105',database: 'wfp'});
fs = require("fs");
ejs = require("ejs");

async function tokencheck(req) {
    const cookieinfo = req.headers.cookie || "";
    if (userinfo.length < 2) return false;
    userinfo = String(cookieinfo).split('=');
    
    try {
        const [results] = await con.promise().query('SELECT * FROM user WHERE id = ?', [userinfo[0]]);
        if (results.length === 0) return false;
        let dbUser = results[0]["token"];
        return dbUser === userinfo[1];
    } catch (error) {
        console.error("Database error:", error);
        return false;
    }
}

app.get('/login',(req, res)=>{
    let token = String(req.headers.cookie)
    if (token != "undefined"){
        token = token.split('=');
        res.cookie(token[0], "token",{path: '/',maxAge: 0});
        con.query('UPDATE user SET token=NULL WHERE id=?', [token[0]] , function (error, result, fields) {});
    }
    res.sendFile(__dirname + '/index/login.html');
})

app.get('/',(req, res)=>{
    if(tokencheck(req) === true){
        const template = fs.readFileSync(`${__dirname}/index/index.ejs`, "utf-8");
        const data = ejs.render(template, {temp : temperature ,rpm : num});
        res.write(data);
        res.end()
    }
})

app.post('/receive', (req, res) => {
    console.log(req.body);
    res.send(req.body);
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});