import serial


ser = serial.Serial('COM3', 3000000, timeout=None)
def main():
    while True:
        byte = ser.read()
        print(byte)