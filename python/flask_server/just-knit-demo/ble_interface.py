import gatt, sys, os, time

def decodeQuat(inBuffer):
    for i in range(0,3):
        outBuffer[i] = (inBuffer[(2*i)+1] << 8) | inBuffer[(2*i)]
    return outBuffer


class BLEDevice(gatt.Device):
    def connect_succeeded(self):
        super().connect_succeeded()
        print("[%s] Connected" % (self.mac_address))

    def connect_failed(self, error):
        super().connect_failed(error)
        print("[%s] Connection failed: %s" % (self.mac_address, str(error)))

    def disconnect_succeeded(self):
        super().disconnect_succeeded()
        print("[%s] Disconnected" % (self.mac_address))

    def characteristic_enable_notifications_succeeded(self, characteristic):
        pass

    def characteristic_enable_notifications_faied(self, characteristic, error):
        pass

    def characteristic_write_value_failed(self, characteristic, error):
        print("Write failed on ", characteristic)
        print("Error: ",error)

    def characteristic_write_value_succeeded(self, characteristic):
        pass

    def services_resolved(self):
        super().services_resolved()

        self.development_service = next(
            s for s in self.services
            if s.uuid == 'df701400-8dce-5092-6d4a-b6203dafe693')

        self.quat_characteristic = next(
            c for c in self.development_service.characteristics
            if c.uuid == 'df701401-8dce-5092-6d4a-b6203dafe693')

        self.fb_char = next(
            c for c in self.development_service.characteristics
            if c.uuid == 'df701402-8dce-5092-6d4a-b6203dafe693')

        #self.quat_characteristic.enable_notifications()
        #circBufferIndex = 0
        self.request_knit()

    def characteristic_value_updated(self, characteristic, value):
        try:
            inBuffer = bytes(self.quat_characteristic.read_value())
            
            circBuffer[circBufferIndex] = decodeQuat;

        except TypeError:
            os.execl(sys.executable, sys.executable, *sys.argv)

        print("Received:", value)

    def request_knit(self):
        try:
            self.fb_char.write_value(b'\x09')
            print("Requested Knit")
        except: 
            print("Request Knit Failed")
            return

    def request_purl(self):
        try:
            self.fb_char.write_value(b'\x0A')
            print("Requested Purl")
        except:
            print("Request Purl Failed")
            return

    def detect_knit(self):
        try:
            self.fb_char.write_value(b'\x0B')
            time.sleep(0.2)
            self.fb_char.write_value(b'\x0D')
            time.sleep(0.2)
        except:
            return

    def detect_purl(self):
        try:
            self.fb_char.write_value(b'\x0C')
            time.sleep(0.2)
            self.fb_char.write_value(b'\x0D')
            time.sleep(0.2)
        except:
            return


def main():
    try:
        manager = gatt.DeviceManager(adapter_name='hci0')
        device = BLEDevice(mac_address='F0:35:18:13:81:EC', manager=manager)
        # TODO Get list of stitches
        device.connect()
        manager.run()
        # Loop all of this
            # TODO Send current list item
            # TODO Build buffer of quat data
            # TODO Send quat data to matlab
            # TODO Send MATLAB response to device
            # TODO If reponse matches list go to next
    finally:
        if device.is_connected():
            device.disconnect()
    
if __name__ == '__main__':
    main()
