// Socket Declarations
var listener, ws;

// Image Declarations
var IMG_BASE = view.center;
IMG_BASE.y = 10;
//new Point(500, 10);

var msp_image = new Raster('msp', IMG_BASE.x + 142.5, 
			   IMG_BASE.y + 185.5);

var reset_button_down_image = new Raster('button_down', 
					 IMG_BASE.x + 215, 
					 IMG_BASE.y + 345);
reset_button_down_image.visible = false;

var P1_3_button_down_image = new Raster('button_down', IMG_BASE.x + 28,
					IMG_BASE.y + 345);
P1_3_button_down_image.visible = false;

var P1_0_LED_image = new Raster('P1_0_LED', IMG_BASE.x + 70, 
				IMG_BASE.y + 361.5);
P1_0_LED_image.visible = false;

var P1_6_LED_image = new Raster('P1_6_LED', IMG_BASE.x + 84, 
				IMG_BASE.y + 361);
P1_6_LED_image.visible = false;

// windows
var serial_window = document.getElementById('serial_window');    
var console_window = document.getElementById('console_window');    
var canvas = document.getElementById('myCanvas');


// Text declarations
var P1_0_value_text = new PointText({
    point: [IMG_BASE.x + 102, IMG_BASE.y + 190],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_1_value_text = new PointText({
    point: [IMG_BASE.x + 102, IMG_BASE.y + 205],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_2_value_text = new PointText({
    point: [IMG_BASE.x + 102, IMG_BASE.y + 220],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_3_value_text = new PointText({
    point: [IMG_BASE.x + 102, IMG_BASE.y + 234],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_4_value_text = new PointText({
    point: [IMG_BASE.x + 102, IMG_BASE.y + 248],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_5_value_text = new PointText({
    point: [IMG_BASE.x + 102, IMG_BASE.y + 262],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_6_value_text = new PointText({
    point: [IMG_BASE.x + 175, IMG_BASE.y + 262],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});

var P1_7_value_text = new PointText({
    point: [IMG_BASE.x + 175, IMG_BASE.y + 248],
    content: '0',
    fontSize: 14,
    fontWeight: 'bold',
    fillColor: 'white',
    fontFamily: 'Terminal',
});


// textArea declarations
var stdout = document.getElementById('console');
stdout.value = "";

var serial = document.getElementById('serial_out');
serial.value = "";

// Button declarations
var pause_button = document.getElementById('pause_button');
var play_button = document.getElementById('play_button');
var enter_button = document.getElementById('enter_button');
var serial_enter_button = document.getElementById('serial_enter_button');
var upload_button = document.getElementById('upload_button');
var show_serial_button = document.getElementById('show_serial_button');


function print_console (text){
    var another = stdout.value + text;
    stdout.value = another;

    stdout.scrollTop = stdout.scrollHeight;
}

function print_serial (text){
    var another = serial.value + text;
    serial.value = another;

    serial.scrollTop = serial.scrollHeight;
}

//var serial_state = "visible";
show_serial_button.onclick = function()
{
    if (serial_window.style.display == "none") {
	serial_window.style.display = "block";
    }
    else {
	serial_window.style.display = "none";
    }
};

// upload binary file //
var contents;

//upload_button.onclick = function(){	     
upload_button.onclick = function() {	     
    ws.send("UPLOAD");
    ws.send(contents);
    ws.send("NPLOAD");
};

function readSingleFile(e) {
    var file = e.target.files[0];
    if (!file) {
	return;
    }

    var reader = new FileReader();
    
    reader.onload = function(e) {
	contents = e.target.result;
    };

    reader.readAsArrayBuffer(file);
}

document.getElementById('file-input').addEventListener('change', readSingleFile, false);
//

var prev_cmd = "";
enter_button.onclick = function(){
    if (stdin.value == "") {
	if (prev_cmd != "")
	    stdin.value = prev_cmd;
	else 
	    return;
    }
    else {
	prev_cmd = stdin.value;
    }

    ws.send(stdin.value);
    stdin.value = "";
};

serial_enter_button.onclick = function(){
    if (serial_in.value == "") {
	return;
    }
    
    ws.send("_SERIAL_");
    ws.send(serial_in.value + "\n");
    serial_in.value = "";
};

pause_button.onclick = function(){
    ws.send("PAUSE");
};

play_button.onclick = function(){
    ws.send("PLAY");
};	 

function onMouseDown (event) {
    var x = event.point.x;
    var y = event.point.y;

    //ws.send(x); ws.send(y);

    if (x >= (IMG_BASE.x + 15) && x <= (IMG_BASE.x + 49)) {
	if (y >= (IMG_BASE.y + 327) && y <= (IMG_BASE.y + 360)) {
	    P1_3_button_down_image.visible = true;
	}
    }

    if (x >= (IMG_BASE.x + 199) && x <= (IMG_BASE.x + 232)) {
	if (y >= (IMG_BASE.y + 327) && y <= (IMG_BASE.y + 360)) {
	    reset_button_down_image.visible = true;
	}
    }	     
    paper.view.update();
}

function onMouseUp (event) {
    P1_3_button_down_image.visible = false;
    reset_button_down_image.visible = false;
    
    paper.view.update();
}

var stdout_mode = false;
var serial_mode = false;

// Websock Declarations
var listener = new WebSocket("ws://127.0.0.1:9000", 'emu-protocol');
//var listener = new WebSocket('ws://poorhackers.com:9000', 'emu-protocol');

listener.onmessage = function (event) {
    var msg = event.data;
    console.log("Here got " + msg);
    ws = new WebSocket("ws://127.0.0.1:" + msg, 'emu-protocol');
    //ws = new WebSocket("ws://poorhackers.com:" + msg, 'emu-protocol');
    
    ws.binaryType = "arraybuffer";

    function ab2str(buf) {
	return String.fromCharCode.apply(null, new Uint16Array(buf));
    }

    ws.onmessage =  function (event) {
	var packet = event.data;
	
	var data = new DataView(packet);
	var opcode = data.getUint8(0);

	var data_len = data.byteLength;
	var message_len = data_len - 1;
	
	var str_array = new Uint16Array(message_len * 2);
	
	for (var i = 1;i < data_len;i = i+1) {
	    str_array[i-1] = data.getUint8(i);
	    //console.log(str_array[i-1]);
	}
	
	var message = ab2str(str_array);
	var control_opcode = null;
	var control_data   = null;
	//console.log("Here got [" + message + "], len " + data_len);

	switch (opcode) {
	   case 0x00: {
	       control_opcode = data.getUint8(1);
	       control_data   = null;

	       //console.log("control: " + data.getUint8(1));
	       break;
	   }

	   case 0x01: {
	       print_console(message);
	       break;
	   }

	   case 0x02: {
	       print_serial(message);
	       break;
	   }
	}
	

	switch (control_opcode) {	    
	    // P1.0 OUTPUT
	    case 0x00: {
		P1_0_LED_image.visible = true;			 
		P1_0_value_text.content = '1';

		break;
	    }	    
	    case 0x01: {
		P1_0_LED_image.visible = false;
		P1_0_value_text.content = '0';

		break;
	    }
		
	    // P1.1 OUTPUT
	    case 0x02: {
		P1_1_value_text.content = '1';

		break;
	    }
	    case 0x03: {
		P1_1_value_text.content = '0';

		break;
	    }

	    // P1.2 OUTPUT
	    case 0x04: {
		P1_2_value_text.content = '1';

		break;
	    }
	    case 0x05: {
		P1_2_value_text.content = '0';

		break;
	    }

	    // P1.3 OUTPUT
	    case 0x06: {
		P1_3_value_text.content = '1';

		break;
	    }
	    case 0x07: {
		P1_3_value_text.content = '0';

		break;
	    }

	    // P1.4 OUTPUT
	    case 0x08: {
		P1_4_value_text.content = '1';

		break;
	    }
	    case 0x09: {
		P1_4_value_text.content = '0';

		break;
	    }

	    // P1.5 OUTPUT
	    case 0x0A: {
		P1_5_value_text.content = '1';

		break;
	    }
	    case 0x0B: {
		P1_5_value_text.content = '0';

		break;
	    }

	    // P1.6 OUTPUT
	    case 0x0C: {
		P1_6_LED_image.visible = true;
		P1_6_value_text.content = '1';

		break;
	    }
	    case 0x0D: {
		P1_6_LED_image.visible = false;
		P1_6_value_text.content = '0';

		break;
	    }
		
	    // P1.7 OUTPUT
	    case 0x0E: {
		P1_7_value_text.content = '1';

		break;
	    }
	    case 0x0F: {
		P1_7_value_text.content = '0';

		break;
	    }

	    default: {
		break;
	    }
	}

	paper.view.update();
    };

}

stdin.onkeydown = function auto_enter (event) {
    // On enter key press
    if (event.keyCode == 13) {
        enter_button.click();
    }
};

serial_in.onkeydown = function auto_enter (event) {
    // On enter key press
    if (event.keyCode == 13) {
        serial_enter_button.click();
    }
};


/// Dragable TextArea console_window
var selected = null, // Object of the element to be moved
    x_pos = 0, y_pos = 0, // Stores x & y coordinates of the mouse pointer
    x_elem = 0, y_elem = 0; // Stores top, left values (edge) of the element

// Will be called when user starts dragging an element
function _drag_init(elem) {
    // Store the object of the element which needs to be moved
    selected = elem;
    x_elem = x_pos - selected.offsetLeft;
    y_elem = y_pos - selected.offsetTop;
}

// Will be called when user dragging an element
function _move_elem(e) {
    x_pos = document.all ? window.event.clientX : e.pageX;
    y_pos = document.all ? window.event.clientY : e.pageY;

    var canvas_left = canvas.offsetLeft;
    var canvas_top = canvas.offsetTop;
    var canvas_right = canvas.offsetRight;
    var canvas_bottom = canvas.offsetBottom;
    
    if (selected !== null) {
	if (selected == console_window) {
            selected.style.left = (x_pos - x_elem - canvas_left) + 'px';
            selected.style.top =  (y_pos - y_elem - canvas_top) + 'px';
	}
	else if (selected == serial_window) {
            selected.style.left   =  (x_pos - x_elem - canvas_left) + 'px';
            selected.style.top    =  (y_pos - y_elem - canvas_top)  + 'px';
	}
	else if (selected == register_window) {
            selected.style.left   =  (x_pos - x_elem - canvas_left) + 'px';
            selected.style.top    =  (y_pos - y_elem - canvas_top) + 'px';
	}	
    }
}

// Destroy the object when we are done
function _destroy() {
    selected = null;
}

// Bind the functions...
document.getElementById('console_window').onmousedown = function () {
    _drag_init(this);
    stdin.focus();

    return false;
};

document.getElementById('serial_window').onmousedown = function () {
    _drag_init(this);
    serial_in.focus();

    return false;
};

document.getElementById('register_window').onmousedown = function () {
    _drag_init(this);

    return false;
};

document.onmousemove = _move_elem;
document.onmouseup = _destroy;

$('#myCanvas').on('mousewheel', function(event) {
    console.log(event.deltaX, event.deltaY, event.deltaFactor);
});

var table = document.getElementById("register_table");
for (var i = 0, row; row = table.rows[i]; i++) {
    //iterate through rows
    //rows would be accessed using the "row" variable assigned in the for loop
    for (var j = 0, col; col = row.cells[j]; j++) {
	col = "X";
	//iterate through columns
	//columns would be accessed using the "col" variable assigned in the for loop
    }  
}
