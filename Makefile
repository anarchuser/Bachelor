# Simple Makefile providing an interface for cmake. Oh, sweet irony

BUILD_DIR = build

.PHONY = run all setup build test clean

# Clean, build, test, and run
all:	clean	test	run

# Initialise cmake build files at BUILD_DIR
setup:	CMakeLists.txt
	cmake -H. -B$(BUILD_DIR) > /dev/zero

# Build the project (lib and src)
build:	setup	generate.cpp
	cmake --build $(BUILD_DIR) --target Peer > /dev/zero
	g++ -o $(BUILD_DIR)/generate generate.cpp

# Build the benchmarking tool
run:	build
	GLOG_alsologtostderr=1 ./$(BUILD_DIR)/Peer

# Build and run the tests
router:	setup
	cmake --build $(BUILD_DIR) --target Router > /dev/zero
	GLOG_alsologtostderr=1 ./$(BUILD_DIR)/Router

# Build and run the tests
test:	build
	cmake --build $(BUILD_DIR) --target Test > /dev/zero
	GLOG_logtostderr=1 ./$(BUILD_DIR)/Test

# Clean all build files
clean:
	rm -rf $(BUILD_DIR)
