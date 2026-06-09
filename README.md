# Servo Backpack

Host-side library and examples for controlling the Teensy-Servo backpack over USB serial. The Teensy 4.0 runs the firmware in [arduino/](arduino/) and exposes a simple 1-byte protocol: send `0` to drive all servos to their OPEN positions, send `1` to drive them to LOCKED.

Hardware: 3–4× Savox SV1261MG servos driven by a Teensy 4.0 at 9600 baud.

## Project layout

```
arduino/             PlatformIO project — firmware for the Teensy 4.0
src/                 Host-side library
  servo_comm.hpp/cpp   C++ implementation
  servo_comm.py        Python implementation
examples/
  cpp/               C++ example programs (built via CMake)
  python/            Python example scripts
CMakeLists.txt       Builds the C++ examples
pyproject.toml       Installable Python package (servo-backpack)
```

## Installing

### Arduino firmware

Open [arduino/](arduino/) in PlatformIO and upload to the Teensy 4.0.

### C++

```bash
mkdir build
cd build
cmake ..
make
```

This produces two executables in `build/`:
- `build/manual` — interactive `0`/`1`/`q` prompt
- `build/comms_example` — automated demo cycling OPEN/LOCKED

### Python

Install the `servo-backpack` package in editable mode so `import servo_comm` works from anywhere:

```bash
pip install --no-build-isolation -e .
```

The `--no-build-isolation` flag works around an issue with older versions of pip (≤22.x) that provision a too-old setuptools during isolated builds. If your pip is newer (`pip install --upgrade pip` to be sure), plain `pip install -e .` works.

Verify:

```bash
pip show servo-backpack
python3 -c "from servo_comm import ServoComm; print('OK')"
```

## Serial port permissions

By default, `/dev/ttyACM*` on Linux is owned by the `dialout` group, so accessing it requires either being in that group or using `sudo`.

### Recommended: add yourself to the `dialout` group (one-time)

```bash
sudo usermod -a -G dialout $USER
```

Log out and back in (or reboot) for the change to take effect. After that, you can run all examples without `sudo`.

### Alternative: use `sudo`

`sudo` works, but **root does not see Python packages installed under your user**, so `sudo python3 examples/python/example.py` will fail with `ModuleNotFoundError: No module named 'servo_comm'` after a user-level `pip install -e .`.

If you need to run with `sudo`, either install the package for root as well:

```bash
sudo pip install --no-build-isolation -e .
```

…or pass your user site-packages through explicitly:

```bash
sudo PYTHONPATH=$HOME/.local/lib/python3.10/site-packages python3 examples/python/example.py
```

The `dialout` group approach is cleaner and avoids both issues.

## Running the examples

Find the Teensy's serial port:

```bash
ls /dev/ttyACM*
```

### Recommended: no `sudo` (after joining the `dialout` group)

```bash
# C++
./build/comms_example                              # automated demo, port hardcoded to /dev/ttyACM0
./build/manual /dev/ttyACM0                        # interactive

# Python
python3 examples/python/example.py                 # automated demo, port hardcoded to /dev/ttyACM0
python3 examples/python/manual.py /dev/ttyACM0     # interactive
```

### Alternative: with `sudo`

If you haven't joined the `dialout` group, prefix the C++ commands with `sudo`:

```bash
sudo ./build/comms_example
sudo ./build/manual /dev/ttyACM0
```

For Python, `sudo` doesn't see your user-level `pip install -e .`, so you also need to either install for root (`sudo pip install --no-build-isolation -e .`) or pass `PYTHONPATH`:

```bash
sudo PYTHONPATH=$HOME/.local/lib/python3.10/site-packages python3 examples/python/example.py
sudo PYTHONPATH=$HOME/.local/lib/python3.10/site-packages python3 examples/python/manual.py /dev/ttyACM0
```

## Protocol

Single-byte commands sent at 9600 8N1:

| Byte | Meaning                            |
|------|------------------------------------|
| `0`  | Drive all servos to OPEN position  |
| `1`  | Drive all servos to LOCKED position|

OPEN and LOCKED angles per servo are defined in [arduino/src/main.cpp](arduino/src/main.cpp).
