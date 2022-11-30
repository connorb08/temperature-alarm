import json
import subprocess
import time
import datetime

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
        self.lastAlert = datetime.datetime(2000, 1, 1, 1, 1, 1)
    
    def shutdown(self):
        args = ['c']
        return self.exec(args)

    def alert(self):
        now = datetime.datetime.now()
        difference = now - self.lastAlert
        mins = difference.total_seconds() / 60
        
        if (mins > self.alert_interval):
            # alert()
            print("alert")
            #send notificaiton
            #sound alarm
            self.lastAlert = datetime.datetime.now()
        
        return

    def playAudio(self):
        pass

    def getTemp(self, args=[]):
        args.insert(0, './controller')
        for i in range(2):
            try:
                popen = subprocess.Popen(args, stdout=subprocess.PIPE)
                popen.wait()
                out = popen.stdout.read()
                break
            except FileNotFoundError: # Build file if not found
                subprocess.run(['make'])
                continue
        return float(str(out)[2:-1])

def main():
    x = Controller()
    temps = []
    while (True):
        #catch q or ctrl c
        temps.append(x.getTemp())
        avg = (sum(temps) / len(temps))
        print(f"avg: {avg}")
        if (len(temps) >= 10):
            temps.pop(0)
            if ((avg < x.temp_low) or (avg > x.temp_high)):
                x.alert()
        time.sleep(1)

if __name__ == '__main__':
    main()