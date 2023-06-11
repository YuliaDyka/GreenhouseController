from tkinter import Tk, Button, ttk
from serial.tools.list_ports import comports
from serial import Serial

COMBOBOX_WIDTH = 20

class Window:
    """Window of application"""

    def __init__(self):
        self.root = Tk()
        self.serial = None
        self.ports = []
        self.get_ports()
        self.create_widgets()

    def create_widgets(self) -> None:
        """Initializes and creates widgets in window"""
        self.ports_box = ttk.Combobox(self.root, values=self.ports, width=COMBOBOX_WIDTH)
        self.btn_select = Button(self.root, text="Select Port", command=self.open_port)
        self.btn_slave1 = Button(self.root, text="Send to slave1", command=self.send_slave1)
        self.btn_slave2 = Button(self.root, text="Send to slave2", command=self.send_slave2)
        self.ports_box.grid(row=0, column=0, columnspan=2, padx=10, pady=10)
        self.btn_select.grid(row=1, column=0, columnspan=2, padx=0, pady=10)

    def serial_write(self, adress: int, message: bytes):
        """Write message in serial port to slve with ardess"""
        self.serial.write(adress)
        self.serial.write(bytes(message))

    def send_slave1(self):
        """Send to slave 1 message 'hello'"""
        self.serial_write(0x01, b"hello")

    def send_slave2(self):
        """Send to slave 2 message 'hello'"""
        self.serial_write(0x02, b"hello")

    def open_port(self):
        """Opening selected port in combobox"""
        port = self.ports_box.get()
        if port == '':
            return
        
        self.serial = Serial(self.ports_box.get())
        self.btn_slave1.grid(row=2, column=0, columnspan=1, padx=5, pady=10)
        self.btn_slave2.grid(row=2, column=1, columnspan=1, padx=5, pady=10)


    def get_ports(self) -> list:
        """Returns all aviable ports"""
        for port in comports():
            self.ports.append(port.device)
        return self.ports

    def mainloop(self) -> None:
        """Main loop of window"""
        self.root.mainloop()
