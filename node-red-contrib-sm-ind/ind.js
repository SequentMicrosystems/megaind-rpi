module.exports = function(RED) {
    "use strict";
    var I2C = require("i2c-bus");
    const DEFAULT_HW_ADD = 0x50;
   
    const I2C_MEM_DIAG_TEMPERATURE = 114; // Byte/Degrees C
    const I2C_MEM_DIAG_24V = 115;  // Word/millivolts
    const I2C_MEM_DIAG_5V = 117;   // Word/millivolts

    const I2C_MEM_OPTO_IN_VAL = 3;
    const I2C_MEM_U0_10_OUT_VAL1 = 4;
    const I2C_MEM_I4_20_OUT_VAL1 = 12;
    const I2C_MEM_OD_PWM1 = 20;
    
    const I2C_MEM_U0_10_IN_VAL1 = 28;
    const I2C_MEM_U_PM_10_IN_VAL1 = 36;
    const I2C_MEM_I4_20_IN_VAL1 = 44;
    const I2C_MEM_OPTO_RISING_ENABLE = 103;
    const I2C_MEM_OPTO_FALLING_ENABLE = 104;
    const I2C_MEM_OPTO_CH_CONT_RESET = 105;
    const I2C_MEM_OPTO_COUNT1 = 106; //2 bytes integers

    const I2C_MEM_RELAY_VAL = 0; // Byte/upper four bits mapped to LEDs
    const I2C_MEM_RELAY_SET = 1; // Byte/values 4-7 map to LEDs 0-3
    const I2C_MEM_RELAY_CLR = 2; // Byte/values 4-7 map to LEDs 0-3

    function VInNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }
                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readI2cBlock(hwAdd, I2C_MEM_U0_10_IN_VAL1 + (channel - 1)*2, 2, buffer,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        msg.payload = res.readIntLE(0, 2) / 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND 0-10V in", VInNode);
   
    function DVInNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }
                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readI2cBlock(hwAdd, I2C_MEM_U_PM_10_IN_VAL1 + (channel - 1)*2, 2, buffer,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        msg.payload = (res.readIntLE(0, 2) - 10000)/ 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND D10V in", DVInNode);   
    
    function VOutNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (this.payloadType == null) {
                myPayload = this.payload;
            } else if (this.payloadType == 'none') {
                myPayload = null;
            } else {
                myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
            }
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else if(isNaN(myPayload)){
              this.status({fill:"red",shape:"ring",text:"Payload type must be a number  ("+this.payload+") value is missing or incorrect myPayload: ("+myPayload+")"});
                return;
            }
            else{
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }

                if(myPayload < 0){
                  myPayload = 0;
                }
                if(myPayload > 10){
                  myPayload = 10;
                }
                var intVal = Math.round(myPayload * 1000);
                
                node.port.writeWord(hwAdd, I2C_MEM_U0_10_OUT_VAL1 + (channel - 1)*2, intVal,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        //rmsg.payload = res.readIntLE(0, 2) / 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND 0-10V out", VOutNode);    
    
// current nodes

    function IInNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }
                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readI2cBlock(hwAdd, I2C_MEM_I4_20_IN_VAL1 + (channel - 1)*2, 2, buffer,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        msg.payload = res.readIntLE(0, 2) / 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND 4-20mA in", IInNode);
    
    
    function IOutNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (this.payloadType == null) {
                myPayload = this.payload;
            } else if (this.payloadType == 'none') {
                myPayload = null;
            } else {
                myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
            }
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else if(isNaN(myPayload)){
              this.status({fill:"red",shape:"ring",text:"Payload type must be a number  ("+this.payload+") value is missing or incorrect myPayload: ("+myPayload+")"});
                return;
            }
            else{
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }

                if(myPayload < 4){
                  myPayload = 4;
                }
                if(myPayload > 20){
                  myPayload = 20;
                }
                var intVal = Math.round(myPayload * 1000);
                
                node.port.writeWord(hwAdd, I2C_MEM_I4_20_OUT_VAL1 + (channel - 1)*2, intVal,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        //rmsg.payload = res.readIntLE(0, 2) / 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND 4-20mA out", IOutNode);    

    function OptoCounterNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.falling = n.falling;
        this.rising = n.rising;
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(4);
        var lastCfgCh = 0;
        var cfgByte = 0;
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            var rising = true;
            var falling = true;
            if(node.rising == false || node.rising == "false" || node.rising == 0)
            {
              rising = false;
            }
            if(node.falling == false || node.falling == "false" || node.falling == 0)
            {
              falling = false;
            }
            var resetIn;
            if(isNaN(msg.reset)) resetIn = 0;
            else resetIn = msg.reset;
            //var buffcount = parseInt(node.count);
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            var hwAdd = DEFAULT_HW_ADD;
            if(stack < 0){
                stack = 0;
            }
            if(stack > 7){
              stack = 7;
            }
            hwAdd += stack;
            
            if(channel < 1){
              channel = 1;
            }
            if(channel > 4){
              channel = 4;
            }
            if(lastCfgCh != channel)
            {
              //node.log("Check configuration");
              node.port.readByte(hwAdd, I2C_MEM_OPTO_RISING_ENABLE,  function(err, rbyte) {
                if (err) { 
                    node.error(err, msg);
                } 
                else{
                  if(((rising == true) && ((rbyte & (1 << (channel - 1))) == 0)) || ((rising == false) && ((rbyte & (1 << (channel - 1))) != 0))){
                    cfgByte = rbyte;
                    if(rising){
                      cfgByte |= 1 << (channel - 1);
                      //node.log("Enable rising edge counting on channel " + channel );
                    }
                    else{
                      cfgByte &= 0xff ^ (1 << (channel -1));
                      //node.log("Disable rising edge counting on channel " + channel );
                    }
                    
                    node.port.writeByte(hwAdd, I2C_MEM_OPTO_RISING_ENABLE, cfgByte, function(err) {
                      if (err) {
                        node.error(err, msg);
                      }
                    });
                  }
                }
                });     
              
              node.port.readByte(hwAdd, I2C_MEM_OPTO_FALLING_ENABLE,  function(err, rbyte) {
                if (err) { 
                    node.error(err, msg);
                } 
                else{
                  if(((falling == true) && ((rbyte & (1 << (channel - 1))) == 0)) || ((falling == false) && ((rbyte & (1 << (channel - 1))) != 0))){
                    cfgByte = rbyte;
                    if(falling){
                      cfgByte |= 1 << (channel - 1);
                      //node.log("Enable falling edge counting on channel " + channel );
                    }
                    else{
                      cfgByte &= 0xff ^ (1 << (channel -1));
                      //node.log("Disable falling edge counting on channel " + channel );
                    }
                    
                    node.port.writeByte(hwAdd, I2C_MEM_OPTO_FALLING_ENABLE, cfgByte, function(err) {
                      if (err) {
                        node.error(err, msg);
                      }
                    });
                  }
                }
                });  
              lastCfgCh = channel;    
            }
            if(resetIn != 0)
            {
              node.port.writeByte(hwAdd, I2C_MEM_OPTO_CH_CONT_RESET, channel, function(err) {
                if (err) {
                  node.error(err, msg);
                }
              });
            }
            else{
              try {
                                  
                  if (this.payloadType == null) {
                      myPayload = this.payload;
                  } else if (this.payloadType == 'none') {
                      myPayload = null;
                  } else {
                      myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                  }
                  node.port.readI2cBlock(hwAdd, I2C_MEM_OPTO_COUNT1 + (channel - 1)*2, 2, buffer,  function(err, size, res) {
                      if (err) { 
                          node.error(err, msg);
                      } 
                      else{
                          msg.payload = res.readIntLE(0, 2);                       
                          node.send(msg);
                      }
                      });     
                      
              } catch(err) {
                  this.error(err,msg);
              }
            }
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND OPT cnt", OptoCounterNode);
    
    
    function OptoInNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.falling = n.falling;
        this.rising = n.rising;
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(4);
      
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
           
            //var buffcount = parseInt(node.count);
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            var hwAdd = DEFAULT_HW_ADD;
            if(stack < 0){
                stack = 0;
            }
            if(stack > 7){
              stack = 7;
            }
            hwAdd += stack;
            
            if(channel < 1){
              channel = 1;
            }
            if(channel > 4){
              channel = 4
            }
            try {
                                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readByte(hwAdd, I2C_MEM_OPTO_IN_VAL ,  function(err, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        //node.log("Opto val read " + res );
                        if ((res & (1 << (channel - 1))) != 0) {
                          msg.payload = true;
                        }
                        else{
                          msg.payload = false;
                        }
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND OPT in", OptoInNode);
    
    function PWMOutNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (this.payloadType == null) {
                myPayload = this.payload;
            } else if (this.payloadType == 'none') {
                myPayload = null;
            } else {
                myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
            }
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else if(isNaN(myPayload)){
              this.status({fill:"red",shape:"ring",text:"Payload type must be a number  ("+this.payload+") value is missing or incorrect myPayload: ("+myPayload+")"});
                return;
            }
            else{
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }

                if(myPayload < 0){
                  myPayload = 0;
                }
                if(myPayload > 100){
                  myPayload = 100;
                }
                var intVal = Math.round(myPayload * 100);
                
                node.port.writeWord(hwAdd, I2C_MEM_OD_PWM1 + (channel - 1)*2, intVal,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        //rmsg.payload = res.readIntLE(0, 2) / 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND OD out", PWMOutNode); 

    function CpuTempNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            stack = parseInt(stack);

            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readByte(hwAdd, I2C_MEM_DIAG_TEMPERATURE, function(err, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else {
                        msg.payload = res;
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND CPU Temp", CpuTempNode);

    function PSVoltageNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            stack = parseInt(stack);

            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readWord(hwAdd, I2C_MEM_DIAG_24V, function(err, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else {
                        msg.payload = res / 1000.0;
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND PS Voltage", PSVoltageNode);


    function RasPiVoltageNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            stack = parseInt(stack);

            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else {
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if (this.payloadType == null) {
                    myPayload = this.payload;
                } else if (this.payloadType == 'none') {
                    myPayload = null;
                } else {
                    myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
                }
                node.port.readWord(hwAdd, I2C_MEM_DIAG_5V, function(err, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else {
                        msg.payload = res / 1000.0;
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND RasPi Voltage", RasPiVoltageNode);

    // LEDs
    function LedOutNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.channel = parseInt(n.channel);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            var channel = node.channel;
            if (isNaN(channel)) channel = msg.channel;
            stack = parseInt(stack);
            channel = parseInt(channel);
            //var buffcount = parseInt(node.count);
            if (this.payloadType == null) {
                myPayload = this.payload;
            } else if (this.payloadType == 'none') {
                myPayload = null;
            } else {
                myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
            }
            if (isNaN(stack)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if (isNaN(channel) ) {
                this.status({fill:"red",shape:"ring",text:"Sensor number  ("+channel+") value is missing or incorrect"});
                return;
            } else if(isNaN(myPayload)){
              this.status({fill:"red",shape:"ring",text:"Payload type must be a number  ("+this.payload+") value is missing or incorrect myPayload: ("+myPayload+")"});
                return;
            }
            else{
                this.status({});
            }
            try {
                var hwAdd = DEFAULT_HW_ADD;
                if(stack < 0){
                    stack = 0;
                }
                if(stack > 7){
                  stack = 7;
                }
                hwAdd += stack;
                
                if(channel < 1){
                  channel = 1;
                }
                if(channel > 4){
                  channel = 4;
                }
		var regAdd = 0;
                if(myPayload != 0){ // turn on LED
		    regAdd = I2C_MEM_RELAY_SET; 

                } else { // turn off LED
		    regAdd = I2C_MEM_RELAY_CLR;
		}
		
		    
                var intVal = channel + 4
                
                node.port.writeByte(hwAdd, regAdd, intVal,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                        //rmsg.payload = res.readIntLE(0, 2) / 1000.0;                       
                        node.send(msg);
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("IND LED out", LedOutNode);    



}
