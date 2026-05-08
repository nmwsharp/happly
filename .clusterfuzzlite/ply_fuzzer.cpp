#include <fuzzer/FuzzedDataProvider.h>
#include <happly.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  char filename[256];
  sprintf(filename, "/tmp/libfuzzer.%d.ply", getpid());

  FILE* fp = fopen(filename, "wb");
  if (!fp) return 0;
  fwrite(data, size, 1, fp);
  fclose(fp);

  try {
    happly::PLYData ply_fuzz(filename);
  } catch (...) {
  }

  unlink(filename);

  return 0;
}