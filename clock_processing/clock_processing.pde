import oscP5.*;
import netP5.*;
import processing.serial.*;

OscP5 oscP5;
Serial port;
float[] inputSequence = {}; // Array to hold the sequence of inputs received from Wekinator
int maxSequenceLength = 3; // Maximum length of sequence to check

void setup() {
  size(400, 200);
  oscP5 = new OscP5(this, 12000); 
  
  println("Available serial ports:");
  printArray(Serial.list());
  port = new Serial(this, Serial.list()[4], 115200);
  
  // Set text properties for visualization
  textSize(20);
  textAlign(CENTER, CENTER);
}

void draw() {
  background(255);
  
  // Draw the current inputSequence on the screen
  drawSequence();
}

void oscEvent(OscMessage msg) {
  // Check for OSC message from Wekinator
  if (msg.checkAddrPattern("/wek/outputs") && msg.typetag().length() > 0) {
    float input = msg.get(0).floatValue();// Assume the input is an integer
    println(input);
    
    
    // Add the new input to the sequence if it's different from the last element
    if (inputSequence.length == 0 || input != inputSequence[inputSequence.length - 1]) {
      inputSequence = append(inputSequence, input);
    }
    
     // Limit sequence length
    if (inputSequence.length > maxSequenceLength) {
      inputSequence = subset(inputSequence, 1); // Keep only the last maxSequenceLength elements
    }
    
    // Determine output based on sequence and send to Arduino
    int output = getOutputForSequence(inputSequence);
    if (output != -1) {
      port.write(str(output));
      println("Output sent to Arduino: " + output);
      
      // Reset sequence if a specific output was recognized
      if (output == 5){
        inputSequence = new float[0];
      }
    }else if(inputSequence.length == 1){
        inputSequence = new float[0];
     }
  }
}


int getOutputForSequence(float[] sequence) {
  // Check the input sequence against known patterns
  // start boat
  if (sequence.length == 1 && sequence[0] == 1) {
    return 1;
    // start first stepper
  } else if (sequence.length == 2 && sequence[0] == 1 && sequence[1] == 2) {
    return 2;
    //start second stepper
  } else if (sequence.length == 3 && sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3) {
    return 3;
    //go back in time
  } else if (sequence.length == 3 && sequence[0] == 1 && sequence[1] == 3 && sequence[2] == 2) {
    return 4;
    // stop
  } else if (sequence.length == 3 && sequence[2] == 5) {
    return 5;
  }
  return -1; // No matching sequence found
}

void drawSequence() {
  // Draw the array on the screen as rectangles
  int boxSize = 50; // Size of each box representing an element in inputSequence
  int startX = 50;  // Starting x position
  int startY = height / 2; // y position to center the sequence vertically
  
  for (int i = 0; i < inputSequence.length; i++) {
    // Draw a rectangle for each element in the sequence
    fill(200, 200, 255);
    rect(startX + i * boxSize, startY, boxSize, boxSize);
    
    // Display the value of the element inside the rectangle
    fill(0);
    text(inputSequence[i], startX + i * boxSize + boxSize / 2, startY + boxSize / 2);
  }
}
