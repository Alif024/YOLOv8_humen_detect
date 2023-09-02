from time import time, ctime, sleep
from datetime import datetime

day = ctime().split()[0]
hour = datetime.now().hour
minute = datetime.now().minute

# day = 'Mon'
# hour = 16

# Monday
if day == 'Mon' and ((hour == 8 and minute > 14 and minute <= 59) or ((hour == 9 or hour == 10) and minute >= 0 and minute <= 59) or (hour == 11 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)
elif day == 'Mon' and ((hour == 13 and minute > 14 and minute <= 59) or (hour >= 14 and hour <= 16 and minute >= 0 and minute <= 59) or (hour == 17 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)

# Tuesday
elif day == 'Tue' and ((hour == 8 and minute > 14 and minute <= 59) or ((hour == 9 or hour == 10) and minute >= 0 and minute <= 59) or (hour == 11 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)
elif day == 'Tue' and ((hour == 13 and minute > 14 and minute <= 59) or (hour >= 14 and hour <= 16 and minute >= 0 and minute <= 59) or (hour == 17 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)

# Wednesday
elif day == 'Wed' and ((hour == 8 and minute > 14 and minute <= 59) or ((hour == 9 or hour == 10) and minute >= 0 and minute <= 59) or (hour == 11 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)
elif day == 'Wed' and ((hour == 13 and minute > 14 and minute <= 59) or (hour >= 14 and hour <= 16 and minute >= 0 and minute <= 59) or (hour == 17 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)

# Thursday
elif day == 'Thu' and ((hour == 8 and minute > 14 and minute <= 59) or ((hour == 9 or hour == 10) and minute >= 0 and minute <= 59) or (hour == 11 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)
elif day == 'Thu' and ((hour == 13 and minute > 14 and minute <= 59) or (hour >= 14 and hour <= 16 and minute >= 0 and minute <= 59) or (hour == 17 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)

# Friday
elif day == 'Fri' and ((hour == 8 and minute > 14 and minute <= 59) or ((hour == 9 or hour == 10) and minute >= 0 and minute <= 59) or (hour == 11 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)
elif day == 'Fri' and ((hour == 13 and minute > 14 and minute <= 59) or (hour >= 14 and hour <= 16 and minute >= 0 and minute <= 59) or (hour == 17 and minute >= 0 and minute <= 45)):
    print("อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 1)
else:
    print("ไม่อยู่ในคาบเรียน")
    # GPIO.output(schedulePin, 0)

'''
if GPIO.input(schedulePinTest) == 0:
    GPIO.output(schedulePin, 1)
    print('test schedule')
else:
    GPIO.output(schedulePin, 0)
if GPIO.input(finishSchedulePinTest) == 0:
    GPIO.output(finishSchedulePin, 1)
    print('test finish schedule')
else:
    GPIO.output(finishSchedulePin, 0)
'''
sleep(30)
