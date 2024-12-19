#!/bin/bash

# Check if rc.local exists
if [ ! -f /etc/rc.local ]; then
  echo "rc.local not found. Creating rc.local with default content."
  sudo bash -c 'echo "#!/bin/sh -e" > /etc/rc.local'
  sudo bash -c 'echo "/etc/ultra" >> /etc/rc.local'
  sudo bash -c 'echo "exit 0" >> /etc/rc.local'
  sudo chmod +x /etc/rc.local
else
  # Add the execution of ultra to rc.local if not already present
  if ! grep -q "/etc/ultra" /etc/rc.local; then
    echo "Adding execution of ultra to rc.local."
    sudo sed -i '/^exit 0$/i /etc/ultra' /etc/rc.local
  fi
fi

# Copy the ultra file to /etc
sudo cp ultra /etc

# Change permissions of the ultra file to allow all users read, write, and execute access
sudo chmod 777 /etc/ultra

# Add necessary configurations to /boot/firmware/config.txt
if ! grep -q "dtoverlay=i2s-mmap" /boot/firmware/config.txt; then
  echo "Adding dtoverlay=i2s-mmap to /boot/firmware/config.txt"
  sudo bash -c 'echo "dtoverlay=i2s-mmap" >> /boot/firmware/config.txt'
fi

if ! grep -q "dtoverlay=googlevoicehat-soundcard" /boot/firmware/config.txt; then
  echo "Replacing dtparam=i2s=on with dtoverlay=googlevoicehat-soundcard in /boot/firmware/config.txt"
  sudo sed -i '/^dtparam=i2s=on$/d' /boot/firmware/config.txt
  sudo bash -c 'echo "dtoverlay=googlevoicehat-soundcard" >> /boot/firmware/config.txt'
fi

# Disable built-in audio if enabled in config.txt
if grep -q "^dtparam=audio=on" /boot/firmware/config.txt; then
  echo "Disabling built-in audio in /boot/firmware/config.txt"
  sudo sed -i 's/^dtparam=audio=on/#dtparam=audio=on/' /boot/firmware/config.txt
fi

# Enable I2C in Raspberry Pi configuration
echo "Please enable I2C in raspi-config. Navigate to Interface Options -> I2C."
read -p "Press Enter to launch raspi-config..."
sudo raspi-config

# Prompt to reboot
read -p "Changes require a reboot to take effect. Reboot now? (Y/n): " choice
case "$choice" in
  [Yy]* ) sudo reboot;;
  * ) echo "Reboot canceled. Please reboot manually to apply changes.";;
esac

# Print a message indicating that the script has run successfully
echo "Files have been copied and permissions set successfully."
