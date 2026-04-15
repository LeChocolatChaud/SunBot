.PHONY: build

build:
	@arduino-cli compile --build-path build

upload:
	@arduino-cli upload --build-path build

setup:
	@arduino-cli compile --build-path build --only-compilation-database
