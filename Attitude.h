//struct to hold attitude data
struct Attitude {
  double pitch, roll, yaw;
  Attitude() {
    pitch = 0.0;
    roll = 0.0;
    yaw = 0.0;
  }
  Attitude(double p, double r, double y) {
    pitch = p;
    roll = r;
    yaw = y;
  }
};






