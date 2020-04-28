#ifndef MYDATA_HH
#define MYDATA_HH

#include <cstdint>
#include <vector>

struct MyData {
  using waveform_t = std::vector<uint32_t>;
  waveform_t a;
  waveform_t b;
};

#endif // MYDATA_HH

