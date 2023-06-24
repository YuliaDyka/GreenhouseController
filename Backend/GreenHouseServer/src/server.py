from flask import Flask
from flask_cors import CORS
from flask import request
from serial_controller import SerialController
import json

class Server:
    def __init__(self, serialController: SerialController):
        self.serialController = serialController
        self.server = Flask("Server")
        CORS(self.server)
        
        @self.server.route('/')
        def hello_world():
            return "Green House controller"

        @self.server.route('/ports/', methods=['GET', 'POST'])
        def getPorts():
            response = {}
            response["ports"] = self.serialController.getPorts()
            response["isOpen"] = self.serialController.isOpen()
            response["openIndex"] = self.serialController.getOpenPortIndex()
            return json.dumps(response)
        
        @self.server.route('/ports/open', methods=['POST'])
        def openPort():
            data = request.get_json()
            index = data["index"]
            print(index)
            response = {}
            response["result"] = self.serialController.open(index)
            return json.dumps(response)
       
        @self.server.route('/ports/close', methods=['POST'])
        def closePort():
            response = {}
            response["result"] = self.serialController.close()
            return json.dumps(response)
        
        @self.server.route('/info', methods=['POST', 'GET'])
        def getInfo():
            response = {}
            response["insideTemp"] = self.serialController.insideTemp
            response["outsideTemp"] = self.serialController.outsideTemp
            response["insideLight"] = self.serialController.insideLight
            response["outsideLight"] = self.serialController.outsideLight
            response["ledRed"] = self.serialController.ledRed
            response["ledGreen"] = self.serialController.ledGreen
            response["ledBlue"] = self.serialController.ledBlue
            response["ledBrightness"] = self.serialController.ledBrightness
            response["hatchPosition"] = self.serialController.hatchPosition
            response["fanSpeed"] = self.serialController.fanSpeed
            return json.dumps(response)
        
        @self.server.route('/leds', methods=['POST'])
        def setLeds():
            data = request.get_json()
            red = data["red"]
            green = data["green"]
            blue = data["blue"]
            brightness = data["brightness"]       
            response = {}
            response["result"] = self.serialController.requestLightChange(red, green, blue, brightness)
            return json.dumps(response)
        
        @self.server.route('/hatch', methods=['POST'])
        def setHatchPosition():
            data = request.get_json()
            position = data["position"]
            response = {}
            response["result"] = self.serialController.requestHatchChange(position)
            return json.dumps(response)
        
        @self.server.route('/fan', methods=['POST'])
        def setFanSpeed():
            data = request.get_json()
            speed = data["speed"]
            response = {}
            response["result"] = self.serialController.requestFanChange(speed)
            return json.dumps(response)        
      
    def startServer(self):    
        self.server.run(host='0.0.0.0', port=5000, threaded=True, use_reloader=False, debug=False)