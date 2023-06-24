from flask import Flask
from serial.tools.list_ports import comports
import threading
from serial_controller import SerialController
from server import Server

class App:   
    def __init__(self):
        self.serialController = SerialController()
        self.server = Server(self.serialController)
        
        self.serialThread = threading.Thread(target=self.serialController.mainloop)
        self.serialThread.setDaemon(True)
        self.serialThread.start()
            
        self.serverThread = threading.Thread(target=self.server.startServer)
        self.serverThread.setDaemon(True)
        self.serverThread.run()    
