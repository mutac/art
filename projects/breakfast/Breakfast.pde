/**
 * http://www.extrapixel.ch/processing/gifAnimation/
 * http://support.microsoft.com/kb/182383
 * http://msdn.microsoft.com/en-us/library/windows/desktop/ms686421(v=vs.85).aspx#ss_afterbuild
 * http://processing.org/discourse/yabb2/YaBB.pl?board=Syntax;action=display;num=1185318989
 * http://docs.oracle.com/javase/1.3/docs/guide/2d/spec/j2d-awt.fm6.html
 */


import java.awt.*;
import javax.swing.*;
import gifAnimation.*;

SpriteArena spriteArena;
PVector lastMousePosition;
Rectangle virtualFrameExtents = new Rectangle();
boolean locationAndFocusSet = false;
boolean ignoreFirstMouseMove = true;

/**
 */
public void init() {
  frame.removeNotify();
  frame.setUndecorated(true);
  super.init();
}

/**
 */
void setupDisplay() {
  GraphicsEnvironment graphicsEnv = GraphicsEnvironment.getLocalGraphicsEnvironment();

  GraphicsDevice[] graphicsDevices = graphicsEnv.getScreenDevices();
  for (int deviceIdx = 0; deviceIdx < graphicsDevices.length; deviceIdx++) {
    GraphicsConfiguration[] graphicsConfigurations = graphicsDevices[deviceIdx].getConfigurations(); 
    for (int deviceConfigIdx = 0; deviceConfigIdx < graphicsConfigurations.length; deviceConfigIdx++) {
      GraphicsConfiguration deviceConfig = graphicsConfigurations[deviceConfigIdx];
      virtualFrameExtents = virtualFrameExtents.union(deviceConfig.getBounds());
    }
  }

  frameRate(30);
  size(virtualFrameExtents.width, virtualFrameExtents.height);
}

/**
 */
void maintainDisplay() {
  if (!locationAndFocusSet) {
    frame.setLocation(virtualFrameExtents.x, virtualFrameExtents.y);
    frame.setAlwaysOnTop(true);
    locationAndFocusSet = true;
  }
}

/**
 */
void parseArgs() {
  //
  // Windows .scr commandline options
  //
  for (int argIdx = 0; argIdx < args.length; argIdx++) {
    //javax.swing.JOptionPane.showMessageDialog(null, args[argIdx]);
    if (args[argIdx].equals("/c")) {
      exit();
    } 
    else if (args[argIdx].equals("/p")) {
      exit();
    } 
    else if (args[argIdx].equals("/s")) {
    } 
    else if (args[argIdx].equals("/a")) {
      exit();
    }
  }
}

/** 
 */
void setup() {  
  parseArgs();
  setupDisplay();

  PVector minExtent = new PVector(0, 0);
  PVector maxExtent = new PVector(virtualFrameExtents.width, virtualFrameExtents.height);

  spriteArena = new SpriteArena(minExtent, maxExtent);

  for (int spriteCount = 0; spriteCount < 50; spriteCount++) {      
    String imageName;
    if (spriteCount % 2 == 0) {
      imageName = "AFlyingToaster.gif";
    } 
    else {
      imageName = "APieceOfToast.gif";
    }

    PVector startMinExtent;
    PVector startMaxExtent;
    if (int(random(0, 3)) == 1) {
      startMinExtent = new PVector(maxExtent.x, 0);
      startMaxExtent = maxExtent;
    } 
    else {
      startMinExtent = minExtent;
      startMaxExtent = new PVector(maxExtent.x, 0);
    }

    float velocity = random(-10, -1);
    PVector negativeOneSlope = new PVector(velocity, abs(velocity));

    RandomVectorGenerator        randomPlacement              = new RandomVectorGenerator(startMinExtent, startMaxExtent);
    ConstantVectorGenerator      constantVelocity             = new ConstantVectorGenerator(negativeOneSlope);
    MotionVectorGenerator        linearMotion                 = new MotionVectorGenerator(constantVelocity);
    PositionedVectorGenerator    linearMotionWithRandomFrame  = new PositionedVectorGenerator(randomPlacement, linearMotion);

    Gif animatedGif = new Gif(this, imageName);
    animatedGif.loop();

    GifSprite toaster = new GifSprite(animatedGif, linearMotionWithRandomFrame);
    spriteArena.add(toaster);
  }
}

/**
 */
void keyPressed() {
  exit();
}

/**
 */
void mouseMoved() {
  if (ignoreFirstMouseMove) {
    ignoreFirstMouseMove = false;
  } 
  else {
    PVector movement = new PVector(mouseX, mouseY);
    movement.sub(lastMousePosition);
    if (movement.mag() > 10) {
      exit();
    }
  }
  lastMousePosition = new PVector(mouseX, mouseY);
}

/**
 */
void mouseClicked() {
  exit();
}

/**
 */
void draw() {
  maintainDisplay();

  background(0);
  spriteArena.draw();
}

