from ble_interface import BLEDevice, decodeQuat
from threading import Thread
import gatt, sys, os, time

manager = gatt.DeviceManager(adapter_name='hci0')
device = BLEDevice(mac_address='F0:35:18:13:81:EC', manager=manager)


def ble_thread():
    manager.run()
        
def main():
    print("Starting")
    try:
        print("Creating thread")
        thread = Thread(target=ble_thread)
        print("Starting thread")
        thread.start() 
        device.connect()
        while True:
            try:
                detected = input("Enter stitch:")
            except KeyboardInterrupt:
                device.disconnect()
                manager.stop()
                thread.join()
                sys.exit()
            if detected == 'K':
                device.detect_knit()
            if detected == 'P':
                device.detect_purl()
    finally:
        print("Closing")
        if device.is_connected():
            device.disconnect()

if __name__ == '__main__':
    main()
