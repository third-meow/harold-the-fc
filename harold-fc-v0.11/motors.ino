void sendMotors() {
  forLeft = map((-errorPitch + errorRoll - errorYaw + reciverData.throttle), MINERRORS, MAXERRORS, MINMOTOR, MAXMOTOR);
  forRight = map((-errorPitch - errorRoll + errorYaw + reciverData.throttle), MINERRORS, MAXERRORS, MINMOTOR, MAXMOTOR);
  backLeft = map((+errorPitch + errorRoll + errorYaw + reciverData.throttle), MINERRORS, MAXERRORS, MINMOTOR, MAXMOTOR);
  backRight = map((+errorPitch - errorRoll - errorYaw + reciverData.throttle), MINERRORS, MAXERRORS, MINMOTOR, MAXMOTOR);
}

