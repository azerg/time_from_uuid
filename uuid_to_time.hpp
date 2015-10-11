#include <cstdint>
#include <vector>
#include <assert.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp> // to lexical_cast uuids
#include <boost/uuid/string_generator.hpp> // to read uuid from string

/*!
* Function extracts time_t from boost::uuid
*
* Description: Boost::uuid (instead of python binding) is not able to process time-based uuids.
*
* @param [in] sourceUuid Valid boost::uuid
* @return uint64_t filetime value
*/
uint64_t ExtractTimeFromUUID(boost::uuids::uuid& sourceUuid){
  // just a list of byte's positions in right order to get time value
  static std::vector<uint8_t> bytesOrder{6,7,4,5,0,1,2,3};

  uint64_t timeVal = 0;
  // reading byte-per-byte from boost::uuid data to the lower byte of uint64_t @timeVal
  // Then shl,8 @timeVal.
  // Repeat previous steps until higher byte of @timeVal is filled. So stop when it is and clear flag bit.
  // Example: 
  // 6E5969C8-588E-11E5-9BC7-20689DBFB469 --> 0x01e5 588e 6e5969c8
  std::for_each(bytesOrder.begin(), bytesOrder.end(), [&sourceUuid, &timeVal](uint8_t& byte){
    assert(byte < boost::uuids::uuid::static_size() && byte >= 0); // dummy overflow check
    timeVal |= sourceUuid.data[byte];
    // Check if higher byte is already processed. 
    // If so, do not shift left not to lose data
    auto isHigherByteNotNull = timeVal & 0xFF00000000000000;
    if (isHigherByteNotNull != 0){
      // Time is not signed, so higher bit mush be cleared to sign positive value
      timeVal &= 0x0FFFFFFFFFFFFFFF;
    }
    else{
      // shift lower byte position to reserve place for the next byte
      timeVal <<= 8;
    }
  });

  /* NT keeps time in FILETIME format which is 100ns ticks since
  Jan 1, 1601. UUIDs use time in 100ns ticks since Oct 15, 1582.
  The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
  + 18 years and 5 leap days. */
  static uint64_t fileTimeTimeShift =
    static_cast<uint64_t>(1000 * 1000 * 10) // seconds
    * (60 * 60 * 24)                        // days
    * (17 + 30 + 31 + 365 * 18 + 5);        // # of days;

  return timeVal - fileTimeTimeShift;
}