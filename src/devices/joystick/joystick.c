#include "joystick.h"

#include <mcp23017.h>
#include <specifications/input_data.h>
#include <types.h>

#include "esp_log.h"

#define I2C_SDA 48
#define I2C_SCL 47
#define MCP_ADDRESS 0x20

static bool joystickEnabled = false;
static _u16 joystickData = 0;

static DeviceSpecification specs = {
    .name = "Joystick",
    .type = TypeInput,
};

static MCPDevice_t mcp23017 = {
    .portNumber = I2C_NUM_0,
    .sdaPin = I2C_SDA,
    .sclPin = I2C_SCL,
    .address = MCP_ADDRESS,
};

static InputDeviceData deviceData;

static bool onInit(void) {
  bool initOk = MCPInit(&mcp23017);

  if (initOk == true) {
    initOk = MCPWriteConfig(&mcp23017, MCP_REG_IOCONA, 1 << 5);

    // config as input
    initOk &= MCPWriteConfig(&mcp23017, MCP_REG_IODIRA, 0xff);
    initOk &= MCPWriteConfig(&mcp23017, MCP_REG_IODIRB, 0xff);

    // add pullups
    initOk &= MCPWriteConfig(&mcp23017, MCP_REG_GPPUA, 0xff);
    initOk &= MCPWriteConfig(&mcp23017, MCP_REG_GPPUB, 0xff);

    // set polarity
    initOk &= MCPWriteConfig(&mcp23017, MCP_REG_IPOLA, 0xff);
    initOk &= MCPWriteConfig(&mcp23017, MCP_REG_IPOLB, 0xff);
  }

  return initOk;
}

static bool onEnable(bool enable) {
  ESP_LOGI(specs.name, "enable: %d", enable);
  joystickEnabled = enable;
  return true;
}

static void onUpdate(void) {
  if (joystickEnabled == true) {
    // save old keymap
    deviceData.previousKeymap = joystickData;
    // fetch new
    MCPReadAllPorts(&mcp23017, &joystickData);
    // and update specification object
    deviceData.keymap = joystickData;
  }
}

DeviceSpecification* JoystickSpecification() {
  specs.data = &deviceData;

  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;

  return &specs;
}