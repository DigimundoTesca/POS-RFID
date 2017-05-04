import serial
import json
import urllib.request
from time import sleep


def main():
    host = "http://suministrosintegrales.cloudkitchen.online/diners/rfid/"
    # host = "http://172.21.231.13:8080/diners/rfid/"
    COM_list = ['COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6']
    print(' Welcome to RFID Module Monitor v2.0')
    print('Made With ♥ by Digimundo Technologies \n')

    arduino = get_connection(COM_list)
    while True:
        try:
            rfid = get_rfid(arduino, host)
            if rfid:
                send_rfid(rfid, host)

        except Exception as e:
            print(e)
            sleep(3)

def get_connection(COM_list):
    rfid_connection = False
    count = 0
    size_COM_list = len(COM_list)
    count_COM = 0

    while rfid_connection is False:
        try:
            arduino = serial.Serial(COM_list[count_COM], baudrate=9600, timeout=1.0)
            return arduino
        except Exception as e:
            if count > 1:
                # print('\nERROR:', e)
                sleep(0.5)
            count += 1
            if count_COM + 1 == size_COM_list:
                count_COM = 0
            else:
                count_COM += 1


def get_rfid(arduino, host):
    line = ''
    line = str(arduino.readline())
    if 'RFID' in line:
        rfid =  line.split('=')

        if len(rfid) > 1:
            """ rfid: list """
            rfid = rfid[1].split('\\')[0].strip(' ')
            if len(rfid) < 7:
                print('Tarjeta Detectada: ', rfid)    
                return rfid
            else:
                print('Tarjeta Inválida\n')
    return False


def send_rfid(rfid, host):
    conditionsSetURL = host
    data = {
        'rfid': rfid
    }
    headers = {
        'content-type': 'application/json',
        'User-Agent': 'Mozilla/5.0',
    }
    params = json.dumps(data).encode('utf8')

    try:
        req = urllib.request.Request(conditionsSetURL, data=params, 
            headers=headers)
        response = urllib.request.urlopen(req)
        print(response.read().decode('utf-8'), '\n')
    except Exception as e:
        print('*'*50)
        print('\nERROR: Revise conexión a Internet.')
        input('Presione Enter para continuar: ---->')


if __name__ == '__main__':
    main()
