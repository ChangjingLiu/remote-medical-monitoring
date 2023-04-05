
#define MAX30205_ADDRESS1        0x49  // 8bit address converted to 7bit
#define MAX30205_ADDRESS2        0x48  // 8bit address converted to 7bit
#define MAX30205_ADDRESS3        0x4C  // 8bit address converted to 7bit//后期增加的


// Registers
#define MAX30205_TEMPERATURE    0x00  //  get temperature ,Read only
#define MAX30205_CONFIGURATION  0x01  //
#define MAX30205_THYST          0x02  //
#define MAX30205_TOS            0x03  //



typedef unsigned char uint8_t;

typedef enum{   	//For configuration registers
  SHUTDOWN,    		// shutdwon mode to reduce power consumption <3.5uA
  COMPARATOR,    	// Bit 0 = operate OS in comparator mode, 1= INTERRUPT MODE
  OS_POLARITY,    	// Polarity bit ;Bit 0 = Active low output, Bit 1 = Active high
  FAULT_QUEUE_0,    // Fault indication bits
  FAULT_QUEUE_1,    // Fault indication bits
  DATA_FORMAT,      //Data Format
  TIME_OUT,          //Time out
  ONE_SHOT           //1= One shot, 0 = Continuos
}configuration;


class MAX30205 {
public:

  float temperature = 0;      // Last temperature
  uint8_t sensorAddress = MAX30205_ADDRESS1;
  void  shutdown(void);   // Instructs device to power-save
  void  printRegisters(void); // Dumps contents of registers for debug
  void begin(void);
  void Initial(void);
  float getTemperature(void);
  bool scanAvailableSensors(void);

private:
  void    I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data);
  uint8_t I2CreadByte(uint8_t address, uint8_t subAddress);
  void    I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);
};
