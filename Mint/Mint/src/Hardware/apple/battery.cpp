// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>

#include "../hwinfo/platform.h"

#ifdef HWINFO_APPLE

#include "../hwinfo/battery.h"

namespace hwinfo {

// =====================================================================================================================
// _____________________________________________________________________________________________________________________
std::string Battery::getVendor() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
std::string Battery::getModel() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
std::string Battery::getSerialNumber() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
std::string Battery::getTechnology() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
uint32_t Battery::getEnergyFull() const { return 0; }

// _____________________________________________________________________________________________________________________
uint32_t Battery::energyNow() const { return 0; }

// _____________________________________________________________________________________________________________________
bool Battery::charging() const { return false; }

// _____________________________________________________________________________________________________________________
bool Battery::discharging() const { return false; }

// =====================================================================================================================
// _____________________________________________________________________________________________________________________
std::vector<Battery> getAllBatteries() {
  // TODO: implement
  return {};
}

}  // namespace hwinfo

#endif