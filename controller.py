import json
import subprocess
import time

class Controller():
    
    def __init__(self):
        
        # Load config
        with open('config.json', 'r') as f:
            config = json.load(f)
        
        # Set config values
        self.temp_low = config['temp_low']
        self.temp_high = config['temp_high']
        self.alert_interval = config['alert_interval']
        self.volume = config['volume']
    
    def shutdown(self):
        args = ['c']
        return self.exec(args)

    def alert(self):
        #send notificaiton
        #sound alarm
        pass

    def playAudio(self):
        pass

    def exec(self, args=[]):
        args.insert(0, './controller')
        popen = subprocess.Popen(args, stdout=subprocess.PIPE)
        popen.wait()
        output = popen.stdout.read()
        return str(output)[2:-1]

def main():
    x = Controller()
    while (True):
        #catch q or ctrl c
        temp = x.exec()
        print(f"temp: {temp}")
        time.sleep(1)

if __name__ == '__main__':
    main()