
void doPID() {
  /*
     Calculate pitch error using PID
  */
  errorPitchP = (IMUData.orientation.x - reciverData.pitch) * pitchGain.P;    //find the difference between the gyro reading and the controller value and apply the P gain
  errorPitchI = errorPitchPrvI + ((IMUData.orientation.x - reciverData.pitch) * pitchGain.I);   //add the previous I error to the difference between gyro reading and controller value after applying I gain. Effectivly adding the amount of time not centered to the error
  errorPitchD = ((IMUData.orientation.x - reciverData.pitch) - (prvGyroX - prvReciverPitch)) * pitchGain.D;   //find the how fast the differnce between gyro reading and reciver value is changeing and applying D gain
  errorPitch = errorPitchP + errorPitchI + errorPitchD;
  
  //set all the "previous" pitch values
  prvGyroX = IMUData.orientation.x;
  prvReciverPitch = reciverData.pitch;
  prvErrorPitchI = errorPitchI;

  /*
   * Calculate roll using PID
   */
  errorRollP = (IMUData.orientation.y - reciverData.roll) * gainP;    //find the difference between the gyro reading and the controller value and apply the P gain
  errorRollI = errorRollPrvI + ((IMUData.orientation.y - reciverData.roll) * gainI);   //add the previous I error to the difference between gyro reading and controller value after applying I gain. Effectivly adding the amount of time not centered to the error
  errorRollD = ((IMUData.orientation.y - reciverData.roll) - (prvGyroY - prvReciverRoll)) * gainD;   //find the how fast the differnce between gyro reading and reciver value is changeing and applying D gain
  errorRoll = errorRollP + errorRollI + errorRollD;
  
  //set all the "previous" roll values
  prvGyroY = IMUData.orientation.y;
  prvReciverRoll = reciverData.roll;
  prvErrorRollI = errorRollI;

  /*
   * Calculate yaw using PID
   */
  errorYawP = (IMUData.orientation.z - reciverData.yaw) * gainP;    //find the difference between the gyro reading and the controller value and apply the P gain
  errorYawI = errorYawPrvI + ((IMUData.orientation.z - reciverData.yaw) * gainI);   //add the previous I error to the difference between gyro reading and controller value after applying I gain. Effectivly adding the amount of time not centered to the error
  errorYawD = ((IMUData.orientation.z - reciverData.yaw) - (prvGyroZ - prvReciverYaw)) * gainD;   //find the how fast the differnce between gyro reading and reciver value is changeing and applying D gain
  errorYaw = errorYawP + errorYawI + errorYawD;
  
  //set all the "previous" values
  prvGyroZ = IMUData.orientation.z;
  prvReciverYaw = reciverData.yaw;
  prvErrorYawI = errorYawI;
}

