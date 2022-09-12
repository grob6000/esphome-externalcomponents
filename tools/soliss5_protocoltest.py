import glob
import serial
from time import sleep
import sys

MSGSPLIT = 55
OUTPUTNAME = "out.csv"
PARSEDNAME = "out_parsed.csv"

def toSigned32(n):
    n = n & 0xffffffff
    return (n ^ 0x80000000) - 0x80000000

def parsemsg(msg,file,timestamp):
  pm = {"file":file,"timestamp":timestamp}
  pm["start"] = msg[0] # 0x7E
  pm["address"] = msg[1] # 1
  pm["command"] = msg[2] # 0xA1
  pm["length"] = msg[3] # 80
  if pm["command"] == 161 and pm["length"] == 80:
    pm["vdc_1"] = (msg[4] + msg[5]*256)*0.1 #V CONFIRMED
    pm["idc_1"] = (msg[6] + msg[7]*256)*0.1 #A CONFIRMED
    pm["vac_1"] = (msg[8] + msg[9]*256)*0.1 #V(RMS) CONFIRMED
    pm["iac_1"] = (msg[10] + msg[11])*0.1 #A(RMS) CONFIRMED
    pm["igbt_temp"] = (msg[12] + msg[13]*256)*0.1 #degC CONFIRMED
    pm["e_total"] = (msg[14] + msg[15]*256)*1.0 #kWh CONFIRMED
    # 16 = 0
    # 17 = 0
    # 18 = 3
    # 19 = 0
    # 20 = 0
    # 21 = 0
    # 22 = 19 \ =13075
    # 23 = 51 /
    pm["freq"] = (msg[24] + msg[25]*256)*0.01 #Hz CONFIRMED
    # 26 = 105  \ =1129
    # 27 = 4    /
    pm["vdc_2"] = (msg[28] + msg[29]*256)*0.1 #V CONFIRMED
    pm["idc_2"] = (msg[30] + msg[31]*256)*0.1 #A CONFIRMED
    # 32 = 190
    pm["e_thismonth"] = (msg[33] + msg[34]*256)*1.0 #kWh CONFIRMED 
    # 35 = 0
    # 36 = 0
    pm["e_today"] = (msg[37] + msg[38]*256)*0.1 #kW CONFIRMED
    pm["e_yesterday"] = (msg[39] + msg[40]*256)*0.1 #kWh
    pm["e_lastmonth"] = (msg[41] + msg[42]*256)*1.0 #kWh
    # 43 = 19
    # 44 = 2
    # 45 = 24
    # 46 = 22
    # 47 = 0
    # 48 = 132
    # 49 = 0
    # 50 = 0
    # 51 = 0
    # 52 = 0
    # 53 = 0
    # 54 = 0
    # 55 = 0
    # 56 = 0
    # 57 = 248 \ = 11000
    # 58 = 42  /
    pm["p_output"] = (msg[59] + msg[60]*256)*1.0 #W CONFIRMED
    # 61=0
    pm["unknown_62"] = msg[62]
    pm["unknown_63"] = msg[63]
    pm["unknown_64"] = msg[64]
    pm["va_output"] = (msg[65] + msg[66]*256)*1.0 #VA CONFIRMED
    # 67=0
    pm["pf"] = (msg[68] + msg[69]*256)*0.001 # 1/1 CONFIRMED
    pm["vac_3"] = (msg[70] + msg[71]*256)*0.1 #V CONFIRMED
    pm["iac_3"] = (msg[72] + msg[73]*256)*0.1 #A CONFIRMED
    pm["vac_2"] = (msg[74] + msg[75]*256)*0.1 #V CONFIRMED
    pm["iac_2"] = (msg[76] + msg[77]*256)*0.1 #A CONFIRMED
    #78 = 0
    #79 = 0
    #80 = 0
    #81 = 2
    #82 = 0
    pm["unknown_83"] = msg[83] # 115
    pm["crc"] = msg[84] # CONFIRMED
    pm["crc_check"] = 0
    for i in range(1,len(msg)-1):
      pm["crc_check"] += msg[i]
    pm["crc_check"]%=256
    print("crc={0},crc_check={1}".format(pm["crc"],pm["crc_check"]))
  elif (pm["command"] == 193 and pm["length"] == 40):
    #4 = 4
    #5 = 5
    #20-35 = '192.168.1.202' (null terminated)
    pass

  return pm

def putword(word, msg, index):
  word = int(word) % (256*256)
  msg[index] = word%256
  msg[index+1] = int(word/256)

def hexprint(msg):
  s = "MSG: "
  for b in msg:
    s += "{0:02X} ".format(b)
  print(s)

def sendmessage(ser=None):
  msg = []
  for i in range(0,85):
    msg.append(0)
  msg[0] = 0x7E
  msg[1] = 0x01
  msg[2] = 0xA1
  msg[3] = 80
  putword(2001,msg,4) #vdc1
  putword(10,msg,6) # idc1
  putword(2002,msg,28) # vdc2
  putword(11,msg,30) # idc2
  putword(2501,msg,8) #vacu
  putword(61,msg,10) #iacu
  putword(2502,msg,74) #vacv
  putword(62,msg,76) #iacv
  putword(2503,msg,70) #vacw
  putword(63,msg,72) #iacw
  putword(401,msg,12) #tigbt
  putword(1001,msg,14) #e_total
  putword(5000,msg,24) #freq
  putword(101,msg,33) #e_month
  putword(301,msg,37) #e_today
  putword(302,msg,39) #e_yesterday
  putword(102,msg,41) #e_lastmonth
  va = 250.1*6.1+250.2*6.2+250.3*5.3
  putword(int(va*.95),msg,59) # outputpower
  putword(950,msg,68) # pf
  putword(int(va),msg,65) # va
  csum = 0
  for i in range(1,84):
    csum += msg[i]
  csum %= 256
  msg[84] = csum
  #hexprint(msg)
  #print(len(bytes(msg)))
  closeport = False
  if not ser:
    ser = serial.Serial("COM7", 9600)
    closeport = True
  bs = bytes(msg)
  print(bs)
  print(len(bs))
  ser.write(bs)
  ser.flush()
  sleep(2)
  if closeport:
    ser.close()


if __name__ == "__main__":
  port = "COM4"
  mode = "dummyinverter"

  for v in sys.argv:
    v = v.lower()
    if v.startswith("port="):
      port = v.split("=")[1].upper()
    elif v.startswith("mode="):
      mode = v.split("=")[1]
  
  if mode == "dummyinverter":
    print("solis message sender")
    ser = None
    try:
      ser = serial.Serial(port, 9600)
    except Exception as e:
      print(e)
      print("invalid serial port, quitting.")
    
    if ser:  
      try:
        while True:
          sendmessage(ser=ser)
          sleep(5)
      except KeyboardInterrupt as e:
        quit()

  elif mode == "processoutput":

    print("solis esphome debug output processor")
    textfilelist = glob.glob("*.txt")
    messages = []
    with open(OUTPUTNAME, "w") as outfile:
      outfile.write("file,timestamp,")
      for i in range(0,86):
        outfile.write(str(i) + ",")
      outfile.write("\n")
      msglist = []
      for t in textfilelist:
        print("reading file: " + t)
        with open(t,"r") as f:
          lines = f.readlines()
          for l in lines:
            if "<<<" in l:
              hexbytes = l.split("<<<")[1].strip().split(":")
              bytes = [ int(h, 16) for h in hexbytes ]
              #print(len(bytes))
              #processmsg(bytes)
              if len(bytes) == 140:
                bytes1 = bytes[:MSGSPLIT]
                outfile.write(t + ",")
                outfile.write(l[1:9] + ",") # timestamp
                for b in bytes1:
                  outfile.write(str(b) + ",")
                outfile.write("\n")
                msglist.append(parsemsg(bytes1, t, l[1:9]))
                bytes2 = bytes[MSGSPLIT:]
                outfile.write(t + ",")
                outfile.write(l[1:9] + ",") # timestamp
                for b in bytes2:
                  outfile.write(str(b) + ",")
                outfile.write("\n")
                msglist.append(parsemsg(bytes2, t, l[1:9]))
              else:
                outfile.write(t + ",")
                outfile.write(l[1:9] + ",") # timestamp
                for b in bytes:
                  outfile.write(str(b) + ",")    
                outfile.write("\n")
                msglist.append(parsemsg(bytes, t, l[1:9]))          
      with open(PARSEDNAME, "w") as outfile2:
        fullkeylist = []
        for msg in msglist:
          for k in msg.keys():
            if not k in fullkeylist:
              fullkeylist.append(k)
        print(fullkeylist)
        for k in fullkeylist:
          outfile2.write(k + ",")
        outfile2.write("\n")
        for msg in msglist:
          for k in fullkeylist:
            if k in msg:
              outfile2.write(str(msg[k]))
            outfile2.write(",")
          outfile2.write("\n")
  else:
    print("invalid mode, no action")

