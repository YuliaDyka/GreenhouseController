from serial.tools.list_ports import comports
from serial import Serial
import time
import json

class SerialController:
    openPortIndex = -1
    
    insideTemp = 0
    outsideTemp = 0
    insideLight = 0
    outsideLight = 0
    ledRed = 0
    ledGreen = 0
    ledBlue = 0
    ledBrightness = 0
    hatchPosition = 0
    fanSpeed = 0
    
    def __init__(self):
        self.serialPort = Serial()
        self.serialPort.baudrate = 115200
        self.ports = []
        self.getPorts()
        
    def getPorts(self) -> list:
        self.ports = []
        for port in comports():
            self.ports.append(port.device)
        return self.ports
    
    def open(self, index):
        if not self.serialPort.is_open:
            if index >= 0 and index < len(self.ports):
                self.openPortIndex = index
                self.serialPort.port = self.ports[index]
                self.serialPort.open()
                return True
        return False
    
    def close(self):
        if self.serialPort.is_open:
            self.serialPort.close()
            self.openPortIndex = -1
            return True
        return False
            
    def isOpen(self):
        return self.serialPort.is_open
    
    def getOpenPortIndex(self):
        return self.openPortIndex
      
    def mainloop(self):
        timer = time.time()
        print("start main loop")
        while True:
            if self.serialPort.is_open:
                if time.time() - timer > 1:
                    timer = time.time()
                    self.requestSensorsInfo()
                    time.sleep(0.1)
                    self.requestLightInfo()
                    time.sleep(0.1)
                    self.requestHatchInfo()
                    time.sleep(0.1)
                    self.requestFanInfo()
                if self.serialPort.in_waiting > 0:
                    time.sleep(0.1)
                    myBytes = self.serialPort.read(1)
                    bufferBytes = self.serialPort.in_waiting
                    myBytes = myBytes + self.serialPort.read(bufferBytes)
                    str = myBytes.decode("utf-8")
                    lines = str.splitlines()
                    for line in lines:
                        self.parseIncoming(line)
            else:
                time.sleep(0.2)            

    def sendRequest(self, request) -> None:
        if self.serialPort.is_open:
            self.serialPort.write(request.encode('ascii'))
            return True
        return False
            
    def requestLightChange(self, r, g, b, br) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 6
        request["R"] = r
        request["G"] = g
        request["B"] = b
        request["Br"] = br
        request = json.dumps(request)
        return self.sendRequest(request)
    
    def requestHatchChange(self, pos) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 10
        request["HPos"] = pos
        request = json.dumps(request)
        return self.sendRequest(request)       
    
    def requestFanChange(self, speed) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 1
        request["CPS"] = speed
        request = json.dumps(request)
        return self.sendRequest(request)
    
    def requestSensorsInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 0
        request = json.dumps(request)
        return self.sendRequest(request)
        
    def requestLightInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 8
        request = json.dumps(request)
        return self.sendRequest(request)        
            
    def requestHatchInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 11
        request = json.dumps(request)
        return self.sendRequest(request)
        
    def requestFanInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 4
        request = json.dumps(request)
        return self.sendRequest(request)                    
    
    def parseIncoming(self, data) -> None:
        obj = json.loads(data)
        if "Type" in obj:
            if "ID" in obj and "Result" in obj and obj["Result"] == 0:
                if obj["ID"] == 0:
                    self.insideTemp = obj["Temp1"]
                    self.outsideTemp = obj["Temp2"]
                    self.insideLight = obj["Light1"]
                    self.outsideLight = obj["Light2"]
                elif obj["ID"] == 4:
                    self.fanSpeed = obj["CPS"]
                elif obj["ID"] == 8:
                    self.ledRed = obj["R"]
                    self.ledGreen = obj["G"]
                    self.ledBlue = obj["B"]
                    self.ledBrightness = obj["Br"]
                elif obj["ID"] == 11:
                    self.hatchPosition = obj["HPos"]  