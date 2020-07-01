#include <chrono>
enum keyType { singular, continuous };

class key {
private:
  bool pressed;
  std::chrono::steady_clock::time_point inputTime;
  double inputDeadTime;
  int continuousRepeatCount;
  keyType type;

public:
  key(keyType myKeyType, double deadTime);
  bool keyPressed(std::chrono::steady_clock::time_point time);
  void keyUnpressed();
};
