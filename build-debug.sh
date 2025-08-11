set -e

cd build
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_C_FLAGS="-g -O0 -fno-omit-frame-pointer -fsanitize=address" \
      -DCMAKE_CXX_FLAGS="-g -O0 -fno-omit-frame-pointer -fsanitize=address" \
      ..
make -j"$(nproc)"
