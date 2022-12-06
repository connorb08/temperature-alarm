import json
import subprocess
import time
import datetime
import smtplib
import sys

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
        self.emails = config['emails']
        self.phones = config['phone_numbers']
        self.auth = (config['auth']['email'],config['auth']['password'])
        self.message = config['alert_message']

    def alert(self, temp):
        now = datetime.datetime.now()
        difference = now - self.lastAlert
        mins = difference.total_seconds() / 60
        
        if (mins > self.alert_interval):
            print("alert")
            self.sendNotifications(temp)
            self.playAudio()
            self.lastAlert = datetime.datetime.now()
        
        return

    def playAudio(self):
        subprocess.run(['amixer', 'sset', 'Master', f'{self.volume}%'])
        subprocess.run(['aplay', 'alert.wav'])
        sys.stdout.flush()
        return

    def sendNotifications(self, temp):
        
        for email in self.emails:
            self.sendEmail(email, temp)
        for number in self.phones:
            self.sendText(number, temp)
        return
    
    def sendEmail(self, email, temp):
        msg = self.message + str(temp) + "F"
        to = email
        server = smtplib.SMTP("smtp.gmail.com", 587)
        server.starttls()
        server.login(self.auth[0], self.auth[1])
        server.sendmail(self.auth[0], to, msg)

    def sendText(self, number, temp):
        msg = self.message + str(temp) + "F"
        to = str(number) + "@vtext.com"
        server = smtplib.SMTP("smtp.gmail.com", 587)
        server.starttls()
        server.login(self.auth[0], self.auth[1])
        server.sendmail(self.auth[0], to, msg)

    def getTemp(self):
        args = ['./controller']
        for i in range(2):
            try:
                popen = subprocess.Popen(args, stdout=subprocess.PIPE)
                popen.wait()
                out = popen.stdout.read()
                out = round(float(str(out)[2:-1]), 2)
                break
            except FileNotFoundError: # Build file if not found
                subprocess.run(['make'])
                continue
        return out

    def exec(self):
        args = ['./controller']
        popen = subprocess.Popen(args, stdout=subprocess.PIPE)
        popen.wait()
        out = popen.stdout.read()
        out = round(float(str(out)[2:-1]), 2)
        return out

def main():
    x = Controller()
    temps = []
    while (True):
        try:
            temp = x.getTemp()
            temps.append(temp)
            avg = (sum(temps) / len(temps))
            print(f"Current temp: {temp} | Average: {avg}")
            if (len(temps) >= 10):
                temps.pop(0)
                if ((avg < x.temp_low) or (avg > x.temp_high)):
                    x.alert(temp)
            time.sleep(3)
        except KeyboardInterrupt:
            print("\nExiting!")
            exit(1)

if __name__ == '__main__':
    main()