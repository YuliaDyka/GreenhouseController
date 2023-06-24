from tkinter import *
from tkinter import ttk
import tkinter.font as tkFont
from serial.tools.list_ports import comports
from serial import Serial
from serial import SerialException
import time
import json

class App:
    requests = ['{"Type":0,"ID":0}',
                # '{"Type":0,"ID":1,"CPS":120}',
                '{"Type":0,"ID":4}',
                # '{"Type":0,"ID":10,"HPos":67}',
                '{"Type":0,"ID":11}',
                # '{"Type":0,"ID":6,"R":255,"G":100,"B":100,"Br":95}',
                '{"Type":0,"ID":8}']

    def __init__(self):
        self.tk = Tk()
        self.tk.protocol("WM_DELETE_WINDOW", self.onClosing)
        self.tk.title("Greenhouse control app");
        # self.tk.geometry("600x400")
        self.ports = []
        self.isRunning = True
        self.is_open = False
        self.serial = Serial()
        self.serial.baudrate = 115200
        self.createWidgets()
        
    def sendRequest(self, request) -> None:
        if self.serial.is_open:
            self.serial.write(request.encode('ascii'))
            
    def requestLightChange(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 6
        request["R"] = self.redControlValue.get()
        request["G"] = self.greenControlValue.get()
        request["B"] = self.blueControlValue.get()
        request["Br"] = self.brightnessControlValue.get()
        request = json.dumps(request)
        self.sendRequest(request)
    
    def requestHatchChange(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 10
        request["HPos"] = self.hatchControlValue.get()
        request = json.dumps(request)
        self.sendRequest(request)       
    
    def requestFanChange(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 1
        request["CPS"] = self.fanControlValue.get()
        request = json.dumps(request)
        self.sendRequest(request)
    
    def requestSensorsInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 0
        request = json.dumps(request)
        self.sendRequest(request)
        
    def requestLightInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 8
        request = json.dumps(request)
        self.sendRequest(request)        
            
    def requestHatchInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 11
        request = json.dumps(request)
        self.sendRequest(request)
        
    def requestFanInfo(self) -> None:
        request = {}
        request["Type"] = 0
        request["ID"] = 4
        request = json.dumps(request)
        self.sendRequest(request)                    
    
    def parseIncoming(self, data) -> None:
        obj = json.loads(data)
        if "Type" in obj:
            if "ID" in obj and "Result" in obj and obj["Result"] == 0:
                if obj["ID"] == 0:
                    self.insideTempText.set(obj["Temp1"])
                    self.outsideTempText.set(obj["Temp2"])
                    self.insideLightText.set(obj["Light1"])
                    self.outsideLightText.set(obj["Light2"])
                elif obj["ID"] == 4:
                    self.fanSpeedText.set(obj["CPS"])
                elif obj["ID"] == 8:
                    self.ledRedText.set(obj["R"])
                    self.ledGreenText.set(obj["G"])
                    self.ledBlueText.set(obj["B"])
                    self.ledBrightnessText.set(obj["Br"])  
                elif obj["ID"] == 11:
                    self.hatchPositionText.set(obj["HPos"])
               
    def connectHandler(self):
        if  self.serial.is_open:
            self.serial.close()            
            self.connectionStatusValueLabel.configure(text="Disconnected")
            self.connectButton.configure(text="Connect")    
        else:
            self.serial.port = self.portsComboBox.get()
            self.serial.open()
            self.connectionStatusValueLabel.configure(text="Connected")
            self.connectButton.configure(text="Disconnect")
        
        self.updateUI()    

        # self.is_open = True
        # time.sleep(5)
        # self.send('{"Type":0,"ID":0}')

    def send(self, message):
        self.serial.write(bytes(message, 'utf-8'))
    
    def onClosing(self):
        self.tk.destroy()
        self.isRunning = False

    def mainloop(self):
        timer = time.time()
        print(timer)
        while self.isRunning:
            if self.serial.is_open:
                if time.time() - timer > 2:
                    timer = time.time()
                    self.requestSensorsInfo()
                    time.sleep(0.1)
                    self.requestLightInfo()
                    time.sleep(0.1)
                    self.requestHatchInfo()
                    time.sleep(0.1)
                    self.requestFanInfo()
                if self.serial.in_waiting > 0:
                    time.sleep(0.1)
                    myBytes = self.serial.read(1)
                    bufferBytes = self.serial.in_waiting
                    myBytes = myBytes + self.serial.read(bufferBytes)
                    str = myBytes.decode("utf-8")
                    lines = str.splitlines()
                    for line in lines:
                        self.parseIncoming(line)

            self.tk.update()
            self.tk.update_idletasks()
            
    def getPorts(self) -> list:
        for port in comports():
            self.ports.append(port.device)
        return self.ports
    
    def updateUI(self) -> None:
        state = 'normal' if self.serial.is_open else 'disable'
        for child in self.ledControlGroup.winfo_children():
            if child.widgetName != 'ttk::separator':
                child.configure(state=state) 
        for child in self.hatchControlGroup.winfo_children():
            if child.widgetName != 'ttk::separator':
                child.configure(state=state)
        for child in self.fanControlGroup.winfo_children():
            if child.widgetName != 'ttk::separator':
                child.configure(state=state)
                    
    def createWidgets(self) -> None:
        self.createConnectionWidgets()
        self.createStatusWidgets()
        self.createLedControlWidgets()
        self.createHatchControlWidgets()
        self.createFanControlWidgets()
        self.updateUI()
            
    def createConnectionWidgets(self) -> None:    
        # ------------------------ Connection ---------------------------
        self.getPorts()
        self.connectionGroup = LabelFrame(self.tk, text="Connection", labelanchor='n', padx=5)
        selectPortLabel = Label(self.connectionGroup, text="Select port:", justify="left")
        self.portsComboBox = ttk.Combobox(self.connectionGroup, values=self.ports)
        self.connectButton = Button(self.connectionGroup, text="Connect", command=self.connectHandler, width=12)
        connectionStatusLabel = Label(self.connectionGroup, text="Status:", justify="left")
        self.connectionStatusValueLabel = Label(self.connectionGroup, text="Disconnected", justify="left")
        self.portsComboBox.current(0)
        
        # self.font: dict[str, any] = tkFont.Font(font=self.connectionStatusValueLabel['font']).actual()
        # self.connectionStatusValueLabel.configure(font=(self.font['family'], self.font['size'], 'bold'))
        
        self.connectionGroup.pack(fill=BOTH, expand=False, padx=5, pady=5)       
        self.connectionGroup.columnconfigure(index=1, weight=1)
        self.connectionGroup.columnconfigure(index=2, pad=15)
        
        selectPortLabel.grid(row=0, column=0, columnspan=2, sticky=W)
        self.portsComboBox.grid(row=1, column=0, columnspan=2, sticky="we")
        self.connectButton.grid(row=1, column=2)
        connectionStatusLabel.grid(row=2, column=0, sticky=W)
        self.connectionStatusValueLabel.grid(row=2, column=1, columnspan=1, sticky=W)
        
    def createStatusWidgets(self) -> None:       
        # ------------------------ Status ---------------------------
        self.insideTempText = StringVar(value="---")
        self.outsideTempText = StringVar(value="---")
        self.insideLightText = StringVar(value="---")
        self.outsideLightText = StringVar(value="---")
        self.ledRedText = StringVar(value="---")
        self.ledGreenText = StringVar(value="---")
        self.ledBlueText = StringVar(value="---")
        self.ledBrightnessText = StringVar(value="---")
        self.hatchStateText = StringVar(value="---")
        self.hatchPositionText = StringVar(value="---")
        self.fanStateText = StringVar(value="---")
        self.fanSpeedText = StringVar(value="---")

        self.statusGroup = LabelFrame(self.tk, text="Status", labelanchor='n', padx=5)   
        insideTempLabel = Label(self.statusGroup, text="Inside temperature:", justify="left")
        outsideTempLabel = Label(self.statusGroup, text="Outside temperature:", justify="left") 
        insideLightLabel = Label(self.statusGroup, text="Inside light:", justify="left")
        outsideLightLabel = Label(self.statusGroup, text="Outside light:", justify="left")
        lightRedLabel = Label(self.statusGroup, text="Light RED:", justify="left")
        lightGreenLabel = Label(self.statusGroup, text="Light GREEN:", justify="left")
        lightBlueLabel = Label(self.statusGroup, text="Light BLUE:", justify="left")
        lightBrightnessLabel = Label(self.statusGroup, text="Light brightness:", justify="left")
        hatchStateLabel = Label(self.statusGroup, text="Hatch state:", justify="left")
        hatchPositionLabel = Label(self.statusGroup, text="Hatch position:", justify="left")
        fanStateLabel = Label(self.statusGroup, text="FAN state:", justify="left")
        fanSpeedLabel = Label(self.statusGroup, text="FAN speed:", justify="left")
      
        insideTempEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.insideTempText)
        outsideTempEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.outsideTempText)
        insideLightEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.insideLightText)
        outsideLightEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.outsideLightText)
        lightRedEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.ledRedText)
        lightGreenEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.ledGreenText)
        lightBlueEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.ledBlueText)
        lightBrightnessEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.ledBrightnessText)
        hatchStateEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.hatchStateText)
        hatchPositionEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.hatchPositionText)
        fanStateEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.fanStateText)
        fanSpeedEntry = Entry(self.statusGroup, width=15, state=DISABLED, textvariable=self.fanSpeedText)
        
        sensorsSeparator = ttk.Separator(self.statusGroup, orient='horizontal')
        lightSeparator = ttk.Separator(self.statusGroup, orient='horizontal')
        hatchSeparator = ttk.Separator(self.statusGroup, orient='horizontal')
        fanSeparator = ttk.Separator(self.statusGroup, orient='horizontal')
        verticalSeparator = ttk.Separator(self.statusGroup, orient='vertical')
                                
        self.statusGroup.pack(fill=BOTH, expand=False, padx=5, pady=5)
        
        for i in range(0, 9):
            self.statusGroup.rowconfigure(i, pad=2)
        
        self.statusGroup.columnconfigure(1, pad=15)
        self.statusGroup.columnconfigure(2, weight=1)
        self.statusGroup.columnconfigure(3, pad=15)
        
        sensorsSeparator.grid(row=0, column=0, columnspan=5, sticky="we")
        
        insideTempLabel.grid(row=1, column=0, sticky=W)
        insideTempEntry.grid(row=1, column=1)
        outsideTempLabel.grid(row=1, column=3, sticky=W)
        outsideTempEntry.grid(row=1, column=4)        
        insideLightLabel.grid(row=2, column=0, sticky=W)
        insideLightEntry.grid(row=2, column=1)
        outsideLightLabel.grid(row=2, column=3, sticky=W)
        outsideLightEntry.grid(row=2, column=4)
        
        lightSeparator.grid(row=3, column=0, columnspan=5, sticky="we")
         
        lightRedLabel.grid(row=4, column=0, sticky=W)
        lightGreenLabel.grid(row=4, column=3, sticky=W)
        lightRedEntry.grid(row=4, column=1) 
        lightGreenEntry.grid(row=4, column=4)
        
        lightBlueLabel.grid(row=5, column=0, sticky=W)
        lightBrightnessLabel.grid(row=5, column=3, sticky=W)
        lightBlueEntry.grid(row=5, column=1) 
        lightBrightnessEntry.grid(row=5, column=4)
        
        hatchSeparator.grid(row=6, column=0, columnspan=5, sticky="we")
        
        hatchStateLabel.grid(row=7, column=0, sticky=W)
        hatchPositionLabel.grid(row=7, column=3, sticky=W)
        hatchStateEntry.grid(row=7, column=1) 
        hatchPositionEntry.grid(row=7, column=4)
        
        fanSeparator.grid(row=8, column=0, columnspan=5, sticky="we")
         
        fanStateLabel.grid(row=9, column=0, sticky=W)
        fanSpeedLabel.grid(row=9, column=3, sticky=W)
        fanStateEntry.grid(row=9, column=1) 
        fanSpeedEntry.grid(row=9, column=4)
        
        verticalSeparator.grid(row=0, column=2, rowspan=10, sticky="ns", pady=2)        
        
    def createLedControlWidgets(self) -> None:     
        # ------------------------ Led Control ---------------------------     
        self.ledControlGroup = LabelFrame(self.tk, text="Light control", labelanchor='n', padx=5, pady=5)
        
        self.redControlValue = IntVar(value=0)
        self.greenControlValue = IntVar(value=0)
        self.blueControlValue = IntVar(value=0)
        self.brightnessControlValue = IntVar(value=0)       
        
        lightRedControlLabel = Label(self.ledControlGroup, text="Red:", justify="left")
        lightGreenControlLabel = Label(self.ledControlGroup, text="Green:", justify="left")
        lightBlueControlLabel = Label(self.ledControlGroup, text="Blue:", justify="left")
        lightBrightnessControlLabel = Label(self.ledControlGroup, text="Brightness:", justify="left")
        
        lightRedControlSlider = ttk.Scale(self.ledControlGroup, orient="horizontal", from_=0, to=255, 
                                          variable=self.redControlValue, command=lambda s:self.redControlValue.set(round(float(s))))
        lightGreenControlSlider = ttk.Scale(self.ledControlGroup, orient="horizontal", from_=0, to=255,
                                            variable=self.greenControlValue, command=lambda s:self.greenControlValue.set(round(float(s))))
        lightBlueControlSlider = ttk.Scale(self.ledControlGroup, orient="horizontal", from_=0, to=255,
                                           variable=self.blueControlValue, command=lambda s:self.blueControlValue.set(round(float(s))))
        lightBrightnessControlSlider = ttk.Scale(self.ledControlGroup, orient="horizontal", from_=0, to=100,
                                                 variable=self.brightnessControlValue, command=lambda s:self.brightnessControlValue.set(round(float(s))))
        
        lightRedControlSpinbox = Spinbox(self.ledControlGroup, from_=0, to=255, textvariable=self.redControlValue)
        lightGreenControlSpinbox = Spinbox(self.ledControlGroup, from_=0, to=255, textvariable=self.greenControlValue)
        lightBlueControlSpinbox = Spinbox(self.ledControlGroup, from_=0, to=255, textvariable=self.blueControlValue)
        lightBrightnessControlSpinbox = Spinbox(self.ledControlGroup, from_=0, to=255, textvariable=self.brightnessControlValue)
        
        self.lightApplyButton = Button(self.ledControlGroup, text="Apply", command=self.requestLightChange)
        
        self.ledControlGroup.pack(fill=BOTH, expand=False, padx=5, pady=5)
        
        self.ledControlGroup.columnconfigure(0, pad=25)
        self.ledControlGroup.columnconfigure(1, weight=1, pad=25)
        self.ledControlGroup.columnconfigure(2, pad=25)
        
        lightRedControlLabel.grid(row=0, column=0, sticky=W)
        lightGreenControlLabel.grid(row=1, column=0, sticky=W)
        lightBlueControlLabel.grid(row=2, column=0, sticky=W)
        lightBrightnessControlLabel.grid(row=3, column=0, sticky=W)
        
        lightRedControlSlider.grid(row=0, column=1, sticky="we")
        lightGreenControlSlider.grid(row=1, column=1, sticky="we")
        lightBlueControlSlider.grid(row=2, column=1, sticky="we")
        lightBrightnessControlSlider.grid(row=3, column=1, sticky="we")
        
        lightRedControlSpinbox.grid(row=0, column=2, sticky=E)
        lightGreenControlSpinbox.grid(row=1, column=2, sticky=E)
        lightBlueControlSpinbox.grid(row=2, column=2, sticky=E)
        lightBrightnessControlSpinbox.grid(row=3, column=2, sticky=E)
        
        self.lightApplyButton.grid(row=4, column=0, columnspan=3, sticky="we")
        
    def createHatchControlWidgets(self) -> None:    
        # ------------------------ Hatch Control ---------------------------
        self.hatchControlGroup = LabelFrame(self.tk, text="Hatch control", labelanchor='n', padx=5, pady=5)
        self.hatchControlValue = IntVar(value=0)
        
        hatchControlLabel = Label(self.hatchControlGroup, text="Position:", justify="left")
        hatchControlSlider = ttk.Scale(self.hatchControlGroup, orient="horizontal", from_=0, to=100, 
                                       variable=self.hatchControlValue, command=lambda s:self.hatchControlValue.set(round(float(s))))    
        hatchControlSpinbox = Spinbox(self.hatchControlGroup, from_=0, to=100, textvariable=self.hatchControlValue)
        hatchApplyButton = Button(self.hatchControlGroup, text="Apply", command=self.requestHatchChange)
        
        self.hatchControlGroup.pack(fill=BOTH, expand=False, padx=5, pady=5)
        
        self.hatchControlGroup.columnconfigure(0, pad=25)
        self.hatchControlGroup.columnconfigure(1, weight=1, pad=25)
        self.hatchControlGroup.columnconfigure(2, pad=25)
        
        hatchControlLabel.grid(row=0, column=0, sticky=W)
        hatchControlSlider.grid(row=0, column=1, sticky="we")
        hatchControlSpinbox.grid(row=0, column=2, sticky=E)
        hatchApplyButton.grid(row=4, column=0, columnspan=3, sticky="we")
    
    def createFanControlWidgets(self) -> None:    
        # ------------------------ Hatch Control ---------------------------
        self.fanControlGroup = LabelFrame(self.tk, text="Fan control", labelanchor='n', padx=5, pady=5)
        self.fanControlValue = IntVar(value=0)
        
        fanControlLabel = Label(self.fanControlGroup, text="Speed:", justify="left")
        fanControlSlider = ttk.Scale(self.fanControlGroup, orient="horizontal", from_=0, to=250, 
                                       variable=self.fanControlValue, command=lambda s:self.fanControlValue.set(round(float(s))))    
        fanControlSpinbox = Spinbox(self.fanControlGroup, from_=0, to=250, textvariable=self.fanControlValue)
        fanApplyButton = Button(self.fanControlGroup, text="Apply", command=self.requestFanChange)
        
        self.fanControlGroup.pack(fill=BOTH, expand=False, padx=5, pady=5)      
        self.fanControlGroup.columnconfigure(0, pad=25)
        self.fanControlGroup.columnconfigure(1, weight=1, pad=25)
        self.fanControlGroup.columnconfigure(2, pad=25)
        
        fanControlLabel.grid(row=0, column=0, sticky=W)
        fanControlSlider.grid(row=0, column=1, sticky="we")
        fanControlSpinbox.grid(row=0, column=2, sticky=E)
        fanApplyButton.grid(row=4, column=0, columnspan=3, sticky="we")
            