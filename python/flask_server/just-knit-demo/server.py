#!/usr/bin/env python
from threading import Thread, Lock
from flask import Flask, render_template, session, request
from flask_socketio import SocketIO, emit, disconnect
from flask_bootstrap import Bootstrap
from random import random
from knitting import Pattern, Stitch
from ble_interface import BLEDevice, decodeQuat
import gatt, sys, os, time

# Set this variable to "threading", "eventlet" or "gevent" to test the
# different async modes, or leave it set to None for the application to choose
# the best option based on installed packages.
# Personally, I install and use eventlet
async_mode = None

app = Flask(__name__)
Bootstrap(app)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None
thread_lock = Lock()
# TODO get pattern from POST request and parse iwth regex

_pattern = Pattern(\
'R:K2,P1,K1,P1,K1,P1,K1,P1,K6\
R:P6,P1,K1,P1,K1,P1,K1,P1,K1\
R:K1,P1,K1,P1,K1,P1,K4,P1,K3\
R:P3,K1,P4,K1,P1,K1,P1,K1,P1')

#TODO create bluetooth manager
manager = gatt.DeviceManager(adapter_name='hci0')
device = BLEDevice(mac_address='F0:35:18:13:81:EC', manager=manager)


#TODO Import gatt functions

# The websocket is maintained in the background, and this
# function outputs a random number every 5 seconds
def background_thread():
    """Example of how to send server generated events to clients."""
    count = 0
    while True:
        #TODO in thread send quat buffer to matlab
        #TODO set counter to check when to send buffer
        #TODO if matlab detects then increment pattern and send
        socketio.sleep(0.1)
        try:
            detected = input("Enter stitch:")
        except KeyboardInterrupt:
            device.disconnect()
            thread.join()
            ble_thread.join()
            manager.stop()
            sys.exit()
        if detected == 'K':
            device.detect_knit()
        if detected == 'P':
            device.detect_purl()
        if detected == _pattern.currentStitch.stitchType:
            if not _pattern.is_finished():
                _pattern.nextStitch()
                if _pattern.currentStitch.stitchType == 'K':
                    print("Requesting Knit")
                    #device.fb_char.write_value(0x09)
                    device.request_knit()
                if _pattern.currentStitch.stitchType == 'P':
                    print("Requesting Purl")
                    device.request_purl()
                json = _pattern.getInstructionBuffer()
                socketio.emit('my_response', json, namespace='/test')
        # Update request after false stitch
        elif detected == 'K':
            device.request_purl()
        elif detected == 'P':
            device.request_knit()

# Route for serving up the index page
@app.route('/')
def index():
    return render_template('index.html', async_mode=socketio.async_mode)

# This function is called when a web browser connects
@socketio.on('connect', namespace='/test')
def test_connect():
    global thread
    device.connect()
    global ble_thread 
    ble_thread = Thread(target=manager.run)
    ble_thread.start()
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(target=background_thread)
        #TODO Send initial pattern as above
        #TODO start up bluetooth and connect
        #TODO Add status message for successful connect/fail
    json = _pattern.getInstructionBuffer()
    socketio.emit('my_response', json, namespace='/test')

# Adjust stitch from web app 
@socketio.on('set_stitch', namespace='/test')
def set_stitch(instructionMoves):
    # Send detect signal for old stitch
    if(_pattern.currentStitch.stitchType == 'K'):
        device.detect_knit()
    if(_pattern.currentStitch.stitchType == 'P'):
        device.detect_purl()

    # Update stitch
    _pattern.setStitch(instructionMoves)

    # Request new stitch
    if(_pattern.currentStitch.stitchType == 'K'):
        device.request_knit()
    if(_pattern.currentStitch.stitchType == 'P'):
        device.request_purl()

    # Update webapp    
    json = _pattern.getInstructionBuffer()
    socketio.emit('my_response', json, namespace='/test')
    socketio.sleep(0.1)



# Ping-pong allows Javascript in the web page to calculate the
# connection latency, averaging over time
@socketio.on('my_ping', namespace='/test')
def ping_pong():
    emit('my_pong')

# Notification that a client has disconnected
@socketio.on('disconnect', namespace='/test')
def test_disconnect():
    print('Client disconnected', request.sid)
    if device.is_connected():
        device.disconnect()

# Run the web app
if __name__ == '__main__':
    try:
        socketio.run(app, debug=False, host='0.0.0.0')

    finally:
        if device.is_connected():
            device.disconnect()
