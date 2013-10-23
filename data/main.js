// JavaScript without jQuery and fancy tools is a bit ugly
//
// TODO error handling and other pro stuff :-D

function dirtyHttpPost(url) {
    var req = new XMLHttpRequest();
    req.open("POST",url);
    req.send();
}

function on(event) {
    dirtyHttpPost("on/"+document.getElementById("effect").value);
}

function off(event) {
    dirtyHttpPost("off");
}

function next(event) {
    var o = document.getElementById("effect");
    o.value = (Number(o.value)+1) % 38;
    dirtyHttpPost("function");
}

