#!/usr/bin/env python
from threading import Lock
from flask import Flask, render_template, session, request
from flask_socketio import SocketIO, emit, disconnect
from flask_bootstrap import Bootstrap
from random import random

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
pattern = ['-', '-', '-', 'K1', 'P2', 'K1', 'P3', 'K2', 'P1', \
            'K4', 'P2', 'K1', 'P3', 'K8', 'P4', 'K3', 'P2', \
            '-', '-', '-']
#TODO create bluetooth manager

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
        socketio.sleep(5)
        count += 1
        if count > len(pattern)-7:
            count = 0
        #socketio.emit('my_response',
        #              {'data': number, 'count': count},
        #              namespace='/test')
        socketio.emit('my_response',      \
                        {'stitch1': pattern[count+6], \
                         'stitch2': pattern[count+5], \
                         'stitch3': pattern[count+4], \
                         'stitch4': pattern[count+3], \
                         'stitch5': pattern[count+2], \
                         'stitch6': pattern[count+1], \
                         'stitch7': pattern[count+0], \
                         'cur'    : '2'             , \
                         'max'    : '6'             , \
                         }, namespace='/test')

# Route for serving up the index page
@app.route('/')
def index():
    return render_template('index.html', async_mode=socketio.async_mode)

# This function is called when a web browser connects
@socketio.on('connect', namespace='/test')
def test_connect():
    global thread
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(target=background_thread)
        #TODO Send initial pattern as above
        #TODO start up bluetooth and connect
        #TODO Add status message for successful connect/fail
    emit('my_response', {'stitch1': 'K1', \
                         'stitch2': 'P1', \
                         'stitch3': 'K3', \
                         'stitch4': 'P6', \
                         'stitch5': 'K2', \
                         'stitch6': 'P1', \
                         'stitch7': 'K1', \
                         'cur'    : '2' , \
                         'max'    : '6'})

# Ping-pong allows Javascript in the web page to calculate the
# connection latency, averaging over time
@socketio.on('my_ping', namespace='/test')
def ping_pong():
    emit('my_pong')

# Notification that a client has disconnected
@socketio.on('disconnect', namespace='/test')
def test_disconnect():
    #TODO disconnect bluetooth
    print('Client disconnected', request.sid)

# Run the web app
if __name__ == '__main__':
    socketio.run(app, debug=True, host='0.0.0.0')
