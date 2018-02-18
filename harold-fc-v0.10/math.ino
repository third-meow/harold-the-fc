
void doPID() {
  /*
     Calculate pitch error using PID
  */
  
  //find the difference between the gyro reading and the controller value and apply the P gain
  errorPitchP = (IMUData.orientation.x - reciverData.pitch) * pitchGain.P;
  
  //add the previous I error to the difference between gyro reading and controller value after applying I gain. Effectivly adding the amount of time not centered to the error
  errorPitchI = prvErrorPitchI + ((IMUData.orientation.x - reciverData.pitch) * pitchGain.I);
  
  //find the how fast the differnce between gyro reading and reciver value is changeing and applying D gain
  errorPitchD = ((IMUData.orientation.x - reciverData.pitch) - (prvGyroX - prvReciverPitch)) * pitchGain.D;

  //combine P, I & D
  errorPitch = errorPitchP + errorPitchI + errorPitchD;
  
  //set all the "previous" pitch values
  prvGyroX = IMUData.orientation.x;
  prvReciverPitch = reciverData.pitch;
  prvErrorPitchI = errorPitchI;



  /*
   * Calculate roll using PID
   */
  
  //find the difference between the gyro reading and the controller value and apply the P gain
  errorRollP = (IMUData.orientation.y - reciverData.roll) * rollGain.P;
  
  //add the previous I error to the difference between gyro reading and controller value after applying I gain. Effectivly adding the amount of time not centered to the error
  errorRollI = prvErrorRollI + ((IMUData.orientation.y - reciverData.roll) * rollGain.I);   
  
  //find the how fast the differnce between gyro reading and reciver value is changeing and applying D gain
  errorRollD = ((IMUData.orientation.y - reciverData.roll) - (prvGyroY - prvReciverRoll)) * rollGain.D;
  
  //combine P, I & D
  errorRoll = errorRollP + errorRollI + errorRollD;
  
  //set all the "previous" roll values
  prvGyroY = IMUData.orientation.y;
  prvReciverRoll = reciverData.roll;
  prvErrorRollI = errorRollI;

  
  
  /*
   * Calculate yaw using PID
   */
  
  //find the difference between the gyro reading and the controller value and apply the P gain
  errorYawP = (IMUData.orientation.z - reciverData.yaw) * yawGain.P;    
  
  //add the previous I error to the difference between gyro reading and controller value after applying I gain. Effectivly adding the amount of time not centered to the error
  errorYawI = prvErrorYawI + ((IMUData.orientation.z - reciverData.yaw) * yawGain.I);   
  
  //find the how fast the differnce between gyro reading and reciver value is changeing and applying D gain
  errorYawD = ((IMUData.orientation.z - reciverData.yaw) - (prvGyroZ - prvReciverYaw)) * yawGain.D;     
  
  //combine P, I & D
  errorYaw = errorYawP + errorYawI + errorYawD;
  
  //set all the "previous" values
  prvGyroZ = IMUData.orientation.z;
  prvReciverYaw = reciverData.yaw;
  prvErrorYawI = errorYawI;
}

