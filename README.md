# Deepin Graphics Driver Manager

## Build & Install
### Build from source
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr .. # Install to /usr
make # Add -jx for parallel
```

### Install
```
sudo make install
sudo systemctl daemon-reload # refresh systemd
sudo systemctl enable driver-installer.service # enable installer service
sudo systemctl start driver-installer.service # start installer service
```