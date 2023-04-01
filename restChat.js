// Rest based chat client
// Jim Skon 2022
// Kenyon College

var baseUrl = 'http://44.202.89.194:5005';
var state="off";
var myname="";
var inthandle;
var masterUserList =[];
var activeUserList =[];

/* Start with text input and status hidden */
document.getElementById('chatinput').style.display = 'none';
document.getElementById('status').style.display = 'none';
document.getElementById('leave').style.display = 'none';
// Action if they push the join button
document.getElementById('login-btn').addEventListener("click", (e) => {
	join();
})

/* Set up buttons */
document.getElementById('leave-btn').addEventListener("click", leaveSession);
document.getElementById('send-btn').addEventListener("click", sendText);

// Watch for enter on message box
document.getElementById('message').addEventListener("keydown", (e)=> {
    if (e.code == "Enter") {
	e.preventDefault();
    sendText();
    clearTextbox();
    return false;
    }   
});


// Call function on page exit
window.onbeforeunload = leaveSession;

function register() {
    username = document.getElementById('register-name');
    email = document.getElementById('register-email');
    password = document.getElementById('register-pass');
    color = document.getElementById('register-color');
    fetch(baseUrl+'/chat/join/'+username+'/'+email+'/'+password+'/'+color, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeRegister(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })
}

function completeRegister(results) {
	var status = results['status'];
	if (status != "success") {
		alert("Username or email already exists!");
		leaveSession();
		return;
	}
	var user = results['user'];
	console.log("Register:"+user);
	startSession(user);
	activateUser(user);
}

function completeJoin(results) {
	var status = results['status'];
	if (status != "success") {
		alert("Username or email already exists!");
		leaveSession();
		return;
	}
	var user = results['user'];
	console.log("Join:"+user);
	startSession(user);
	activateUser(user);
}

function join() {
	myname = document.getElementById('yourname').value;
	fetch(baseUrl+'/chat/join/'+myname, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeJoin(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })
}

function completeSend(results) {
	var status = results['status'];
	if (status == "success") {
		console.log("Send succeeded")
	} else {
		alert("Error sending message!");
	}
}

//function called on submit or enter on text input
function sendText() {
    var message = document.getElementById('message').value;
    console.log("Send: "+myname+":"+message);
	fetch(baseUrl+'/chat/send/'+myname+'/'+message, {
        method: 'get'
    })

    .then (response => response.json() )
    .then (data =>completeSend(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })
    clearTextbox();    
}

function clearTextbox() {
    document.getElementById('message').value = "";
}

function completeFetch(result) {
	names = result["userlist"];
	names.forEach(function (m,i) {
		name = m["name"] + ", ";
		if  (masterUserList.includes(name) == false) {
		masterUserList += name;
		}
    });
    messages = result["messages"];
	messages.forEach(function (m,i) {
		name = m['user'];
		message = m['message'];
		document.getElementById('chatBox').innerHTML +=
	    	"<font color='red'>" + name + ": </font>" + message + "<br />";
		activateUser(name);
	});
}

/* Check for new messaged */
function fetchMessage() {
	fetch(baseUrl+'/chat/fetch/'+myname, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeFetch(data))
    .catch(error => {
        {console.log("Server appears down");}
    })  	
}

function updateChatMembers() {
    var masterUserArray = masterUserList.split(", ");
	document.getElementById('members').innerHTML = ""
    masterUserArray.forEach(function (element,i) {
        var user = element;
        if (activeUserList.includes(user)) {
            document.getElementById('members').innerHTML +=
            "<font color='green'>" + user + " " +"</font> ";
        }
        else {
            document.getElementById('members').innerHTML +=
            "<font color='grey'>" + user + " " + "</font>";
        } 
    });
}

function activateUser(name) {
    if (activeUserList.includes(name) == false) {
        activeUserList.push(name);
    }
}

/* Functions to set up visibility of sections of the display */
function startSession(name){
    state="on";
    
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'none';
    document.getElementById('user').innerHTML = "User: " + name;
    document.getElementById('chatinput').style.display = 'block';
    document.getElementById('members').style.display = 'block';
    document.getElementById('status').style.display = 'block';
    document.getElementById('leave').style.display = 'block';
    /* Check for messages every 500 ms */
    inthandle=setInterval(fetchMessage,500);
    inthandle=setInterval(updateChatMembers,1000);
}

function completeLogout(user){
   // something to remove the user from the ActiveUsers list/array but not just client side
}

function logout(){
    fetch(baseUrl+'/chat/logout/'+myname, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data => completeLogout(data))
    .catch(error => {
        {console.log("Server appears down");}
    })
	completeLogout(myname);
}

function leaveSession(){
    state="off";
    logout();
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'block';
    document.getElementById('user').innerHTML = "";
    document.getElementById('chatinput').style.display = 'none';
    document.getElementById('members').style.display = 'none';
    document.getElementById('status').style.display = 'none';
    document.getElementById('leave').style.display = 'none';
	clearInterval(inthandle);
}