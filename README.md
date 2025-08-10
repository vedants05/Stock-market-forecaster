# Trading-simulator
Trading simulator developed in C++

## How to Build and Run `api.cpp`

### Prerequisites
- CMake (version 3.18 or higher)
- vcpkg (for dependency management)
- CPR and nlohmann_json libraries (installed via vcpkg)

### Steps
1. **Install dependencies** (if not already):
	 - Make sure `cpr` and `nlohmann_json` are installed using vcpkg.
	 - Example:
		 ```bash
		 vcpkg install cpr nlohmann-json
		 ```

2. **Build and run using the provided script:**
	 - Open a terminal in the project root directory.
	 - Run:
		 ```bash
		 ./build_and_run.sh
		 ```
	 - This will configure, build, and run the executable (`trader.exe`).

3. **Manual build (optional):**
	 - Configure with CMake:
		 ```bash
		 cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake"
		 ```
	 - Build:
		 ```bash
		 cmake --build build
		 ```
	 - Run:
		 ```bash
		 ./build/Debug/trader.exe
		 ```

### Notes
- The API key in `api.cpp` is for demonstration. Replace it with your own key for production use.