// Init

const BLANK = 0;
const LIGHTBULB = 1;
const MARK = 2;
const BLACK = 8;
const BLACK0 = 8;
const BLACK1 = 9;
const BLACK2 = 10;
const BLACK3 = 11;
const BLACK4 = 12;
const BLACKU = 13;
const LIGHTED = 16;
const ERROR = 32;

var mario = new Image();
mario.src = "images/mario.png";
var background = new Image();
background.src = "images/mansion.png";
var boo = new Image();
boo.src = "images/boo.png";
var shyboo = new Image();
shyboo.src = "images/shyboo.png";
var kingboo = new Image();
kingboo.src = "images/kingboo.png";
var luigi = new Image();
luigi.src = "images/luigi.png";
var mist = new Image();
mist.src = "images/mist.png";

var block0 = new Image();
block0.src = "images/block0.png";
var block0r = new Image();
block0r.src = "images/block0r.png";
var block1 = new Image();
block1.src = "images/block1.png";
var block1r = new Image();
block1r.src = "images/block1r.png";
var block2 = new Image();
block2.src = "images/block2.png";
var block2r = new Image();
block2r.src = "images/block2r.png";
var block3 = new Image();
block3.src = "images/block3.png";
var block3r = new Image();
block3r.src = "images/block3r.png";
var block4 = new Image();
block4.src = "images/block4.png";
var block4r = new Image();
block4r.src = "images/block4r.png";
var blockU = new Image();
blockU.src = "images/blockU.png";

Module.onRuntimeInitialized = () => { start(); }

elm = document.getElementById("mycanvas");
ctx = elm.getContext("2d");
window.addEventListener('load', start);
elm.addEventListener('click', LeftClickEvent);
elm.addEventListener('contextmenu', RightClickEvent);

// Buttons

function restart() {
    console.log("Restart");
    Module._restart(g);
    ctx.clearRect(0, 0, Module._nb_rows(g) * ecart, Module._nb_cols(g) * ecart);
    drawgrid();
    printGame(g);
}

function undo() {
    console.log("Undo");
    Module._undo(g);
    printGame(g);
}

function redo() {
    console.log("Redo");
    Module._redo(g);
    printGame(g);
}

function solve() {
    console.log("Solve");
    Module._solve(g);
    printGame(g);
}

function new_game() {
    console.log("New");
    g = Module._new_random(7, 7, 0, 10, false);
    printGame(g);
}

// Events

function LeftClickEvent(event) {
    event.preventDefault();
    console.log("left click at position:", event.offsetX, event.offsetY);

    var i = Math.floor(event.offsetX / ecart);
    var j = Math.floor(event.offsetY / ecart);
    if (Module._is_blank(g, i, j)) {
        Module._play_move(g, i, j, LIGHTBULB);
    } else if (Module._is_lightbulb(g, i, j)) {
        Module._play_move(g, i, j, BLANK);
    }
    printGame(g);
}

function RightClickEvent(event) {
    event.preventDefault();
    console.log("right click at position:", event.offsetX, event.offsetY);

    var i = Math.floor(event.offsetX / ecart);
    var j = Math.floor(event.offsetY / ecart);
    if (Module._is_blank(g, i, j)) {
        Module._play_move(g, i, j, MARK);
    } else if (Module._is_marked(g, i, j)) {
        Module._play_move(g, i, j, BLANK);
    }
    printGame(g);
}

// Draw

function drawgrid() {

    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);

    var ecart = elm.height / nb_rows;

    ctx.strokeStyle = 'white'
    //rows
    for (var h = ecart; h < elm.height; h += ecart) {
        ctx.moveTo(0, h);
        ctx.lineTo(elm.width, h);
    }
    //cols
    var ecart = elm.height / nb_cols;
    for (var w = ecart; w < elm.width; w += ecart) {
        ctx.moveTo(w, 0);
        ctx.lineTo(w, elm.height);
    }
    ctx.stroke();
}

function printGame(g) {
    var elm = document.getElementById("mycanvas");
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    ecart = elm.height / nb_rows;
    var ecart2 = ecart * 0.9;
    var ecart3 = (ecart - ecart2) / 2;
    drawgrid()
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var blank = Module._is_blank(g, row, col);
            var black = Module._is_black(g, row, col);
            var ligthed = Module._is_lighted(g, row, col);
            var lightbulb = Module._is_lightbulb(g, row, col);
            var marked = Module._is_marked(g, row, col);
            var error = Module._has_error(g, row, col);
            if (black && !error) {
                if (Module._get_black_number(g, row, col) == 0) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block0, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 1) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block1, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 2) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block2, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 3) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block3, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 4) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block4, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == -1) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(blockU, row * ecart, col * ecart, ecart, ecart);
                }
            } else if (black && error) {
                if (Module._get_black_number(g, row, col) == 0) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block0r, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 1) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block1r, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 2) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block2r, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 3) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block3r, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == 4) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(block4r, row * ecart, col * ecart, ecart, ecart);
                } else if (Module._get_black_number(g, row, col) == -1) {
                    ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                    ctx.drawImage(blockUr, row * ecart, col * ecart, ecart, ecart);
                }
            } else if (lightbulb && Module._is_over(g)) {
                console.log("Game won");
                ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                ctx.drawImage(kingboo, row * ecart, col * ecart, ecart, ecart);
            } else if (lightbulb && !error) {
                ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                ctx.drawImage(boo, row * ecart, col * ecart, ecart, ecart);
            } else if (lightbulb && error) {
                ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                ctx.drawImage(shyboo, row * ecart, col * ecart, ecart, ecart);
            } else if (marked && !error) {
                ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                if (ligthed) {
                    ctx.drawImage(mist, row * ecart, col * ecart, ecart, ecart);
                }
                ctx.drawImage(luigi, row * ecart, col * ecart, ecart, ecart);
            } else if (ligthed && !error) {
                ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
                ctx.drawImage(mist, row * ecart, col * ecart, ecart, ecart);
            } else if (blank && !error) {
                ctx.clearRect(row * ecart, col * ecart, ecart, ecart);
            }
        }
    }
}

function ShybooToBoo() {
    let elt = document.getElementById("boo");
    elt.src = "images/shyboo.png";
}

function BooToShyboo() {
    let elt = document.getElementById("boo");
    elt.src = "images/boo.png";
}

function start() {
    console.log("call start routine");
    g = Module._new_random(7, 7, 0, 10, false);
    printGame(g);
}
