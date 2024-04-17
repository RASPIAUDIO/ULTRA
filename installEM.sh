#
sudo cp ultraEM /etc
sudo chmod 777 /etc/ultraEM
sudo rm /etc/rc.local
sudo cp rc.localEM /etc
sudo mv /etc/rc.localEM /etc/rc.local
sudo chmod 777 /etc/rc.local
sleep 2
sudo reboot

