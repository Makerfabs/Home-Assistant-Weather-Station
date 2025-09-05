# Home Assistant & Matouch Ai 2.8 Board Display Configuration Guide

**Version**: 1.0

**Author**: Makerfabs

**Last Updated**: 2025-09-02

## I. Overview

This document is an advanced configuration guide that explains how to configure the Matouch Ai 2.8 Board to fetch and display sensor data from the Weather Station via the Home Assistant API.

Before you begin, please ensure that you have successfully completed all of the following basic configurations:

- [Guide to Installing Home Assistant and ESPHome with Docker](https://github.com/Makerfabs/markdown/blob/main/Home_Assistant_ESPHome_Docker_Installation.md)
- [Home Assistant Weather Station Configuration Guide](https://github.com/Makerfabs/Home-Assistant-Weather-Station/blob/main/README.md)
- [Basic Usage of ESPHome and Home Assistant](https://github.com/Makerfabs/markdown/blob/main/Using_ESPHome_Home_Assistant.md)

## II. Prerequisites

- **Hardware**
  - A Raspberry Pi with Home Assistant and ESPHome services running stably.
  - A Weather Station device that has been successfully configured and connected to Home Assistant.
  - Makerfabs Matouch Ai 2.8 Board.
  - Two USB-C cables.
- **Software**
  - A modern web browser (Chrome or Edge recommended).
  - ESPHome version 2025.7.x or higher.
  - An SSH client (e.g., PuTTY, Termius, or the system's built-in terminal).

## III. Configuration Steps

> **Important Note**: The following steps are also based on your familiarity with the [basic operations of ESPHome](https://github.com/Makerfabs/markdown/blob/main/Using_ESPHome_Home_Assistant.md). This guide only lists the special configuration requirements for the Matouch Ai 2.8 Board.

### Step 1: Create a New Device

- In the ESPHome web interface, create a new device. When prompted to select a device type, be sure to choose **ESP32-S3**.

### Step 2: Upload Font Files

1. Download the complete project files from the [Home-Assistant-Weather-Station GitHub repository](https://github.com/Makerfabs/Home-Assistant-Weather-Station).

2. Find and navigate to the `example/Matouch28` folder, where you will see several font files ending in `.ttf`.

3. Use the `scp` command to upload **all** of these `.ttf` font files to the directory you created for ESPHome.

   > **Tip**: The font files should be uploaded to the `/home/pi/docker/esphome/config/fonts/` directory. Please adjust the path according to your actual setup.

   ```
   # Example: Transfer a single font file to the specified directory on the Raspberry Pi
   # You will need to repeat this for each .ttf file
   scp <path_to_local_font_file>/font.ttf <your_username>@<your_raspberry_pi_ip>:<remote_path>
   ```

### Step 3: Update the Device YAML Configuration

1. In the downloaded repository, locate the `example/Matouch28/MatouchAi28.yaml` file.
2. Open the YAML configuration file you created for the Matouch device in **Step 1**.
3. **Completely copy and overwrite** the contents of your device's YAML file with all the configuration content from `MatouchAi28.yaml`. This configuration contains all the necessary code to drive the screen, call the API, and display the data.

### Step 4: Complete Installation and Integration

After completing the above configurations, please return to and continue following the subsequent steps in the [Basic Usage of ESPHome and Home Assistant](https://github.com/Makerfabs/markdown/blob/main/Using_ESPHome_Home_Assistant.md) document to compile and upload the firmware. Once successful, your Matouch screen will automatically begin to fetch and display weather station data from Home Assistant.