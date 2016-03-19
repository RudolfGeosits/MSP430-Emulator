// Websock Declarations
var ws = new WebSocket('ws://127.0.0.1:9000', 'emu-protocol');
//var ws = new WebSocket('ws://poorhackers.com:9000', 'emu-protocol');


// Image Declarations
var IMG_BASE = new Point(100, 10);

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

var serial_state = "visible";
show_serial_button.onclick = function(){

    if (serial_state == "visible") {
	serial.style.display = "none";
	serial_in.style.display = "none";
	serial_state = "hidden";
    }
    else if (serial_state == "hidden") {
	serial.style.display = "block";
	serial_in.style.display = "block";
	serial_state = "visible";
    }

};

// upload binary file //
var contents;

upload_button.onclick = function(){	     
    ws.send("UPLOAD", { binary: false});
    ws.send(contents, { binary: true });
    ws.send("NPLOAD", { binary: false});
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

    ws.send(x); ws.send(y);

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
ws.onmessage =  function (event) {
    var msg = event.data;

    if (stdout_mode) {
	print_console(msg);		 
	stdout_mode = false;
    }
    
    else if (serial_mode) {
	print_serial(msg);
	serial_mode = false;
    }
    else {
	switch (msg) {

	// P1.0 OUTPUT
	case "P1.0 1": {
	    P1_0_LED_image.visible = true;			 
	    P1_0_value_text.content = '1';

	    break;
	}	    
	case "P1.0 0": {
	    P1_0_LED_image.visible = false;
	    P1_0_value_text.content = '0';

	    break;
	}
	    
	// P1.1 OUTPUT
	case "P1.1 1": {
	    P1_1_value_text.content = '1';

	    break;
	}
	case "P1.1 0": {
	    P1_1_value_text.content = '0';

	    break;
	}

	// P1.2 OUTPUT
	case "P1.2 1": {
	    P1_2_value_text.content = '1';

	    break;
	}
	case "P1.2 0": {
	    P1_2_value_text.content = '0';

	    break;
	}

	// P1.3 OUTPUT
	case "P1.3 1": {
	    P1_3_value_text.content = '1';

	    break;
	}
	case "P1.3 0": {
	    P1_3_value_text.content = '0';

	    break;
	}

	// P1.4 OUTPUT
	case "P1.4 1": {
	    P1_4_value_text.content = '1';

	    break;
	}
	case "P1.4 0": {
	    P1_4_value_text.content = '0';

	    break;
	}

	// P1.5 OUTPUT
	case "P1.5 1": {
	    P1_5_value_text.content = '1';

	    break;
	}
	case "P1.5 0": {
	    P1_5_value_text.content = '0';

	    break;
	}

	// P1.6 OUTPUT
	case "P1.6 1": {
	    P1_6_LED_image.visible = true;
	    P1_6_value_text.content = '1';

	    break;
	}
	case "P1.6 0": {
	    P1_6_LED_image.visible = false;
	    P1_6_value_text.content = '0';

	    break;
	}
	    
	// P1.7 OUTPUT
	case "P1.7 1": {
	    P1_7_value_text.content = '1';

	    break;
	}
	case "P1.7 0": {
	    P1_7_value_text.content = '0';

	    break;
	}

	case "_STDOUT_": {
	    stdout_mode = true;
	    break;
	}

	case "_SERIAL_": {
	    serial_mode = true;
	    break;
	}

	    // Line of text for the console
	default: {
	    //print_console(msg);
	    break;
	}
	}
    }

    paper.view.update();
};

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
