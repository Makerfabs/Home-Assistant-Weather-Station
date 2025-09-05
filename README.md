# Home Assistant Weather Station Configuration Guide

**Version**: 1.1

**Author**: Makerfabs

**Last Updated**: 2025-09-02

## I. Overview

This document provides detailed instructions on how to connect the Makerfabs Weather Station device to your deployed Home Assistant environment.

Before you begin, it is assumed that you have successfully completed the basic environment setup. If needed, please refer to the following documents:

- [How to Install Raspberry Pi OS](https://raspberrytips.com/install-raspberry-pi-os/)
- [How to Install Docker on Raspberry Pi](https://itsfoss.com/raspberry-pi-install-docker/)
- [Guide to Installing Home Assistant and ESPHome with Docker](https://github.com/Makerfabs/markdown/blob/main/Home_Assistant_ESPHome_Docker_Installation.md)
- [Basic Usage of ESPHome and Home Assistant](https://github.com/Makerfabs/markdown/blob/main/Using_ESPHome_Home_Assistant.md)

## II. Prerequisites

Before proceeding with the configuration, please ensure you have the following hardware and software ready:

- **Hardware**
  - A Raspberry Pi with the Docker environment successfully installed.
  - Makerfabs Weather Station device.
  - A USB-C cable.
- **Software**
  - A modern web browser (Chrome or Edge recommended).
  - ESPHome version 2025.7.x or higher.
  - An SSH client (e.g., PuTTY, Termius, or the system's built-in terminal).

## III. Device Configuration Steps

> **Important Note**: The following steps are based on the assumption that you are already familiar with the **"Creating a New Device"** section in the [Basic Usage of ESPHome and Home Assistant](https://github.com/Makerfabs/markdown/blob/main/Using_ESPHome_Home_Assistant.md) guide. This guide only lists the specific operations for the Weather Station product.

### Step 1: Create an ESPHome Device

- During the **"Creating a New Device"** step, when prompted to select a device type, be sure to choose **ESP32**.

### Step 2: Upload Custom Components

1. Before proceeding with the **"Configure & Install"** step, you must first upload the `common_components` folder provided by Makerfabs to your Raspberry Pi.

2. You can download this folder from the [Home Assistant Weather Station GitHub repository](https://github.com/Makerfabs/Home-Assistant-Weather-Station).

3. Use the `scp` command to upload the folder to the ESPHome configuration mapping directory.

   > **Tip**: Please adjust the mapping directory according to your actual setup.

   ```
   # Example command: Transfer the local common_components folder to the Raspberry Pi
   scp -r <path_to_local_common_components_folder> <your_username>@<your_raspberry_pi_ip>:<remote_path>
   ```

4. After the upload is successful, please verify that your directory structure is as follows:

   ```
   /home/pi/docker/esphome/config/
   ├── your_weather_station.yaml
   ├── common_components/
   │   └── ssap10/
   │       ├── ssap10.h
   │       ├── sensor.c
   │       ├── sensor.py
   │       └── __init__.py
   └── ... (other configuration files)
   ```

### Step 3: Update the Device YAML Configuration

1. Obtain the content of the `weather_station.yaml` file from the [Home Assistant Weather Station GitHub repository](https://github.com/Makerfabs/Home-Assistant-Weather-Station).
2. Open the YAML file for the device you created (e.g., `your_weather_station.yaml`).
3. **Overwrite** or **merge** all the configuration content from `weather_station.yaml` into your own device's YAML file.

### Step 4: Complete Installation and Integration

After completing the above configurations, please return to and continue following the subsequent steps in the [Basic Usage of ESPHome and Home Assistant](https://github.com/Makerfabs/markdown/blob/main/Using_ESPHome_Home_Assistant.md) document to compile, upload the firmware, and integrate the device in Home Assistant.