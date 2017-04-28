import serial
import json
import urllib.request


host = "http://172.21.231.13:8080/diners/rfid/"

print('Welcome to RFID Module Monitor')
print('Made With ♥ by Digimundo')

try:
    arduino = serial.Serial('COM3', baudrate=9600, timeout=1.0)
    while True:
        line = ''
        line = str(arduino.readline())

        if line.find('RFID') > -1:
            rfid =  line.split('=')
            print('Tarjeta Detectada: ', rfid)

            if len(rfid) > 1:
                rfid = rfid[1].split('\\')[0]

                conditionsSetURL = host
                data = {
                    'rfid': rfid
                }
                headers = {
                    'content-type': 'application/json',
                    'User-Agent': 'Mozilla/5.0',
                }
                params = json.dumps(data).encode('utf8')

                req = urllib.request.Request(conditionsSetURL, data=params, 
                    headers=headers)
                response = urllib.request.urlopen(req)
                print(response.read().decode('utf-8'))


except Exception as e:
    print('ERROR: ', e)


            