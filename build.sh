set -e

BUILD_DIR=build

echo "Configuring project..."
cmake -S . -B ${BUILD_DIR}

echo "Building project..."
cmake --build ${BUILD_DIR}

echo "Done."
