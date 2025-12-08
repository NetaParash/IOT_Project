class Screen{
  HardwareSerial& serial;
  public:
    Screen(HardwareSerial& s) : serial(s){};
  void setup() {
          // Start OLED
          display.begin(OLED_ADDR, true);
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(SH110X_WHITE);
          display.setCursor(0, 0);
  }    
  void print(string text){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(text);
    display.display();
  }
  
}