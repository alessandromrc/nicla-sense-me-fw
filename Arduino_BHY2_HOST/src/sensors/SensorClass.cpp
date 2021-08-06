#include "SensorClass.h"
#include "SensorManager.h"
#include "Arduino_BHY2_HOST.h"
#include "EslovHandler.h"

SensorClass::SensorClass() : 
  _id(0),
  _subscribed(false)
{
}

SensorClass::SensorClass(uint8_t id) : 
  _id(id),
  _subscribed(false)
{
}

SensorClass::~SensorClass()
{
  disable();
}

uint8_t SensorClass::id() 
{
  return _id;
}

void SensorClass::configure(float rate, uint32_t latency, CommunicationProtocol protocol)
{
  SensorConfigurationPacket config {_id, rate, latency};

  if (protocol == ESLOV) {
    eslovHandler.toggleEslovIntPin();
    uint8_t ack = 0;
    while(ack != 15) {
      BHY2_HOST.configureSensor(config);
      ack = BHY2_HOST.requestAck();
    }
  } else {
      BHY2_HOST.configureSensor(config);
  }

  if (rate == 0 && _subscribed) {
    // unregister sensor
    sensorManager.unsubscribe(this);
    _subscribed = false;
  } else if (rate > 0 && !_subscribed) {
    // register sensor
    sensorManager.subscribe(this);
    _subscribed = true;
  }

}

void SensorClass::disable()
{
  configure(0, 0);
}
