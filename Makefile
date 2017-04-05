ARDUINO_PATH = /home/rafaame/Applications/arduino-studio
SKETCHBOOK   = /home/rafaame/Applications/arduino-studio
SKETCH       = src/main.cpp
TARGET_DIR   = $(CURDIR)/build
MONITOR_PORT = /dev/ttyACM0

monitor: flash
	rm -rf arduino.log
	gnome-terminal -x bash -c "screen -L ./arduino.log $(MONITOR_PORT) 115200"

flash: compile
	if pgrep screen &> /dev/null ; then killall screen ; fi

	$(ARDUINO_PATH)/hardware/tools/avr/bin/avrdude \
	-C$(ARDUINO_PATH)/hardware/tools/avr/etc/avrdude.conf \
	-v \
	-patmega2560 \
	-cwiring \
	-P$(MONITOR_PORT) \
	-b115200 \
	-D \
	-Uflash:w:$(TARGET_DIR)/main.cpp.hex:i

compile:
	@ mkdir -p $(TARGET_DIR)

	$(ARDUINO_PATH)/arduino-builder -compile -logger=machine \
	-hardware "$(ARDUINO_PATH)/hardware" \
	-hardware "$(SKETCHBOOK)/hardware" \
	-tools "$(ARDUINO_PATH)/tools-builder" \
	-tools "$(ARDUINO_PATH)/hardware/tools/avr" \
	-built-in-libraries "$(ARDUINO_PATH)/libraries" \
	-libraries "$(SKETCHBOOK)/libraries" \
	-libraries "./" \
	-fqbn=arduino:avr:mega:cpu=atmega2560 \
	-ide-version=10612 \
	-build-path "$(TARGET_DIR)" \
	-warnings=all \
	-prefs=build.warn_data_percentage=75 \
	-verbose "$(SKETCH)"

clean:
	rm -rf $(TARGET_DIR)/sketch/*
	rm -rf $(TARGET_DIR)/$(SKETCH).*