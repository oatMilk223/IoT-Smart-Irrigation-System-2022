import serial
import time
#load the flask module into your python script
from flask import Flask, render_template

#create flask object called app
app = Flask(__name__)


@app.route('/')
def index():
    return "IoT Smart Irrigation Page"
    
# once this  script is running from the command line at the terminal, the 
# server starts to listen on port 8080, reporting any errors. note that 
# the "host=0.0.0.0" part is essential to telling the system  that we want the
# app visible to the outside world.
#extra stuff




if __name__== '__main__':
    #ser = serial.Serial('/dev/ttyS0',9600, timeout=1)
    #ser.flush()
    app.run(host='0.0.0.0', port=3000, debug = True)
