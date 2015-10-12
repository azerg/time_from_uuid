#include <iostream>
#include <time.h> 
#include <gtest/gtest.h>
#include <uuid_to_time.hpp>

#define EPOCH_DIFF 0x019DB1DED53E8000 /* 116444736000000000 nsecs */
#define RATE_DIFF 10000000 /* 100 nsecs */

/* Convert a Windows filetime_t into a UNIX time_t */
time_t fileTimeToUnixTime(uint64_t ftime){
  int64_t tconv = (ftime - EPOCH_DIFF) / RATE_DIFF;
  return (time_t)tconv;
}

TEST(uuidToTime, iexploreUuidTest){
  boost::uuids::string_generator strGen;
  boost::uuids::uuid uuid = strGen("{12486CD4-5890-11E5-9BC7-20689DBFB469}");

  auto convertedTime = ExtractTimeFromUUID(uuid);

  time_t rawtime = fileTimeToUnixTime(convertedTime);

  auto ptm = std::gmtime(&rawtime);
  EXPECT_EQ(2015, ptm->tm_year + 1900);
  EXPECT_EQ(9, ptm->tm_mon + 1);
  EXPECT_EQ(11, ptm->tm_mday);
  EXPECT_EQ(14, ptm->tm_hour);
  EXPECT_EQ(19, ptm->tm_min);
  EXPECT_EQ(10, ptm->tm_sec);
}

/*!
* We cant guess if uuid is valid or not, so function will try to process everything T_T
*/
TEST(uuidToTime, uninitializedUuidTest){
  EXPECT_NO_THROW({
    boost::uuids::uuid uuid;
    ExtractTimeFromUUID(uuid);
  });
  SUCCEED();
}