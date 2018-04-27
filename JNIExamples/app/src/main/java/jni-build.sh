#!/bin/bash
# This script builds and generates the necessary jni wrappers/native libraries.
# Note: Must have SWIG and Android NDK packages installed.
# WARNING: This only works with android ndk r14b.
cd src/main/java

if test -d "../jniLibs/";
then echo "../jniLibs/";
else mkdir "../jniLibs/";
fi

if test -f "ndk_path.txt";
then echo "NDK path file exists."
else {
    echo "NDK path file does not exists."
    cat > "ndk_path.txt"
}
fi

NDK_PATH=$(cat "ndk_path.txt")

$NDK_PATH/ndk-build NDK_LIBS_OUT=../jniLibs/
