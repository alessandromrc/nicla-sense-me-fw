#include "DFUManager.h"

// half the flash (512KB) is dedicated as dfu temporary storage
FlashIAPBlockDevice DFUManager::_bd(0x80000, 0x80000);
mbed::LittleFileSystem DFUManager::_fs("fs");

DFUManager::DFUManager() :
  _target(NULL)
{
}

DFUManager::~DFUManager()
{
}

void DFUManager::begin()
{
  int err = _fs.mount(&_bd);
  if (err) {
    err = _fs.reformat(&_bd);
  }
}

void DFUManager::processPacket(DFUType dfuType, const uint8_t* data)
{
  DFUPacket* packet = (DFUPacket*)data;
  if (packet->index == 0) {
    if (dfuType == DFU_INTERNAL) {
      _target = fopen("/fs/UPDATE.BIN", "wb");
    } else {
      _target = fopen("/fs/BH104.BIN", "wb");
    }
  }
  if (_target != NULL) {
    fwrite(packet->data, packet->last ? packet->remaining : sizeof(packet->data), 1, _target);
  }
  if (packet->last) {
    fclose(_target);
    _target = NULL;
    if (dfuType == DFU_INTERNAL) {
      // reboot
    } else {
      //apply bosch update
    }
  }
}

DFUManager dfuManager;
